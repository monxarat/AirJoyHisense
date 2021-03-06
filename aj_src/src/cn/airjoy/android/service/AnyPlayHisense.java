package cn.airjoy.android.service;


import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.DialogInterface.OnCancelListener;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.airjoy.bonjour.Bonjour.Status;
import com.fqx.airjoy.server.AirJoy;
import com.fqx.airjoy.service.AJService;
import com.fqx.anyplay.api.APPEnum;
import com.fqx.anyplay.api.AnyPlayUtils;
import com.fqx.anyplay.api.CallBackEvent;
import com.fqx.anyplay.api.LocalInfo;
import com.fqx.anyplay.api.SangProgressDialog;
import com.fqx.anyplay.api.SystemConfig;
import com.fqx.anyplay.service.APController;
import com.fqx.anyplay.service.APService;
import com.fqx.sang.video.LibsChecker;
import com.umeng.analytics.MobclickAgent;
import com.umeng.update.UmengUpdateAgent;
import com.umeng.update.UmengUpdateListener;
import com.umeng.update.UpdateResponse;

/**
* <p>文件名: AnyPlayHisense.java</p>
* <p>描述: AirJoy主页面UI</p>
* 
* @author sangwencheng
* @version 1.0
*/
public class AnyPlayHisense extends Activity {
//	private Intent AJServiceIntent;
//	private Intent APServiceIntent;
	private AlertDialog alert;
	private boolean isShowToast = false;
	private TextView mONOFFTextView;
	private TextView mOldNameTextView;
	private SangNote mSangNote;
	private SystemConfig mSystemConfig;
	private AJService.AJBinder mAJServerBinder;
	private APController mAPController;
	private APService.MyBinder mAPServerBinder;
	private AirJoy mAirJoy;
	private Button mDevNameButton;
	private TextView mDevNameTextView;
	private Dialog mDialog;
	private TextView mErr;
	private Handler mHandler = new Handler();
	private LocalInfo mLocalInfo;
	private EditText mNewNameEditText;
	private Button mONOFFButton;
	

	public void onCreate(Bundle paramBundle) {
	    super.onCreate(paramBundle);
	    setContentView(R.layout.hs_main);
	    MobclickAgent.setSessionContinueMillis(10000L);
	    MobclickAgent.setDefaultReportPolicy(this, 0);
	    MobclickAgent.updateOnlineConfig(this);
	    MobclickAgent.onError(this);
	    UmengUpdateAgent.update(this);
	    UmengUpdateAgent.setUpdateOnlyWifi(false);
	    this.mAPController = null;
	    this.mAPServerBinder = null;
	    this.mAJServerBinder = null;
	    this.mAirJoy = null;
	
	    set_update(this);
	    this.isShowToast = false;
	    this.mSystemConfig = new SystemConfig(this);
	    AnyPlayUtils.LOG_DEBUG("AirJoy", "system running...");
	    this.mLocalInfo = LocalInfo.getInstance(this);
	    this.mLocalInfo.setServerState(APPEnum.ServiceState.Fail.GetValue());
	    checkScreenPdi();
	    if (this.mLocalInfo.g_IsLocalPlayer_m3u8 == false) {
		    if (!LibsChecker.checkVitamioLibs(this, R.string.init_decoders))
		      return;
	    }
	    this.mONOFFTextView = ((TextView)findViewById(R.id.lab_onoff));
	    this.mDevNameTextView = ((TextView)findViewById(R.id.lab_dev_name));
	    this.mONOFFButton = ((Button)findViewById(R.id.btn_onoff));
	    this.mDevNameButton = ((Button)findViewById(R.id.btn_dev_name));
	    this.mONOFFButton.setOnClickListener(this.mONOFFClickListener);
	    this.mDevNameButton.setOnClickListener(this.mDevNameClickListener);
	    setVerInfo();
	    startAPControllerService();
	    startAirJoyService();
	    IntentFilter airPlayOnFilter = new IntentFilter();
	    airPlayOnFilter.addAction(AnyPlayUtils.ACTION_PLAYER_SWCOMPLETE);
	    registerReceiver(airplay_on_event, airPlayOnFilter);
	    
	    loadDevName();
	    if(AnyPlayUtils.isNetOK(this) == false) {
	    	netErr();
	    }
	}
	
