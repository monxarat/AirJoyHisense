
package com.fqx.sang.download;

import java.util.ArrayList;



import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Binder;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;

public class DLService extends Service {
	
	private static final String TAG = "DownloadServer";  
	public static final String ACTION_DOWNLOAD_RESULT_NOTE = "action.Anypush.ServerDownload.Result";  
//	public static final String ActionDownloadAddNode = "action.Anypush.ServerDownload.AddNode";  
	private DownloadBinder mBinder = new DownloadBinder(); 
	private HttpAsyncDownload g_HttpAsyncDownload;
    private DownloadResult g_DownloadResult;
    private static int sg_download_session_id;
	private long g_time_ms;
	private boolean g_DownloadResultBroadcast;
	private int mMaxTask;
	private volatile boolean g_DownloadCompleteAllFlag;
	
	@Override
	public IBinder onBind(Intent arg0) {
		// TODO Auto-generated method stub
		Log.i(TAG, "start IBinder~~~");  
		return mBinder;
	}
	
	@Override  
    public void onCreate() {  
       // Log.e(TAG, "start onCreate~~~");  
        super.onCreate();
        Log.i(TAG, "start onCreate~~~");
        g_HttpAsyncDownload = new HttpAsyncDownload(this, 1);
        g_DownloadResult = new DownloadResult(this, g_HttpAsyncDownload);
        sg_download_session_id = -1002;
		this.g_time_ms = System.currentTimeMillis();
		g_DownloadResultBroadcast = false;
		g_DownloadCompleteAllFlag = true;
		DLUtils.LOG_Debug("DLServer running...");
    }  
    
    @Override  
    public void onStart(Intent intent, int startId) {  
        super.onStart(intent, startId); 
    }  
      
    @Override  
    public void onDestroy() {  
        super.onDestroy();  
    }   
      
      
    @Override  
    public boolean onUnbind(Intent intent) {  
        return super.onUnbind(intent);  
    }  
     
    
    
    public DownloadResult getDownloadResult() {
    	return this.g_DownloadResult;
    }
     
//	private void sendAddNode(DownloadNode adn) {
//		Intent intent = new Intent();
//        Bundle bundle = new Bundle();
//        bundle.putParcelable("AddNode", adn);
//	    intent.setAction(ActionDownloadAddNode);
//	    intent.putExtras(bundle);
//	    this.sendBroadcast(intent);
//	}
    public class DownloadBinder extends Binder{  
    	public HttpAsyncDownload getDownloadService(){  
            return g_HttpAsyncDownload;  
        }  
    	
    	public void startDownload(boolean isNotice) {
	    	
    		g_HttpAsyncDownload.startDownLoadTask();
    	}
    	
    	public void showBackstageDownloadNotity() {
    	}
    	
    	public void stopBackstageDownloadNotity() {
    	}
    	
    	public void setDownloadMaxTaskNum(int num) {
    		mMaxTask = num;
			g_DownloadCompleteAllFlag = true;
    		g_HttpAsyncDownload.setMaxTaskNum(num);
    	}
    	
    	public ArrayList<DownloadNode>  getDownloadlist() {
    		return g_HttpAsyncDownload.getDownloadList();
    	}
    	
    	public int loadDownloadTask(String url, String spath, long size, int sid) {
    		int id;
    		if(sid < 0) {
	    		id = sg_download_session_id - 1;
    		} else {
    			id = sid;
    		}
    		DownloadNode dn = g_HttpAsyncDownload.loadDownloadTask(id, url, spath, size);
    		if(dn == null) {
    			return -1;
    		}
    		sg_download_session_id = id;
    		
//    		if(g_DownloadCompleteAllFlag) {
//				g_DownloadCompleteAllFlag = false;
//			    g_HttpAsyncDownload.startDownLoadTask();
//		    	Sang.LOG_Debug("startDownLoadTask id=" +id);
//	   		}
    		
    		int runningTask = g_HttpAsyncDownload.getTaskNum();
    		if(runningTask < mMaxTask) {
			    g_HttpAsyncDownload.addDownLoadTask();
    		}
    		
    		return id;
    	}
    	
    	public boolean stopDownloadTask(int id) {
    		return g_HttpAsyncDownload.stopDownloadTask(id);
    	}
    	public boolean stopAllDownloadTask() {
    		return g_HttpAsyncDownload.stopAllDownloadTask();
    	}
    	public boolean isCompleteAll() {
    		return g_DownloadCompleteAllFlag;
    	}
    	
    	public void setDownloadResultBroadcast(boolean isEnable) {
    		g_DownloadResultBroadcast = isEnable;
    	}
    	
