package com.fqx.anyplay.service;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Binder;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.SystemClock;
import android.util.Log;

import com.fqx.anyplay.api.APPEnum;
import com.fqx.anyplay.api.AnyPlayUtils;
import com.fqx.anyplay.api.LocalInfo;
import com.fqx.anyplay.api.Notice;
import com.fqx.anyplay.api.PhoneNetInfo;
import com.fqx.anyplay.api.PublishState;

public class APService extends Service {
	  private APAirTunesServerListener mAirTunesServerListener;
	  private APController mApController;
	  private APResultListener mApResultListener;
	  private MyBinder mBinder = new MyBinder();
	  private LocalInfo mLocalInfo;
	  private Thread mMdnsStarThread;
	  private PlaybackInfo mPlaybackInfo;
	  private PublishState mPublishState;
	  private String mRandomMacString;
	  private VideoInfo mVideoInfo; 

	  private Handler mPublishHandler;
	  private boolean m_isEther = false;
	  private boolean m_isWifi = false;
	  private final static long TIME_PUBLISH = 15*1000;

	  static {
	    AnyPlayUtils.LOG_DEBUG("APController", "Loadlib start");
	    try {
	      System.loadLibrary("dns_sd_aj");
	      System.loadLibrary("bonjour_aj");
	      System.loadLibrary("ssl_aj");
	      System.loadLibrary("crypto_aj");
	      System.loadLibrary("airplay");
	      System.loadLibrary("airtunes");
	      System.loadLibrary("jniAnyplay");
	      AnyPlayUtils.LOG_DEBUG("APController", "Loadlib ok");
	    } catch (Exception localException) {
	      Log.e("APController", "Loadlib ERR=" + localException.toString());
	    }
	}
	  

	public void onCreate() {
	    super.onCreate();
	    APController.native_init();
	    this.mApController = new APController(this);
	    this.mAirTunesServerListener = APAirTunesServerListener.getInstance(this);
	    this.mApResultListener = new APResultListener(this, this.mApController);
	    this.mApController.SetAirTunesServerListener(this.mAirTunesServerListener);
	    this.mApController.SetAirPlayListener(this.mApResultListener);

	    mPublishHandler = new Handler();
	    AnyPlayUtils.LOG_DEBUG("APService", "start onCreate~~~");
	    this.mVideoInfo = VideoInfo.getInstance();
	    this.mPlaybackInfo = PlaybackInfo.getInstance();
	    this.mRandomMacString = AnyPlayUtils.getRandomMac();
	    StartBonjourThread();
	    this.mLocalInfo = LocalInfo.getInstance(this);
	    this.mLocalInfo.setServerState(APPEnum.ServiceState.Fail.GetValue());
	    IntentFilter localIntentFilter1 = new IntentFilter();
	    localIntentFilter1.addAction("anyplay.service.videoProcess");
	    registerReceiver(this.send_event, localIntentFilter1);
	    IntentFilter localIntentFilter2 = new IntentFilter();
	    localIntentFilter2.addAction("com.hisense.TVSearchStop");
	    localIntentFilter2.addAction("com.hisense.TVSearchStart");
	    registerReceiver(this.tv_event, localIntentFilter2);

	    IntentFilter localIntentFilter3 = new IntentFilter();
	    localIntentFilter3.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);//android.net.wifi.WIFI_STATE_CHANGED");
	    localIntentFilter3.addAction("android.net.wifi.STATE_CHANGE");
	    localIntentFilter3.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
	    localIntentFilter3.addAction("android.intent.action.SCREEN_ON");
	    localIntentFilter3.addAction("android.intent.action.SCREEN_OFF");
	    registerReceiver(this.MyWifiReciver, localIntentFilter3);
	    this.mPublishState = PublishState.getInstance();