	private BroadcastReceiver airplay_on_event = new BroadcastReceiver() {
	    public void onReceive(Context paramContext, Intent paramIntent) {
	      Bundle localBundle = paramIntent.getExtras();
	      String which = localBundle.getString("Which");
	      if(which.equals("STOP")) {
	    	  stopProgressDialog();
			  setState(Status.Stopped);
	      }else if(which.equals("START")) {
	    	  stopProgressDialog();
			  setState(Status.Started);
	      }
	    }
	};
	
	private SangProgressDialog progressDialog = null;
	private void startProgressDialog(String msg) {
	    try {
	      if (this.progressDialog == null) {
	        this.progressDialog = SangProgressDialog.createDialog(this);
		    this.progressDialog.setOnCannel(new OnCancelListener() {
				@Override
				public void onCancel(DialogInterface dialog) {
					stopProgressDialog();
					AnyPlayHisense.this.finish();
				}
			});
	      }
	      progressDialog.setMessage(msg);
	      this.progressDialog.show();
	    } catch (Exception localException) {
	      localException.printStackTrace();
	    }
	} 
	
	private void stopProgressDialog() {
	    if (this.progressDialog == null) {
	    	return;
	    }
	    try {
	      this.progressDialog.dismiss();
	      this.progressDialog = null;
	    } catch (Exception localException) {
	      localException.toString();
	    }
	}
	
	private boolean is_try_opt() {
	    if(AnyPlayUtils.isNetOK(this) == false) {
	    	netErr();
	    	return false;
	    }
	    return true;
	}
	
	
	private ServiceConnection mAJServiceConnection = new ServiceConnection() {
	    public void onServiceConnected(ComponentName paramComponentName, IBinder paramIBinder) {
	      AnyPlayHisense.this.mAJServerBinder = ((AJService.AJBinder)paramIBinder);
	      AnyPlayHisense.this.mAirJoy = AnyPlayHisense.this.mAJServerBinder.getAirJoy();
	      AnyPlayHisense.this.openONOFFState(false);
	    }
	
	    public void onServiceDisconnected(ComponentName paramComponentName) {
	    }
	};
	private ServiceConnection mAPServiceConnection = new ServiceConnection() {
	    public void onServiceConnected(ComponentName paramComponentName, IBinder paramIBinder) {
	      AnyPlayHisense.this.mAPServerBinder = ((APService.MyBinder)paramIBinder);
	      AnyPlayHisense.this.mAPController = AnyPlayHisense.this.mAPServerBinder.getService();
	      AnyPlayHisense.this.openONOFFState(true);
	      setState(mAPServerBinder.getStartus());
	    }
	
	    public void onServiceDisconnected(ComponentName paramComponentName) {
	    }
	};
	private View.OnClickListener mDevNameClickListener = new View.OnClickListener() {
	    public void onClick(View paramView) { 
	    	if(is_try_opt()) {
		    	AnyPlayHisense.this.EvtModefyDevName();
	    	}
	    }
	};
	private View.OnClickListener mONOFFClickListener = new View.OnClickListener() {
	    public void onClick(View paramView) {
	    	if(is_try_opt()) { 
	    		AnyPlayHisense.this.EvtOnoff();
	    	}
	    }
	};
	private Runnable mStartRunnable = new Runnable() {
	    public void run() {
	      AnyPlayHisense.this.mAPServerBinder.Start();
	    }
	};
	private Runnable mStopRunnable = new Runnable() {
	    public void run() {
//	      AnyPlayHisense.this.mAPServerBinder.stopBonjour();
	    }
	};

	
	private void EvtModefyDevName() {
	    initDialog(this.mDevNameTextView.getText().toString());
	    this.mDialog.show();
	}
	
