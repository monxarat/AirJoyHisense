package com.fqx.sang.download;

import java.io.File;   
import java.io.FileOutputStream;   
import java.io.IOException;
import java.io.InputStream;   
import java.net.URL;   
import java.net.URLConnection;   
import java.util.ArrayList;
import java.util.concurrent.atomic.AtomicInteger;



  
import android.content.Context;
import android.os.Handler;   
import android.os.Message;   
import android.util.Log;

class HttpDownloadID {
	private int mid;
	private AtomicInteger m_download_state;
	
	public HttpDownloadID(int id, int state) {
		this.mid = id;
		m_download_state = new AtomicInteger(state);
	}
	
	public int getState() {
		return this.m_download_state.get();
	}
	public int getID() {
		return this.mid;
	}
	
	public void setState(int state) {
		this.m_download_state.set(state);
	}
	
	public boolean isState(int state) {
		if(this.m_download_state.get() == state)
			return true;
		else
			return false;
	}
	
	public boolean isID(int id) {
		if(this.mid == id)
			return true;
		else
			return false;
	}
}



public class HttpAsyncDownload {   
	private int sg_downloadTask_num;
	private ArrayList<DownloadNode> sg_download_list;
	
	
	
    public HttpAsyncDownload(Context context, int TaskNum) {   
	//	this.sg_download_state_list = new ArrayList<HttpDownloadID>();
		this.sg_download_list = new ArrayList<DownloadNode>();
		this.sg_downloadTask_num = TaskNum;
		
    }   
    
    public void setMaxTaskNum(int num) {
    	this.sg_downloadTask_num = num;
    }
    
    public DownloadNode loadDownloadTask(int id, String url, String spath, long size) {

    	DownloadNode dn = new DownloadNode(id, url, spath, size);
    	dn.getState().setReady();
    	this.sg_download_list.add(dn);
    	//Log.e("loadDownloadTask", "Name=" + dn.getSavePath());
    	
    	return dn;
    }
    
    public ArrayList<DownloadNode>  getDownloadList() {
    	return this.sg_download_list;
    }
    
    
    public boolean stopDownloadTask(int id) {
		for(DownloadNode tmp:this.sg_download_list) {
    		if((tmp.isID(id)) && (tmp.getState().isDoing())) {
    			tmp.stopThread();
    			return true;
    		}
    	}    	return false;    }
    
    
    
    public boolean stopAllDownloadTask() {
		for(DownloadNode tmp:this.sg_download_list) {
    		if(tmp.getState().isDoing()) {
    			tmp.stopThread();
    		}
    		tmp.getState().setStop();
    	}
    	return false;
    }
    
    public int getTaskNum() {
    	int i=0;
	 	for(DownloadNode tmp:this.sg_download_list) {
    		if(tmp.getState().isDoing()) {
    			i++;
    		}
    	}   	
	 	
	 	return i;
    }
    
    
	 public boolean clearDownloadTask() {
		int j = this.sg_download_list.size();
		for(int i=0; i<j; i++) {
			if(sg_download_list.get(i).getState().isNeedBeCleaned()) {
				sg_download_list.remove(i);
				j--;
				i--;
			}
		}	//	Log.e("clearDownloadTask", "Size=" + sg_download_list.size());
    	return true;
    }    
    public void startDownLoadTask() {
    	int n = this.sg_downloadTask_num;
		for(DownloadNode tmp:this.sg_download_list) {
//			if(tmp.isReady()) {
			if(tmp.getState().isReady()) {
				this.bulidDownloadTask(tmp);
				n--;
			}
			if(n == 0){
				return;
			}
		}
    }
     public boolean tryAgainDownloadByTimes(int id) {
 		for(DownloadNode tmp:this.sg_download_list) {
			if(tmp.isID(id)) {
				if(tmp.isTry()) {
					tmp.getState().setReady();
					this.bulidDownloadTask(tmp);
					return true;
				}else{
					return false;
				}
			}
		}   	
		return false;
    }   
     
