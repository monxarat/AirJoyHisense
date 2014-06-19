package com.fqx.anyplay.api;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Environment;
import android.os.StatFs;
import android.util.Log;
import android.widget.Toast;
import cn.airjoy.android.service.R;

import com.fqx.audio.player.AudioInfo;

import java.io.File;
import java.net.URLEncoder;
import java.util.Random;

public class AnyPlayUtils {
  public static String ACTION_HX_EXIT;
  public static String ACTION_SERVER_SLIDESHOW;
  public static String ACTION_SERVER_STATE;
  public static int NodeDelayTimes = 3;
  public static String fuckString;
  public static AudioInfo gAudioInfo;
  public static volatile boolean is_anyplay;
  private static String mMacString;

  static {
    ACTION_SERVER_STATE = "com.fqx.airjoy.state";
    ACTION_SERVER_SLIDESHOW = "com.fqx.airjoy.slideshows";
    ACTION_HX_EXIT = "are.you.ready";
    is_anyplay = false;
    gAudioInfo = new AudioInfo();
    mMacString = null;
  }

  private static boolean mIsDebug = true;
	public static void LOG_DEBUG(String paramString) {
		if(mIsDebug == false) return;
	    Log.d("AirJoy Debug", paramString);
	}
	
	public static void LOG_DEBUG(String paramString1, String paramString2) {
		if(mIsDebug == false) return;
	    Log.d(paramString1, paramString2);
	}
	
	public static void LOG_ERR(String paramString1, String paramString2) {
		if(mIsDebug == false) return;
	    Log.e(paramString1, paramString2);
	}

	public static void LOG_ERR(String paramString2) {
		if(mIsDebug == false) return;
	    Log.e("AirJoy Err", paramString2);
	}
	
	public static void SendServerState(Context paramContext, int paramInt) {
	    Intent localIntent = new Intent();
	    Bundle localBundle = new Bundle();
	    localBundle.putInt("State", paramInt);
	    localIntent.setAction(ACTION_SERVER_STATE);
	    localIntent.putExtras(localBundle);
	    paramContext.sendBroadcast(localIntent);
	}

	  public static void ShowMsgBox(Context mContext, String title) {
			AlertDialog.Builder builder = new AlertDialog.Builder(mContext); 
			String btnTxt = mContext.getResources().getString(R.string.ok);
			builder.setMessage(title) 
			       .setCancelable(false) 
			       .setPositiveButton(btnTxt, new DialogInterface.OnClickListener() { 
			           public void onClick(DialogInterface dialog, int id) { 
			                dialog.cancel(); 
			           } 
			 }) ;
			AlertDialog alert = builder.create(); 
			alert.show();
	 }	 




	public static void checkDirIsExist(String paramString) {
	    File localFile = new File(paramString);
	    if (localFile.exists())
	      return;
	    localFile.mkdir();
	}
	  
	  
	public static void delAllFiles(File file){
	      if(file.isFile()){
	          file.delete();
	          return;
	      }
	      if(file.isDirectory()){
	          File[] childFile = file.listFiles();
	          if(childFile == null || childFile.length == 0){
	              file.delete();
	              return;
	          }
	          for(File f : childFile){
	        	  delAllFiles(f);
	          }
	          file.delete();
	      }
	}
	public static void delAllFiles(String filePath){
		delAllFiles(new File(filePath));
	}
	
	public static boolean delfile(String paramString) {
	      File localFile = new File(paramString);
	      if (localFile.exists()) {
	    	  localFile.delete();
	    	  return true;
	      }
	      return false;
	}
	
	public static String getLocalMacAddress(Context paramContext) {
	    return ((WifiManager)paramContext.getSystemService(Context.WIFI_SERVICE)).getConnectionInfo().getMacAddress();
	}
	
	public static String getRandomMac() {
	    if (mMacString != null) {
	    	return mMacString;
	    }
	    Random localRandom = new Random(System.currentTimeMillis());
	    int[] arrayOfInt = new int[6];
	    for (int i = 0; i<6; i++) {
	      arrayOfInt[i] = localRandom.nextInt(99);
	    }

	    Object[] arrayOfObject = new Object[6];
	    arrayOfObject[0] = Integer.valueOf(arrayOfInt[0]);
	    arrayOfObject[1] = Integer.valueOf(arrayOfInt[1]);
	    arrayOfObject[2] = Integer.valueOf(arrayOfInt[2]);
	    arrayOfObject[3] = Integer.valueOf(arrayOfInt[3]);
	    arrayOfObject[4] = Integer.valueOf(arrayOfInt[4]);
	    arrayOfObject[5] = Integer.valueOf(arrayOfInt[5]);
	    mMacString = String.format("%02d:%02d:%02d:%02d:%02d:%02d", arrayOfObject);
	    Log.d("getRandomMac", "MAC=" + mMacString);
	    return mMacString;
	}
  
//  public static String getSuffixFromPath(String path) {
//		 return path.substring(path.lastIndexOf(".")+1);
//  }
//	 

