package com.fqx.anyplay.sysnsd;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;

public class AirNSDService extends Service {
  private static final String TAG = "AirDiscoveryService";
  private MyBinder mBinder = new MyBinder();

  public IBinder onBind(Intent paramIntent) {
    return this.mBinder;
  }

  public void onCreate() {
    Log.d("AirDiscoveryService", "Sysrunning...");
    super.onCreate();
  }

  public void onDestroy() {
    super.onDestroy();
  }

  public boolean onUnbind(Intent paramIntent) {
    return super.onUnbind(paramIntent);
  }

  public class MyBinder extends Binder {
    public MyBinder() {
    }
  }
}
