package cn.airjoy.android.service;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.IBinder;
import android.view.KeyEvent;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import com.fqx.anyplay.api.APPEnum;
import com.fqx.anyplay.api.AnyPlayUtils;
import com.fqx.anyplay.service.APService;

/**
* <p>文件名: AudioView.java</p>
* <p>描述: 音频播放页面，只实现苹果设备的音频流播放逻辑.  </p>
* 
* @author sangwencheng
* @version 1.0
*/
public class AudioView extends Activity {

	private APService.MyBinder mAPServerBinder;
	
	private ImageView mAudioImg;
	private TextView mAudioInfo;
	private TextView mAudioInfo2;
	private ImageView mAudioPauseImg;
	private TextView mAudioTitle;
	
	protected void onCreate(Bundle paramBundle) {
	    super.onCreate(paramBundle);
	    setContentView(R.layout.audio_view);
	    this.mAudioImg = ((ImageView)findViewById(R.id.iv_audio_img));
	    this.mAudioPauseImg = ((ImageView)findViewById(R.id.iv_audio_pause));
	    this.mAudioTitle = ((TextView)findViewById(R.id.tv_audio_title));
	    this.mAudioInfo = ((TextView)findViewById(R.id.tv_audio_info));
	    this.mAudioInfo2 = ((TextView)findViewById(R.id.tv_audio_info2));
	    this.mAudioPauseImg.setVisibility(View.GONE);
	    startAPControllerService();
	    handleIntent(getIntent());
	}
	
	private ServiceConnection mAPServiceConnection = new ServiceConnection() {
	public void onServiceConnected(ComponentName paramComponentName, IBinder paramIBinder) {
	      AudioView.this.mAPServerBinder = ((APService.MyBinder)paramIBinder);
	    }
	
	    public void onServiceDisconnected(ComponentName paramComponentName) {
	    }
	};
	private void handleIntent(Intent paramIntent) {
	    int i = paramIntent.getExtras().getInt("CMD");
	    byte[] arrayOfByte;
	    if (i == APPEnum.AirAudioCmd.didSetAudioInfo.GetValue()) {
	      String str1 = AnyPlayUtils.gAudioInfo.nameString;
	      String str2 = AnyPlayUtils.gAudioInfo.artString;
	      String str3 = AnyPlayUtils.gAudioInfo.albumString;
	      arrayOfByte = AnyPlayUtils.gAudioInfo.imgData;
	      this.mAudioTitle.setText(str1);
	      if (!str2.trim().equals("")) {
	        this.mAudioInfo.setText("演唱: " + str2);
	      }else{
	        this.mAudioInfo.setText(" ");
	      }
	      if (str3.trim().equals("")) {
	        this.mAudioInfo2.setText(" ");
	      }else{
	        this.mAudioInfo2.setText("专辑: " + str3);
	      }
	      if (arrayOfByte == null){
	        this.mAudioImg.setImageResource(R.drawable.audio_img);
	      }else{
		      Bitmap localBitmap = BitmapFactory.decodeByteArray(arrayOfByte, 0, arrayOfByte.length);
		      this.mAudioImg.setImageBitmap(localBitmap);
	      }
		  this.mAudioPauseImg.setVisibility(View.GONE);
	    } else if (i == APPEnum.AirAudioCmd.didPause.GetValue())  {
		    this.mAudioPauseImg.setVisibility(View.VISIBLE);
	    }else if (i == APPEnum.AirAudioCmd.didStop.GetValue()) {
	    	finish();
	    }
	}
	
	private void startAPControllerService() {
	    Intent localIntent = new Intent(this, APService.class);
	    startService(localIntent);
	    bindService(localIntent, this.mAPServiceConnection, 1);
	}
	
	
	protected void onDestroy() {
	    super.onDestroy();
	    if (this.mAPServerBinder == null)
	      return;
	    unbindService(this.mAPServiceConnection);
	}
	
	public boolean onKeyDown(int paramInt, KeyEvent paramKeyEvent) {
	    if (paramInt == KeyEvent.KEYCODE_BACK) {
	      this.mAPServerBinder.stopAudio();
	      finish();
	      return true;
	    }
	    return super.onKeyDown(paramInt, paramKeyEvent);
	}
	
	protected void onNewIntent(Intent paramIntent) {
	    super.onNewIntent(paramIntent);
	    handleIntent(paramIntent);
	}

}