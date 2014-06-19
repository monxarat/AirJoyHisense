package com.fqx.airjoy.server;

public class AJVideoInfo
{
  private static AJVideoInfo instance = null;
  private float m_postion = 0.0F;
  private float m_rate = 0.0F;

  public static AJVideoInfo getInstance() {
		if (instance==null) {
			instance=new AJVideoInfo();
		}
		return instance;
	}


  public float getPostion() {
    return this.m_postion;
  }

  public float getRate() {
    return this.m_rate;
  }

  public void setPostion(float paramFloat) {
    this.m_postion = paramFloat;
  }

  public void setRate(float paramFloat) {
    this.m_rate = paramFloat;
  }
}
