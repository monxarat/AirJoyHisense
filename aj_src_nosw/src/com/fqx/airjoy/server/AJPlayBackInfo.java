package com.fqx.airjoy.server;



public class AJPlayBackInfo {

	private static AJPlayBackInfo instance = null;
	private float mduration = 0.0F;
	private String mid = "";
	private String mname = "";
	private float mposition = 0.0F;
	private float mrate = 0.0F;
	private String murl = "";


	public static AJPlayBackInfo getInstance() {
		if (instance==null) {
			instance=new AJPlayBackInfo();
		}
		return instance;
	}

	public float getDuration() {
		return this.mduration;
	}

	public String getID() {
		return this.mid;
	}

	public String getName() {
		return this.mname;
	}

	public float getPositon() { 
		return this.mposition;
	}

	public float getRate() {
		return this.mrate;
	}

	public String getUrl() {
		return this.murl;
	}

	public void setDuration(float paramFloat) {
		this.mduration = paramFloat;
	}

	public void setID(String paramString) {
		this.mid = paramString;
	}

	public void setName(String paramString) {
		this.mname = paramString;
	}

	public void setPositon(float paramFloat) {
		this.mposition = paramFloat;
	}

	public void setRate(float paramFloat) {
		this.mrate = paramFloat;
	}

	public void setUrl(String paramString) {
		this.murl = paramString;
	}
}
