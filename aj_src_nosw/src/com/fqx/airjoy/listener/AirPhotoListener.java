package com.fqx.airjoy.listener;

public abstract interface AirPhotoListener {
  public abstract int cachePhoto(String paramString1, String paramString2, String paramString3, String paramString4);

  public abstract int displayCachedPhoto(String paramString);

  public abstract int displayPhoto(String paramString1, String paramString2, String paramString3, String paramString4);

  public abstract int movePhoto(String paramString, float paramFloat1, float paramFloat2);

  public abstract int rotatePhoto(String paramString, int paramInt);

  public abstract int stopPhoto();

  public abstract int zoomPhoto(String paramString, float paramFloat);
}