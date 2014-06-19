package com.fqx.anyplay.listener;

public interface AirTunesServerListener { 

	public void didAudioDriverInit(int paramInt1, int paramInt2, int paramInt3);

	public void didAudioDriverPlay(byte[] paramArrayOfByte, long paramLong);
	
	public void didPause(String paramString);
	
	public void didSetAudioInfo(String paramString1, String paramString2, String paramString3);
	
	public void didSetImage(byte[] paramArrayOfByte, long paramLong);
	
	public void didSetVolume(float paramFloat);
	
	public void didStartPlayAudio(String paramString);
	
	public void didStop(String paramString);
}