    	public void tryAgainDownload(int id) {
    		g_HttpAsyncDownload.tryAgainDownload(id);
    	}
    	public void clearCompletedTask() {
    		g_HttpAsyncDownload.clearDownloadTask();
    	}
    }
    
	
	private class DownloadResult implements
		HttpAsyncDownload.OnDownloadCompleteListener,   
		HttpAsyncDownload.OnDownloadUpdateListener,   
		HttpAsyncDownload.OnDownloadStopListener,
		HttpAsyncDownload.OnDownloadCompleteAllListener,
		HttpAsyncDownload.OnDownloadSizeListener,
		HttpAsyncDownload.OnDownloadErrorListener {
		
		
		private Context mContext;
	
		public DownloadResult(Context cont, HttpAsyncDownload manager) {
			manager.setOnDownloadCompleteListener(this);   
			manager.setOnDownloadUpdateListener(this); 
			manager.setOnDownloadErrorListener(this);
			manager.setOnDownloadStopListener(this);
			manager.setOnDownloadSizeListener(this);
			manager.setOnDownloadCompleteAllListener(this);
			
			this.mContext = cont;
		}
		
		public void onDownloadError(HttpAsyncDownload manager, int id, int errState) {
			
			DLUtils.LOG_Error("onDownloadError -------------- 1");
		    	Intent intent = new Intent();
		        Bundle bundle = new Bundle();
		        bundle.putInt("ResultCMD", DLEnum.DownloadResult.ResultCodeError.GetValue());
		        bundle.putInt("ResultID", id);
		        bundle.putInt("ErrState", errState);
			    intent.setAction(ACTION_DOWNLOAD_RESULT_NOTE);
			    intent.putExtras(bundle);
			    this.mContext.sendBroadcast(intent);
		}
	
		public void onDownloadUpdate(HttpAsyncDownload manager, int percent,
				int id) {
			//Log.e("onDownloadUpdate", "Progress=" + percent);
			long time=System.currentTimeMillis();
	    	if((time - g_time_ms < 300) && (percent != 100))
	    		return;
	    	g_time_ms = time;
	    	
	    	
			if(g_DownloadResultBroadcast) {
		    	Intent intent = new Intent();
		        Bundle bundle = new Bundle();
		        bundle.putInt("ResultCMD", DLEnum.DownloadResult.ResultCodeProgress.GetValue());
		        bundle.putInt("ResultID", id);
		        bundle.putInt("ResultProgress", percent);
			    intent.setAction(ACTION_DOWNLOAD_RESULT_NOTE);
			    intent.putExtras(bundle);
			    this.mContext.sendBroadcast(intent);
			}
		}
	
		public void onDownloadComplete(HttpAsyncDownload manager, int id,
				Object result) {
			
		   //   Log.e("onDownloadComplete", "1 onDownloadComplete id =" + id);
		    	Intent intent = new Intent();
		        Bundle bundle = new Bundle();
		        bundle.putInt("ResultCMD", DLEnum.DownloadResult.ResultCodeComplete.GetValue());
		        bundle.putInt("ResultID", id);
			    intent.setAction(ACTION_DOWNLOAD_RESULT_NOTE);
			    intent.putExtras(bundle);
			    this.mContext.sendBroadcast(intent);
		}

		public void OnDownloadCompleteAll(HttpAsyncDownload manager) {
			// TODO Auto-generated method stub
		    	Intent intent = new Intent();
		        Bundle bundle = new Bundle();
		        bundle.putInt("ResultCMD", DLEnum.DownloadResult.ResultCodeCompleteALL.GetValue());
			    intent.setAction(ACTION_DOWNLOAD_RESULT_NOTE);
			    intent.putExtras(bundle);
			    this.mContext.sendBroadcast(intent);
				g_DownloadCompleteAllFlag = true;
		}

		public void onDownloadStop(HttpAsyncDownload manager, int id,
				Object info) {
		    	Intent intent = new Intent();
		        Bundle bundle = new Bundle();
		        bundle.putInt("ResultCMD", DLEnum.DownloadResult.ResultCodeStop.GetValue());
		        bundle.putInt("ResultID", id);
			    intent.setAction(ACTION_DOWNLOAD_RESULT_NOTE);
			    intent.putExtras(bundle);
			    this.mContext.sendBroadcast(intent);
		}

		public void onDownloadSize(HttpAsyncDownload manager, int id, long size) {
				Intent intent = new Intent();
		        Bundle bundle = new Bundle();
		        bundle.putInt("ResultCMD", DLEnum.DownloadResult.ResultCodeSize.GetValue());
		        bundle.putInt("ResultID", id);
		        bundle.putLong("ResultSize", size);
			    intent.setAction(ACTION_DOWNLOAD_RESULT_NOTE);
			    intent.putExtras(bundle);
			    this.mContext.sendBroadcast(intent);
		}
	}
	
}
