package com.fqx.anyplay.service;

import android.content.Context;
import android.content.Intent;
import android.media.AudioFormat;
import android.os.Bundle;
import android.os.Handler;

import cn.airjoy.android.service.AudioView;

import com.fqx.anyplay.api.APPEnum;
import com.fqx.anyplay.api.AnyPlayUtils;
import com.fqx.anyplay.api.PublishState;
import com.fqx.anyplay.listener.AirTunesServerListener;
import com.fqx.audio.player.AudioParam;
import com.fqx.audio.player.AudioPlayer;

public class APAirTunesServerListener implements AirTunesServerListener
{
  private static APAirTunesServerListener instance = null;
  private boolean isAirTunesEnable;
  private boolean is_show = false;
  private AudioParam mAudioParam = new AudioParam();
  private AudioPlayer mAudioPlayer = new AudioPlayer();
  private int mChannel = APPEnum.AirChannel.AirPlay.GetValue();
  private int mCmd;
  private Context mContext;
  private Handler mHandler;
  private PublishState mPublishState = PublishState.getInstance();
//  private Runnable mavRunnable = new APAirTunesServerListener.1(this);

	public APAirTunesServerListener(Context paramContext) {
	    this.mContext = paramContext;
	    this.is_show = false;
	    this.mHandler = new Handler();
	    this.isAirTunesEnable = false;
	}
	
	public static APAirTunesServerListener getInstance(Context paramContext) {
	      if (instance == null) {
	        instance = new APAirTunesServerListener(paramContext);
	      }
	      return instance;
	}
	
	private Runnable mavRunnable = new Runnable() {
		@Override
		public void run() {
			  Intent localIntent = new Intent(mContext, AudioView.class);
		      Bundle localBundle = new Bundle();
		      localBundle.putInt("CMD", mCmd);
		      localIntent.putExtras(localBundle);
		      localIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		      mContext.startActivity(localIntent);
			  is_show = true;
		      AnyPlayUtils.LOG_DEBUG("E_Time=" + System.currentTimeMillis());
		}
	};
	
	private void sendIntent(int paramInt) {
	    this.mCmd = paramInt;
	    if (!this.is_show) {
	      AnyPlayUtils.LOG_DEBUG("S_Time=" + System.currentTimeMillis());
	      this.mHandler.removeCallbacks(this.mavRunnable);
	      this.mHandler.postDelayed(this.mavRunnable, 2000L);
	    }else {
	      this.mHandler.removeCallbacks(this.mavRunnable);
	      this.mHandler.post(this.mavRunnable);
	    }
	}
	
	public void didAudioDriverInit(int hz, int ch, int bit) {
	    if (!this.isAirTunesEnable) { 
	    	return; 
	    }
	    AnyPlayUtils.LOG_DEBUG("didAudioDriverInit: HZ=" + hz + " CH=" + ch + " BIT=" + bit);
	    if (AnyPlayUtils.is_anyplay) {
		      AnyPlayUtils.LOG_DEBUG("Anyplay isTrue");
	    	  return;
	    }
	    this.mAudioParam.mFrequency = hz;
	    if(ch == 1) {
	    	mAudioParam.mChannel = AudioFormat.CHANNEL_CONFIGURATION_MONO;
	    }else{
	    	mAudioParam.mChannel = AudioFormat.CHANNEL_CONFIGURATION_STEREO;
	    }
	    
	    if((bit == 16) || (bit == 32)) {
	    	mAudioParam.mSampBit = AudioFormat.ENCODING_PCM_16BIT;
	    }else{
	    	mAudioParam.mSampBit = AudioFormat.ENCODING_DEFAULT;
	    }
	    this.mAudioPlayer.setAudioParam(this.mAudioParam);
	    this.mAudioPlayer.play();
	    this.is_show = false;
	    AnyPlayUtils.gAudioInfo.imgData = null;
	    this.mPublishState.UpdateResInfo("", "", "");
	    this.mPublishState.SetMediaAudio(APPEnum.EventState.EventStatePlaying.GetValue());
	    sendIntent(APPEnum.AirAudioCmd.didSetAudioInfo.GetValue());
	}
	
	public void didAudioDriverPlay(byte[] paramArrayOfByte, long paramLong) {
	    if (!this.isAirTunesEnable) {
	    	return;
	    }
	    if (AnyPlayUtils.is_anyplay) {
	    	return;
	    }
	    this.mAudioPlayer.setDataSource(paramArrayOfByte);
	}
	