    public void tryAgainDownload(int id) {
 		for(DownloadNode tmp:this.sg_download_list) {
			if(tmp.isID(id)) {
				tmp.getState().setReady();
				this.bulidDownloadTask(tmp);
			}
		}   	
    }
    public boolean addDownLoadTask() {
		for(DownloadNode tmp:this.sg_download_list) {
			if(tmp.getState().isReady()) {
				this.bulidDownloadTask(tmp);
				return true;
			}
		}
		//Log.e("addDownLoadTask", "--------------false");
		
		return false;
    }
    private boolean checkCompleteAll() {
		for(DownloadNode tmp:this.sg_download_list) {
			if(tmp.getState().isComplete()==false) {
				return false;
			}
		}		return true;
    }
  
    private boolean deleteFile(String name) {
    	File file = new File(name);
		boolean result = false;
		if (file != null) {
			try {
				File file2 = file;
				file2.delete();
				result = true;
			}
			catch (Exception e) {
				e.printStackTrace();
				result = false;
			}
		}
		return result;
	} 
    
  
    EventHandler mHandler = new EventHandler(this);   
  
    public int bulidDownloadTask(final DownloadNode dn) {   
    	final int id = dn.getID();
    	final String url = dn.getUrl();
    	final String savePath = dn.getSavePath();
        dn.getState().setDoing();
        boolean ret = DLUtils.isSDSize(dn.getSize());
    	if(ret == false) {
    		stopAllDownloadTask();
		    sendMessage(FILE_DOWNLOAD_ERROR, id, DLEnum.DownloadState.S_SD_FULL_ERR.GetValue());   
    		DLUtils.LOG_Error("SD is Full");
    		return -1;
    	}
    	
    	Thread ThreadHanle =  new Thread(new Runnable() {   
    		InputStream inputStream = null;
    		FileOutputStream outputStream = null;
		    
            public void run() {   
                try {   
                    URL sourceUrl = new URL(url);   
                    URLConnection conn = sourceUrl.openConnection();   
                    inputStream = conn.getInputStream();   
                    
                    long rSize = conn.getContentLength();   
                    
                    sendMessage(FILE_DOWNLOAD_CONNECT, id);   
                    
                    File savefile = new File(savePath);   
                    if (savefile.exists()) {   
                        savefile.delete();   
                    }   
                    savefile.createNewFile();   
                    outputStream = new FileOutputStream(savePath);   
                    byte[] buffer = new byte[8192];  
                    long readCount = 0;   
                    int readNum = 0;   
				    long mJSTime = System.currentTimeMillis();
                    while (readCount < rSize && readNum != -1) {   
                        readNum = inputStream.read(buffer);   
                        if (readNum > -1) {   
                            outputStream.write(buffer,0,readNum);
                            readCount = readCount + readNum;   
                            long nowTime = System.currentTimeMillis();
					    	if(nowTime - mJSTime > 500) {
					    		sendSizeMessage(FILE_DOWNLOAD_SIZE, id, readCount);   
					    		mJSTime = nowTime;
					    	}
                        }   
                        
                        if(dn.isInterrupt()) {
                        	break;
                        }
                    }   
  
                    inputStream.close();
	                outputStream.close();   
                    if(dn.isInterrupt()) {
                    	deleteFile(savePath);
	                	dn.getState().setStop();
	                    sendMessage(FILE_DOWNLOAD_STOP, id, " ");   
                    }else {
	                    sendSizeMessage(FILE_DOWNLOAD_SIZE, id, rSize);   
	                	dn.getState().setComplete();
                        dn.setProgress(100);
	                    sendMessage(FILE_DOWNLOAD_COMPLETE, id, savePath);   
                    }
                } catch (Exception e) {   
                	if(inputStream != null) {
						try {
							inputStream.close();
						} catch (IOException e1) {
							e1.printStackTrace();
						}
                	}
                	if(outputStream != null) {
						try {
							outputStream.close();
		                    deleteFile(savePath);
						} catch (IOException e1) {
							e1.printStackTrace();
						}
                	}
                	dn.getState().setErr();
	                sendMessage(FILE_DOWNLOAD_ERROR, id, DLEnum.DownloadState.S_ERR.GetValue());   
                    Log.e("MyError2", e.toString());   
                }   
            }   
        });
    	dn.setThreadHandle(ThreadHanle);
    	ThreadHanle.start();
        
        return 1;
    }   
   

  
    // 锟斤�?Handler 锟斤拷锟斤拷锟斤拷息   
    private void sendMessage(int what, int id, Object obj) {   
        // 锟斤拷锟斤拷锟斤拷要锟斤�?Handler 锟斤拷锟酵碉拷锟斤拷�?  
//        Message msg = mHandler.obtainMessage(what, id, obj);   
        Message msg = mHandler.obtainMessage(what, id, id, obj);
        mHandler.sendMessage(msg);   
    }   
    
