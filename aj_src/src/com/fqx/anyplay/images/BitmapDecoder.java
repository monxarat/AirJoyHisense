package com.fqx.anyplay.images;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

public class BitmapDecoder {

	public BitmapDecoder(Context context) {
	}

	public Bitmap getPhotoItem(String filepath, int size) {
		BitmapFactory.Options options = new BitmapFactory.Options();
		options.inSampleSize = size;
		Bitmap bitmap = BitmapFactory.decodeFile(filepath, options);
		bitmap = Bitmap.createScaledBitmap(bitmap, 180, 251, true);
		return bitmap;
	}
}