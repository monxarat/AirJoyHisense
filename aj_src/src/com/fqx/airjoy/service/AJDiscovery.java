package com.fqx.airjoy.service;

import com.fqx.airjoy.listener.AirJoyNsdDiscoveryListener;
import com.fqx.airjoy.server.ServerInfo;
import com.fqx.anyplay.api.AnyPlayUtils;

public class AJDiscovery implements AirJoyNsdDiscoveryListener {

  public void onDiscoveryStarted(String paramString) {
    AnyPlayUtils.LOG_DEBUG("AirJoy", "onDiscoveryStarted");
  }

  public void onDiscoveryStopped(String paramString) {
    AnyPlayUtils.LOG_DEBUG("AirJoy", "onDiscoveryStopped");
  }

  public void onServiceFound(ServerInfo paramServerInfo) {
    AnyPlayUtils.LOG_DEBUG("AirJoy", "onServiceFound");
    paramServerInfo.printInfo("onServiceFound");
  }

  public void onServiceLost(ServerInfo paramServerInfo) {
    AnyPlayUtils.LOG_ERR("AirJoy", "onServiceLost");
    paramServerInfo.printInfo("onServiceLost");
  }

  public void onStartDiscoveryFailed(String paramString, int paramInt) {
    AnyPlayUtils.LOG_ERR("AirJoy", "onStartDiscoveryFailed");
  }

  public void onStopDiscoveryFailed(String paramString, int paramInt) {
    AnyPlayUtils.LOG_ERR("AirJoy", "onStopDiscoveryFailed");
  }

}