	public static String getsuffix(String paramString, int paramInt) {
	    int i = paramString.lastIndexOf('.');
	    if (i == -1) {
	    	return " ";
	    }
	    
	    return paramString.substring(i+paramInt);
	}
	
	public static long getAvailaleSize(){
		  File path = Environment.getExternalStorageDirectory(); //取得sdcard文件路径
		  StatFs stat = new StatFs(path.getPath()); 
		  long blockSize = stat.getBlockSize(); 
		  long availableBlocks = stat.getAvailableBlocks();
		  return availableBlocks * blockSize; 
	  }

		   
	  public static boolean isEnoughSize(long paramLong) {
		  long gsize = getAvailaleSize();
		  if(gsize > paramLong) {
			  return true;
		  }else{
			  return false;
		  }
	  }
	  
	public static boolean isExistSDCard() {
	    if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
	    	return true;
	    }
	    return false;
	}
	
	public static boolean isFileExists(String paramString) {
	    if (paramString == null) {
	    	return false;
	    }
	    return new File(paramString).exists();
	}

//  public static boolean isNetOK0(Context paramContext)
//  {
//    int i = 1;
//    ConnectivityManager localConnectivityManager = (ConnectivityManager)paramContext.getSystemService(Context.CONNECTIVITY_SERVICE);
//    NetworkInfo localNetworkInfo1 = localConnectivityManager.getNetworkInfo(9);
//    Log.d("isNetOK", "NetInfo" + localNetworkInfo1);
//    if ((localNetworkInfo1 != null) && (localNetworkInfo1.isConnected()));
//    while (true)
//    {
//      return i;
//      NetworkInfo localNetworkInfo2 = localConnectivityManager.getNetworkInfo(i);
//      if ((localNetworkInfo2 != null) && (localNetworkInfo2.isConnected()))
//        continue;
//      PhoneNetInfo localPhoneNetInfo = new PhoneNetInfo(paramContext);
//      if (localPhoneNetInfo.isAPMode(localPhoneNetInfo.getLocalIpAddress()))
//        continue;
//      i = 0;
//    }
//  }

	public static boolean isNetOK(Context paramContext) {
	    ConnectivityManager localConnectivityManager = (ConnectivityManager)paramContext.getSystemService(Context.CONNECTIVITY_SERVICE);
	    NetworkInfo localNetworkInfo1 = localConnectivityManager.getNetworkInfo(ConnectivityManager.TYPE_ETHERNET);
	    Log.d("isNetOK", "NetInfo" + localNetworkInfo1);
	    if ((localNetworkInfo1 != null) && (localNetworkInfo1.isConnected())) {
	    	return true;
	    }	
	    NetworkInfo localNetworkInfo2 = localConnectivityManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
	    if ((localNetworkInfo2 != null) && (localNetworkInfo2.isConnected())) {
	    	return true;
	    }
	    PhoneNetInfo localPhoneNetInfo = new PhoneNetInfo(paramContext);
	    if (localPhoneNetInfo.isAPMode()) {
	    	return true;
	    }
	    return false;
	}

	public static String myURLEncoder(String url) throws Exception {
		String enUrl;
		enUrl = URLEncoder.encode(url, "UTF-8");
		enUrl = enUrl.replaceAll("%3A", ":");
		enUrl = enUrl.replaceAll("%2F", "/");
		enUrl = enUrl.replaceAll("\\+", "%20");
		
		return enUrl;
	}

	public static void send2Other(Context paramContext) {
	}
	
	public static void sendVideoState(Context paramContext, int paramInt) {
	    Intent localIntent = new Intent();
	    Bundle localBundle = new Bundle();
	    localBundle.putInt("ServiceCmd", APPEnum.ServiceCmd.SessionEvent.GetValue());
	    localBundle.putInt("VideoState", paramInt);
	    localIntent.setAction("anyplay.service.callevent");
	    localIntent.putExtras(localBundle);
	    paramContext.sendBroadcast(localIntent);
	}
	
	public static void showNote(Context mContext, String info) {
			Toast toast;
			toast = Toast.makeText(mContext, info, Toast.LENGTH_SHORT);
			toast.show();
	}
}
