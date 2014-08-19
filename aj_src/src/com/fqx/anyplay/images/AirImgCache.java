package com.fqx.anyplay.images;

import android.util.Log;

/**
* <p>描述: 推送图片的缓存，最多缓存4张，AirPlay推送的会设定那张图片需要缓存，每张缓存的图片都有唯一的ID，可以通过ID调出要显示的图片. </p>
* 
* @author sangwencheng
* @version 1.0
*/
public class AirImgCache {
  private static final int mCacheSize = 4;
  private static long mtimes = 0L;
  public static byte[] sg_img_data;
  private ImgNode[] mImgNode = new ImgNode[4];

  public AirImgCache() {
    for (int i = 0; i<mCacheSize; i++) {
      this.mImgNode[i] = new ImgNode();
    }
    cls();
  }

  
  private int getNewID() {
		for(int i=0; i<mCacheSize; i++) {
			if (this.mImgNode[i].id == -1L) {
				return i;
			}
		}
		
		int j = 0;
		long t = this.mImgNode[0].id;
		for(int i=0; i<mCacheSize; i++) {
			if (this.mImgNode[i].id < t) {
				t = this.mImgNode[i].id;
				j = i;
			}
		}
		return j;
	}
  
 

  private long getTimes() {
    mtimes = 1L + mtimes;
    return mtimes;
  }

  public void add(String paramString, byte[] paramArrayOfByte) {
    int i = getNewID();
    if (i < 0) {
      Log.e("AirImgCache", "Err: add getNewID=-1");
      return;
    }
      this.mImgNode[i].IDString = paramString;
      this.mImgNode[i].pImgData = paramArrayOfByte;
      this.mImgNode[i].id = getTimes();
  }

  public void cls() {
    mtimes = 0L;
    for (int i = 0; i<4; i++) {
      this.mImgNode[i].IDString = "";
      this.mImgNode[i].pImgData = null;
      this.mImgNode[i].id = -1L;
    }
  }

  public byte[] getImgData(String paramString) {
    for (int i = 0; i<4; i++) {
       if (this.mImgNode[i].IDString.equals(paramString)) {
	        this.mImgNode[i].id = getTimes();
	        return this.mImgNode[i].pImgData;
       }
    }
    return null;
  }

}

