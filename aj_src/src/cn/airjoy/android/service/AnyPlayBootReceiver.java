package cn.airjoy.android.service;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import com.fqx.airjoy.service.AJService;
import com.fqx.anyplay.service.APService;


/**
* <p>文件名: AnyPlayBootReceiver.java</p>
* <p>描述: AirJoy随机启动服务的入口 </p>
* 
* @author sangwencheng
* @version 1.0
*/
public class AnyPlayBootReceiver extends BroadcastReceiver {

  private void startAPControllerService(Context paramContext) {
	  Intent intent = new Intent(paramContext, APService.class);
	  intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
	  paramContext.startService(intent);
  }

  private void startAirJoyService(Context paramContext) {
	  Intent intent = new Intent(paramContext, AJService.class);
	  intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
	  paramContext.startService(intent);
  }

  public void onReceive(Context paramContext, Intent paramIntent) {
//    if (!paramIntent.getAction().equals("android.intent.action.BOOT_COMPLETED"))
//      return;
    startAPControllerService(paramContext);
    startAirJoyService(paramContext);
  }
}