	private void setState(Status status) {
	    String open = getResources().getString(R.string.open);
	    String close = getResources().getString(R.string.close);
		if(status == Status.Started) {
			this.mONOFFButton.setText(close);
			this.mONOFFTextView.setText(open);
			this.mSystemConfig.setSwitchON(true);
			LocalInfo.isSwitchON = true;
		}else if(status == Status.Stopped) {
		    this.mONOFFButton.setText(open);
		    this.mONOFFTextView.setText(close);
		    LocalInfo.isSwitchON = false;
		}else if(status == Status.Stopping) {
		    this.mONOFFTextView.setText(getResources().getString(R.string.closing));
		}else if(status == Status.Starting) {
		    this.mONOFFTextView.setText(getResources().getString(R.string.stating));
		}
	}
	private void EvtOnoff() {
		Status mStatus;
	    String str1 = getResources().getString(R.string.open);
	    String str2 = getResources().getString(R.string.close);
	    mStatus = mAPServerBinder.getStartus();
	    if (this.mONOFFButton.getText().toString().endsWith(str1)) {
			this.mHandler.removeCallbacks(this.mStopRunnable);
			this.mAJServerBinder.reStart();
//			this.mSystemConfig.setSwitchON(true);
//			LocalInfo.isSwitchON = true;
//			this.mONOFFButton.setText(str2);
//			this.mONOFFTextView.setText(str1);
//			if(mAPServerBinder.isStarted() == false) {
			if(mStatus == Status.Stopped) {
				this.mHandler.postDelayed(this.mStartRunnable, 300L);
				startProgressDialog("正在启动，请稍后...");
			}else if(mStatus == Status.Starting) {
				startProgressDialog("正在启动，请稍后...");
				setState(mStatus);
			}
	    }else{
		    this.mHandler.removeCallbacks(this.mStartRunnable);
		    this.mAJServerBinder.stop();
//		    LocalInfo.isSwitchON = false;
//		    this.mSystemConfig.setSwitchON(false);
//		    this.mONOFFButton.setText(str1);
//		    this.mONOFFTextView.setText(str2);
			if(mStatus == Status.Started) {
				startProgressDialog("正在关闭，请稍后...");
			    this.mAPServerBinder.stop();
			    this.mHandler.postDelayed(this.mStopRunnable, 200L);
			}else if(mStatus == Status.Stopping) {
				startProgressDialog("正在关闭，请稍后...");
				setState(mStatus);
			}
	    }
	}
	
	private void callbackEvt() {
		if(mSangNote == null) {
			return;
		}
	    this.mSangNote.dismiss();
	}
	
	private void checkScreenPdi() {
	    this.mLocalInfo.initScreen(this);
	    int i = this.mLocalInfo.getScreenWidth();
	    int j = this.mLocalInfo.getScreenHeight();
	    if (j > i)
	      i = j;
	    Log.d("AnyPlay", "w=" + i + " h=" + j);
	    if (i >= 1200)
	      return;
	    AnyPlayUtils.ShowMsgBox(this, getResources().getString(R.string.screen_note));
	}
	
	private void clsErr() {
	    this.mErr.setText("");
	} 
	
	private void exit() {
	    this.mDialog.dismiss();
	}
	
