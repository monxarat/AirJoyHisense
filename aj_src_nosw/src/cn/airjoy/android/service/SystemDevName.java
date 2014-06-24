package cn.airjoy.android.service;

import java.lang.reflect.Method;


public class SystemDevName {

	private static String get(String key) {
		init();
		
		String value = "大屏TV";
		
		try {
			value = (String) mGetMethod.invoke(mClassType, key);
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return value;
	}
	
	public static String tryRead() {
		return get("persist.sys.product.name");
	}
	
	private static Class<?> mClassType = null;
	private static Method mGetMethod = null;
	private static void init() {
		try {
			if (mClassType == null) {
				mClassType = Class.forName("android.os.SystemProperties");
				mGetMethod = mClassType.getDeclaredMethod("get", String.class);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}