package com.fqx.airjoy.service;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Environment;
import android.os.IBinder;
import android.util.Log;
import com.fqx.anyplay.api.AnyPlayUtils;
import com.fqx.sang.download.DLEnum;
import com.fqx.sang.download.DLService;
import com.fqx.sang.download.DLUtils;


public class AJPhotoManger {
  private static final int TASKINIT = 0;
  private static final int TASKOK = 2;
  private static final int TASKSTART = 1;
  private static final int mCacheSize = 4;
  private static long mtimes = 0L;
  private String exPathString;
  private Context mContext;
  private DLService.DownloadBinder mDownloadBinder = null;
  private ImgNode[] mImgList;
  private boolean misExistSDCard;

  public AJPhotoManger(Context paramContext) {
    this.mContext = paramContext;
    initImgList();
    initDownloadServer();
    this.misExistSDCard = AnyPlayUtils.isExistSDCard();
    if (this.misExistSDCard) {
    	this.exPathString = Environment.getExternalStorageDirectory().getAbsolutePath();
    }else{
    	this.exPathString = this.mContext.getCacheDir().getPath();
    }
  }

  
  private BroadcastReceiver downloadResultEvt = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			Bundle bundle = intent.getExtras();
			int cmd = bundle.getInt("ResultCMD");
			if(cmd == DLEnum.DownloadResult.ResultCodeError.GetValue()) {
//				int id = bundle.getInt("ResultID");
//				int errState = bundle.getInt("ErrState");
			}else if(cmd == DLEnum.DownloadResult.ResultCodeComplete.GetValue()) {
				int id = bundle.getInt("ResultID");
				setTaskComplete(id);
			}else if(cmd == DLEnum.DownloadResult.ResultCodeStop.GetValue()) {
//				int id = bundle.getInt("ResultID");
			}else if(cmd == DLEnum.DownloadResult.ResultCodeCompleteALL.GetValue()) {
			}
		}
	};
	
	

  
	private ServiceConnection downloadConnection = new ServiceConnection() {

		public void onServiceConnected(ComponentName arg0, IBinder service) {
			mDownloadBinder = (DLService.DownloadBinder)service;
			mDownloadBinder.setDownloadMaxTaskNum(2);
			mDownloadBinder.setDownloadResultBroadcast(true);
			DLUtils.LOG_Debug("downloadConnection is ok");
		}

		public void onServiceDisconnected(ComponentName arg0) {
			mDownloadBinder = null;
		}  
	};
	private void addImgNode(String paramString1, String paramString2) {
	    int i = getNewID();
	    if (i < 0) {
	      Log.e("AirImgCache", "Err: add getNewID=-1");
	      return;
	    }
	    this.mImgList[i].mID = paramString1;
	    this.mImgList[i].mPath = paramString2;
	    this.mImgList[i].isReady = true;
	    this.mImgList[i].mState = TASKOK;
	    this.mImgList[i].mDownloadID = 0L;
	    this.mImgList[i].id = getTimes();
	}
	
	private int getNewID() {
		for(int i=0; i<mCacheSize; i++) {
			if (this.mImgList[i].id == -1L) {
				return i;
			}
		}
		
		int j = 0;
		long t = this.mImgList[0].id;
		for(int i=0; i<mCacheSize; i++) {
			if (this.mImgList[i].id < t) {
				t = this.mImgList[i].id;
				j = i;
			}
		}
		
		return j;
	}
	
	private int getNewImgNode(String paramString1, String paramString2, long paramLong) {
	    int i = getNewID();
	    if (i < 0) {
	      Log.e("AirImgCache", "Err: add getNewID=-1");
	      return  -1;
	    }
	    this.mImgList[i].mID = paramString1;
	    this.mImgList[i].mPath = paramString2;
	    this.mImgList[i].isReady = false;
	    this.mImgList[i].mState = TASKSTART;
	    this.mImgList[i].mDownloadID = paramLong;
	    this.mImgList[i].id = getTimes();
	    if (this.misExistSDCard == false) {
		    AnyPlayUtils.delfile(this.mImgList[i].mPath);
	    }
	    
	    return i;
	}
	
	private long getTimes() {
	    mtimes = 1L + mtimes;
	    return mtimes;
	}
	
	private void initDownloadServer() {
	    Intent localIntent = new Intent(this.mContext, DLService.class);
	    this.mContext.startService(localIntent);
	    this.mContext.bindService(localIntent, this.downloadConnection, 1);
	    IntentFilter localIntentFilter = new IntentFilter();
	    localIntentFilter.addAction(DLService.ACTION_DOWNLOAD_RESULT_NOTE);
	    this.mContext.registerReceiver(this.downloadResultEvt, localIntentFilter);
	}
	
	private void initImgList() {
	    this.mImgList = new ImgNode[mCacheSize];
	    for (int i = 0; i<mCacheSize; i++) {
	      this.mImgList[i] = new ImgNode();
	    }
	}
	
	private void mycls() {
	    mtimes = 0L;
	    for (int i = 0; i<mCacheSize; i++) {
	      this.mImgList[i].mID = "";
	      this.mImgList[i].mPath = "";
	      this.mImgList[i].isReady = false;
	      this.mImgList[i].mState = TASKINIT ;
	      this.mImgList[i].mDownloadID = 0L;
	      this.mImgList[i].id = -1L;
	    }
	}
	
	private void setTaskComplete(long paramLong) {
	    for (int i = 0; i<mCacheSize; i++) {
	      if (this.mImgList[i].mDownloadID != paramLong) {
	        continue;
	      }
	      this.mImgList[i].mState = TASKOK;
	      this.mImgList[i].isReady = true;
	    }
	}
	
	public void cls() {
	    mycls();
	    Log.i("AJPhotoManger", "Clear");
	}
	
	public void exit() {
	    if (this.mDownloadBinder != null) {
	      this.mContext.unbindService(this.downloadConnection);
	    }
	    this.mContext.unregisterReceiver(this.downloadResultEvt);
	}
	
	protected void finalize() {
	}
	
	
	public String getImgPath(String paramString) {
		for(int i=0; i<mCacheSize; i++){
			if (!this.mImgList[i].mID.equals(paramString)) {
				continue;
			}
	        if (!this.mImgList[i].isReady) {
	        	return null;
	        }
	        this.mImgList[i].id = getTimes();
	        return this.mImgList[i].mPath;
		}
		
		return "NOID";
	
	}
	public void tryBulidDownloadTask(String paramString1, String paramString2, String paramString3, String paramString4) {
	    String str1, str2;
	    str1 = this.exPathString + paramString3;
	    str2 = str1 + paramString4;
	    if (this.misExistSDCard) {
	      if (AnyPlayUtils.isFileExists(str2)) {
		      addImgNode(paramString2, str2);
		      Log.d("tryBulidDownloadTask", " File exists id=" + paramString2);
		      return;
	      }
	    }
	
	    String str4;
		try {
			str4 = AnyPlayUtils.myURLEncoder(paramString1);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			str4 = paramString1;
			e.printStackTrace();
		}
	    int i = this.mDownloadBinder.loadDownloadTask(str4, str2, 0L, -1);
	    if (i == -1) {
	        AnyPlayUtils.LOG_ERR("tryBulidDownloadTask", "SD is full");
	    	return;
	    }
	    getNewImgNode(paramString2, str2, i);
	}
}

