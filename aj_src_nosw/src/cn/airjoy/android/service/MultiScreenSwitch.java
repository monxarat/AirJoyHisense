package cn.airjoy.android.service;

import android.content.ContentResolver;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;
import android.provider.Settings;
import android.util.Log;

public class MultiScreenSwitch {
	private static final String TAG = MultiScreenSwitch.class.getSimpleName();
	private static final String URI_SETTINT = "content://provider.hisense.com/systemconfig/";
	public static boolean getSwitchState(Context context) {
		int vision = getSwitchOfVision(context);
		int vidaa = getSwitchStateOfVidaa(context);
		Log.d(TAG,"vision -->"+vision+", vidaa-->"+vidaa);
		if(vision==0)//判断vision开关,0表示关
		{
			return false;
		}else if(vidaa==0)
		{
			return false;
		}else
		{
			return true;
		}
		
	}

	private static int getSwitchOfVision(final Context context) {
		String URI = URI_SETTINT;
		int ret = -100;
		Uri uri = Uri.parse(URI);
		String key = "MultiScreen_Switch";
		ContentResolver resolver = context.getContentResolver();
		Cursor cursor = null;
		try {
			cursor = resolver.query(uri, null, "KeyName='" + key + "'", null,
					null);
			if (cursor != null) {
				if (cursor.moveToFirst()) {		
					ret = cursor.getInt(cursor.getColumnIndex("KeyValue"));
				}
			} else {
			}
		} finally {
			// TODO: handle exception
			if (cursor != null) {
				try {
					cursor.close();
				} catch (Exception e) {
					// TODO: handle exception
				}
			}
		}
		return ret;
	}

	private static int getSwitchStateOfVidaa(final Context context) {
		ContentResolver resolver = context.getContentResolver();
		int multiScreenSwitch = -100;
		try {
			
			multiScreenSwitch = Settings.System.getInt(resolver,
					"MultiScreen_Switch", -100);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return multiScreenSwitch;
	}
	
//	public static String getDevName() {
//		String name; 
//		
//		name = __system_property_get("persist.sys.product.name", name);
//		return name;
//	}
}
