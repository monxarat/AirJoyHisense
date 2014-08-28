package cn.airjoy.android.service;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.DialogInterface.OnCancelListener;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.ImageView;

import com.airjoy.airplay.server.AirPlayServer;
import com.fqx.airjoy.server.AJEnum;
import com.fqx.anyplay.api.APPEnum;
import com.fqx.anyplay.api.AnyPlayUtils;
import com.fqx.anyplay.api.CallBackEvent;
import com.fqx.anyplay.api.LocalInfo;
import com.fqx.anyplay.api.PublishState;
import com.fqx.anyplay.api.SangProgressDialog;
import com.fqx.anyplay.api.Statistics;
import com.fqx.anyplay.api.VerLeg;
import com.fqx.anyplay.images.AirImgCache;
import com.fqx.anyplay.images.GetSampleBmp;
import com.fqx.anyplay.images.MyImageView;
import com.fqx.anyplay.service.APService;
import com.fqx.sang.download.DLEnum;
import com.umeng.analytics.MobclickAgent;
import com.umeng.update.UmengUpdateAgent;


import java.io.File;
import org.json.JSONException;
import org.json.JSONObject;

/**
* <p>功能描述: 图片显示UI页面.</p>
* 
* @author sangwencheng
* @version 1.0
*/
public class ShowImgView extends Activity {

	private static final int TASKAIRJOY = 2;
	private static final int TASKAJERR = 4;
	private static final int TASKAJERRNOID = 6;
	private static final int TASKAJERRTIMEOUT = 5;
	private static final int TASKERR = 3;
	private static final int TASKOK = 1;
	//private static final int TASKREAD = 1;
	//private static final int TASKSTART = 2;
	private static final int TASKSTOP = 0;
	private ImageView gMyImageView;
	
//	private float mInitRate = 0f;
	private float mInitRate = 1.0f;
	
	protected void onCreate(Bundle paramBundle) {
	    super.onCreate(paramBundle);
	    setContentView(R.layout.img_view);
	    
	    trySendReady();
	    this.mContext = this;
	    UmengUpdateAgent.update(this);
	    UmengUpdateAgent.setUpdateOnlyWifi(false);
	    MobclickAgent.onError(this);
	    AnyPlayUtils.send2Other(this);
	    startAPControllerService();
	    this.slidesShowsHandler = new Handler();
	    this.mLocalInfo = LocalInfo.getInstance(this);
	    this.mLocalInfo.initScreen(this);
	    this.gMyImageView = ((ImageView)findViewById(R.id.show_img_view));
	    this.mMyImageView = ((MyImageView)findViewById(R.id.show_my_img_view));
	
	    IntentFilter localIntentFilter1 = new IntentFilter();
	    localIntentFilter1.addAction(AnyPlayUtils.ACTION_SERVER_STATE);
	    registerReceiver(this.serStateReceiver, localIntentFilter1);
	
	    IntentFilter localIntentFilter2 = new IntentFilter();
	    localIntentFilter2.addAction("action.Anypush.ServerDownload.Result");
	    registerReceiver(this.imgDownloadResultEvt, localIntentFilter2);
	
	    verifyLeg();
	    this.mPublishState = PublishState.getInstance();
	    handleIntent(getIntent());
	    if ((AnyPlayUtils.isExistSDCard()) && (AnyPlayUtils.isEnoughSize(128L) == false)) {
		    	AnyPlayUtils.delAllFiles(this.mLocalInfo.getWholeCachePath());
	    }
	}
	
	private void trySendReady() {
	  Intent localIntent = new Intent();
	  localIntent.setAction(AnyPlayUtils.ACTION_HX_EXIT);
	  sendBroadcast(localIntent);
	}
	
	private BroadcastReceiver imgDownloadResultEvt = new BroadcastReceiver() {
	    public void onReceive(Context paramContext, Intent paramIntent) {
	      Bundle localBundle = paramIntent.getExtras();
	      int i = localBundle.getInt("ResultCMD");
	      if (i == DLEnum.DownloadResult.ResultCodeError.GetValue()) {
	        ShowImgView.this.stopProgressDialog();
	        if (localBundle.getInt("ErrState") == DLEnum.DownloadState.S_SD_FULL_ERR.GetValue()) {
	          AnyPlayUtils.showNote(ShowImgView.this.mContext, mContext.getResources().getString(R.string.img_msg_room));
	        }else{
	    	  AnyPlayUtils.showNote(ShowImgView.this.mContext, mContext.getResources().getString(R.string.img_msg_getfail));
	    	}
	     }else if (i == DLEnum.DownloadResult.ResultCodeStop.GetValue()) {
	      ShowImgView.this.stopProgressDialog();
	      AnyPlayUtils.showNote(ShowImgView.this.mContext, mContext.getResources().getString(R.string.img_msg_getfail));
	     }
	    }
	};
	
