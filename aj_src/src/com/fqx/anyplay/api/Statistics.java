package com.fqx.anyplay.api;

import android.annotation.SuppressLint;
import android.content.Context;
import com.umeng.analytics.MobclickAgent;
import java.util.HashMap;

public class Statistics {
	private static final String AirJoy = "AirJoy";
	private static final String AirNone = "AirNone";
	private static final String AirPlay = "AirPlay";
	private static final String EvtProtocolTypes = "EvtProtocolTypes";
	private static final String EvtRecPhotos = "EventRecPhotos";
	private static final String EvtRecVideoTypes = "EvtRecVideoTypes";
	private static final String EvtRecVideos = "EventRecVideos";

	public static void addPhoto(Context paramContext, int paramInt) {
	    HashMap<String, String> localHashMap = new HashMap<String, String>();
	    localHashMap.put(EvtProtocolTypes, getProString(paramInt));
	    MobclickAgent.onEvent(paramContext, EvtRecPhotos, localHashMap);
	}

	@SuppressLint("DefaultLocale")
	public static void addVideo(Context paramContext, String paramString, int paramInt) {
	    String str1 = AnyPlayUtils.getsuffix(paramString.replaceAll("\\?.*", ""), 1);
	    if (str1 == null) {
	    	return;
	    }
	    HashMap<String, String> localHashMap = new HashMap<String, String>();
	    localHashMap.put(EvtProtocolTypes, getProString(paramInt));
	    localHashMap.put(EvtRecVideoTypes, str1.toLowerCase());
	    MobclickAgent.onEvent(paramContext, EvtRecVideos, localHashMap);
	}

	private static String getProString(int paramInt) {
	    if (paramInt == APPEnum.AirChannel.AirPlay.GetValue()) {
	    	return AirPlay;
	    } else  if (paramInt != APPEnum.AirChannel.AirJoy.GetValue()) {
	    	return AirJoy;
	    }
	    return AirNone;
	} 
}