	    isAP();
	}
	
	private Runnable mPublishRunnable = new Runnable() {
		@Override
		public void run() {
//		    mApController.AirplayStop();
//			SystemClock.sleep(2000);
//			mBinder.Start();
			mApController.AirplayPublishService(false);
			mApController.AirplayPublishService(true);
	        mPublishHandler.postDelayed(mPublishRunnable, TIME_PUBLISH);
		 AnyPlayUtils.LOG_DEBUG("mPublishRunnable", "------------------------------------------------------------");
		}
	};
	
	
	private BroadcastReceiver MyWifiReciver = new BroadcastReceiver() {
	    public void onReceive(Context paramContext, Intent paramIntent) {
	      String str = paramIntent.getAction();
		 AnyPlayUtils.LOG_DEBUG("MyWifiReciver", "-------------- ACTION:" + str);
	      if ((str.equals(WifiManager.WIFI_STATE_CHANGED_ACTION)) || (str.equals("android.net.wifi.STATE_CHANGE"))) {
	      	int wifiState = paramIntent.getIntExtra(WifiManager.EXTRA_WIFI_STATE, 0);   
		 AnyPlayUtils.LOG_DEBUG("MyWifiReciver", "-------------- wifiState:" + wifiState);
	        switch (wifiState) {   
	            case WifiManager.WIFI_STATE_DISABLED:   
		        	AnyPlayUtils.LOG_DEBUG("WIFI_STATE_CHANGED", "WIFI_STATE_DISABLED");
			        APService.this.m_isWifi = false;
			        if (APService.this.m_isEther)
			        	break;
			        APService.this.stopAirpaly();
			        APService.this.SendNetErr();
	                break;   
	            case WifiManager.WIFI_STATE_ENABLED:   
		        	AnyPlayUtils.LOG_DEBUG("WIFI_STATE_CHANGED", "WIFI_STATE_ENABLED");
			        APService.this.m_isWifi = true;
			        APService.this.getWifiMac();
			        APService.this.startAirJoy();
	                break;   
	        	}
	       }else if (str.equals(ConnectivityManager.CONNECTIVITY_ACTION)) {
	          NetworkInfo localNetworkInfo = ((ConnectivityManager)paramContext.getSystemService(Context.CONNECTIVITY_SERVICE)).getNetworkInfo(ConnectivityManager.TYPE_ETHERNET);
	          if (localNetworkInfo == null) {
	        	  AnyPlayUtils.LOG_DEBUG("CONNECTIVITY_ACTION", "localNetworkInfo = null");
	        	  return;
	          }
	          if (localNetworkInfo.isConnected()) {
	            AnyPlayUtils.LOG_DEBUG("CONNECTIVITY_ACTION", "STATE_ENABLED");
	            APService.this.m_isEther = true;
	            APService.this.mLocalInfo.setMac(APService.this.mRandomMacString);
	            APService.this.startAirJoy();
	          }else{
		          AnyPlayUtils.LOG_DEBUG("CONNECTIVITY_ACTION", "STATE_DISENABLED");
		          APService.this.m_isEther = false;
		          if (APService.this.m_isWifi)
		        	  return;
		          APService.this.stopAirpaly();
		          APService.this.SendNetErr();
	          }
	        }else if (str.equals("android.intent.action.SCREEN_ON")) {
	        	AnyPlayUtils.LOG_DEBUG("AirPlay", "ACTION_SCREEN_ON");
	        	APService.this.startAirJoy();
	        }else if (str.equals("android.intent.action.SCREEN_OFF")) {
	        	AnyPlayUtils.LOG_DEBUG("AirPlay", "ACTION_SCREEN_OFF");
	        	APService.this.stopAirpaly();
	      }
	    }
	  };
	
	
	private BroadcastReceiver send_event = new BroadcastReceiver() {
	    public void onReceive(Context paramContext, Intent paramIntent) {
	      Bundle localBundle = paramIntent.getExtras();
	      int i = localBundle.getInt("Dur");
	      int j = localBundle.getInt("Pos");
	      APService.this.mVideoInfo.setValue(i, j);
	      APService.this.mPlaybackInfo.setDurValues(i);
	      APService.this.mPlaybackInfo.set_playPosition(j);
	    }
	};
	private BroadcastReceiver tv_event = new BroadcastReceiver() {
	    public void onReceive(Context paramContext, Intent paramIntent) {
	      String str = paramIntent.getAction();
	      if (str.equals("com.hisense.TVSearchStart")) {
	        APService.this.stopAirpaly();
	      }else if (str.equals("com.hisense.TVSearchStop")) {
	        APService.this.startAirJoy();
	      }
	    }
	};
	

	  private int Airplay_Start(String paramString1, String paramString2, String paramString3) {
	    if (this.mApController.AirplayStart(paramString1, paramString2, paramString3) < 0) {
	      Log.e("AirplayStart", "Start Err");
	      return -1;
	    }
	    if (!this.mApController.PublishAirplayService()) {
	      this.mApResultListener.onResultListener(APPEnum.AirChannel.AirPlay.GetValue(), 400);
	    }else{
	      this.mApResultListener.onResultListener(APPEnum.AirChannel.AirPlay.GetValue(), 200);
	    }
	    
	    
	    if(this.mApController.AirTunesStart(paramString1, paramString2, paramString3) < 0) {
	    	Log.e("AirTunesStart", "Start Err");
	    	return -1;
	    }
	    if (!this.mApController.PublishAirTunesService()) {
	      this.mApResultListener.onResultListener(APPEnum.AirChannel.AirTunes.GetValue(), 400);
	    }else{
	      this.mApResultListener.onResultListener(APPEnum.AirChannel.AirTunes.GetValue(), 200);
	    }
	    
	    return 1;
	  }
	
	private void SendNetErr() {
	    AnyPlayUtils.SendServerState(this, APPEnum.ServerState.S_NET_ERR.GetValue());
	}
	
	private void StartBonjourThread() {
	    AnyPlayUtils.LOG_DEBUG("APController", "StartBonjourThrad..Test 1..");
	    this.mMdnsStarThread = new Thread(new Runnable() {
	      public void run()
	      {
	        AnyPlayUtils.LOG_DEBUG("APController", "StartBonjourThrad......................................");
	        APService.this.mApController.mdnsd_start(0);
	        AnyPlayUtils.LOG_DEBUG("APController", "StopBonjourThrad OK");
	      }
	    });
	    this.mMdnsStarThread.start();
	}
	
	private void StopBonjourThread() {
	    AnyPlayUtils.LOG_DEBUG("APController", "StopBonjourThrad...");
	    try {
	      if (this.mMdnsStarThread == null)
	        return;
	      this.mApController.mdnsd_stop();
	    } catch (Exception localException) {
	      Log.e("AirPlay", "StopBonjourThread:" + localException.toString());
	    }
	}
	
	
	private void getWifiMac() {
	    try {
	      WifiInfo localWifiInfo = ((WifiManager)getSystemService("wifi")).getConnectionInfo();
	      this.mLocalInfo.setMac(localWifiInfo.getMacAddress());
	    } catch (Exception localException) {
	      this.mLocalInfo.setMac(null);
	    }
	}
	
	private boolean isAP() {
	     PhoneNetInfo localPhoneNetInfo = new PhoneNetInfo(this);
	     if (localPhoneNetInfo.isAPMode()) {
	        getWifiMac();
	        startAirJoy();
	        return true;
	      }
	      return false;
	}
	
	private void startAirJoy() {
	     if (!LocalInfo.isSwitchON) {
	        AnyPlayUtils.LOG_DEBUG("startAirJoy", "isSwitchON=false");
	        return;
	     }
	      Airplay_Start(this.mLocalInfo.getMac(), this.mLocalInfo.getName(), "AppleTV2,1");
	      int i = this.mApController.getAirplayPort();
	      this.mLocalInfo.setPort(i);
	      Notice.AirJoyStateOK(this);
	      AnyPlayUtils.LOG_DEBUG("APController", "start(" + i + ")......................................");
	      this.mPublishState.tryInit();
		  mApController.AirTunesPublishService(true);
		  mApController.AirplayPublishService(true);
//	      mPublishHandler.postDelayed(mPublishRunnable, 1000);
	}
	
	private void stopAirpaly() {
	    this.mApController.Airplay_Stop();
	    AnyPlayUtils.LOG_DEBUG("AirPlay", "Server Stop ");
	}
	
	public IBinder onBind(Intent paramIntent) {
	    return this.mBinder;
	}
	
	public void onDestroy() {
	    super.onDestroy();
	    StopBonjourThread();
	    this.mApController.Airplay_Stop();
	    unregisterReceiver(this.MyWifiReciver);
	    unregisterReceiver(this.send_event);
	    unregisterReceiver(this.tv_event);
	}
	
	public void onStart(Intent paramIntent, int paramInt) {
	    super.onStart(paramIntent, paramInt);
	}
	
	public boolean onUnbind(Intent paramIntent) {
	    return super.onUnbind(paramIntent);
	}
	
	public void re_name(String paramString) {
	    this.mApController.Airplay_Stop();
	    Airplay_Start(this.mLocalInfo.getMac(), paramString, "AppleTV2,1");
	}
	
	public class MyBinder extends Binder {
	    public MyBinder() {
	    }
	
	    public void Start() {
	      APService.this.Airplay_Start(APService.this.mLocalInfo.getMac(), APService.this.mLocalInfo.getName(), "AppleTV2,1");
	    }
	
	    public APController getService() {
	      return APService.this.mApController;
	    }
	
	    public void reName(String paramString) {
	      APService.this.re_name(paramString);
	    }
	
	    public void reStart() {
	      if (!LocalInfo.isSwitchON) {
	        AnyPlayUtils.LOG_DEBUG("startAirJoy", "isSwitchON=false");
	        return;
	      }
	       APService.this.mApController.Airplay_Stop();
	       APService.this.Airplay_Start(APService.this.mLocalInfo.getMac(), APService.this.mLocalInfo.getName(), "AppleTV2,1");
	    }
	

	    public void stop() {
	      APService.this.stopAirpaly();
	    }
	
	    public void stopAudio() {
	      APService.this.mAirTunesServerListener.doStop();
	    }
	
	    public void stopBonjour() {
	    }
	  }
}