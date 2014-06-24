package com.fqx.anyplay.service;

import android.content.Context;
import android.os.Handler;

import com.fqx.anyplay.api.APPEnum;
import com.fqx.anyplay.api.AnyPlayUtils;
import com.fqx.anyplay.api.LocalInfo;
import com.fqx.anyplay.listener.AirPlayResultListener;

public class APResultListener implements AirPlayResultListener {
  private int R_OK = 200;
  private Handler mAirPlayHandler;
  private Handler mAirTunesHandler;
  private APController mApController;
  private LocalInfo mLocalInfo;
  private int mAPtimes = 0;
  private int mATtimes = 0;


	public APResultListener(Context paramContext, APController paramAPController) {
	    this.mApController = paramAPController;
	    this.mLocalInfo = LocalInfo.getInstance(paramContext);
	    this.mAirPlayHandler = new Handler();
	    this.mAirTunesHandler = new Handler();
		mAPtimes = 0;
		mATtimes = 0;
	}
	
	private Runnable tryStartAirTunesRunnable = new Runnable() {
		@Override
		public void run() {
		    mApController.AirTunesStop();
		    mApController.AirTunesStart(mLocalInfo.getMac(), mLocalInfo.getName(), "AppleTV2,1");
		    mApController.PublishAirTunesService();
		}
	};
	
	private Runnable tryStartAirPalyRunnable = new Runnable() {
		@Override
		public void run() {
		    mApController.AirplayStop();
		    mApController.AirplayStart(mLocalInfo.getMac(), mLocalInfo.getName(), "AppleTV2,1");
		    mApController.PublishAirplayService();
		}
	};
	
	public void onResultListener(int type, int code) {
	    AnyPlayUtils.LOG_DEBUG("AirResult", "onResultListener type:" + type + " code:" + code);
	    if(type == APPEnum.AirChannel.AirPlay.GetValue()) {
	    	if(code == R_OK) {
		      mAPtimes = 0;
		      this.mAirPlayHandler.removeCallbacks(this.tryStartAirPalyRunnable);
	    	}else{
		      this.mAirPlayHandler.removeCallbacks(this.tryStartAirPalyRunnable);
		      if(mAPtimes < 6){
		    	  this.mAirPlayHandler.postDelayed(this.tryStartAirPalyRunnable, 1000L);
		      }
		      mAPtimes++;
	    	}
	    	
	    }else if(type == APPEnum.AirChannel.AirTunes.GetValue()) {
	    	if(code == R_OK) {
		      mATtimes = 0;
		      this.mAirTunesHandler.removeCallbacks(this.tryStartAirTunesRunnable);
	    	}else{
		      this.mAirTunesHandler.removeCallbacks(this.tryStartAirTunesRunnable);
		      if(mATtimes < 6){
			      this.mAirTunesHandler.postDelayed(this.tryStartAirTunesRunnable, 1000L);
		      }
		      mATtimes++;
	    	}
	
	    }
	}

}
