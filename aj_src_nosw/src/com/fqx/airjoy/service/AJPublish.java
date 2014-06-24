package com.fqx.airjoy.service;

import android.util.Log;
import com.fqx.airjoy.listener.AirJoyNsdPublishListener;
import com.fqx.airjoy.server.ServerInfo;

public class AJPublish implements AirJoyNsdPublishListener {

  public void onPublishFailed(ServerInfo paramServerInfo, int paramInt) {
    Log.e("AirJoy", "onPublishFailed");
  }

  public void onServicePublished(ServerInfo paramServerInfo) {
    Log.e("AirJoy", "onServicePublished");
  }

  public void onServiceUnPublished(ServerInfo paramServerInfo) {
    Log.e("AirJoy", "onServiceUnPublished");
  }

  public void onUnPulishFailed(ServerInfo paramServerInfo, int paramInt) {
    Log.e("AirJoy", "onUnPulishFailed");
  }
}
