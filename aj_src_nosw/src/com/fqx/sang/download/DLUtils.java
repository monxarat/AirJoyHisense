package com.fqx.sang.download;

import java.io.File;

import android.os.Environment;
import android.os.StatFs;
import android.util.Log;

public class DLUtils
{
  public static void LOG_Debug(String paramString)
  {
    Log.d("SangDowload", paramString);
  }

  public static void LOG_Debug(String paramString1, String paramString2)
  {
    Log.d(paramString1, paramString2);
  }

  public static void LOG_Error(String paramString)
  {
    Log.e("QiHiShare", paramString);
  }

  public static void LOG_Error(String paramString1, String paramString2)
  {
    Log.e(paramString1, paramString2);
  }

  public static long getAvailaleSize(){
	  File path = Environment.getExternalStorageDirectory(); //取得sdcard文件路径
	  StatFs stat = new StatFs(path.getPath()); 
	  long blockSize = stat.getBlockSize(); 
	  long availableBlocks = stat.getAvailableBlocks();
	  return availableBlocks * blockSize; 
  }

	   
  public static boolean isSDSize(long paramLong) {
	  long gsize = getAvailaleSize();
	  LOG_Error("gsize = " + gsize);
	  if(gsize > paramLong) {
		  return true;
	  }else{
		  return false;
	  }
  }
}

/* Location:           E:\AirJoy\AnyPlay152\classes_dex2jar.jar
 * Qualified Name:     com.fqx.sang.download.DLUtils
 * JD-Core Version:    0.5.4
 */