	private volatile boolean is_stop = false;
	private APService.MyBinder mAPServerBinder;
	private ServiceConnection mAPServiceConnection = new ServiceConnection() {
	    public void onServiceConnected(ComponentName paramComponentName, IBinder paramIBinder) {
	      ShowImgView.this.mAPServerBinder = ((APService.MyBinder)paramIBinder);
	    }
	
	    public void onServiceDisconnected(ComponentName paramComponentName) {
	    }
	};
	private Context mContext;
	private Handler mHandler = new Handler() {
	    public void handleMessage(Message paramMessage) {
	      ShowImgView.this.stopProgressDialog();
	      switch (paramMessage.what) {
		     case TASKAJERRTIMEOUT:
		    	 String str3 = (String)paramMessage.obj;
		    	 ShowImgView.this.showErr(str3, 5);
		    	  break;
		      case TASKAIRJOY:
		    	  String str2 = (String)paramMessage.obj;
		    	  ShowImgView.this.showImgByPath(str2);
		    	  break;
		      case TASKOK:
			      String str1 = (String)paramMessage.obj;
			      ShowImgView.this.showImg(str1);
		      case TASKSTOP:
		    	  break;
		      case TASKAJERR:
		    	  break;
		      case TASKERR:
		      case TASKAJERRNOID:
		    	  break;
		      default:
		    	  break;
		    }
	    }
	};
	
	private LocalInfo mLocalInfo;
	private MyImageView mMyImageView;
	private PublishState mPublishState;
	private SangNote mSangNote;
	
	private Handler mSendHandler = new Handler() {
	    public void handleMessage(Message paramMessage) {
	      switch (paramMessage.what) {
	      case 1:
		      ShowImgView.this.getSlidesShowsPic();
		      break;
	      default:
		      break;
	      }
	    }
	};
	
	private SangProgressDialog progressDialog = null;
	private BroadcastReceiver serStateReceiver = new BroadcastReceiver() {
	    public void onReceive(Context paramContext, Intent paramIntent) {
	      if (paramIntent.getExtras().getInt("State") != APPEnum.ServerState.S_NET_ERR.GetValue())
	        return;
	      ShowImgView.this.netErr();
	    }
	};
	
	private Handler slidesShowsHandler;
	private Runnable slidesShowsTasks = new Runnable() {
	    public void run() {
	      ShowImgView.this.sendSlidesMsg(1);
	    }
	};
	
	private void AJSendMsg(int paramInt, String paramString) {
	    Message localMessage = new Message();
	    localMessage.what = paramInt;
	    localMessage.obj = paramString;
	    this.mHandler.sendMessage(localMessage);
	}
	
	private Handler mImgAJHandler = new Handler();
	private int mImgAJTimes = 0;
	private String mImgAJID;
	private Runnable mImgAJRunnable = new Runnable() {
		@Override
		public void run() {
			mImgAJTimes ++;
			if(mImgAJTimes > 32) {
				AJSendMsg(TASKAJERRTIMEOUT, mImgAJID);
				mImgAJHandler.removeCallbacks(mImgAJRunnable);
				return;
			}
	        String str = LocalInfo.gAjPhotoManger.getImgPath(mImgAJID);
	        if (str == null) {
	            AJSendMsg(TASKERR, mImgAJID);
				mImgAJHandler.postDelayed(mImgAJRunnable, 1000);
	        }else if (str.equals("NOID")) {
	            AJSendMsg(TASKAJERRNOID, mImgAJID);
				mImgAJHandler.removeCallbacks(mImgAJRunnable);
	        }else{
	            AJSendMsg(TASKAIRJOY, mImgAJID);
				mImgAJHandler.removeCallbacks(mImgAJRunnable);
	        }
		}
	};
	
	private void AirJoyWaitTheard(String paramString) {
		mImgAJID = paramString;
		mImgAJHandler.removeCallbacks(mImgAJRunnable);
		mImgAJHandler.post(mImgAJRunnable);
	}
	
	private void MovePhoto(String paramString) {
	    try {
	      JSONObject localJSONObject = new JSONObject(paramString);
	      float f1 = (float)localJSONObject.getDouble("rateX");
	      float f2 = (float)localJSONObject.getDouble("rateY");
	      System.currentTimeMillis();
	      this.mMyImageView.translatePhoto(f1, f2);
	    } catch (JSONException localJSONException) {
	      localJSONException.printStackTrace();
	      Log.e("MovePhoto", "##############Err:");
	    }
	}
	
