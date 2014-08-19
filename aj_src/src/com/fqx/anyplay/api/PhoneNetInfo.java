package com.fqx.anyplay.api;

import java.lang.reflect.Method;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;


import android.content.Context;
import android.net.ConnectivityManager;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;

import android.util.Log;

/**
* <p>描述: 网络状态信息. </p>
* 
* @author sangwencheng
* @version 1.0
*/
public class PhoneNetInfo {

	private ConnectivityManager conMan;
	private Context	mContext;
	
	public  PhoneNetInfo(Context act) {
		mContext = act;
		conMan = (ConnectivityManager)mContext.getSystemService(Context.CONNECTIVITY_SERVICE);
	}

	public boolean IsWifiMode() {
        android.net.NetworkInfo.State wifi = conMan.getNetworkInfo(ConnectivityManager.TYPE_WIFI).getState();
        
        if(wifi.toString() == "CONNECTED")
			return true;
		else
			return false;
    }

	public boolean Is3GMode() {
		android.net.NetworkInfo.State mobile = conMan.getNetworkInfo(ConnectivityManager.TYPE_MOBILE).getState();
		if(mobile.toString() == "CONNECTED")
			return true;
		else
			return false;
    }
	
	public boolean isAPMode() {
		try {  
			WifiManager wifiManager = (WifiManager)mContext.getSystemService(Context.WIFI_SERVICE);  
			 Method method = wifiManager.getClass().getMethod("isWifiApEnabled");  
			 method.setAccessible(true);  
	         return (Boolean) method.invoke(wifiManager); 
		} catch (Exception e) {  
		     return false;  
		}  
	}
	public String getLocalIpAddress() {  
        try {  
            for (Enumeration<NetworkInterface> en = NetworkInterface  
                    .getNetworkInterfaces(); en.hasMoreElements();) {  
                NetworkInterface intf = en.nextElement();  
                
                for (Enumeration<InetAddress> enumIpAddr = intf  
                        .getInetAddresses(); enumIpAddr.hasMoreElements();) {  
                    InetAddress inetAddress = enumIpAddr.nextElement();  
                    if (!inetAddress.isLoopbackAddress()&& (inetAddress instanceof Inet4Address)) {  
                        return inetAddress.getHostAddress().toString();  
                    }  
                }  
            }  
        } catch (SocketException ex) {
            Log.e("WifiPreference IpAddress", ex.toString());  
        }  
        return null;  
    }  

	public String getLocalMacAddress() {  
        WifiManager wifi = (WifiManager)mContext.getSystemService(Context.WIFI_SERVICE);  
        WifiInfo info = wifi.getConnectionInfo();  
        return info.getMacAddress();  
    }
}
