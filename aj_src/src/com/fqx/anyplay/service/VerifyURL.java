package com.fqx.anyplay.service;

public class VerifyURL {
	
	private final static String S_QIYI_STRING = "http://localhost:";
	private final static String S_PPTV_STRING = "?type=ppvod";
	public static String tryDo(String url) {
		if(url.contains(S_QIYI_STRING)) {
			return null;
		}
		
		if(url.contains(S_PPTV_STRING)) {
			return getUrlNoParam(url);
		}
		
		return url;
	}
	
	
	 public static String getUrlNoParam(String fileName) {
			int lastIndexOfDot = fileName.indexOf(".m3u8");
	      return fileName.substring(0, lastIndexOfDot + 5);
	 }

}
