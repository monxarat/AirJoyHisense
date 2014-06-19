package com.fqx.airjoy.server;

import android.util.Log;

public class ServerInfo {
  private String mID;
  private String mIP;
  private String mName;
  private int mPort;
  private int mType;

  public String getID() {
    return this.mID;
  }

  public String getIP() {
    return this.mIP;
  }

  public String getName() {
    return this.mName;
  }

  public int getPort() {
    return this.mPort;
  }

  public int getType() {
    return this.mType;
  }

  public void printInfo(String paramString) {
    Log.e(paramString, "Name=" + this.mName + " ID=" + this.mID + " IP=" + this.mIP + " Type=" + this.mType + " Port=" + this.mPort);
  }

  public void setID(String paramString) {
    this.mID = paramString;
  }

  public void setIP(String paramString) {
    this.mIP = paramString;
  }

  public void setName(String paramString) {
    this.mName = paramString;
  }

  public void setPort(int paramInt)
  {
    this.mPort = paramInt;
  }

  public void setType(int paramInt)
  {
    this.mType = paramInt;
  }
}

/* Location:           E:\AirJoy\AnyPlay152\classes_dex2jar.jar
 * Qualified Name:     com.fqx.airjoy.server.ServerInfo
 * JD-Core Version:    0.5.4
 */