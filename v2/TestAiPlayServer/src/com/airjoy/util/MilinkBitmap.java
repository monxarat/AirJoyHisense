
package com.airjoy.util;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.media.ExifInterface;
import android.os.Build;
import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class MilinkBitmap {

    private static final String TAG = MilinkBitmap.class.getSimpleName();
    private static final int BMP_QUALITY = 80;

    public static byte[] getFrameFromGIF(String filePath) {
        return null;
    }

    public static byte[] getFrameFromVideo(String filePath) {
        long start = System.currentTimeMillis();

        Bitmap bm = createVideoFrame(filePath);
        if (bm == null)
            return null;

        ByteArrayOutputStream baos = null;
        try {
            baos = new ByteArrayOutputStream();
            bm.compress(Bitmap.CompressFormat.JPEG, BMP_QUALITY, baos);
        } finally {
            try {
                if (baos != null)
                    baos.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        long end = System.currentTimeMillis();
        Log.v(TAG, String.format("getFrameFromVideo duration: %dms", (end - start)));

        return baos.toByteArray();
    }

    private static Bitmap createVideoFrame(String filePath) {

        // MediaMetadataRetriever is available on API Level 8
        // but is hidden until API Level 10
        Class<?> clazz = null;
        Object instance = null;
        try {
            clazz = Class.forName("android.media.MediaMetadataRetriever");
            instance = clazz.newInstance();

            Method method = clazz.getMethod("setDataSource", String.class);
            method.invoke(instance, filePath);

            // The method name changes between API Level 9 and 10.
            if (Build.VERSION.SDK_INT <= 9) {
                return (Bitmap) clazz.getMethod("captureFrame").invoke(instance);
            } else {
                byte[] data = (byte[]) clazz.getMethod("getEmbeddedPicture").invoke(instance);
                if (data != null) {
                    Bitmap bitmap = BitmapFactory.decodeByteArray(data, 0, data.length);
                    if (bitmap != null)
                        return bitmap;
                }
                return (Bitmap) clazz.getMethod("getFrameAtTime").invoke(instance);
            }
        } catch (IllegalArgumentException ex) {
            // Assume this is a corrupt video file
        } catch (RuntimeException ex) {
            // Assume this is a corrupt video file.
        } catch (InstantiationException e) {
            Log.e(TAG, "createVideoThumbnail", e);
        } catch (InvocationTargetException e) {
            Log.e(TAG, "createVideoThumbnail", e);
        } catch (ClassNotFoundException e) {
            Log.e(TAG, "createVideoThumbnail", e);
        } catch (NoSuchMethodException e) {
            Log.e(TAG, "createVideoThumbnail", e);
        } catch (IllegalAccessException e) {
            Log.e(TAG, "createVideoThumbnail", e);
        } finally {
            try {
                if (instance != null) {
                    clazz.getMethod("release").invoke(instance);
                }
            } catch (Exception ignored) {
            }
        }

        return null;
    }

    public static byte[] getSmallBitmap(String filePath) {
        long start = System.currentTimeMillis();

        boolean fileExist = true;
        final BitmapFactory.Options options = new BitmapFactory.Options();
        options.inJustDecodeBounds = true;

        try {
            BitmapFactory.decodeFile(filePath, options);
        } catch (Exception e) {
            Log.d(TAG, String.format("%s is not exist!", filePath));
            fileExist = false;
        }

        if (!fileExist)
            return null;

        // Calculate inSampleSize
        options.inSampleSize = MilinkBitmap.calculateInSampleSize(options, 1920, 1024);

        // Decode bitmap with inSampleSize set
        options.inJustDecodeBounds = false;

        Bitmap bm = BitmapFactory.decodeFile(filePath, options);
        if (bm == null) {
            return null;
        }

        int degree = MilinkBitmap.readPictureDegree(filePath);
        bm = MilinkBitmap.rotateBitmap(bm, degree);
        ByteArrayOutputStream baos = null;
        try {
            baos = new ByteArrayOutputStream();
            bm.compress(Bitmap.CompressFormat.JPEG, BMP_QUALITY, baos);

            // recycle memory
            bm.recycle();
            bm = null;
        } finally {
            try {
                if (baos != null)
                    baos.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        long end = System.currentTimeMillis();
        Log.v(TAG, String.format("getSmallBitmap duration: %dms", (end - start)));

        return baos.toByteArray();
    }

    private static int calculateInSampleSize(BitmapFactory.Options options, int reqWidth,
            int reqHeight) {

        // Raw height and width of image
        final int height = options.outHeight;
        final int width = options.outWidth;
        int inSampleSize = 1;

        if (height > reqHeight || width > reqWidth) {
            // Calculate ratios of height and width to requested height and
            // width
            final int heightRatio = Math.round((float) height / (float) reqHeight);
            final int widthRatio = Math.round((float) width / (float) reqWidth);

            // Choose the smallest ratio as inSampleSize value, this will
            // guarantee
            // a final image with both dimensions larger than or equal to
            // the
            // requested height and width.
            inSampleSize = heightRatio < widthRatio ? widthRatio : heightRatio;
        }

        return inSampleSize;
    }

    private static int readPictureDegree(String path) {
        int degree = 0;
        try {
            ExifInterface exifInterface = new ExifInterface(path);
            int orientation = exifInterface.getAttributeInt(ExifInterface.TAG_ORIENTATION,
                    ExifInterface.ORIENTATION_NORMAL);
            switch (orientation) {
                case ExifInterface.ORIENTATION_ROTATE_90:
                    degree = 90;
                    break;
                case ExifInterface.ORIENTATION_ROTATE_180:
                    degree = 180;
                    break;
                case ExifInterface.ORIENTATION_ROTATE_270:
                    degree = 270;
                    break;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return degree;
    }

    private static Bitmap rotateBitmap(Bitmap bitmap, int rotate) {
        if (bitmap == null)
            return null;

        int w = bitmap.getWidth();
        int h = bitmap.getHeight();

        // Setting post rotate to 90
        Matrix mtx = new Matrix();
        mtx.postRotate(rotate);
        return Bitmap.createBitmap(bitmap, 0, 0, w, h, mtx, true);
    }
}