	private Handler mAPHandler = new Handler();
	private int mAPIMGTimes = 0;
	private String mAPIMGID = "";
	private Runnable mAPIMGRunnable = new Runnable() {
		@Override
		public void run() {
			if(mAPIMGTimes > 32) {
				AJSendMsg(TASKSTOP, mAPIMGID);
				mAPHandler.removeCallbacks(mAPIMGRunnable);
				return;
			}
	        if (LocalInfo.gAirImgCache.getImgData(mAPIMGID) != null) {
				AJSendMsg(TASKOK, mAPIMGID);
				mAPHandler.removeCallbacks(mAPIMGRunnable);
				return;
	        }
			mAPHandler.postDelayed(mAPIMGRunnable, 1000);
		}
	};
	
	private void WaitTheard(String paramString) {
	    startProgressDialog();
		mAPHandler.removeCallbacks(mAPIMGRunnable);
		mAPIMGID = paramString;
		mAPHandler.postDelayed(mAPIMGRunnable, 1000);
	}
	
	private void callbackEvt() {
	    this.mSangNote.dismiss();
	    startActivity(new Intent(this, AnyPlayHisense.class));

	    AnyPlayUtils.LOG_DEBUG("Exit 3");
	    finish();
	}
	
	private void exit() {
	    AnyPlayUtils.LOG_DEBUG("Exit 2");
	    finish();
	}
	
	private void getSlidesShowsPic() {
	    if (this.mAPServerBinder == null) {
	      Log.e("getSlidesShowsPic", "mAPController=null");
	      return;
	    }
	    String str = this.mPublishState.getClientIP();
	    AirPlayServer.getInstance().getSlideshowsPicture(str);
	}
	
	
	private void handleIntent(Intent paramIntent) {
	    Bundle localBundle = paramIntent.getExtras();
	    int j = localBundle.getInt("IMGCMD");
	    AnyPlayUtils.LOG_DEBUG("AirPlay CMD:" + j);
	    if (localBundle.getInt("IMGChannel") == APPEnum.AirChannel.AirPlay.GetValue()) {
	      if (j == APPEnum.AirImgCmd.didPutPhote.GetValue()) {
	        Statistics.addPhoto(this, APPEnum.AirChannel.AirPlay.GetValue());
	        showImg(AirImgCache.sg_img_data);
	      }else if (j == APPEnum.AirImgCmd.didDisplayCachedPhoto.GetValue()) {
	          Statistics.addPhoto(this, APPEnum.AirChannel.AirPlay.GetValue());
	          AnyPlayUtils.LOG_DEBUG("Stop");
	          showImg(localBundle.getString("IMGID"));
	      }else if (j == APPEnum.AirImgCmd.didStopPhotoOrSlideshow.GetValue()) {
	          AnyPlayUtils.LOG_DEBUG("Exit 5");
	          finish();
	      }else if (j == APPEnum.AirImgCmd.willPutPhoto.GetValue()) {
	          startProgressDialog();
	      }else {
	        tryHandleShiderPicture(j);
	      }
	    }else {
	        if (j == AJEnum.AirJoyPhotoCmd.displayPhoto.GetValue()) {
	          showImgByPath(localBundle.getString("IMGID"));
	          Statistics.addPhoto(this, APPEnum.AirChannel.AirJoy.GetValue());
	        }else if (j == AJEnum.AirJoyPhotoCmd.displayCachedPhoto.GetValue()) {
	          showImgByPath(localBundle.getString("IMGID"));
	          Statistics.addPhoto(this, APPEnum.AirChannel.AirJoy.GetValue());
	        }else if (j == AJEnum.AirJoyPhotoCmd.stopPhoto.GetValue()) {
		        AnyPlayUtils.LOG_DEBUG("--------------Stop");
		        finish();
		    }else if (j == AJEnum.AirJoyPhotoCmd.rotatePhoto.GetValue()) {
		    }else if (j == AJEnum.AirJoyPhotoCmd.zoomPhoto.GetValue()) {
		      float f3 = localBundle.getFloat("RATE");
		      String id = localBundle.getString("IMGID");
		    AnyPlayUtils.LOG_ERR("handleIntent", "zoomPhoto: " + "ID=" + id + " rate=" + f3);
		      if(id.equals("setInitRate")) {
		    	  mInitRate = f3;
		    	  return;
		      }
		      this.mMyImageView.zoomPhoto(f3);
		    }else if (j == AJEnum.AirJoyPhotoCmd.movePhoto.GetValue()) {
			    float f1 = localBundle.getFloat("RATE");
			    float f2 = localBundle.getFloat("RATEY");
			    this.mMyImageView.translatePhoto(f1, f2);
		    }
		 }
	}
	
