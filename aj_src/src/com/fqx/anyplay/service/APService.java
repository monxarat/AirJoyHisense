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

import com.airjoy.airplay.server.AirPlayServer;
import com.airjoy.airplay.server.AirPlayServerListener;
import com.airjoy.airplay.server.AirPlayServer.EventCategory;
import com.airjoy.airtunes.server.AirTunesServer;
import com.airjoy.asio.http.server.HttpServer;
import com.airjoy.bonjour.Bonjour;
import com.airjoy.bonjour.serviceinfo.AirPlayServiceInfo;
import com.airjoy.bonjour.serviceinfo.AirTunesServiceInfo;
import com.airjoy.util.NetWork;
import com.example.testaiplayserver.MainActivity;
import com.fqx.anyplay.api.APPEnum;
import com.fqx.anyplay.api.AnyPlayUtils;
import com.fqx.anyplay.api.LocalInfo;
import com.fqx.anyplay.api.Notice;
import com.fqx.anyplay.api.PhoneNetInfo;
import com.fqx.anyplay.listener.AirPlayBonjourListener;
import com.fqx.anyplay.listener.AirPlayListener;
import com.fqx.anyplay.listener.AirTunesListener;

public class APService extends Service {
	  private MyBinder mBinder = new MyBinder();
	  private LocalInfo mLocalInfo;
	  private PlaybackInfo mPlaybackInfo;
	  private String mRandomMacString;
	  private VideoInfo mVideoInfo; 
	  
	  private AirPlayBonjourListener mAirPlayBonjourListener;
	  private AirPlayListener mAirPlayListener;
	  private AirTunesListener mAirTunesListener;

	  private boolean m_isEther = false;
	  private boolean m_isWifi = false;

	  private Bonjour mBonjour = null;

	  private static final String TAG = MainActivity.class.getSimpleName();


	  public void onCreate() { 
		  super.onCreate();
		  
		  mAirPlayBonjourListener = new AirPlayBonjourListener(this);
		  mAirPlayListener = new AirPlayListener(this);
		  mAirTunesListener = new AirTunesListener(this);

		  mBonjour = Bonjour.getInstance();
		  mBonjour.setContent(this);
		  mBonjour.setListener(mAirPlayBonjourListener);
		  mBonjour.start();
		
		  AnyPlayUtils.LOG_DEBUG("APService", "start onCreate~~~");
		  this.mVideoInfo = VideoInfo.getInstance();
		  this.mPlaybackInfo = PlaybackInfo.getInstance();
		  this.mRandomMacString = AnyPlayUtils.getRandomMac();
		  this.mLocalInfo = LocalInfo.getInstance(this);
		  this.mLocalInfo.setServerState(APPEnum.ServiceState.Fail.GetValue());

		  IntentFilter videoFilter = new IntentFilter();
		  videoFilter.addAction("anyplay.service.videoProcess");
		  registerReceiver(this.send_event, videoFilter);

		  IntentFilter evtFilter = new IntentFilter();
		  evtFilter.addAction("com.hisense.TVSearchStop");
		  evtFilter.addAction("com.hisense.TVSearchStart");
		  registerReceiver(this.tv_event, evtFilter);
			
		  IntentFilter netFilter = new IntentFilter();
		  netFilter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);//android.net.wifi.WIFI_STATE_CHANGED");
		  netFilter.addAction("android.net.wifi.STATE_CHANGE");
		  netFilter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
		  netFilter.addAction("android.intent.action.SCREEN_ON");
		  netFilter.addAction("android.intent.action.SCREEN_OFF");
		  registerReceiver(this.MyWifiReciver, netFilter);
//		  this.mPublishState = PublishState.getInstance();
		
		  isAP();
	  }
	  
	  
	  private void AirPlayStart() {
		  String name = mLocalInfo.getName();
		  byte[] deviceId = NetWork.getMacAddress();
     
          int airplayPort = 7000;
          int airtunesPort = 5000;

          AirPlayServer.getInstance().start(deviceId, airplayPort, mAirPlayListener);
          AirTunesServer.getInstance().start(deviceId, airtunesPort, mAirTunesListener);

          Log.d(TAG, "airplay port: " + AirPlayServer.getInstance().getPort());
          Log.d(TAG, "airtunes port: " + AirTunesServer.getInstance().getPort());

          AirPlayServiceInfo info1 = new AirPlayServiceInfo(deviceId,
                  name, 
                  AirPlayServer.getInstance().getPort());
          mBonjour.publishService(info1);

          AirTunesServiceInfo info2 = new AirTunesServiceInfo(deviceId, 
                  name, 
                  AirTunesServer.getInstance().getPort());
          mBonjour.publishService(info2);	    
	  }
	
	
	private BroadcastReceiver MyWifiReciver = new BroadcastReceiver() { 
		public void onReceive(Context context, Intent intent) { 
			String str = intent.getAction(); 
			AnyPlayUtils.LOG_DEBUG("MyWifiReciver", "ACTION:" + str); 
			if ((str.equals(WifiManager.WIFI_STATE_CHANGED_ACTION)) || (str.equals("android.net.wifi.STATE_CHANGE"))) { 
				int wifiState = intent.getIntExtra(WifiManager.EXTRA_WIFI_STATE, 0);   
				AnyPlayUtils.LOG_DEBUG("MyWifiReciver", "wifiState:" + wifiState); 
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
	    	   NetworkInfo localNetworkInfo = ((ConnectivityManager)context.getSystemService(Context.CONNECTIVITY_SERVICE)).getNetworkInfo(ConnectivityManager.TYPE_ETHERNET);
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
	


	  private void SendNetErr() {
		  AnyPlayUtils.SendServerState(this, APPEnum.ServerState.S_NET_ERR.GetValue());
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
	     AirPlayStart();
	     this.mLocalInfo.setPort(AirPlayServer.getInstance().getPort());
	     Notice.AirJoyStateOK(this);
	  }
	
	  private void stopAirpaly() {
		  AnyPlayUtils.LOG_DEBUG("AirPlay", "Server Stop ");
		  AirPlayServer.getInstance().stop();
	      AirTunesServer.getInstance().stop();
	  }
	
	  public IBinder onBind(Intent paramIntent) {
		  return this.mBinder;
	  }
	
	  public void onDestroy() {
		  super.onDestroy();
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
		  //Airplay_Start(this.mLocalInfo.getMac(), paramString, "AppleTV2,1");
	  }
	
	  public class MyBinder extends Binder { 
		  public MyBinder() { 
		  }
	
		  public void Start() {
			  AirPlayStart();
		  }
	
//		  public APController getService() {
//			  return null;
//		  }
	
		  public void reName(String paramString) {
			  APService.this.re_name(paramString);
		  }
	
		  public void reStart() {
		      if (!LocalInfo.isSwitchON) {
		        AnyPlayUtils.LOG_DEBUG("startAirJoy", "isSwitchON=false");
		        return;
		      }
//		      APService.this.Airplay_Start(APService.this.mLocalInfo.getMac(), APService.this.mLocalInfo.getName(), "AppleTV2,1");
		  }
	

		  public void stop() {
			  APService.this.stopAirpaly();
		  }
	
		  public void stopAudio() {
		  }
	
		  public void stopBonjour() { 
		  }
	  }
}