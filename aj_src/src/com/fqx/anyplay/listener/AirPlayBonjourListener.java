
package com.fqx.anyplay.listener;

import java.util.Map;

import android.content.Context;

import com.airjoy.bonjour.BonjourListener;

/**
* <p>描述: 自动发现回调接口 </p>
* 
* @author sangwencheng
* @date 2014-8-20
* @version 1.0
*/
public class AirPlayBonjourListener implements BonjourListener{

	public AirPlayBonjourListener(Context context) {
		// TODO Auto-generated constructor stub
	}

	/* (non-Javadoc)
	 * @see com.airjoy.bonjour.BonjourListener#onStarted()
	 */
	@Override
	public void onStarted() {
		// TODO Auto-generated method stub
		
	}

	/* (non-Javadoc)
	 * @see com.airjoy.bonjour.BonjourListener#onStartFailed()
	 */
	@Override
	public void onStartFailed() {
		// TODO Auto-generated method stub
		
	}

	/* (non-Javadoc)
	 * @see com.airjoy.bonjour.BonjourListener#onStopped()
	 */
	@Override
	public void onStopped() {
		// TODO Auto-generated method stub
		
	}

	/* (non-Javadoc)
	 * @see com.airjoy.bonjour.BonjourListener#onServiceFound(java.lang.String, java.lang.String, java.lang.String, int, java.util.Map)
	 */
	@Override
	public void onServiceFound(String name, String type, String ip, int port,
			Map<String, String> properties) {
		// TODO Auto-generated method stub
		
	}

	/* (non-Javadoc)
	 * @see com.airjoy.bonjour.BonjourListener#onServiceLost(java.lang.String, java.lang.String, java.lang.String)
	 */
	@Override
	public void onServiceLost(String name, String type, String ip) {
		// TODO Auto-generated method stub
		
	}

}
