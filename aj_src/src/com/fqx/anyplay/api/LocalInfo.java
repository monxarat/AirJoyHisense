package com.fqx.anyplay.api;

import android.R.integer;
import android.app.Activity;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Environment;
import android.util.Log;
import cn.airjoy.android.service.R;

import com.fqx.airjoy.service.AJPhotoManger;
import com.fqx.anyplay.images.AirImgCache;


import java.io.File;
import java.security.PublicKey;

/**
* <p>描述: 项目中的所需要的参数缓存. </p>
* 
* @author sangwencheng
* @version 1.0
*/
public class LocalInfo {
  public static boolean APVideoisRuning = false;
  public static AirImgCache gAirImgCache;
  public static AJPhotoManger gAjPhotoManger;
  public static int gVideoSessionID;
  private static LocalInfo instance;
  public static boolean isSwitchON;
  public static boolean isSysRuning = false;
  private String mApkName;
  private String mCachePath;
  private Context mContext;
  private String mMac;
  private byte[] mMacBytes;
  private String mMode;
  private String mName;
  private int mPort;
  private int mScreenHeight;
  private int mScreenWidth;
  public int mServerState;
  private SystemConfig mSystemConfig;
  private String mVerName;
  private String mWholeCachePath;
//  private boolean misHisense;
  private int curPlayPostion;

  static {
    isSwitchON = true;
    instance = null;
  }

  private LocalInfo(Context paramContext) {
    this.mContext = paramContext;

    gAirImgCache = new AirImgCache();
    gVideoSessionID = -1;
    isSysRuning = true;
    ReadMetaData();
    setCurPlayPostion(0);
    mMacBytes = new byte[6];

    this.mServerState = APPEnum.ServiceState.Init.GetValue();
    makeCatchDir(this.mContext);
    this.mSystemConfig = new SystemConfig(paramContext);
    setMac(AnyPlayUtils.getLocalMacAddress(paramContext));
    if(g_isDefaultEnable) {
	    isSwitchON = this.mSystemConfig.getSwithcON(true); //modefy 2014-03-11   ����Ҫ���Ϊ����������
    }else{
	    isSwitchON = this.mSystemConfig.getSwithcON(false);
    }
    mSystemConfig.setDefaultName(g_isDefaultName);
    setName(this.mSystemConfig.getLocalDevName());
    String str = this.mContext.getPackageName();
      try
      {
        PackageInfo localPackageInfo = this.mContext.getPackageManager().getPackageInfo(str, 0);
        this.mVerName = localPackageInfo.versionName;
        this.mApkName = localPackageInfo.versionName;
      }
      catch (PackageManager.NameNotFoundException localNameNotFoundException)
      {
        Log.e("StartUP---", "Err: Find verCode fail");
        this.mVerName = "";
      }
  }
  
  public void setCurPlayPostion(int pos) {
	  this.curPlayPostion = pos;
	  AnyPlayUtils.LOG_DEBUG("setCurPlayPostion = " + pos);
  }
  
  public int getCurPlayPostion() {
	  return this.curPlayPostion;
  }
  
  public int g_CustomUI;
  public boolean g_IsLocalPlayer_m3u8;
  public boolean g_isDefaultEnable;
  private String g_isDefaultName;
  public String g_UseType;
  private void ReadMetaData() {
	  g_CustomUI = APPEnum.MDCustomUI.fqx.GetValue(); 
	  g_IsLocalPlayer_m3u8 = false;
	  ApplicationInfo appInfo; 
	  try {
			appInfo = this.mContext.getPackageManager().getApplicationInfo(this.mContext.getPackageName(), PackageManager.GET_META_DATA);
	  } catch (NameNotFoundException e) {
			e.printStackTrace();
			return; 
	  } 
	  g_CustomUI =appInfo.metaData.getInt("CUSTOM_UI");
	  g_IsLocalPlayer_m3u8 = appInfo.metaData.getBoolean("IS_LOCALPALYER_M3U8");
	  g_isDefaultEnable =  appInfo.metaData.getBoolean("DEFAULT_ENABLE");
	  g_isDefaultName = appInfo.metaData.getString("DEFAULT_NAME");
	  g_UseType = appInfo.metaData.getString("USE_TYPE");
  }

  public static LocalInfo getInstance(Context paramContext) {
      if (instance == null) {
    	  instance = new LocalInfo(paramContext);
      }
      return instance;
  }

  private void makeCatchDir(Context paramContext) {
    this.mWholeCachePath = "";
    if (Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED)) {
      this.mWholeCachePath = (Environment.getExternalStorageDirectory().toString() + paramContext.getResources().getString(R.string.localCatch));
    }

    this.mCachePath = paramContext.getResources().getString(R.string.localCatch);

     File localFile = new File(this.mWholeCachePath);
     if (!localFile.exists())
        localFile.mkdirs();
     return;
  }

  public void cls() {
  }

  public String getApkName() {
    return this.mApkName;
  }

  public String getCachePath() {
    return this.mCachePath;
  }

  public String getMac() {
    return this.mMac;
  }

  public String getMode() {
    return this.mMode;
  }

  public String getName() {
    return this.mName;
  }

  public int getPort() {
    return this.mPort;
  }

  public int getScreenHeight() {
    return this.mScreenHeight;
  }

  public int getScreenWidth() {
    return this.mScreenWidth;
  }

  public int getServerState() {
    return this.mServerState;
  }

  public String getVerName() {
    return this.mVerName;
  }

  public String getWholeCachePath() {
    return this.mWholeCachePath;
  }

  public void initScreen(Context paramContext) {
    this.mScreenWidth = ((Activity)paramContext).getWindow().getWindowManager().getDefaultDisplay().getWidth();
    this.mScreenHeight = ((Activity)paramContext).getWindow().getWindowManager().getDefaultDisplay().getHeight();
    Log.d("onCreate", "W=" + this.mScreenWidth + " H=" + this.mScreenHeight);
  }


  public boolean isServerFail() {
    if (this.mServerState == APPEnum.ServiceState.Fail.GetValue()) {
    	return true;
    }else{
    	return false;
    }
  }

  public void setMac(String paramString) {
    if (paramString == null) {
    	this.mMac = "00:30:1B:BA:02:DB"; 
    }else{
    	this.mMac = paramString;
    }
  }

  public void setMode(String paramString) {
    this.mMode = paramString;
  }

  public void setName(String paramString) {
    this.mName = paramString;
  }

  public void setPort(int paramInt) {
    this.mPort = paramInt;
  }

  public void setServerState(int paramInt) {
    this.mServerState = paramInt;
  }

  public byte[] getMacBytes() {
	return mMacBytes;
  }

  public void setMacBytes(byte[] mMacBytes) {
	  for(int i=0; i<6; i++) {
		this.mMacBytes[i] = mMacBytes[i];
	  }
	  mMac = AnyPlayUtils.getMacBybytes(mMacBytes);
	  Log.e("setMacBytes", " MAC:" + mMac);
  }
}
