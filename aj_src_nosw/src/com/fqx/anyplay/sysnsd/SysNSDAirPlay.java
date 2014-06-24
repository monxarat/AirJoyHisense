package com.fqx.anyplay.sysnsd;

import android.annotation.SuppressLint;
import android.content.Context;
import android.net.nsd.NsdManager;
import android.net.nsd.NsdServiceInfo;
import android.util.Log;
import java.util.HashMap;
import java.util.Map;

@SuppressLint({"NewApi"})
public class SysNSDAirPlay {
//  private static final String TAG = "SysNSDAirPlay";
  private Context mContext;
  private NsdManager.DiscoveryListener mDiscoveryListener;
  private NsdManager mNsdManager;
  private NsdManager.RegistrationListener mRegistrationListener;
  private NsdServiceInfo mServiceInfo;
  private Map<String, String> strMap;

  public SysNSDAirPlay(Context paramContext) {
    this.mContext = paramContext;
    initializeRegistrationListener();
    initializeDiscoveryListener();
  }

  private void initMapTxt(String paramString) {
    this.strMap = new HashMap();
  }

  private void initializeRegistrationListener()
  {
    this.mRegistrationListener = new NsdManager.RegistrationListener()
    {
      public void onRegistrationFailed(NsdServiceInfo paramNsdServiceInfo, int paramInt)
      {
        Log.e("RegistrationListener", "onRegistrationFailed");
      }

      public void onServiceRegistered(NsdServiceInfo paramNsdServiceInfo)
      {
        Log.e("RegistrationListener", "onServiceRegistered");
        SysNSDAirPlay.this.mNsdManager.discoverServices("_airplay._tcp", 1, SysNSDAirPlay.this.mDiscoveryListener);
      }

      public void onServiceUnregistered(NsdServiceInfo paramNsdServiceInfo)
      {
        Log.e("RegistrationListener", "onServiceUnregistered");
        SysNSDAirPlay.this.trySetEnabled(false);
      }

      public void onUnregistrationFailed(NsdServiceInfo paramNsdServiceInfo, int paramInt)
      {
        Log.e("RegistrationListener", "onUnregistrationFailed");
      }
    };
  }

  private boolean trySetEnabled(boolean paramBoolean)
  {
    return true;
  }

  public void initializeDiscoveryListener()
  {
    this.mDiscoveryListener = new NsdManager.DiscoveryListener()
    {
      public void onDiscoveryStarted(String paramString)
      {
        Log.d("SysNSDAirPlay", "Service discovery started");
      }

      public void onDiscoveryStopped(String paramString)
      {
        Log.i("SysNSDAirPlay", "Discovery stopped: " + paramString);
      }

      public void onServiceFound(NsdServiceInfo paramNsdServiceInfo)
      {
        Log.d("SysNSDAirPlay", "Service discovery success" + paramNsdServiceInfo);
      }

      public void onServiceLost(NsdServiceInfo paramNsdServiceInfo)
      {
        Log.e("SysNSDAirPlay", "service lost" + paramNsdServiceInfo);
      }

      public void onStartDiscoveryFailed(String paramString, int paramInt)
      {
        Log.e("SysNSDAirPlay", "Discovery failed: Error code:" + paramInt);
        SysNSDAirPlay.this.mNsdManager.stopServiceDiscovery(this);
      }

      public void onStopDiscoveryFailed(String paramString, int paramInt)
      {
        Log.e("SysNSDAirPlay", "Discovery failed: Error code:" + paramInt);
        SysNSDAirPlay.this.mNsdManager.stopServiceDiscovery(this);
      }
    };
  }

  public boolean registerService(String paramString1, String paramString2, int paramInt)
  {
    initMapTxt(paramString2);
    this.mServiceInfo = new NsdServiceInfo();
    this.mServiceInfo.setServiceName(paramString1);
    this.mServiceInfo.setServiceType("_airplay._tcp");
    this.mServiceInfo.setPort(paramInt);
    this.mNsdManager = ((NsdManager)this.mContext.getSystemService("servicediscovery"));
    this.mNsdManager.registerService(this.mServiceInfo, 1, this.mRegistrationListener);
    return true;
  }

  public void unRegisterService()
  {
    try
    {
      this.mNsdManager.stopServiceDiscovery(this.mDiscoveryListener);
      this.mNsdManager.unregisterService(this.mRegistrationListener);
      return;
    }
    catch (Exception localException)
    {
      localException.printStackTrace();
    }
  }
}

