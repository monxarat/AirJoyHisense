package com.fqx.audio.player;

import android.media.AudioManager;
import android.media.AudioTrack;
import com.fqx.anyplay.api.AnyPlayUtils;

public class AudioPlayer {
  private volatile boolean isTryStop = false;
  private AudioParam mAudioParam;
  private AudioTrack mAudioTrack;
  private volatile boolean misPlay = false;
  private volatile boolean misStop = true;

	private void createAudioTrack() throws Exception {
	    int minBufSize = AudioTrack.getMinBufferSize(
	    		this.mAudioParam.mFrequency, 
	    		this.mAudioParam.mChannel, 
	    		this.mAudioParam.mSampBit);
	    this.mAudioTrack = new AudioTrack(
	    		AudioManager.STREAM_MUSIC, 
	    		this.mAudioParam.mFrequency, 
	    		this.mAudioParam.mChannel, 
	    		this.mAudioParam.mSampBit, 
	    		minBufSize, 
	    		AudioTrack.MODE_STREAM);
	    this.mAudioTrack.flush();
	    this.mAudioTrack.play();
	}
	
	public void pause() {
	    this.mAudioTrack.pause();
	}
	
	public boolean play() {
	    try {
	      createAudioTrack();
	      this.misStop = false;
	      this.misPlay = true;
	      this.isTryStop = false;
	      return true;
	    } catch (Exception localException) {
	      localException.printStackTrace();
	    }
	    return false;
	}
	
	public void setAudioParam(AudioParam paramAudioParam) {
	    this.mAudioParam = paramAudioParam;
	}
	
	public void setDataSource(byte[] paramArrayOfByte) {
	    if (this.misStop) {
	      AnyPlayUtils.LOG_DEBUG("setDataSource AudioPlayer is Stop");
	      return;
	    }
	      this.mAudioTrack.write(paramArrayOfByte, 0, paramArrayOfByte.length);
	      if (this.isTryStop) {
		      stop();
	      }
	}
	
	public boolean stop() {
	    this.misStop = true;
	    if (!this.misPlay) {
	    	return true;
	    }
	    if (this.mAudioTrack != null) {
	        this.mAudioTrack.flush();
	        this.mAudioTrack.stop();
	        this.mAudioTrack.release();
	    }
	    this.misPlay = false;
	    return true;
	}
	
	public void tryStop() {
	    this.isTryStop = true;
	}
	
	  class PlayAudioThread extends Thread
	  {
	    PlayAudioThread()
	    {
	    }
	
	    public void run()
	    {
	    }
	  }
}
