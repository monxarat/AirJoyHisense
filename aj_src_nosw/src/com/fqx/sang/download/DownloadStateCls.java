package com.fqx.sang.download;

public class DownloadStateCls {
	private int mState;
	
	public DownloadStateCls() {
		this.mState = DLEnum.DownloadState.S_INIT.GetValue();
	}
	
	public int getValue() {
		return this.mState;
	}
	public void setState(int state) {
		this.mState = state;
	}
	
	
	public void setReady() {
		this.mState = DLEnum.DownloadState.S_READY.GetValue();
	}
	public boolean isReady() {
		if(this.mState == DLEnum.DownloadState.S_READY.GetValue()) {
			return true;
		}
		return false;
	}
	
	public void setComplete() {
		this.mState = DLEnum.DownloadState.S_COMPLETE.GetValue();
	}
	public boolean isComplete() {
		if(this.mState == DLEnum.DownloadState.S_COMPLETE.GetValue()) {
			return true;
		}
		return false;
	}
	
	
	public void setErr() {
		this.mState = DLEnum.DownloadState.S_ERR.GetValue();
	}
	public boolean isErr() {
		if(this.mState == DLEnum.DownloadState.S_ERR.GetValue()) {
			return true;
		}
		return false;
	}
	
	
	public void setStop() {
		this.mState = DLEnum.DownloadState.S_STOP.GetValue();
	}
	public boolean isStop() {
		if(this.mState == DLEnum.DownloadState.S_STOP.GetValue()) {
			return true;
		}
		return false;
	}
	
	public void setDoing() {
		this.mState = DLEnum.DownloadState.S_DOING.GetValue();
	}
	public boolean isDoing() {
		if(this.mState == DLEnum.DownloadState.S_DOING.GetValue()) {
			return true;
		}
		return false;
	}
	
	public boolean isNeedBeCleaned() {
		if((this.mState == DLEnum.DownloadState.S_COMPLETE.GetValue()) || 
		   (this.mState == DLEnum.DownloadState.S_STOP.GetValue()) 	||
		   (this.mState == DLEnum.DownloadState.S_ERR.GetValue())) {
			return true;
		}
		return false;
	}
	
	
}