    private void sendSizeMessage(int what, int id, long size) {   
    	Long lsize = Long.valueOf(size); 
        Message msg = mHandler.obtainMessage(what, id, id, lsize);
        mHandler.sendMessage(msg);   
    }   
 
  
    private void sendMessage(int what, int id) {   
        Message msg = mHandler.obtainMessage(what, id);   
        mHandler.sendMessage(msg);   
    }   
  
    private void sendMessage(int what, int arg1, int arg2) {   
        Message msg = mHandler.obtainMessage(what, arg1, arg2);   
        mHandler.sendMessage(msg);   
    }   
  
    private static final int FILE_DOWNLOAD_ERROR = -1;   
    private static final int FILE_DOWNLOAD_CONNECT = 0;   
    private static final int FILE_DOWNLOAD_UPDATE = 1;   
    private static final int FILE_DOWNLOAD_COMPLETE = 2;   
    private static final int FILE_DOWNLOAD_STOP = 3;   
    private static final int FILE_DOWNLOAD_SIZE = 5;   
  //  private static final int FILE_DOWNLOAD_COMPLETE_ALL = 4;   
  
    // 锟皆讹拷锟斤拷锟�Handler   
    private class EventHandler extends Handler {   
        private HttpAsyncDownload mManager;   
  
        public EventHandler(HttpAsyncDownload manager) {   
            mManager = manager;   
        }   
  
        // 锟斤拷锟斤拷锟斤拷盏锟斤拷锟斤拷锟较�? 
        @Override  
        public void handleMessage(Message msg) {   
        	boolean ret;
        	try {
	            switch (msg.what) {   
	            case FILE_DOWNLOAD_CONNECT:   
	                if (mOnDownloadConnectListener != null){
	                    mOnDownloadConnectListener.onDownloadConnect(mManager, msg.arg1);   
	                }
	                break;   
	            case FILE_DOWNLOAD_UPDATE:   
	                if (mOnDownloadUpdateListener != null)   
	                    mOnDownloadUpdateListener.onDownloadUpdate(mManager,msg.arg1, msg.arg2);   
	                break;   
	            case FILE_DOWNLOAD_SIZE:   
	                if (mOnDownloadSizeListener != null) {   
	                	Long a = (Long) msg.obj;
	                    mOnDownloadSizeListener.onDownloadSize(mManager, msg.arg1, a.longValue());   
	                }
	                break;   
	            case FILE_DOWNLOAD_COMPLETE:   
		      //     Log.e("bulidDownloadTask", "0 FILE_DOWNLOAD_COMPLETE id =" + msg.arg1);
	                if (mOnDownloadCompleteListener != null)  { 
	                    mOnDownloadCompleteListener.onDownloadComplete(mManager, msg.arg1, msg.obj);   
	                }
		            ret = addDownLoadTask();
		            if((ret == false) && (mOnDownloadCompleteAllListener != null)) {
		            	  if(checkCompleteAll()) {
							  mOnDownloadCompleteAllListener.OnDownloadCompleteAll(mManager);
//			            	  Log.i("checkCompleteAll", "--------------OK 0");
		            	  }
		            }
	                break;   
	            case FILE_DOWNLOAD_ERROR:   
	            	int did = msg.arg1;
	            	int state = msg.arg2;
	            	if(state == DLEnum.DownloadState.S_SD_FULL_ERR.GetValue()) {
	            		if (mOnDownloadErrorListener != null) {
			                 mOnDownloadErrorListener.onDownloadError(mManager,did, state);   
		                }
	            		break;
	            	}
	                boolean isok= tryAgainDownloadByTimes(did);
                	if(isok == false) {
			            if (mOnDownloadErrorListener != null) {
			                 mOnDownloadErrorListener.onDownloadError(mManager,did, state);   
		                }
			            ret = addDownLoadTask();
			            if((ret == false) && (mOnDownloadCompleteAllListener != null)) {
			            	  if(checkCompleteAll()) {
								  mOnDownloadCompleteAllListener.OnDownloadCompleteAll(mManager);
	//			            	  Log.e("checkCompleteAll", "--------------OK 1");
			            	  }
			            }
                	}else{
                		DLUtils.LOG_Debug("tryAgainDownloadByTimes id:" + did);
                	}

	                break;   
	            case FILE_DOWNLOAD_STOP:   
	                if (mOnDownloadStopListener != null)   
	                	mOnDownloadStopListener.onDownloadStop(mManager,msg.arg1, msg.obj);   
		            ret = addDownLoadTask();
		            if((ret == false) && (mOnDownloadCompleteAllListener != null)) {
		            	  if(checkCompleteAll()) {
							  mOnDownloadCompleteAllListener.OnDownloadCompleteAll(mManager);
//			            	  Log.e("checkCompleteAll", "--------------OK 1");
		            	  }
		            }
	                break;   
	            default:   
	                break;   
	            }   
        	}catch(Exception e) {   
        		Log.e("handleMessage", e.toString());
        	}
        }   
    }   
  
