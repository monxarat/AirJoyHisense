package com.fqx.anyplay.service;

import android.content.Context;

public class APController extends AirplayListener {
	private int APControllerFlag;
	private Context mContext;
	
	public APController(Context paramContext) {
	    super(paramContext);
	    this.mContext = paramContext;
	    native_setup();
	}
	
	public static native void PublishEvent(int paramInt1, int paramInt2, String paramString);
	
	private final native void native_finalize();
	
	public static final native void native_init();
	
	private final native void native_setup();
	
	public native int AirTunesStart(String paramString1, String paramString2, String paramString3);
	
	public native boolean AirTunesStop();
	
	public native int AirplayStart(String paramString1, String paramString2, String paramString3);
	
	public native boolean AirplayStop();
	
	public int Airplay_Stop() {
	    AirplayStop();
	    AirTunesStop();
	    return 1;
	}
	
	public native boolean AirTunesPublishService(boolean isTrue);
	public native boolean AirplayPublishService(boolean isTrue);

	public native boolean PublishAirTunesService();
	
	public native boolean PublishAirplayService();
	
	public native void SetAirPlayListener(APResultListener paramAPResultListener);
	
	public native void SetAirTunesServerListener(APAirTunesServerListener paramAPAirTunesServerListener);
	
	protected void finalize() {
	    native_finalize();
	}
	
	public native int getAirplayPort();
	
	public native void getSlideshowsPicture(String paramString);
	
	public native boolean mdnsd_start(int paramInt);
	
	public native boolean mdnsd_stop();
	
	public native void setNSDRunning(boolean paramBoolean);
}
