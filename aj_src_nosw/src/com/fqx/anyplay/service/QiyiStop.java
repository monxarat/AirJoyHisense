package com.fqx.anyplay.service;

import android.os.Handler;

public class QiyiStop {
	
	private Handler mHandler = new Handler();
	private boolean mQiyiStopEnable = true;

	public QiyiStop() {
		mQiyiStopEnable = true;
	}
	
	private Runnable mRunnable = new Runnable() {
		@Override
		public void run() {
			mQiyiStopEnable = true;
		}
	};

	public void tryDelayStop(String url) {
		mQiyiStopEnable = true;
		if(url.contains("video.qiyi.com") == false) {
			mQiyiStopEnable = false;
		}
		mHandler.postDelayed(mRunnable, 3000);
	}
	
	public boolean isQiyiStop() {
		return mQiyiStopEnable;
	}

}
