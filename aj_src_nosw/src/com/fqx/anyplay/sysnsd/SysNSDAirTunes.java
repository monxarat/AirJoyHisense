package com.fqx.anyplay.sysnsd;

import android.annotation.SuppressLint;
import android.content.Context;
import android.net.nsd.NsdManager;
import android.net.nsd.NsdManager.RegistrationListener;
import android.net.nsd.NsdServiceInfo;
import android.util.Log;
import java.util.HashMap;
import java.util.Map;

@SuppressLint({"NewApi"})
public class SysNSDAirTunes
{
  private Context mContext;
  private NsdManager mNsdManager;
  private NsdManager.RegistrationListener mRegistrationListener;
  private Map<String, String> mStrMap;
  private NsdServiceInfo serviceInfo;

  public SysNSDAirTunes(Context paramContext)
  {
    this.mContext = paramContext;
    initMapTxt();
    this.serviceInfo = new NsdServiceInfo();
    initializeRegistrationListener();
  }

  private void initMapTxt()
  {
    this.mStrMap = new HashMap();
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
      }

      public void onServiceUnregistered(NsdServiceInfo paramNsdServiceInfo)
      {
        Log.e("RegistrationListener", "onServiceUnregistered");
      }

      public void onUnregistrationFailed(NsdServiceInfo paramNsdServiceInfo, int paramInt)
      {
        Log.e("RegistrationListener", "onUnregistrationFailed");
      }
    };
  }

  private String makeATunesName(String paramString1, String paramString2)
  {
    return paramString1.replaceAll(":", "") + "@" + paramString2;
  }

  public boolean registerService(String paramString1, String paramString2, int paramInt)
  {
    String str = makeATunesName(paramString2, paramString1);
    this.serviceInfo.setServiceName(str);
    this.serviceInfo.setServiceType("_raop._tcp");
    this.serviceInfo.setPort(paramInt);
    Log.e("registerService", "Info:" + this.serviceInfo.toString());
    this.mNsdManager = ((NsdManager)this.mContext.getSystemService("servicediscovery"));
    this.mNsdManager.registerService(this.serviceInfo, 1, this.mRegistrationListener);
    return true;
  }

  public void unRegisterService()
  {
    try
    {
      this.mNsdManager.unregisterService(this.mRegistrationListener);
      return;
    }
    catch (Exception localException)
    {
    }
  }
}

