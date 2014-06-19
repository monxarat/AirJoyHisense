package com.fqx.anyplay.api;

import android.content.Context;
import android.content.SharedPreferences;
import android.util.Log;

public class SystemConfig {
  private static final String SwitchON = "SwitchON";
  private static final String cfgFile = "airpaly_config";
  private static final String localDevName = "localDevName";
  private SharedPreferences.Editor editor;
  private Context mContext;
  private static String mNameString;
  private SharedPreferences sp;

  public SystemConfig(Context paramContext) {
    this.mContext = paramContext;
    this.sp = this.mContext.getSharedPreferences(cfgFile, Context.MODE_PRIVATE);
    this.editor = this.sp.edit();
  }

  public void setDefaultName(String name) {
	  mNameString = name;
  }
 

  public String getLocalDevName() {
    return this.sp.getString(localDevName, mNameString);
  }

  public boolean getSwithcON(boolean defaultV) {
    return this.sp.getBoolean(SwitchON, defaultV);
  }

  public void setLocalDevName(String paramString) {
    this.editor.putString(localDevName, paramString);
    this.editor.commit();
  }

  public void setSwitchON(boolean paramBoolean) {
    this.editor.putBoolean(SwitchON, paramBoolean);
    this.editor.commit();
    LocalInfo.isSwitchON = paramBoolean;
  }
}
