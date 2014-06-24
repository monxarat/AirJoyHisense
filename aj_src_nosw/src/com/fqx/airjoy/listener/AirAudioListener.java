package com.fqx.airjoy.listener;

public abstract interface AirAudioListener {

	public abstract int getPlayAudioProgress(String paramString, float paramFloat1, float paramFloat2);
	
	public abstract int playAudio(String paramString1, String paramString2, String paramString3, float paramFloat, String paramString4);
	
	public abstract int setPlayAudioProgress(String paramString, float paramFloat);
	
	public abstract int setPlayAudioRate(String paramString, float paramFloat);
	
	public abstract int stopPlayAudio(String paramString);
}