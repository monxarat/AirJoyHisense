package com.fqx.sang.download;

import android.os.Parcel;
import android.os.Parcelable;


public class DownloadNode implements Parcelable {
	
	private int mID;
	private String mUrl;
	private String mSavaPath;
	private long mSize;
	
	private int mProgress;
	private DownloadStateCls mState;
	private volatile boolean isInterrupt;
	private int mTimes;
	private Thread  mThreadHandle;
	
	public DownloadNode(int id, String url, String spath, long size) {
		this.mID = id;
		this.mUrl = url;
		this.mSavaPath = spath;
		this.mSize = size;
		
		this.mState = new DownloadStateCls();
		this.isInterrupt = false;
		this.mThreadHandle = null;
		this.mProgress = 0;
		this.mTimes = 3;
	}
	
	public DownloadNode() {
		this.isInterrupt = false;
		this.mThreadHandle = null;
		this.mProgress = 0;
	}
	
	public int getID() {
		return this.mID;
	}
	public boolean isID(int id) {
		if(this.mID == id) {
			return true;
		}
		return false;
	}
	
	public DownloadStateCls getState() {
		return this.mState;
	}
	
	public boolean isTry() {
		mTimes--;
		if(mTimes < 0) {
			return false;
		}
		
		return true;
	}
	
	public String getUrl() {
		return this.mUrl;
	}
	
	public long getSize() {
		return this.mSize;
	}
	
	public void setProgress(int v) {
		this.mProgress = v;
	}
	
	public int getProgress() {
		return this.mProgress;
	}
	
	public String getSavePath() {
		return this.mSavaPath;
	}
	
	public void setThreadHandle(Thread handle) {
		this.mThreadHandle = handle;
	}
	
	public void stopThread() {
		this.mState.setStop();
		this.isInterrupt = true;
		if(this.mThreadHandle != null) {
			this.mThreadHandle.interrupt();
		}
	}
	
	public boolean isInterrupt() {
		return this.isInterrupt;
	}

	public int describeContents() {
		return 0;
	}
	
	public static final Parcelable.Creator<DownloadNode> CREATOR = new Creator<DownloadNode>() {    
        public DownloadNode createFromParcel(Parcel source) {    
        	DownloadNode mFileNode = new DownloadNode();    
            mFileNode.mID = source.readInt();
            mFileNode.mUrl = source.readString();    
            mFileNode.mSavaPath = source.readString();    
            mFileNode.mSize = source.readLong();
            mFileNode.mProgress = source.readInt();    
            
            return mFileNode;    
        }    
        public DownloadNode[] newArray(int size) {    
            return new DownloadNode[size];    
        }    
    };    

	public void writeToParcel(Parcel arg0, int arg1) {
		arg0.writeInt(mID);
		arg0.writeString(mUrl);
		arg0.writeString(mSavaPath);
		arg0.writeLong(mSize);
		arg0.writeInt(mProgress);
	}
}
