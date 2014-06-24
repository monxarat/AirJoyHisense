package com.fqx.anyplay.service;

public class PlaybackInfo
{
  private static PlaybackInfo instance = null;
  private volatile int duration = 0;
  private volatile int loaded_duration = 0;
  private volatile int loaded_startPosition = 0;
  private volatile int playPosition = 0;
  private boolean playbackBufferEmpty = true;
  private boolean playbackBufferFull = false;
  private boolean playbackLikelyToKeepUp = true;
  private volatile int rate = 1;
  private boolean readyToPlay = true;
  private volatile int seek_duration = 0;
  private volatile int seek_startPosition = 0;

  public static PlaybackInfo getInstance() {
      if (instance == null) {
        instance = new PlaybackInfo();
      }
      return instance;
  }

  public int get_duration() {
    return this.duration;
  }

  public int get_loaded_duration() {
    return this.loaded_duration;
  }

  public int get_loaded_startPosition() {
    return this.loaded_startPosition;
  }

  public int get_playPosition() {
    return this.playPosition;
  }

  public boolean get_playbackBufferEmpty() {
    return this.playbackBufferEmpty;
  }

  public boolean get_playbackBufferFull() {
    return this.playbackBufferFull;
  }

  public boolean get_playbackLikelyToKeepUp() {
    return this.playbackLikelyToKeepUp;
  }

  public int get_rate() {
    return this.rate;
  }

  public boolean get_readyToPlay() {
    return this.readyToPlay;
  }

  public int get_seek_duration() {
    return this.seek_duration;
  }

  public int get_seek_startPosition() {
    return this.seek_startPosition;
  }

  public void setDurValues(int paramInt) {
    this.duration = paramInt;
    this.seek_duration = paramInt;
    this.loaded_duration = paramInt;
  }

  public void set_duration(int paramInt) {
    this.duration = paramInt;
  }

  public void set_loaded_duration(int paramInt) {
    this.loaded_duration = paramInt;
  }

  public void set_loaded_startPosition(int paramInt) {
    this.loaded_startPosition = paramInt;
  }

  public void set_playPosition(int paramInt) {
    this.playPosition = paramInt;
  }

  public void set_playbackBufferEmpty(boolean paramBoolean) {
    this.playbackBufferEmpty = paramBoolean;
  }

  public void set_playbackBufferFull(boolean paramBoolean) {
    this.playbackBufferFull = paramBoolean;
  }

  public void set_playbackLikelyToKeepUp(boolean paramBoolean) {
    this.playbackLikelyToKeepUp = paramBoolean;
  }

  public void set_rate(int paramInt) {
    this.rate = paramInt;
  }

  public void set_readyToPlay(boolean paramBoolean) {
    this.readyToPlay = paramBoolean;
  }

  public void set_seek_duration(int paramInt) {
    this.seek_duration = paramInt;
  }

  public void set_seek_startPosition(int paramInt) {
    this.seek_startPosition = paramInt;
  }
}