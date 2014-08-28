package cn.airjoy.android.service;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.res.Configuration;
import android.media.AudioManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.view.Display;
import android.view.GestureDetector;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.ImageView;
import com.fqx.anyplay.api.APPEnum;
import com.fqx.anyplay.api.AnyPlayUtils;
import com.fqx.anyplay.api.CallBackEvent;
import com.fqx.anyplay.api.LocalInfo;
import com.fqx.anyplay.api.PublishState;
import com.fqx.anyplay.api.SangProgressDialog;
import com.fqx.anyplay.api.Statistics;
import com.fqx.anyplay.api.VerLeg;
import com.fqx.anyplay.service.APService;
import com.fqx.anyplay.service.QiyiStop;
import com.fqx.sang.video.LibsChecker;
import com.fqx.sang.video.MediaController;
import com.fqx.sang.video.VideoView;
import com.umeng.analytics.MobclickAgent;
import com.umeng.update.UmengUpdateAgent;
import io.vov.vitamio.MediaPlayer;

/**
* <p>描述: 利用第三方视频库（vitamio）实现软解码服务，目前版本没有调用. </p>
* 
* @author sangwencheng
* @version 1.0
*/
public class APVideo extends Activity implements 
					MediaPlayer.OnCompletionListener, 
					MediaPlayer.OnSeekCompleteListener, 
					MediaPlayer.OnBufferingUpdateListener, 
					MediaPlayer.OnErrorListener, 
					MediaPlayer.OnInfoListener, 
					MediaPlayer.OnPreparedListener {

	private static final int TASKDO = 4;
	private static final int TASKERR = 3;
	public static MediaController sgMediaController;
	
	private boolean g_seek_doing = false;
	private APService.MyBinder mAPServerBinder;
	private AudioManager mAudioManager;
	private float mBrightness = -1.0F;
	private String mDevErrString;
	private GestureDetector mGestureDetector;
	private int mLayout = VideoView.VIDEO_LAYOUT_SCALE;
	private int mLayout_scale = 1;
	private LocalInfo mLocalInfo;
	private int mMaxVolume;
	private String mNetErrString;
	private ImageView mOperationBg;
	private ImageView mOperationPercent;
	private String mPath;
	private String mPlayerErrString;
	private PublishState mPublishState;
	private SangNote mSangNote;
	private long mSeektoValue;
	private int mStartPosition;
	private String mTitle;
	private VideoView mVideoView;
	private int mVolume = -1;
	private View mVolumeBrightnessLayout;
	private Handler mtimeHandler = new Handler();
	private SangProgressDialog progressDialog = null;
	private QiyiStop mQiyiStop;

	
	public void onCreate(Bundle paramBundle) {
	    super.onCreate(paramBundle);
	    setContentView(R.layout.videoview);
	    UmengUpdateAgent.update(this);
	    UmengUpdateAgent.setUpdateOnlyWifi(false);
	    MobclickAgent.onError(this);
	    AnyPlayUtils.send2Other(this);
	    startAPControllerService();
	    if (!LibsChecker.checkVitamioLibs(this, R.string.init_decoders)) {
	    	return;
	    }
	
	    mQiyiStop = new QiyiStop();
	    Intent localIntent;
	    localIntent = getIntent();
	    if(localIntent == null) {
	    	return;
	    }
	    this.mLocalInfo = LocalInfo.getInstance(this);
	    this.mNetErrString = getResources().getString(R.string.net_err_msg);
	    this.mDevErrString = getResources().getString(R.string.dev_net_err_msg);
	    this.mPlayerErrString = getResources().getString(R.string.player_err_msg);
	    this.mPublishState = PublishState.getInstance();
	    this.mVideoView = ((VideoView)findViewById(R.id.surface_view));
	    this.mVolumeBrightnessLayout = findViewById(R.id.operation_volume_brightness);
	    this.mOperationBg = ((ImageView)findViewById(R.id.operation_bg));
	    this.mOperationPercent = ((ImageView)findViewById(R.id.operation_percent));
	    this.mAudioManager = ((AudioManager)getSystemService("audio"));
	    this.mMaxVolume = this.mAudioManager.getStreamMaxVolume(3);
//	    this.mGestureDetector = new GestureDetector(this, new MyGestureListener());
	    setRequestedOrientation(0);
	    LocalInfo.APVideoisRuning = true;
	    IntentFilter localIntentFilter = new IntentFilter();
	    localIntentFilter.addAction(AnyPlayUtils.ACTION_SERVER_STATE);
	    registerReceiver(this.serStateReceiver, localIntentFilter);
	    verifyLeg();
	    this.progressDialog = null;
	    handleIntent(localIntent);
	}
	
	private Runnable atimeRunnable = new Runnable() {
	    public void run() {
	      Message localMessage = new Message();
	      localMessage.what = 4;
	      APVideo.this.mSendHandler.sendMessage(localMessage);
	      APVideo.this.mtimeHandler.postDelayed(this, 1000L);
	    }
	};
	
	private ServiceConnection mAPServiceConnection = new ServiceConnection() {
	    public void onServiceConnected(ComponentName paramComponentName, IBinder paramIBinder) {
	      APVideo.this.mAPServerBinder = ((APService.MyBinder)paramIBinder);
	    }
	
	    public void onServiceDisconnected(ComponentName paramComponentName)
	    {
	    }
	};
	private Handler mDismissHandler = new Handler() {
	    public void handleMessage(Message paramMessage) {
	      APVideo.this.mVolumeBrightnessLayout.setVisibility(View.GONE);
	    }
	};
	
	private Handler mSendHandler = new Handler() {
	    public void handleMessage(Message paramMessage) {
	      switch (paramMessage.what) {
		      case 4:
		        APVideo.this.sendVideoProcess();
		        break;
		      case 3:
			    APVideo.this.SendTheard();
		        break;
		      default:
		        break;
	      }
	    }
	};
	

	private BroadcastReceiver serStateReceiver = new BroadcastReceiver() {
	    public void onReceive(Context paramContext, Intent paramIntent) {
	      if (paramIntent.getExtras().getInt("State") != APPEnum.ServerState.S_NET_ERR.GetValue())
	        return;
	      APVideo.this.showSangNode(APVideo.this.mNetErrString);
	    }
	};
	
	private void SendTheard() {
	    this.atimeRunnable.run();
	}
	
	private void StartSeekTo(long paramLong) {
	    long l = paramLong * (this.mVideoView.getDuration() / 1000L);
	    this.mVideoView.seekTo(l);
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
	    exit();
	}
	
	private void clsDialog() {
	    try {
	      if (this.mSangNote != null)
	        this.mSangNote.dismiss();
	      stopProgressDialog();
	    } catch (Exception localException) {
	      localException.printStackTrace();
	    }
	}
	
	
	private void exit() {
	    clsDialog();
	    this.mtimeHandler.removeCallbacks(this.atimeRunnable);
	    AnyPlayUtils.is_anyplay = false;
	    LocalInfo.APVideoisRuning = false;
	    this.mPublishState.SetMediaVideo(APPEnum.EventState.EventStateStopped.GetValue());
	    if (this.mVideoView != null)
	      this.mVideoView.stopPlayback();
	
	    finish();
	}
	
	
	
	
	private void handleIntent(Intent paramIntent) {
	    Bundle localBundle = paramIntent.getExtras();
	    int i = localBundle.getInt("VideoCmd");
	    AnyPlayUtils.LOG_DEBUG("handleIntent", "cmd=" + i);
	    if (i == APPEnum.AirVideoCmd.didStartPlayVideo.GetValue()) {
	      this.mPath = localBundle.getString("UriString");
	      this.mStartPosition = localBundle.getInt("StartPositon");
	      int k = localBundle.getInt("AirChannel");
	      mQiyiStop.tryDelayStop(mPath);
	      AnyPlayUtils.LOG_DEBUG("APVideo", "Path:" + this.mPath + " Pos:" + this.mStartPosition);
	      Statistics.addVideo(this, this.mPath, k);
	      startProgressDialog();
	      playVideo(this.mPath, this.mStartPosition);
	    }else if (i == APPEnum.AirVideoCmd.didTryPlay.GetValue()) {
	      this.mPath = localBundle.getString("UriString");
	      this.mStartPosition = localBundle.getInt("StartPositon");
	      int j = localBundle.getInt("AirChannel");
	      mQiyiStop.tryDelayStop(mPath);
	      AnyPlayUtils.LOG_DEBUG("APVideo", "Path:" + this.mPath + " Pos:" + this.mStartPosition);
	      Statistics.addVideo(this, this.mPath, j);
	      startProgressDialog("  "+ this.getResources().getString(R.string.video_tryplay_other));
	      playVideo(this.mPath, this.mStartPosition);
	    }else if (i == APPEnum.AirVideoCmd.didCreateEventSession.GetValue()) {
	      LocalInfo.gVideoSessionID = localBundle.getInt("EvrntSessionId");
	      AnyPlayUtils.LOG_DEBUG("APVideo", "gVideoSessionID:" + LocalInfo.gVideoSessionID);
	    }else if (i == APPEnum.AirVideoCmd.didSetPlaybackRate.GetValue()) {
	    	if(sgMediaController != null) {
	    		sgMediaController.show(3000);
	    	}
	      if (localBundle.getLong("Rate") == 0L) {
	        this.mVideoView.pause();
	      }else{
	    	  try {
		        this.mVideoView.start();
			} catch (Exception e) {
				// TODO: handle exception
				e.printStackTrace();
			}
	      }
	    }else if (i == APPEnum.AirVideoCmd.setCurrentPlaybackProgress.GetValue()) {
	      playSeekto(localBundle.getLong("PlayPosition"));
	    }else if (i == APPEnum.AirVideoCmd.didStopPlayback.GetValue()) {
	      AnyPlayUtils.LOG_DEBUG("handleIntent", "#s# Stop ");
	      if(mQiyiStop.isQiyiStop()){
		      exit();
	      }
	    }else if (i == APPEnum.AirVideoCmd.didSetVolume.GetValue()) {
	
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
//			// 鏄剧ず
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
//	
	private void playSeekto(long paramLong) {
	    this.mVideoView.seekTo(1000L * paramLong);
	    startProgressDialog();
	    sgMediaController.show(30000);
	    this.mSeektoValue = paramLong;
	    this.g_seek_doing = true;
	}
	
	private void playVideo(String paramString, long paramLong) {
	    this.g_seek_doing = false;
	    this.mVideoView.setVideoPath(paramString);
	    this.mVideoView.setBufferSize(2048);
	    this.mVideoView.setOnCompletionListener(this);
	    this.mVideoView.setOnPreparedListener(this);
	    this.mVideoView.setOnSeekCompleteListener(this);
	    this.mVideoView.setOnBufferingUpdateListener(this);
//	    this.mVideoView.setVideoLayout(this.mLayout, 0.0F);
	    this.mVideoView.setOnInfoListener(this);
	    this.mVideoView.setOnErrorListener(this);
	    sgMediaController = new MediaController(this);
	    sgMediaController.setFileName(this.mTitle);
	    this.mVideoView.setMediaController(sgMediaController);
	    this.mVideoView.requestFocus();
	    publishEvt(APPEnum.EventState.EventStateLoading.GetValue());

	    AnyPlayUtils.LOG_DEBUG("playVideo", "Start");
	}
	
	private void publishEvt(int paramInt) {
	    this.mPublishState.SetMediaVideo(paramInt);
	}
	
	private void sendVideoProcess() {
	    LocalInfo.APVideoisRuning = true;
	    Intent localIntent = new Intent();
	    Bundle localBundle = new Bundle();
	    int i = (int)(this.mVideoView.getDuration() / 1000L);
	    int j;
	    if (this.g_seek_doing) {
	    	j = (int)this.mSeektoValue; 
	    }else{
	    	j = (int)(this.mVideoView.getCurrentPosition() / 1000L);
	    }
	
	    localBundle.putInt("Dur", i);
	    localBundle.putInt("Pos", j);
	    localIntent.setAction("anyplay.service.videoProcess");
	    AnyPlayUtils.LOG_DEBUG("playVideo", "-------------------- Dur=" + i + "  Pos="  + j);
	    localIntent.putExtras(localBundle);
	    sendBroadcast(localIntent);
	}
	
	private void setProgressValue(int paramInt) {
		try {
	        if (this.progressDialog == null) {
	          return;
	        }
	        if (paramInt >= 0) {
	        	this.progressDialog.setProgress(paramInt);
	        }
		} catch (Exception localException) {
	        localException.printStackTrace();
	    }
	}
	
	private void showSangNode(String paramString) {
	    this.mSangNote = new SangNote(this);
	    this.mSangNote.setMsg(paramString);
	    this.mSangNote.setConfirmOnListener(new View.OnClickListener() {
	      public void onClick(View paramView) {
	        APVideo.this.callbackEvt();
	      }
	    });
	    this.mSangNote.setDelayCallbackEvent(AnyPlayUtils.NodeDelayTimes, new CallBackEvent() {
	      public void delayCallbackEvent() {
	        APVideo.this.callbackEvt();
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
	
	private void startProgressDialog(String paramString) {
	    try {
	      if (this.progressDialog == null) {
	        this.progressDialog = SangProgressDialog.createDialog(this);
	        this.progressDialog.setMessage(paramString);
	        this.progressDialog.setOnCannel(new OnCancelListener() {
				@Override
				public void onCancel(DialogInterface dialog) {
					exit();
				}
			});
	      }
	      if (this.progressDialog != null) {
	        this.progressDialog.show();
	      }
	    } catch (Exception localException) {
	      localException.printStackTrace();
	    }
	}
	
	private void stopProgressDialog() {
	    try {
	      if (this.progressDialog != null) {
	        this.progressDialog.dismiss();
	        this.progressDialog = null;
	      }
	    } catch (Exception localException) {
	      localException.printStackTrace();
	    }
	}
	
	private boolean verifyLeg() {
	    VerLeg localVerLeg = new VerLeg(this);
	    return localVerLeg.showTVMark();
	}
	
	public void onBufferingUpdate(MediaPlayer paramMediaPlayer, int paramInt) {
	    setProgressValue(paramInt);
	}
	
	public void onCompletion(MediaPlayer paramMediaPlayer) {
	    exit();
	}
	
	public void onConfigurationChanged(Configuration paramConfiguration) {
	    if (this.mVideoView != null)
	      this.mVideoView.setVideoLayout(this.mLayout, 0.0F);
	    super.onConfigurationChanged(paramConfiguration);
	}
	

	protected void onDestroy() {
	    super.onDestroy();
	    try {
		    unregisterReceiver(this.serStateReceiver);
		    if (this.mAPServerBinder != null) {
		      unbindService(this.mAPServiceConnection);
		    }
		} catch (Exception e) {
			e.printStackTrace();
		}
	    AnyPlayUtils.LOG_DEBUG("APVideo", "----------------------------onDestroy1");
	}
	
	public boolean onError(MediaPlayer paramMediaPlayer, int paramInt1, int paramInt2) {
	    AnyPlayUtils.LOG_ERR("APVideo", "#########################ERR arg1=" + paramInt1 + "  arg2=" + paramInt2);
	    showSangNode(this.mPlayerErrString);
	    return true;
	}
	
	public boolean onInfo(MediaPlayer paramMediaPlayer, int paramInt1, int paramInt2) {
	    if (paramInt1 == 701) {
	      paramMediaPlayer.pause();
	      startProgressDialog();
	    }else if (paramInt1 == 702) {
	        stopProgressDialog();
	        paramMediaPlayer.start();
	    }else if (paramInt1 == 901) {
	      AnyPlayUtils.LOG_DEBUG("APVideo", "download rate:" + paramInt2);
	    }
		return false;
	}
	
	public boolean onKeyDown(int paramInt, KeyEvent paramKeyEvent) {
	    if (paramInt == KeyEvent.KEYCODE_BACK) {
	      AnyPlayUtils.LOG_DEBUG("APVideo", "----------------onKeyDown");
	      exit();
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
	    if (this.mVideoView != null)
	      this.mVideoView.pause();
	    MobclickAgent.onPause(this);
	}
	
	public void onPrepared(MediaPlayer paramMediaPlayer) {
	    int i = paramMediaPlayer.getVideoHeight();
	    int j = paramMediaPlayer.getVideoWidth();
	    AnyPlayUtils.LOG_DEBUG("APVideo", "w=" + j + "  h=" + i);
	    if (j >= 720) {
	      paramMediaPlayer.setVideoQuality(0);
	    }else if ((j < 720) && (j >= 640)) {
	        paramMediaPlayer.setVideoQuality(0);
	    }else{
	      paramMediaPlayer.setVideoQuality(-16);
	    }
		paramMediaPlayer.setPlaybackSpeed(1.0f);
	    this.mVideoView.setVideoLayout(this.mLayout_scale, 0.0F);
	    if (this.mStartPosition > 0) {
	        StartSeekTo(this.mStartPosition);
	    }
	    SendTheard();
	}
	
	protected void onResume() {
	    super.onResume();
	    if (this.mVideoView != null)
	      this.mVideoView.resume();
	    MobclickAgent.onResume(this);
	}
	
	public void onSeekComplete(MediaPlayer paramMediaPlayer) {
	    stopProgressDialog();
	    AnyPlayUtils.LOG_DEBUG("APVideo", "onSeekComplete");
	    this.g_seek_doing = false;
	    sgMediaController.show(3000);
	}
	
	protected void onStop() {
	    super.onStop();
	    AnyPlayUtils.LOG_ERR("APVideo", "onStop");
//	    LocalInfo.APVideoisRuning = false;
//	    AnyPlayApi.is_anyplay = false;
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
//
//	private class MyGestureListener extends SimpleOnGestureListener {
//		@Override
//		public boolean onDoubleTap(MotionEvent e) {
//			if (mLayout == VideoView.VIDEO_LAYOUT_ZOOM)
//				mLayout = VideoView.VIDEO_LAYOUT_ORIGIN;
//			else
//				mLayout++;
//			if (mVideoView != null)
//				mVideoView.setVideoLayout(mLayout, 0);
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
//			if (mOldX > windowWidth * 4.0 / 5)// 鍙宠竟婊戝姩
//				onVolumeSlide((mOldY - y) / windowHeight);
//			else if (mOldX < windowWidth / 5.0)// 宸﹁竟婊戝姩
//				onBrightnessSlide((mOldY - y) / windowHeight);
//
//			return super.onScroll(e1, e2, distanceX, distanceY);
//		}
//	}

}