	private void initDialog(String paramString) {
	    this.mDialog = new Dialog(this, R.style.my_dialog);
	    View localView = LayoutInflater.from(this).inflate(R.layout.hisense_modefy_name, null);
	    this.mDialog.setContentView(localView);
	    this.mOldNameTextView = ((TextView)localView.findViewById(R.id.old_name));
	    this.mNewNameEditText = ((EditText)localView.findViewById(R.id.new_name));
	    this.mErr = ((TextView)localView.findViewById(R.id.modefy_name_msg));
	    this.mOldNameTextView.setText(paramString);
	    this.mNewNameEditText.setOnTouchListener(new View.OnTouchListener() {
	      public boolean onTouch(View paramView, MotionEvent paramMotionEvent) {
	        AnyPlayHisense.this.clsErr();
	        Log.i("", "setOnTouchListener");
	        return false;
	      }
	    });
	
	    this.mNewNameEditText.setOnEditorActionListener(new TextView.OnEditorActionListener() {
	      public boolean onEditorAction(TextView paramTextView, int paramInt, KeyEvent paramKeyEvent) {
	        int i = paramTextView.getText().toString().trim().length();
	        Log.i("setOnEditorActionListener", "Len=" + i);
	        if (i > 16)
	          AnyPlayHisense.this.showLimitErr();
	        return false;
	      }
	    });
	    this.mDialog.setOnDismissListener(new DialogInterface.OnDismissListener() {
	      public void onDismiss(DialogInterface paramDialogInterface) {
	        AnyPlayHisense.this.exit();
	      }
	    });
	    this.mDialog.setOnCancelListener(new DialogInterface.OnCancelListener() {
	      public void onCancel(DialogInterface paramDialogInterface) {
	        AnyPlayHisense.this.exit();
	      }
	    });
	
	    ((Button)localView.findViewById(R.id.modefy_name_confirm)).setOnClickListener(new View.OnClickListener() {
	      public void onClick(View paramView) {
	        AnyPlayHisense.this.modefyName();
	      }
	    });
	
	    ((Button)localView.findViewById(R.id.modefy_name_cancel)).setOnClickListener(new View.OnClickListener() {
	      public void onClick(View paramView) {
	        AnyPlayHisense.this.exit();
	      }
	    });
	}
	
	private void loadDevName() {
	    String str = this.mSystemConfig.getLocalDevName();
	    this.mDevNameTextView.setText(str);
	}
	
	private void modefyName() {
	    String str = this.mNewNameEditText.getText().toString();
	    if (str.trim().equals("")) {
	      this.mErr.setText(getResources().getString(R.string.modefy_name_err));
	      return;
	    }
	    if (str.trim().length() > 16) {
	      this.mErr.setText(getResources().getString(R.string.modefy_name_err_lenth));
	      return;
	    }
	    if (this.mAPController == null) {
	      this.mDialog.dismiss();
	      return;
	    }
	    this.mAPServerBinder.reName(str);
	    this.mAJServerBinder.reName(str);
	    this.mLocalInfo.setName(str);
	    this.mSystemConfig.setLocalDevName(str);
	    this.mDevNameTextView.setText(str);
	    this.mDialog.dismiss();
	}
	
	
	private void netErr() {
	    this.mSangNote = new SangNote(this);
	    this.mSangNote.setMsg(getResources().getString(R.string.net_err_msg));
	    this.mSangNote.setConfirmOnListener(new View.OnClickListener() {
	      public void onClick(View paramView) {
	        AnyPlayHisense.this.callbackEvt();
	      }
	    });
	    this.mSangNote.setDelayCallbackEvent(AnyPlayUtils.NodeDelayTimes, new CallBackEvent() {
	      public void delayCallbackEvent() {
	        AnyPlayHisense.this.callbackEvt();
	      }
	    });
	    this.mSangNote.show();
	}
	
	private void openONOFFState(boolean paramBoolean) {
	    String str1 = getResources().getString(R.string.open);
	    String str2 = getResources().getString(R.string.close);
	    boolean defauleEnable = mLocalInfo.g_isDefaultEnable;
	    if (this.mSystemConfig.getSwithcON(defauleEnable)) {
	      this.mONOFFButton.setText(str2);
	      this.mONOFFTextView.setText(str1);
	      if (paramBoolean) {
	        this.mHandler.postDelayed(this.mStartRunnable, 300L);
	      }else{
	    	  this.mAJServerBinder.reStart();
	      }
	    }else{
		    this.mONOFFButton.setText(str1);
		    this.mONOFFTextView.setText(str2);
		    if (paramBoolean) {
		      this.mAPServerBinder.stop();
		      this.mHandler.postDelayed(this.mStopRunnable, 300L);
		    }else{
		    	this.mAJServerBinder.stop();
		    }
	    }
	}
	
