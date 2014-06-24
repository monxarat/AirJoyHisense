package com.fqx.airjoy.server;


import com.fqx.airjoy.listener.AirAudioListener;
import com.fqx.airjoy.listener.AirJoyNsdDiscoveryListener;
import com.fqx.airjoy.listener.AirJoyNsdPublishListener;
import com.fqx.airjoy.listener.AirMediaListener;
import com.fqx.airjoy.listener.AirPhotoListener;
import com.fqx.airjoy.listener.AirVideoListener;

public class AirJoy {

	public AirJoy() {
	    native_setup();
	}
	
	private final native void native_finalize();
	
	public static final native void native_init();
	
	private final native void native_setup();
	
	public static native boolean publishMediaEvent(String paramString1, String paramString2, String paramString3, String paramString4, String paramString5, String paramString6);
	
	public native void ServerStart();
	
	public native void clsListenner();
	
	public native void config(String paramString1, String paramString2, int paramInt);
	
	protected void finalize() {
	    native_finalize();
	}
	
	public native void nsdStart();
	
	public native void publishService();
	
	public native void setAudioListener(AirAudioListener paramAirAudioListener);
	
	public native void setMediaListener(AirMediaListener paramAirMediaListener);
	
	public native void setNsdDiscoveryListener(AirJoyNsdDiscoveryListener paramAirJoyNsdDiscoveryListener);
	
	public native void setNsdPublishListener(AirJoyNsdPublishListener paramAirJoyNsdPublishListener);
	
	public native void setPhotoListener(AirPhotoListener paramAirPhotoListener);
	
	public native void setVideoListener(AirVideoListener paramAirVideoListener);
	
	public native void stop();
}

/* Location:           E:\AirJoy\AnyPlay152\classes_dex2jar.jar
 * Qualified Name:     com.fqx.airjoy.server.AirJoy
 * JD-Core Version:    0.5.4
 */