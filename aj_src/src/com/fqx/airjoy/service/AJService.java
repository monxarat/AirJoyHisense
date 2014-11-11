package com.fqx.airjoy.service;

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
import android.os.IBinder;
import android.util.Log;

import com.fqx.airjoy.server.AJEnum;
import com.fqx.airjoy.server.AJPlayBackInfo;
import com.fqx.airjoy.server.AJVideoInfo;
import com.fqx.airjoy.server.AirJoy;
import com.fqx.anyplay.api.AnyPlayUtils;
import com.fqx.anyplay.api.LocalInfo;
import com.fqx.anyplay.api.PublishState;

public class AJService extends Service {

	

  private AJDiscovery mAJDiscovery;
  private AJMediaListener mAJMediaListener;
  private AJPhoto mAJPhoto;
  private AJPlayBackInfo mAJPlayBackInfo;
  private AJPublish mAJPublish;
  private AJVideo mAJVideo;
  private AJVideoInfo mAJVideoInfo;
  private AirJoy mAirJoy;
  private AJBinder mBinder = new AJBinder();
  private LocalInfo mLocalInfo;
  private PublishState mPublishState;

  static {
    Log.d("AirJoy", "Loadlib start");
    try {
      System.loadLibrary("airjoy");
      System.loadLibrary("AirJoyServer");
      Log.d("AirJoy", "Loadlib OK");
    } catch (Exception localException) {
      Log.d("AirJoy", "Loadlib Err=" + localException.toString());
    }
  }

  private void AJStart() {
//    this.mAirJoy.ServerStart();
//    this.mAirJoy.nsdStart();
//    this.mAirJoy.publishService();
  }

  private void AJStop() {
    this.mAirJoy.stop();
  }

 

  public IBinder onBind(Intent paramIntent) {
    return this.mBinder;
  }

  public void onCreate() {
    super.onCreate();
    this.mLocalInfo = LocalInfo.getInstance(this);
    LocalInfo.gAjPhotoManger = new AJPhotoManger(this);
    LocalInfo.gAjPhotoManger.cls();
    this.mPublishState = PublishState.getInstance();
    this.mAirJoy = new AirJoy();
    this.mAJDiscovery = new AJDiscovery();
    this.mAJPublish = new AJPublish();
    this.mAJPhoto = new AJPhoto(this);
    this.mAJVideo = new AJVideo(this);
    this.mAJMediaListener = new AJMediaListener(this);
    this.mAirJoy.setNsdDiscoveryListener(this.mAJDiscovery);
    this.mAirJoy.setNsdPublishListener(this.mAJPublish);
    this.mAirJoy.setPhotoListener(this.mAJPhoto);
    this.mAirJoy.setVideoListener(this.mAJVideo);
    this.mAirJoy.setMediaListener(this.mAJMediaListener);
    this.mAirJoy.config(this.mLocalInfo.getName(), this.mLocalInfo.getMac(), AJEnum.DeviceType.DeviceTypeTv.GetValue());
    AJStart();
    this.mAJVideoInfo = AJVideoInfo.getInstance();
    this.mAJPlayBackInfo = AJPlayBackInfo.getInstance();
    IntentFilter localIntentFilter1 = new IntentFilter();
    localIntentFilter1.addAction("anyplay.service.videoProcess");
    registerReceiver(this.send_event, localIntentFilter1);
    IntentFilter localIntentFilter2 = new IntentFilter();
    localIntentFilter2.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);
    localIntentFilter2.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
    localIntentFilter2.addAction("android.intent.action.SCREEN_ON");
    localIntentFilter2.addAction("android.intent.action.SCREEN_OFF");
    registerReceiver(this.MyWifiReciver, localIntentFilter2);
    this.mPublishState.init();
    if(AnyPlayUtils.isNetOK(this)) {
    	AJStart();
    }
  }
  

  public void onDestroy() {
    super.onDestroy();
    this.mAirJoy.stop();
    LocalInfo.gAjPhotoManger.exit();
    unregisterReceiver(this.MyWifiReciver);
    unregisterReceiver(this.send_event);
  }

  public void re_name(String paramString) {
    this.mAirJoy.stop();
    this.mLocalInfo.setName(paramString);
    this.mAirJoy.config(this.mLocalInfo.getName(), this.mLocalInfo.getMac(), AJEnum.DeviceType.DeviceTypeTv.GetValue());
    AJStart();
  }

  public class AJBinder extends Binder {
    public AJBinder() {
    }

    public AirJoy getAirJoy() {
      return AJService.this.mAirJoy;
    }

    public void reName(String paramString) {
      AJService.this.re_name(paramString);
    }

    public void reStart() {
      AJService.this.AJStart();
    }

    public void stop() {
      AJService.this.AJStop();
    }
  }

  private BroadcastReceiver send_event = new BroadcastReceiver() {
    public void onReceive(Context paramContext, Intent paramIntent) {
      Bundle localBundle = paramIntent.getExtras();
      int i = localBundle.getInt("Dur");
      int j = localBundle.getInt("Pos");
      AJService.this.mAJVideoInfo.setPostion(j);
      AJService.this.mAJPlayBackInfo.setDuration(i);
      AJService.this.mAJPlayBackInfo.setPositon(j);
    }
  };


	private BroadcastReceiver MyWifiReciver = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			// TODO Auto-generated method stub
			String action = intent.getAction();
            if (action.equals(WifiManager.WIFI_STATE_CHANGED_ACTION)) {
            	int wifiState = intent.getIntExtra(WifiManager.EXTRA_WIFI_STATE, 0);   
                Log.d("WIFI状态", "wifiState"+wifiState);  
                switch (wifiState) {   
	                case WifiManager.WIFI_STATE_DISABLED:   
	                	AJStop();
	                    break;   
	                case WifiManager.WIFI_STATE_ENABLED:   
			            AJStart();
	                    break;   
                }   
            }else if (action.equals(ConnectivityManager.CONNECTIVITY_ACTION)) {
            	ConnectivityManager connectivityManager = (ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE);
            	NetworkInfo info = connectivityManager.getActiveNetworkInfo();  
            	 if(info != null && info.isAvailable()) {
            		 AJStart();
            	 }else{
            		 AJStop();
            	 }
            }else if (action.equals("android.intent.action.SCREEN_ON")) {
            	AJStart();
            }else if (action.equals("android.intent.action.SCREEN_OFF")) {
            	AJStop();
            }
		}
	};

}
