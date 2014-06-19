package com.fqx.airjoy.listener;

import com.fqx.airjoy.server.ServerInfo;

public abstract interface AirJoyNsdDiscoveryListener {
	public abstract void onDiscoveryStarted(String paramString);
	public abstract void onDiscoveryStopped(String paramString);
	public abstract void onServiceFound(ServerInfo paramServerInfo);
	public abstract void onServiceLost(ServerInfo paramServerInfo);
	public abstract void onStartDiscoveryFailed(String paramString, int paramInt);
	public abstract void onStopDiscoveryFailed(String paramString, int paramInt);
}
