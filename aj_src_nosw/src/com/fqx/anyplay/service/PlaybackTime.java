package com.fqx.anyplay.service;

public class PlaybackTime {
  private long duration = 0L;
  private long playPosition = 0L;

  public long getDuration() {
    return this.duration;
  }

  public long getPlayPosition() {
    return this.playPosition;
  }

  public void setDuration(long paramLong) {
    this.duration = paramLong;
  }

  public void setPlayPosition(long paramLong) {
    this.playPosition = paramLong;
  }
}