    // 锟斤拷锟斤拷锟斤拷锟斤拷锟铰硷拷   
    private OnDownloadConnectListener mOnDownloadConnectListener;   
    public interface OnDownloadConnectListener {   
        void onDownloadConnect(HttpAsyncDownload manager, int id);   
    }   
    public void setOnDownloadConnectListener(OnDownloadConnectListener listener) {   
        mOnDownloadConnectListener = listener;   
    }   
  
    // 锟斤拷锟斤拷锟斤拷锟截斤拷雀锟斤拷锟斤拷录锟�? 
    private OnDownloadUpdateListener mOnDownloadUpdateListener;   
    public interface OnDownloadUpdateListener {   
        void onDownloadUpdate(HttpAsyncDownload manager, int percent, int id);   
    }   
    public void setOnDownloadUpdateListener(OnDownloadUpdateListener listener) {   
        mOnDownloadUpdateListener = listener;   
    }   
    
    private OnDownloadSizeListener mOnDownloadSizeListener;   
    public interface OnDownloadSizeListener {   
        void onDownloadSize(HttpAsyncDownload manager, int id, long result);   
    }   
    public void setOnDownloadSizeListener(OnDownloadSizeListener listener) {   
    	mOnDownloadSizeListener = listener;   
    }   
  
    // 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷录锟�? 
    private OnDownloadCompleteListener mOnDownloadCompleteListener;   
    public interface OnDownloadCompleteListener {   
        void onDownloadComplete(HttpAsyncDownload manager, int id, Object result);   
    }   
    public void setOnDownloadCompleteListener(OnDownloadCompleteListener listener) {   
        mOnDownloadCompleteListener = listener;   
    }   
  
    // 锟斤拷锟斤拷锟斤拷锟斤拷锟届常锟铰硷�?  
    private OnDownloadErrorListener mOnDownloadErrorListener;   
    public interface OnDownloadErrorListener {   
        void onDownloadError(HttpAsyncDownload manager, int id, int errState);   
    }   
    public void setOnDownloadErrorListener(OnDownloadErrorListener listener) {   
        mOnDownloadErrorListener = listener;   
    }   
    
 // 锟斤拷锟斤拷锟斤拷锟斤拷停止锟铰硷拷   
    private OnDownloadStopListener mOnDownloadStopListener;   
    public interface OnDownloadStopListener {   
        void onDownloadStop(HttpAsyncDownload manager, int id,  Object info);   
    }   
    public void setOnDownloadStopListener(OnDownloadStopListener listener) {   
        mOnDownloadStopListener = listener;   
    }   
    
     // 锟斤拷锟斤拷锟斤拷全锟斤拷锟截斤拷锟斤拷锟铰硷拷   
    private OnDownloadCompleteAllListener mOnDownloadCompleteAllListener;   
    public interface OnDownloadCompleteAllListener {   
        void OnDownloadCompleteAll(HttpAsyncDownload manager);   
    }   
    public void setOnDownloadCompleteAllListener(OnDownloadCompleteAllListener listener) {   
    	mOnDownloadCompleteAllListener = listener;   
    } 
}   

