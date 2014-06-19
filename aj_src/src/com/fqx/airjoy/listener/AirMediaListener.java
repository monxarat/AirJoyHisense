package com.fqx.airjoy.listener;

public abstract interface AirMediaListener {
  public abstract void closeSession(String paramString);

  public abstract int getVolume();

  public abstract void initSession(String paramString);

  public abstract int setVolume(int paramInt);
}