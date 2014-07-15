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
import android.content.res.Configuration;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.util.Log;
import android.view.GestureDetector;
import android.view.KeyEvent;
import android.view.View;
import android.widget.ImageView;
import com.fqx.airjoy.callback.OnCheckNetEvent;
import com.fqx.anyplay.api.APPEnum;
import com.fqx.anyplay.api.AnyPlayUtils;
import com.fqx.anyplay.api.CallBackEvent;
import com.fqx.anyplay.api.CheckVideoNetState;
import com.fqx.anyplay.api.LocalInfo;
import com.fqx.anyplay.api.PublishState;
import com.fqx.anyplay.api.SangProgressDialog;
import com.fqx.anyplay.api.Statistics;
import com.fqx.anyplay.api.VerLeg;
import com.fqx.anyplay.service.APController;
import com.fqx.anyplay.service.APService;
import com.fqx.anyplay.svideo.SMediaController;
import com.fqx.anyplay.svideo.SVideoView;
import com.fqx.sang.video.LibsChecker;
import com.umeng.analytics.MobclickAgent;
import com.umeng.update.UmengUpdateAgent;

public class LocalPlayer extends Activity implements 
					MediaPlayer.OnCompletionListener, 
					MediaPlayer.OnSeekCompleteListener, 
					MediaPlayer.OnBufferingUpdateListener, 
					MediaPlayer.OnErrorListener, 
					MediaPlayer.OnInfoListener, 
					MediaPlayer.OnPreparedListener {

	private static final int TASKDO = 4;
	private static final int TASKERR = 3;
	private boolean g_seek_doing = false;
	private APController mAPController;
	private APService.MyBinder mAPServerBinder;
	private AudioManager mAudioManager;
	private float mBrightness = -1.0F;
	private CheckVideoNetState mCheckVideoNetState;
	private String mDevErrString;
	private GestureDetector mGestureDetector;
	private LocalInfo mLocalInfo;
	private int mMaxVolume;
	private String mNetErrString;
	private ImageView mOperationBg;
	private ImageView mOperationPercent;
	private String mPath;
	private PublishState mPublishState;
	private SMediaController mSMediaController;
	private SangNote mSangNote;
	private long mSeektoValue;
	private int mStartPosition;
	private SVideoView mVideoView;
	private int mVolume = -1;
	private View mVolumeBrightnessLayout;
	private int mch;
	private Handler mtimeHandler = new Handler();
	private SangProgressDialog progressDialog = null;
	
	public void onCreate(Bundle paramBundle) {
	    super.onCreate(paramBundle);
	    setContentView(R.layout.svideoview);
	    UmengUpdateAgent.update(this);
	    UmengUpdateAgent.setUpdateOnlyWifi(false);
	    MobclickAgent.onError(this);
	    AnyPlayUtils.send2Other(this);
	    this.mLocalInfo = LocalInfo.getInstance(this);
	    startAPControllerService();
	    if (this.mLocalInfo.g_IsLocalPlayer_m3u8 == false) {
		    if (!LibsChecker.checkVitamioLibs(this, R.string.init_decoders))
		      return;
	    }
	    Intent localIntent;
	    localIntent = getIntent();
	    if(localIntent == null) {
	    	return;
	    }
	
	    mSangNote = null;
	    this.mNetErrString = getResources().getString(R.string.net_err_msg);
	    this.mDevErrString = getResources().getString(R.string.dev_net_err_msg);
	    this.mPublishState = PublishState.getInstance();
	    this.mVideoView = ((SVideoView)findViewById(R.id.surface_view));
	    this.mVolumeBrightnessLayout = findViewById(R.id.operation_volume_brightness);
	    this.mOperationBg = ((ImageView)findViewById(R.id.operation_bg));
	    this.mOperationPercent = ((ImageView)findViewById(R.id.operation_percent));
	    this.mSMediaController = ((SMediaController)findViewById(R.id.s_media_contorller));
	    this.mAudioManager = ((AudioManager)getSystemService("audio"));
	    this.mMaxVolume = this.mAudioManager.getStreamMaxVolume(3);
	    this.mCheckVideoNetState = new CheckVideoNetState();
//	    this.mGestureDetector = new GestureDetector(this, new MyGestureListener());
	    setRequestedOrientation(0);
	    LocalInfo.APVideoisRuning = true;
	    IntentFilter localIntentFilter = new IntentFilter();
	    localIntentFilter.addAction(AnyPlayUtils.ACTION_SERVER_STATE);
	    registerReceiver(this.serStateReceiver, localIntentFilter);
	    verifyLeg();
	    handleIntent(localIntent);
	}
	
	private Runnable atimeRunnable = new Runnable() {
	    public void run() {
	      Message localMessage = new Message();
	      localMessage.what = TASKDO;
	      mSendHandler.sendMessage(localMessage);
	      mtimeHandler.postDelayed(atimeRunnable, 1000);
	    }
	};
	
	private ServiceConnection mAPServiceConnection = new ServiceConnection() {
	    public void onServiceConnected(ComponentName paramComponentName, IBinder paramIBinder) {
	      LocalPlayer.this.mAPServerBinder = ((APService.MyBinder)paramIBinder);
	      LocalPlayer.this.mAPController = LocalPlayer.this.mAPServerBinder.getService();
	    }
	
	    public void onServiceDisconnected(ComponentName paramComponentName) {
	    }
	};
	private Handler mDismissHandler = new Handler() {
	    public void handleMessage(Message paramMessage) {
	      LocalPlayer.this.mVolumeBrightnessLayout.setVisibility(8);
	    }
	  };
	private OnCheckNetEvent mOnCheckNetEvent = new OnCheckNetEvent() {
	    public void doEvt(int paramInt) {
	      if (paramInt == CheckVideoNetState.NetCode.Buff.GetValue()) {
	        startProgressDialog();
	      }else if (paramInt == CheckVideoNetState.NetCode.OK.GetValue()) {
	        stopProgressDialog();
	      }else if(paramInt == CheckVideoNetState.NetCode.Err.GetValue()) {
	    	mVideoView.pause();
	    	stopProgressDialog();
	    	AnyPlayUtils.LOG_ERR("mOnCheckNetEvent", "showSangNode");
	    	showSangNode(mDevErrString);
		  }
	    }
	};
	
	
	private Handler mSendHandler = new Handler() {
	    public void handleMessage(Message paramMessage) {
	      switch (paramMessage.what) {
	      case TASKDO:
	    	  sendVideoProcess();
		      break;
	      case TASKERR:
		      SendTheard();
		      break;
	      default:
	        return;
	      }
	    }
	};
	
	private BroadcastReceiver serStateReceiver = new BroadcastReceiver() {
	    public void onReceive(Context paramContext, Intent paramIntent) {
	      if (paramIntent.getExtras().getInt("State") == APPEnum.ServerState.S_NET_ERR.GetValue()) {
		    	AnyPlayUtils.LOG_ERR("serStateReceiver", "showSangNode");
		      LocalPlayer.this.showSangNode(LocalPlayer.this.mNetErrString);
	      }
	    }
	};
	
	private void SendTheard() {
	    this.atimeRunnable.run();
	}
	
	private void StartSeekTo(long paramLong) {
	    long l = this.mVideoView.getDuration();
	    int i = (int)(paramLong * (l / 1000L));
	    this.mSMediaController.setDurTime(l);
	    this.mSMediaController.setCurProgress((int)paramLong);
	    AnyPlayUtils.LOG_ERR("playSeekto", "---------------------- POS0:" + i);
	    this.mVideoView.seekTo(i);
	    startProgressDialog();
	    this.mSeektoValue = paramLong;
	    this.g_seek_doing = true;
	}
	
	private void callbackEvt() {
	    this.mSangNote.dismiss();
	    this.mSangNote = null;
		Intent localIntent;
	    localIntent = new Intent(this, AnyPlayHisense.class); 
	    startActivity(localIntent);
	    AnyPlayUtils.LOG_DEBUG("callbackEvt");
	    this.mPublishState.SetMediaVideo(APPEnum.EventState.EventStateStopped.GetValue());
	    exit();
	}
	
	
	private void exit() {
		if(mSangNote != null) {
			mSangNote.dismiss();
		}
	    AnyPlayUtils.is_anyplay = false;
	    LocalInfo.APVideoisRuning = false;
	    this.mtimeHandler.removeCallbacks(this.atimeRunnable);
		if (this.mVideoView != null) { 
			this.mVideoView.stopPlayback();
		}
	    finish();
	}
	
	private void handleIntent(Intent paramIntent) {
	    Bundle localBundle = paramIntent.getExtras();
	    int i = localBundle.getInt("VideoCmd");
	    if (i == APPEnum.AirVideoCmd.didStartPlayVideo.GetValue()) {
	      this.mPath = localBundle.getString("UriString");
	      this.mStartPosition = localBundle.getInt("StartPositon");
	      this.mch = localBundle.getInt("AirChannel");
	      playVideo(this.mPath, this.mStartPosition);
	      LocalInfo.APVideoisRuning = true;
	      Statistics.addVideo(this, this.mPath, this.mch);
	    }else if (i == APPEnum.AirVideoCmd.didCreateEventSession.GetValue()) {
	        LocalInfo.gVideoSessionID = localBundle.getInt("EvrntSessionId");
	        AnyPlayUtils.LOG_DEBUG("MainActivity", "gVideoSessionID:" + LocalInfo.gVideoSessionID);
	    }else if (i == APPEnum.AirVideoCmd.didSetPlaybackRate.GetValue()) {
	        showMediaController(3000);
	        if (localBundle.getLong("Rate") == 0L) {
	          this.mVideoView.pause();
	          this.mCheckVideoNetState.stop();
		      AnyPlayUtils.LOG_DEBUG("handleIntent", "Video State: pause");
	        }else {
	          this.mVideoView.start();
	          this.mCheckVideoNetState.start();
		      AnyPlayUtils.LOG_DEBUG("handleIntent", "Video State: start");
	        }
	    }else if (i == APPEnum.AirVideoCmd.setCurrentPlaybackProgress.GetValue()) {
	        playSeekto(localBundle.getLong("PlayPosition"));
	    }else if (i == APPEnum.AirVideoCmd.didStopPlayback.GetValue()) {
	      this.mCheckVideoNetState.stop();
	      exit();
	      AnyPlayUtils.LOG_DEBUG("handleIntent", "#s#Stop ");
	    }else if (i == APPEnum.AirVideoCmd.didSetVolume.GetValue()) {
	    }else if(i== APPEnum.AirVideoCmd.didLifeVale.GetValue()) {
	    }
	}
	
//	private void onBrightnessSlide(float percent) {
//		if (mBrightness < 0) {
//			mBrightness = getWindow().getAttributes().screenBrightness;
//			if (mBrightness <= 0.00f)
//				mBrightness = 0.50f;
//			if (mBrightness < 0.01f)
//				mBrightness = 0.01f;
//	
//			// 显示
//			mOperationBg.setImageResource(R.drawable.video_brightness_bg);
//			mVolumeBrightnessLayout.setVisibility(View.VISIBLE);
//		}
//		WindowManager.LayoutParams lpa = getWindow().getAttributes();
//		lpa.screenBrightness = mBrightness + percent;
//		if (lpa.screenBrightness > 1.0f)
//			lpa.screenBrightness = 1.0f;
//		else if (lpa.screenBrightness < 0.01f)
//			lpa.screenBrightness = 0.01f;
//		getWindow().setAttributes(lpa);
//	
//		ViewGroup.LayoutParams lp = mOperationPercent.getLayoutParams();
//		lp.width = (int) (findViewById(R.id.operation_full).getLayoutParams().width * lpa.screenBrightness);
//		mOperationPercent.setLayoutParams(lp);
//	}
//	
	
//	private void onVolumeSlide(float percent) {
//		if (mVolume == -1) {
//			mVolume = mAudioManager.getStreamVolume(AudioManager.STREAM_MUSIC);
//			if (mVolume < 0)
//				mVolume = 0;
//	
//			mOperationBg.setImageResource(R.drawable.video_volumn_bg);
//			mVolumeBrightnessLayout.setVisibility(View.VISIBLE);
//		}
//	
//		int index = (int) (percent * mMaxVolume) + mVolume;
//		if (index > mMaxVolume)
//			index = mMaxVolume;
//		else if (index < 0)
//			index = 0;
//	
//		mAudioManager.setStreamVolume(AudioManager.STREAM_MUSIC, index, 0);
//	
//		ViewGroup.LayoutParams lp = mOperationPercent.getLayoutParams();
//		lp.width = findViewById(R.id.operation_full).getLayoutParams().width * index / mMaxVolume;
//		mOperationPercent.setLayoutParams(lp);
//	}
	
//	private void StartSeekTo(long paramLong) {
//	    long l = this.mVideoView.getDuration();
//	    int i = (int)(paramLong * (l / 1000L));
//	    this.mSMediaController.setDurTime(l);
//	    this.mSMediaController.setCurProgress((int)paramLong);
//	    this.mVideoView.seekTo(i);
//	    startProgressDialog();
//	    this.mSeektoValue = paramLong;
//	    this.g_seek_doing = true;
//	}

	private void playSeekto(long paramLong) {
	    startProgressDialog();
	    int i = (int)(1000 * paramLong);
	    AnyPlayUtils.LOG_ERR("playSeekto", "---------------------- POS:" + i);
	    this.mVideoView.seekTo(i);
	    this.mSMediaController.setCurProgress((int)paramLong);
	    showMediaController(30000);
	    this.mSeektoValue = paramLong;
	    this.g_seek_doing = true;
	}
	
	private void playVideo(String paramString, long paramLong) {
	    startProgressDialog();
	    this.g_seek_doing = false;
	    if (paramString.startsWith("http")) {
	      this.mVideoView.setVideoURI(Uri.parse(paramString));
	    }else{
	      this.mVideoView.setVideoPath(paramString);
	    }
	    AnyPlayUtils.LOG_ERR("playVideo", "--------------------- URL:" + paramString);
	    this.mVideoView.setOnCompletionListener(this);
	    this.mVideoView.setOnPreparedListener(this);
	    this.mVideoView.setOnSeekCompleteListener(this);
	    this.mVideoView.setOnBufferingUpdateListener(this);
	    this.mVideoView.setOnInfoListener(this);
	    this.mVideoView.setOnErrorListener(this);
	    this.mVideoView.start();
	    this.mCheckVideoNetState.init();
	    this.mCheckVideoNetState.setOnCheckNetEvent(this.mOnCheckNetEvent);
	    this.mSMediaController.setCurProgress((int)paramLong);
	}
	
	private void sendVideoProcess() {
	    LocalInfo.APVideoisRuning = true;
	    Intent localIntent = new Intent();
	    Bundle localBundle = new Bundle();
	    long l1 = this.mVideoView.getDuration();
	    long l2 = this.mVideoView.getCurrentPosition();
	    int i;
	    if (this.g_seek_doing) {
	      i = (int)this.mSeektoValue;
	    } else {
	       i = (int)(l2 / 1000L);
	    }
	    localBundle.putInt("Dur", (int)(l1 / 1000L));
	    localBundle.putInt("Pos", i);
	    localIntent.setAction("anyplay.service.videoProcess");
	    localIntent.putExtras(localBundle);
	    sendBroadcast(localIntent);
	    try {
	        this.mCheckVideoNetState.checkNetState(l2);
	        this.mSMediaController.setDurTime(l1);
	        this.mSMediaController.setCurTime(l2);
	    } catch (Exception localException) {
	        localException.printStackTrace();
	    }
//	    AnyPlayApi.LOG_ERR("sendVideoProcess", "Cur=" + (l1 / 1000L)  + " POS:" + i);
	}
	
	private void setProgressValue(int paramInt) {
	    if (this.progressDialog == null)
	      return;
	    if (paramInt < 0)
	      paramInt = 0;
	    this.progressDialog.setProgress(paramInt);
	}
	
	private void showMediaController(int paramInt) {
	    this.mSMediaController.show(paramInt);
	}
	
	private void showSangNode(String paramString) {
		if(mSangNote != null) {
			mSangNote.dismiss();
		}
	    this.mSangNote = new SangNote(this);
	    this.mSangNote.setMsg(paramString);
	    this.mSangNote.setConfirmOnListener(new View.OnClickListener() {
	      public void onClick(View paramView) {
	        LocalPlayer.this.callbackEvt();
	      }
	    });
	    this.mSangNote.setDelayCallbackEvent(AnyPlayUtils.NodeDelayTimes, new CallBackEvent() {
	      public void delayCallbackEvent() {
	        LocalPlayer.this.callbackEvt();
	      }
	    });
	    this.mSangNote.show();
	}
	
	private void startAPControllerService() {
	    Intent localIntent = new Intent(this, APService.class);
	    startService(localIntent);
	    bindService(localIntent, this.mAPServiceConnection, 1);
	}
	
	private void startProgressDialog() {
	    try {
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
	
	private void tryPlay(String paramString, int paramInt) {
		if(LocalInfo.APVideoisRuning == false) {
		    AnyPlayUtils.LOG_ERR("LocalPlayer", "exit, tryPlay fail");
			return;
		}
		if (this.mLocalInfo.g_IsLocalPlayer_m3u8) {
		    AnyPlayUtils.LOG_ERR("LocalPlayer", "exit, tryPlay fail");
			return;
	    }
	    AnyPlayUtils.LOG_ERR("LocalPlayer", "TryPlay to APVideo");
	    Intent localIntent = new Intent(this, APVideo.class);
	    Bundle localBundle = new Bundle();
	    localBundle.putInt("VideoCmd", APPEnum.AirVideoCmd.didTryPlay.GetValue());
	    localBundle.putString("UriString", paramString);
	    localBundle.putInt("StartPositon", paramInt);
	    localBundle.putInt("AirChannel", this.mch);
	    localIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
	    localIntent.putExtras(localBundle);
	    startActivity(localIntent);
	    this.mtimeHandler.removeCallbacks(this.atimeRunnable);
	    finish();
	}
	
	private boolean verifyLeg() {
	    VerLeg localVerLeg = new VerLeg(this);
	    return localVerLeg.showTVMark();
	}
	
	public void onBufferingUpdate(MediaPlayer paramMediaPlayer, int paramInt) {
	    setProgressValue(paramInt);
	}
	
	public void onCompletion(MediaPlayer paramMediaPlayer) {
	    AnyPlayUtils.LOG_DEBUG("onCompetion");
	    exit();
	}
	
	public void onConfigurationChanged(Configuration paramConfiguration) {
	    if (this.mVideoView == null)
	      return;
	    super.onConfigurationChanged(paramConfiguration);
	}
	
	protected void onDestroy() {
	    super.onDestroy();
	    AnyPlayUtils.LOG_DEBUG("LocalPlayer", "exit");
	    try {

		    unregisterReceiver(this.serStateReceiver);
		    if (this.mAPController != null) {
		    	unbindService(this.mAPServiceConnection);
		    }
		} catch (Exception e) {
			e.printStackTrace();
		}

	}
	
	private void process_video_err(int what, int extra) {
		switch (what) {
			case -110:
				return;
			case -1007:
		    	showSangNode(mDevErrString);
				return;
			case -1004:
		    	showSangNode(mNetErrString);
				return;
			case 1:
				if(extra == -8006) {
					exit();
					return;
				}else if(extra == -8007) {
					showSangNode(mNetErrString);
					return;
				}
			default:
				break;
		}
//	    tryPlay(this.mPath, this.mStartPosition);
		showSangNode(mNetErrString);
	}

	public boolean onError(MediaPlayer paramMediaPlayer, int paramInt1, int paramInt2) {
	    String str = "OnERR arg1=" + paramInt1 + "  arg2=" + paramInt2;
	    Log.e("LocalPlayer", "Err:" + str);
	    process_video_err(paramInt1, paramInt2);
	    return true;
	}
	
	public boolean onInfo(MediaPlayer paramMediaPlayer, int what, int extra) {
		if(what == MediaPlayer.MEDIA_INFO_BUFFERING_START) {
			startProgressDialog();
		}else if(what == MediaPlayer.MEDIA_INFO_BUFFERING_END) {
			stopProgressDialog();
		}

	    return false;
	}
	
	public boolean onKeyDown(int paramInt, KeyEvent paramKeyEvent) {
	    if (paramInt == KeyEvent.KEYCODE_BACK) {
	      AnyPlayUtils.LOG_DEBUG("LocalVideo", "onKeyDown");
	      this.mPublishState.SetMediaVideo(APPEnum.EventState.EventStateStopped.GetValue());
	      exit();
	    }
	    return super.onKeyDown(paramInt, paramKeyEvent);
	}
	
	protected void onNewIntent(Intent paramIntent) {
	    super.onNewIntent(paramIntent);
	    handleIntent(paramIntent);
	}
	
	protected void onPause() {
	    super.onPause();
	    if (this.mVideoView != null)
	      this.mVideoView.pause();
	    MobclickAgent.onPause(this);
	}
	
	public void onPrepared(MediaPlayer paramMediaPlayer) {
	    AnyPlayUtils.LOG_DEBUG("LocalVideo", "onPrepared total=" + paramMediaPlayer.getDuration());
	    this.mPublishState.SetMediaVideo(APPEnum.EventState.EventStatePlaying.GetValue());
	    if (this.mStartPosition > 0) {
	    	StartSeekTo(this.mStartPosition);
		}else{
		    stopProgressDialog();
		}
	    SendTheard();
//	    paramMediaPlayer.start();
	    this.mCheckVideoNetState.start();
	}
	
	protected void onResume() {
	    super.onResume();
	    if (this.mVideoView != null)
	      this.mVideoView.resume();
	    MobclickAgent.onResume(this);
	}
	
	public void onSeekComplete(MediaPlayer paramMediaPlayer) {
	    this.mPublishState.SetMediaVideo(APPEnum.EventState.EventStatePlaying.GetValue());
	    AnyPlayUtils.LOG_DEBUG("LocalVideo", "onSeekComplete");
	    stopProgressDialog();
	    this.g_seek_doing = false;
	    paramMediaPlayer.start();
	    showMediaController(3000);
	}
	
	protected void onStop() {
	    super.onStop();
	    AnyPlayUtils.LOG_DEBUG("LocalVideo", "onStop---------------------------------");
	    this.mCheckVideoNetState.stop();
//	    AnyPlayApi.is_anyplay = false;
//	    LocalInfo.APVideoisRuning = false;
//	    exit();
	}
	
//	@Override
//	public boolean onTouchEvent(MotionEvent event) {
//		if (mGestureDetector.onTouchEvent(event))
//			return true;
//	
//		switch (event.getAction() & MotionEvent.ACTION_MASK) {
//		case MotionEvent.ACTION_UP:
//			endGesture();
//			break;
//		}
//	
//		return super.onTouchEvent(event);
//	}
//	
//	private void endGesture() {
//		mVolume = -1;
//		mBrightness = -1f;
//	
//		mDismissHandler.removeMessages(0);
//		mDismissHandler.sendEmptyMessageDelayed(0, 500);
//	}
	
//	private int mLayout;
//	private class MyGestureListener extends SimpleOnGestureListener {
//		@Override
//		public boolean onDoubleTap(MotionEvent e) {
//			if (mLayout == VideoView.VIDEO_LAYOUT_ZOOM)
//				mLayout = VideoView.VIDEO_LAYOUT_ORIGIN;
//			else
//				mLayout++;
//	//		if (mVideoView != null)
//	//			mVideoView.setVideoLayout(mLayout, 0);
//			return true;
//		}
//	
//		@Override
//		public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
//			float mOldX = e1.getX(), mOldY = e1.getY();
//			int y = (int) e2.getRawY();
//			Display disp = getWindowManager().getDefaultDisplay();
//			int windowWidth = disp.getWidth();
//			int windowHeight = disp.getHeight();
//	
//			if (mOldX > windowWidth * 4.0 / 5)// 右边滑动
//				onVolumeSlide((mOldY - y) / windowHeight);
//			else if (mOldX < windowWidth / 5.0)// 左边滑动
//				onBrightnessSlide((mOldY - y) / windowHeight);
//	
//			return super.onScroll(e1, e2, distanceX, distanceY);
//		}
//	}
}