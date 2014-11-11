package com.fqx.anyplay.service;

import java.util.Map;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiManager;
import android.os.Binder;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.util.Log;

import com.airjoy.bonjour.Bonjour;
import com.airjoy.bonjour.Bonjour.Status;
import com.airjoy.bonjour.BonjourListener;
import com.airjoy.bonjour.serviceinfo.AirPlayServiceInfo;
import com.airjoy.bonjour.serviceinfo.AirTunesServiceInfo;
import com.airjoy.util.NetWork;
import com.fqx.anyplay.api.APPEnum;
import com.fqx.anyplay.api.AnyPlayUtils;
import com.fqx.anyplay.api.LocalInfo;
import com.fqx.anyplay.api.Notice;
import com.fqx.anyplay.api.PhoneNetInfo;
import com.fqx.anyplay.api.PublishState;

public class APService extends Service implements BonjourListener{
	  private APAirTunesServerListener mAirTunesServerListener;
	  private APController mApController;
	  private APResultListener mApResultListener;
	  private MyBinder mBinder = new MyBinder();
	  private LocalInfo mLocalInfo;
	  private PlaybackInfo mPlaybackInfo;
	  private PublishState mPublishState;
	  private VideoInfo mVideoInfo; 
	  private Handler mHandler;

	  private Bonjour mBonjour = null;
	  private boolean m_is_starting = false;
//	  private boolean m_is_stop = false;

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

	    AnyPlayUtils.LOG_DEBUG("APService", "start onCreate~~~");
	    this.mVideoInfo = VideoInfo.getInstance();
	    this.mPlaybackInfo = PlaybackInfo.getInstance();
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
	    mHandler = new Handler();

	    isAP();
	}
	

	private BroadcastReceiver MyWifiReciver = new BroadcastReceiver() {
	    public void onReceive(Context paramContext, Intent paramIntent) {
	      String str = paramIntent.getAction();
	      if (str.equals(ConnectivityManager.CONNECTIVITY_ACTION)) {
	    	  ConnectivityManager mConnectivityManager = (ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE);  
	    	  NetworkInfo netInfo = mConnectivityManager.getActiveNetworkInfo();    
	          if(netInfo != null && netInfo.isAvailable()) {  
	        	  // 0 TYPE_MOBILE 
	        	  // 1 TYPE_WIFI
	        	  // 9 TYPE_ETHERNET
	        	  AnyPlayUtils.LOG_DEBUG("CONNECTIVITY_ACTION", "STATE_ENABLED NetInfoType:" + netInfo.getType());
				  APService.this.mLocalInfo.setMacBytes(NetWork.getMacAddress());
		          APService.this.startAirJoy();
	          } else {  
	             ////////网络断开  
		          AnyPlayUtils.LOG_DEBUG("CONNECTIVITY_ACTION", "STATE_DISENABLED");
		          APService.this.stopAirpaly();
		          APService.this.SendNetErr();
	         }  
		  }
	    }
	  };
	
	
