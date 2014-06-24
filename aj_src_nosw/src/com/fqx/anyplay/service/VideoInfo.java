package com.fqx.anyplay.service;

public class VideoInfo {
  private static VideoInfo instance = null;
  private volatile int duration = 0;
  private volatile int position = 0;

  public static VideoInfo getInstance() {
      if (instance == null) {
        instance = new VideoInfo();
      }
      return instance;
  }

  public int getDuration() {
    return this.duration;
  }

  public int getPosition() {
    return this.position;
  }

  public void setCur(int paramInt) {
    this.position = paramInt;
  }

  public void setValue(int paramInt1, int paramInt2) {
    this.duration = paramInt1;
    this.position = paramInt2;
  }
}