	private void netErr() {
	    this.mSangNote = new SangNote(this);
	    this.mSangNote.setMsg(getResources().getString(R.string.net_err_msg));
	    this.mSangNote.setConfirmOnListener(new View.OnClickListener() {
	      public void onClick(View paramView) {
	        ShowImgView.this.callbackEvt();
	      }
	    });
	    this.mSangNote.setDelayCallbackEvent(AnyPlayUtils.NodeDelayTimes, new CallBackEvent() {
	      public void delayCallbackEvent() {
	        ShowImgView.this.callbackEvt();
	      }
	    });
	    this.mSangNote.show();
	}
	
	private void sendSlidesMsg(int paramInt) {
	    Message localMessage = new Message();
	    localMessage.what = paramInt;
	    this.mSendHandler.sendMessage(localMessage);
	}
	
	private void showErr(String paramString, int paramInt) {
	    switch (paramInt) {
		    case TASKAJERRNOID:
		    	AnyPlayUtils.showNote(this, paramString + "  =NULL");
		    	break;
		    case TASKAJERRTIMEOUT:
			    AnyPlayUtils.showNote(this, paramString + "  =TimeOut");
			    break;
		    default:
		    	break;
	    }
	}
	
	private void showImg(String paramString) {
	    this.mMyImageView.setVisibility(View.GONE);
	    this.gMyImageView.setVisibility(View.VISIBLE);
	    stopProgressDialog();
	    byte[] arrayOfByte = LocalInfo.gAirImgCache.getImgData(paramString);
	    if (arrayOfByte == null) {
	      this.gMyImageView.setImageResource(R.drawable.transparent);
	      WaitTheard(paramString);
	    }else{
		    Bitmap localBitmap = GetSampleBmp.creatbyByteArray(arrayOfByte, this.mLocalInfo.getScreenWidth(), this.mLocalInfo.getScreenHeight());
		    this.gMyImageView.setImageBitmap(localBitmap);
		    this.mMyImageView.zoomPhoto(0f);
	    }
	}
	
	private void showImg(byte[] paramArrayOfByte) {
	    this.mMyImageView.setVisibility(View.GONE);
	    this.gMyImageView.setVisibility(View.VISIBLE);
	    stopProgressDialog();
	    if (paramArrayOfByte == null) {
	      Log.e("showImg", "##############Err: showImg pdata=null");
	    }else{
		    Bitmap localBitmap = GetSampleBmp.creatbyByteArray(paramArrayOfByte, this.mLocalInfo.getScreenWidth(), this.mLocalInfo.getScreenHeight());
		    this.gMyImageView.setImageBitmap(localBitmap);
		    this.mMyImageView.zoomPhoto(0f);
	    }
	}
	
	private void showImgByPath(String paramString) {
	    int i = this.mLocalInfo.getScreenWidth();
	    int j = this.mLocalInfo.getScreenHeight();
	    this.gMyImageView.setVisibility(View.GONE);
	    this.mMyImageView.setVisibility(View.VISIBLE);
	    this.mMyImageView.setScreenSize(j, i);
	    String str = LocalInfo.gAjPhotoManger.getImgPath(paramString);
	    Bitmap localBitmap;
	    if (str == null) {
	      Log.e("showImgByPath", "##############Err: showImg pdata=null");
	      this.mMyImageView.setImageResource(R.drawable.transparent);
	      AirJoyWaitTheard(paramString);
	    }else if (str.equals("NOID")) {
	        showErr(paramString, 6);
	    }else {
	      localBitmap = GetSampleBmp.creat(new File(str), i, j);
	      if(localBitmap != null) {
		    this.mMyImageView.setImageBitmap(localBitmap);
		    this.mMyImageView.zoomPhoto(mInitRate);
		    AnyPlayUtils.LOG_ERR("showImgByPath", "showImgByPath: rate=" + mInitRate);
		    mMyImageView.setAlpha(60);    
		    intCounter = 12;
		    mImgHandler.removeCallbacks(fadeInTask);
		    mImgHandler.post(fadeInTask);  
	      }
	    }
	}
	
