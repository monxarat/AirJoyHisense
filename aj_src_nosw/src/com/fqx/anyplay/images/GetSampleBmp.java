package com.fqx.anyplay.images;

import java.io.File;

import android.content.ContentResolver;
import android.content.Context;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Rect;
import android.provider.MediaStore;
import android.provider.MediaStore.Images;
import android.util.Log;

public class GetSampleBmp {

	public static int sg_bmp_width;
	public static int sg_bmp_hight;
	
	private static int computeSampleSize(BitmapFactory.Options options, int minSideLength, int maxNumOfPixels) {
	    int initialSize = computeInitialSampleSize(options, minSideLength, maxNumOfPixels);
	    int roundedSize;
	    
	    if (initialSize <= 8) { 
	    	roundedSize = 1;
	        while (roundedSize < initialSize) {
	            roundedSize <<= 1;
	        }
	    } else {
	        roundedSize = (initialSize + 7) / 8 * 8;
	    }
	    if (roundedSize > 3) { 
	    	roundedSize = roundedSize / 2;
	    }

	    Log.e("computeSampleSize", "roundedSize=" + roundedSize);
	    return roundedSize;
	}



	 private static Rect getScreenRegion(int width , int height) {  
		  return new Rect(0,0,width,height);  
	}  
	 
	private static int computeInitialSampleSize(BitmapFactory.Options options, int minSideLength, int maxNumOfPixels) {
	    double w = options.outWidth;
	    double h = options.outHeight;

	    int lowerBound = (maxNumOfPixels == -1) ? 1 : (int) Math.ceil(Math.sqrt(w * h / maxNumOfPixels));
	    int upperBound = (minSideLength == -1) ? 128: (int) Math.min(Math.floor(w / minSideLength), Math.floor(h / minSideLength));
	    if (upperBound < lowerBound) {
	        return lowerBound;
	    }

	    if ((maxNumOfPixels == -1) && (minSideLength == -1)) {
	        return 1;
	    } else if (minSideLength == -1) {
	        return lowerBound;
	    } else {
	        return upperBound;
	    }
	}	
	
	public static Bitmap creatbyByteArray(byte[] pdata, int sw, int sh) {
		BitmapFactory.Options opts = new BitmapFactory.Options();
		opts.inJustDecodeBounds = true;
		//BitmapFactory.decodeFile(file.getAbsolutePath(), opts);
		BitmapFactory.decodeByteArray(pdata, 0, pdata.length, opts);
		
		sg_bmp_width = opts.outWidth;
		sg_bmp_hight = opts.outHeight;
	//	Log.e("Bitmap0", "W=" + sg_bmp_width + " H=" + sg_bmp_hight);
		 Rect r = getScreenRegion(sw,sh);  
		   int w = r.width();  
		   int h = r.height();  
		   int maxSize = w > h ? w : h;  

		opts.inSampleSize = computeSampleSize(opts, maxSize, w*h);
		opts.inJustDecodeBounds = false;
		try {
			Bitmap bmp = BitmapFactory.decodeByteArray(pdata, 0, pdata.length, opts);
//			Log.e("Bitmap1", "W=" + bmp.getWidth() + " H=" + bmp.getHeight());
			return bmp;
		 } catch (OutOfMemoryError err) {
			return null;
		 }
	}
	
	
	public static Bitmap creat(File file, int w0, int h0) {
		BitmapFactory.Options opts = new BitmapFactory.Options();
		opts.inJustDecodeBounds = true;
		BitmapFactory.decodeFile(file.getAbsolutePath(), opts);
		
		sg_bmp_width = opts.outWidth;
		sg_bmp_hight = opts.outHeight;
	//	Log.e("Bitmap0", "W=" + sg_bmp_width + " H=" + sg_bmp_hight);
		 Rect r = getScreenRegion(w0,h0);  
		   int w = r.width();  
		   int h = r.height();  
		   int maxSize = w > h ? w : h;  

		opts.inSampleSize = computeSampleSize(opts, maxSize, w*h);
		opts.inJustDecodeBounds = false;
		try {
			Bitmap bmp = BitmapFactory.decodeFile(file.getAbsolutePath(), opts);
//			Log.e("Bitmap1", "W=" + bmp.getWidth() + " H=" + bmp.getHeight());
			return bmp;
		 } catch (OutOfMemoryError err) {
			return null;
		 }
	}
	
	public static Bitmap getContentImagesThumbnails(Context mContext,String fpath) {
		long imgID;
		Bitmap  bm = null;
		ContentResolver resolver = mContext.getContentResolver();
		try{
			Cursor c = resolver.query(MediaStore.Images.Media.EXTERNAL_CONTENT_URI,
   	          new String[]{MediaStore.Images.Media._ID +""},
   	          MediaStore.Images.Media.DATA +" like '" + "%" + fpath +"'",
   	          null, null);
			  c.moveToNext();
			  if (!c.isAfterLast()) {
				  imgID = c.getLong(0);
				  bm = MediaStore.Images.Thumbnails.getThumbnail(resolver, imgID, Images.Thumbnails.MICRO_KIND, null); 
	//			  Log.e("SharePushWin", " Id="+imgID + "bm=" + bm.toString() );
			  }
			  c.close();
		}
		catch (Exception e) {   
            Log.e("MyError", e.toString());   
        }   
		return bm;
	}
	
	public static Bitmap getOriginalBitmap(File file) {
//		BitmapFactory.Options bfOptions=new BitmapFactory.Options();
		Bitmap bmpBitmap = BitmapFactory.decodeFile(file.getAbsolutePath());
		return bmpBitmap;
	}
	
}