	private void setVerInfo() {
	    TextView localTextView = (TextView)findViewById(R.id.hs_ver_info);
	    String localObject = "海信" + mLocalInfo.g_UseType + "版 ";
	    try {
	      PackageInfo localPackageInfo = getPackageManager().getPackageInfo(getPackageName(), PackageManager.GET_CONFIGURATIONS);
	      String str = localObject + "(" + localPackageInfo.versionName + ") ";
	      localObject = str;
	      localTextView.setText(localObject + getResources().getString(R.string.murl));
	    } catch (Exception e) {
	    	e.printStackTrace();
	    }
	}
	
	private void set_update(Context paramContext) {
	    final Context mContext = paramContext;
	    UmengUpdateAgent.setUpdateAutoPopup(false);
	    UmengUpdateAgent.setUpdateListener(new UmengUpdateListener() {
	            @Override
	            public void onUpdateReturned(int updateStatus,UpdateResponse updateInfo) {
	                switch (updateStatus) {
	                case 0: // has update
	                    UmengUpdateAgent.showUpdateDialog(mContext, updateInfo);
	                    break;
	                case 1: // has no update
	                	if(isShowToast == false) {
	                		return;
	                	}
	                    Toast.makeText(mContext, "没有更新", Toast.LENGTH_SHORT).show();
			            isShowToast = false;
	                    break;
	                case 2: // none wifi
	                	if(isShowToast == false) {
	                		return;
	                	}
	                    Toast.makeText(mContext, "没有wifi连接， 只在wifi下更新", Toast.LENGTH_SHORT).show();
			            isShowToast = false;
	                    break;
	                case 3: // time out
	                	if(isShowToast == false) {
	                		return;
	                	}
	                    Toast.makeText(mContext, "超时", Toast.LENGTH_SHORT).show();
			            isShowToast = false;
	                    break;
	                }
	            }
	    });
	}
	
	private void showLimitErr() {
	    if (this.mErr == null)
	      return;
	    this.mErr.setText(getResources().getString(R.string.modefy_name_err_lenth));
	}
	
	private void startAPControllerService() {
	    Intent APServiceIntent = new Intent(this, APService.class);
	    startService(APServiceIntent);
	    bindService(APServiceIntent, this.mAPServiceConnection, 1);
	}
	
	private void startAirJoyService() {
	    Intent AJServiceIntent = new Intent(this, AJService.class);
	    startService(AJServiceIntent);
	    bindService(AJServiceIntent, this.mAJServiceConnection, 1);
	}
	

	protected void onDestroy() {
	    super.onDestroy();
	    unregisterReceiver(this.airplay_on_event);
	    try {
	      if (this.mAPController != null)
	        unbindService(this.mAPServiceConnection);
	      if (this.mAirJoy != null)
	        unbindService(this.mAJServiceConnection);
	    } catch (Exception localException) {
	    	localException.printStackTrace();
	    }

	    Log.d("AnyPlayHisense", "EXIT:");
	}
	
	public boolean onKeyDown(int paramInt, KeyEvent paramKeyEvent) {

	    if (paramInt == KeyEvent.KEYCODE_BACK) {
	      finish();
	      return true;
	    }
	    return super.onKeyDown(paramInt, paramKeyEvent);
	}
	
	protected void onPause() {
	    super.onPause();
	    MobclickAgent.onPause(this);
	    if ((this.alert == null) || (!this.alert.isShowing()))
	      return;
	    this.alert.dismiss();
	}
	
	protected void onResume() {
	    super.onResume();
	    MobclickAgent.onResume(this);
	}

}