//	private BroadcastReceiver MyWifiReciver = new BroadcastReceiver() {
//	    public void onReceive(Context paramContext, Intent paramIntent) {
//	      String str = paramIntent.getAction();
//		 AnyPlayUtils.LOG_DEBUG("MyWifiReciver", "ACTION:" + str);
//	      if ((str.equals(WifiManager.WIFI_STATE_CHANGED_ACTION)) || (str.equals("android.net.wifi.STATE_CHANGE"))) {
//	      	int wifiState = paramIntent.getIntExtra(WifiManager.EXTRA_WIFI_STATE, 0);   
//		 AnyPlayUtils.LOG_DEBUG("MyWifiReciver", "wifiState:" + wifiState);
//	        switch (wifiState) {   
//	            case WifiManager.WIFI_STATE_DISABLED:   
//		        	AnyPlayUtils.LOG_DEBUG("WIFI_STATE_CHANGED", "WIFI_STATE_DISABLED");
//			        APService.this.m_isWifi = false;
//			        if (APService.this.m_isEther)
//			        	break;
//			        APService.this.stopAirpaly();
//			        APService.this.SendNetErr();
//	                break;   
//	            case WifiManager.WIFI_STATE_ENABLED:   
//		        	AnyPlayUtils.LOG_DEBUG("WIFI_STATE_CHANGED", "WIFI_STATE_ENABLED");
//			        APService.this.m_isWifi = true;
//	    	        APService.this.m_isEther = false;
//			        APService.this.getWifiMac();
//			        APService.this.startAirJoy();
//	                break;   
//	        	}
//	       }else if (str.equals(ConnectivityManager.CONNECTIVITY_ACTION)) {
//	          NetworkInfo localNetworkInfo = ((ConnectivityManager)paramContext.getSystemService(Context.CONNECTIVITY_SERVICE)).getNetworkInfo(ConnectivityManager.TYPE_ETHERNET);
//	          if (localNetworkInfo == null) {
//	        	  AnyPlayUtils.LOG_DEBUG("CONNECTIVITY_ACTION", "localNetworkInfo = null");
//	        	  return;
//	          }
//	          if (localNetworkInfo.isConnected()) {
//	            AnyPlayUtils.LOG_DEBUG("CONNECTIVITY_ACTION", "STATE_ENABLED");
//	            APService.this.m_isEther = true;
//			    APService.this.m_isWifi = false;
////	            APService.this.mLocalInfo.setMac(APService.this.mRandomMacString);
//			    APService.this.mLocalInfo.setMacBytes(NetWork.getMacAddress());
////			    APService.this.mLocalInfo.setMacBytes(AnyPlayUtils.getRandomMacBytes());
//	            APService.this.startAirJoy();
//	          }else{
//		          AnyPlayUtils.LOG_DEBUG("CONNECTIVITY_ACTION", "STATE_DISENABLED");
//		          APService.this.m_isEther = false;
//		          if (APService.this.m_isWifi) {
//			          AnyPlayUtils.LOG_DEBUG("CONNECTIVITY_ACTION", "#ERR WIFI Connect!");
//		        	  return;
//		          }
//		          APService.this.stopAirpaly();
//		          APService.this.SendNetErr();
//	          }
//	        }else if (str.equals("android.intent.action.SCREEN_ON")) {
//	        	AnyPlayUtils.LOG_DEBUG("AirPlay", "ACTION_SCREEN_ON");
//	        	APService.this.startAirJoy();
//	        }else if (str.equals("android.intent.action.SCREEN_OFF")) {
//	        	AnyPlayUtils.LOG_DEBUG("AirPlay", "ACTION_SCREEN_OFF");
//	        	APService.this.stopAirpaly();
//	      }
//	    }
//	  };
//	
	
	private BroadcastReceiver send_event = new BroadcastReceiver() {
	    public void onReceive(Context paramContext, Intent paramIntent) {
	      Bundle localBundle = paramIntent.getExtras();
	      int i = localBundle.getInt("Dur");
	      int j = localBundle.getInt("Pos");
	      int r = localBundle.getInt("Rate");
	      APService.this.mVideoInfo.setValue(i, j);
	      APService.this.mVideoInfo.setRate(r);
	      APService.this.mPlaybackInfo.set_rate(r);
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
	    
	    if(this.mApController.AirTunesStart(paramString1, paramString2, paramString3) < 0) {
	    	Log.e("AirTunesStart", "Start Err");
	    	return -1;
	    }
	    Log.d("AirplayStart", "################ mBonjour start");
	    mBonjour.start();
	    return 1;
	  }
	
	private void SendNetErr() {
	    AnyPlayUtils.SendServerState(this, APPEnum.ServerState.S_NET_ERR.GetValue());
	}
	
	private void StartBonjourThread() {
        mBonjour = Bonjour.getInstance();
        mBonjour.setContent(this);
        mBonjour.setListener(this);
	}
	
	private void StopBonjourThread() {
	}
	
	
	private void getWifiMac() {
	   this.mLocalInfo.setMacBytes(NetWork.getMacAddress());

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
//	     if(mBonjour.isStarted()) {
//	        AnyPlayUtils.LOG_DEBUG("startAirJoy", "Waiting Stop...");
//	    	 return;
//	     }
	     m_is_starting = true;
	     if(mBonjour.getStatus() != Status.Stopped) {
//	     if(m_is_stop) {
	        AnyPlayUtils.LOG_DEBUG("startAirJoy", "Waiting Stop...");
	    	 return;
	     }
	      Airplay_Start(this.mLocalInfo.getMac(), this.mLocalInfo.getName(), "AppleTV2,1");
	      int i = this.mApController.getAirplayPort();
	      this.mLocalInfo.setPort(i);
	      Notice.AirJoyStateOK(this);
	      AnyPlayUtils.LOG_DEBUG("APController", "start(" + i + ")......................................");
	      this.mPublishState.tryInit();
	}
	
	private void stopAirpaly() {
//		m_is_stop = true;
//	     if(mBonjour.isStarted() == false) {
//	        AnyPlayUtils.LOG_DEBUG("startAirJoy", "Waiting Start...");
//	    	 return;
//	     }
		if(mBonjour.getStatus() != Status.Started) {
	        AnyPlayUtils.LOG_DEBUG("startAirJoy", "Waiting Start...");
	    	 return;
	     }
	    this.mApController.Airplay_Stop();
		mBonjour.stop();
	    AnyPlayUtils.LOG_DEBUG("AirPlay", "Server Stop ");
	}
	
	public IBinder onBind(Intent paramIntent) {
	    return this.mBinder;
	}
	
	public void onDestroy() {
	    super.onDestroy();
	    StopBonjourThread();
	    stopAirpaly();
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
		stopAirpaly();
		mLocalInfo.setName(paramString);
		mBonjour.start();
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
	    
	    public Status getStartus() {
	    	return mBonjour.getStatus();
	    }
	
	    public void reName(String paramString) {
	      APService.this.re_name(paramString);
	    }
	
	    public void reStart() { 
	    	if (!LocalInfo.isSwitchON) {
		        AnyPlayUtils.LOG_DEBUG("startAirJoy", "isSwitchON=false");
		        return;
		    }
			stopAirpaly();
			mBonjour.start();
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

	/* (non-Javadoc)
	 * @see com.airjoy.bonjour.BonjourListener#onStarted()
	 */
	@Override
	public void onStarted() {
		// TODO Auto-generated method stub
		 Log.d("AirPlay", String.format("Bonjour onStarted"));
//		 byte[] mDeviceId = NetWork.getMacAddress();
		 byte[] mDeviceId = mLocalInfo.getMacBytes();

		 Log.d("AirPlay", "Bonjour onStarted ---- ID=" + mDeviceId);
	     AirPlayServiceInfo info1 = new AirPlayServiceInfo(mDeviceId,
	                mLocalInfo.getName(), 
	                mLocalInfo.getPort());
	     mBonjour.publishService(info1);

	     AirTunesServiceInfo info2 = new AirTunesServiceInfo(mDeviceId, 
	                mLocalInfo.getName(), 
	                mLocalInfo.getPort());
	     mBonjour.publishService(info2);
		AnyPlayUtils.SendAirPlaySwitch(APService.this, "START");
		m_is_starting = false;
	}

	private Runnable mRunnable = new Runnable() {
		@Override
		public void run() {
			// TODO Auto-generated method stub
			startAirJoy();
		}
	};
	/* (non-Javadoc)
	 * @see com.airjoy.bonjour.BonjourListener#onStartFailed()
	 */
	@Override
	public void onStartFailed() {
		// TODO Auto-generated method stub
		AnyPlayUtils.LOG_DEBUG("BonjourListener", "onStartFailed");
		m_is_starting = false;
		mHandler.postDelayed(mRunnable, 500);
	}

	/* (non-Javadoc)
	 * @see com.airjoy.bonjour.BonjourListener#onStopped()
	 */
	@Override
	public void onStopped() {
		// TODO Auto-generated method stub
		AnyPlayUtils.LOG_DEBUG("BonjourListener", "onStopped");
//		m_is_stop = false;
//		if(mBonjour.getStatus() == Status.Starting) {
		if(m_is_starting) {
			startAirJoy();
		}
		AnyPlayUtils.SendAirPlaySwitch(APService.this, "STOP");
	}

	/* (non-Javadoc)
	 * @see com.airjoy.bonjour.BonjourListener#onServiceFound(java.lang.String, java.lang.String, java.lang.String, int, java.util.Map)
	 */
	@Override
	public void onServiceFound(String name, String type, String ip, int port,
			Map<String, String> properties) {
		// TODO Auto-generated method stub
		
	}

	/* (non-Javadoc)
	 * @see com.airjoy.bonjour.BonjourListener#onServiceLost(java.lang.String, java.lang.String, java.lang.String)
	 */
	@Override
	public void onServiceLost(String name, String type, String ip) {
		// TODO Auto-generated method stub
		
	}
}