	public void didPause(String paramString) {
	    if (!this.isAirTunesEnable) {
	    	return;
	    }
	    AnyPlayUtils.LOG_DEBUG("didPause ");
	    if (AnyPlayUtils.is_anyplay) {
	        AnyPlayUtils.LOG_DEBUG("Anyplay isTrue");
	        return;
	    }
	    this.mAudioPlayer.pause();
	    sendIntent(APPEnum.AirAudioCmd.didPause.GetValue());
	}
	
	public void didSetAudioInfo(String paramString1, String paramString2, String paramString3) {
	    if (!this.isAirTunesEnable) {
	    	return;
	    }
	      AnyPlayUtils.gAudioInfo.nameString = paramString1;
	      AnyPlayUtils.gAudioInfo.artString = paramString2;
	      AnyPlayUtils.gAudioInfo.albumString = paramString3;
	      AnyPlayUtils.LOG_DEBUG("didSetAudioInfo: Name=" + paramString1 + " Artist=" + paramString2 + " Albun=" + paramString3);
	      if (AnyPlayUtils.is_anyplay) {
	        AnyPlayUtils.LOG_DEBUG("Anyplay isTrue");
	        return;
	      }
	      sendIntent(APPEnum.AirAudioCmd.didSetAudioInfo.GetValue());
	}
	
	public void didSetImage(byte[] paramArrayOfByte, long paramLong) {
	    if (!this.isAirTunesEnable) {
	    	return;
	    }
	    AnyPlayUtils.LOG_DEBUG("didSetImage: len=" + paramArrayOfByte.length);
	    AnyPlayUtils.gAudioInfo.imgData = paramArrayOfByte;
	    if (AnyPlayUtils.is_anyplay) {
	        return;
	    }
	    AnyPlayUtils.LOG_DEBUG("Anyplay isTrue");
	}
	
	public void didSetVolume(float paramFloat) {
	    if (!this.isAirTunesEnable) {
	    	return;
	    }
	    AnyPlayUtils.LOG_DEBUG("didSetVolume: value=" + paramFloat);
	}
	
	public void didStartPlayAudio(String paramString) {
	    if (!this.isAirTunesEnable) {
	    	return;
	    }
	      AnyPlayUtils.LOG_DEBUG("didStartPlayAudio");
	      if (AnyPlayUtils.is_anyplay) {
	        AnyPlayUtils.LOG_DEBUG("Anyplay isTrue");
	        return;
	      }
	      com.fqx.anyplay.images.AirImgCache.sg_img_data = null;
	      this.mPublishState.SetChannel(this.mChannel, paramString);
	}
	
	public void didStop(String paramString) {
	    if (!this.isAirTunesEnable) {
	    	return;
	    }
	      AnyPlayUtils.LOG_DEBUG("didStop");
	      if (AnyPlayUtils.is_anyplay) {
	        AnyPlayUtils.LOG_DEBUG("Anyplay isTrue");
	        return;
	      }
	      this.mAudioPlayer.tryStop();
	      if (this.is_show) {
	        sendIntent(APPEnum.AirAudioCmd.didStop.GetValue());
	      }
	      this.is_show = false;
	}
	
	public void doStop() {
	    if (!this.isAirTunesEnable) {
	    	return;
	    }
	      if (AnyPlayUtils.is_anyplay) {
	    	  return;
	      }
	      this.mHandler.removeCallbacks(this.mavRunnable);
	      AnyPlayUtils.is_anyplay = true;
	      AnyPlayUtils.LOG_DEBUG("TryStop");
	      try
	      {
	        this.mAudioPlayer.stop();
	        this.mPublishState.SetMediaAudio(APPEnum.EventState.EventStateStopped.GetValue());
	      } catch (Exception e) {
	    	  e.printStackTrace();
	      }
	      this.is_show = false;
	}
	
	public void tryStop() {
	    if (!this.isAirTunesEnable) {
	    	return;
	    }
	     this.mHandler.removeCallbacks(this.mavRunnable);
	     AnyPlayUtils.is_anyplay = true;
	     AnyPlayUtils.LOG_DEBUG("TryStop");
	      try
	      {
	        this.mAudioPlayer.stop();
	        if (this.is_show)  {
	        	sendIntent(APPEnum.AirAudioCmd.didStop.GetValue());
	        }
	      } catch (Exception e) {
	    	  e.printStackTrace();
	      }
	}
}