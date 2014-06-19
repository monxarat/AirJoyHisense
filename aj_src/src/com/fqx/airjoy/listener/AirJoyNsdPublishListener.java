package com.fqx.airjoy.listener;

import com.fqx.airjoy.server.ServerInfo;

public abstract interface AirJoyNsdPublishListener {
	public abstract void onPublishFailed(ServerInfo paramServerInfo, int paramInt);
	
	public abstract void onServicePublished(ServerInfo paramServerInfo);
	
	public abstract void onServiceUnPublished(ServerInfo paramServerInfo);
	
	public abstract void onUnPulishFailed(ServerInfo paramServerInfo, int paramInt);
}