	private int intCounter = 12;  
    private Handler mImgHandler = new Handler();  
	 /* 设定ImageView的透明度渐显出来 */  
    private Runnable fadeInTask = new Runnable() {  
        public void run() {  
            intCounter = intCounter + 1;  
            mMyImageView.setAlpha(intCounter * 5);  
            if(intCounter < 51) {
	            mImgHandler.postDelayed(fadeInTask, 15);  
            }
        }  
    };  
  
//    /* 设定ImageView的透明度渐暗下去 */  
//    private Runnable fadeOutTask = new Runnable() {  
//        public void run() {  
//            intCounter = intCounter - 1;  
//            mMyImageView.setAlpha(intCounter * 15);  
//            if(intCounter == 0)mHandler.postDelayed(fadeInTask, 50);  
//            else mImgHandler.postDelayed(fadeOutTask, 50);  
//        }  
//    };  
//	
	
	
	private void startAPControllerService() {
	    Intent localIntent = new Intent(this, APService.class);
	    startService(localIntent);
	    bindService(localIntent, this.mAPServiceConnection, 1);
	}
	
	private void startProgressDialog() {
	    if (this.progressDialog == null) {
	      this.progressDialog = SangProgressDialog.createDialog(this);
		  this.progressDialog.setOnCannel(new OnCancelListener() {
				@Override
				public void onCancel(DialogInterface dialog) {
					exit();
				}
			});
	    }
	    this.progressDialog.show();
	}
	
	private void stopProgressDialog() {
	    if (this.progressDialog == null)
	      return;
	    this.progressDialog.dismiss();
	    this.progressDialog = null;
	}
	
	
	private void tryHandleShiderPicture(int paramInt) {
	    if (paramInt == APPEnum.AirImgCmd.didSlidePictureStart.GetValue()) {
	      this.mPublishState.setSlidesShowMode(true);
	      this.slidesShowsHandler.post(this.slidesShowsTasks);
	    }
	    
	    if(this.mPublishState.iSlidesShowMode() == false) {
		    this.mPublishState.setSlidesShowMode(false);
		    AnyPlayUtils.LOG_DEBUG("Exit 4");
		    finish();
	    }
	
	    if (paramInt == APPEnum.AirImgCmd.didGetSlideshowsPicture.GetValue()) {
	        this.slidesShowsHandler.postDelayed(this.slidesShowsTasks, 2000L);
	        showImg(AirImgCache.sg_img_data);
	    }else if (paramInt == APPEnum.AirImgCmd.didSlidePictureStop.GetValue()) {
	        this.slidesShowsHandler.removeCallbacks(this.slidesShowsTasks);
		    this.mPublishState.setSlidesShowMode(false);
	    }else if (paramInt == APPEnum.AirImgCmd.didSlidePictureOver.GetValue()) {
	      this.slidesShowsHandler.removeCallbacks(this.slidesShowsTasks);
	    }
	}
	
	private boolean verifyLeg() {
	    VerLeg localVerLeg = new VerLeg(this);
	    return localVerLeg.showTVMark();
	}
	
	
	protected void onDestroy() {
	    super.onDestroy();
	    this.slidesShowsHandler.removeCallbacks(this.slidesShowsTasks);
	    this.mPublishState.setSlidesShowMode(false);
	    unregisterReceiver(this.serStateReceiver);
	    unregisterReceiver(this.imgDownloadResultEvt);
	    stopProgressDialog();
	    if (this.mAPServerBinder != null)
	      unbindService(this.mAPServiceConnection);
	    Log.d("AnyPlay", "ShowImgView quit.");
	}
	
	public boolean onKeyDown(int paramInt, KeyEvent paramKeyEvent) {
	    Log.d("AnyPlayHS", "-------------------------------------------- Key = " + paramInt);
	    if (paramInt == KeyEvent.KEYCODE_BACK) {
	      this.slidesShowsHandler.removeCallbacks(this.slidesShowsTasks);
	      this.mPublishState.setSlidesShowMode(false);
	      this.mPublishState.SetMediaSlideshow(APPEnum.EventState.EventStateStopped.GetValue());
	      this.mPublishState.SetMediaPhotoState(APPEnum.EventState.EventStateStopped.GetValue());
	      AnyPlayUtils.LOG_DEBUG("Exit 1");
	      finish();
	      return true;
	    }
	    return super.onKeyDown(paramInt, paramKeyEvent);
	}
	
	protected void onNewIntent(Intent paramIntent) {
	    super.onNewIntent(paramIntent);
	    handleIntent(paramIntent);
	}
	
	protected void onPause() {
	    super.onPause();
	    MobclickAgent.onPause(this);
	}
	
	protected void onResume() {
	    super.onResume();
	    MobclickAgent.onResume(this);
	}
}
