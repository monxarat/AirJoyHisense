package cn.airjoy.android.service;

import android.app.Dialog;
import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import com.fqx.anyplay.api.CallBackEvent;

/**
* <p>功能描述: TV提示对话框，有超时自动取消功能 </p>
* 
* @author sangwencheng
* @version 1.0
*/
public class SangNote {

	private CallBackEvent mCallBackEvent = null;
	private Button mCencelButton;
	private Button mConfirmButton;
	private Context mContext;
	private Dialog mDialog;
	private TextView mMsg;
	private TextView mTitle;
	private int m_s = 0;
	private Handler mHandler;
	

	public SangNote(Context paramContext) {
	    this.mContext = paramContext;
	    this.mDialog = new Dialog(this.mContext, R.style.my_dialog);
	    View localView = LayoutInflater.from(this.mContext).inflate(R.layout.sang_note, null);
	    this.mDialog.setContentView(localView);
	    this.mTitle = ((TextView)localView.findViewById(R.id.note_title));
	    this.mMsg = ((TextView)localView.findViewById(R.id.note_msg));
	    this.mConfirmButton = ((Button)localView.findViewById(R.id.note_confirm));
	    this.mCencelButton = ((Button)localView.findViewById(R.id.note_cancel));
	    mHandler = new Handler();
	}
	
	private Handler mDelayHandler = new Handler() {
	    public void handleMessage(Message paramMessage) {
	      switch (paramMessage.what) {
	      case 1:
	    	  SangNote.this.showConfirmDelayText(paramMessage.arg1);
	    	  break;
	      case 2:
	    	  if(SangNote.this.mCallBackEvent != null) {
			      SangNote.this.mCallBackEvent.delayCallbackEvent();
	    	  }
	    	  break;
	      }
	    }
	};
	
	
	private void sendMsg(int paramInt1, int paramInt2) {
	    Message localMessage = new Message();
	    localMessage.what = paramInt1;
	    localMessage.arg1 = paramInt2;
	    this.mDelayHandler.sendMessage(localMessage);
	}
	
	private void showConfirmDelayText(int paramInt) {
	    String str = this.mContext.getResources().getString(R.string.confirm) + " " + String.valueOf(paramInt);
	    this.mConfirmButton.setText(str);
	}
	
	private Runnable mRunnable = new Runnable() {
		@Override
		public void run() {
	        SangNote.this.sendMsg(1, m_s);
	        if(m_s <= 0) {
		        SangNote.this.sendMsg(2, m_s);
				mHandler.removeCallbacks(mRunnable);
	        }else{
				mHandler.postDelayed(mRunnable, 1500);
	        }
			m_s = m_s - 1;
		}
	};

	private void startDelayThread() {
		mHandler.removeCallbacks(mRunnable);
		mHandler.postDelayed(mRunnable, 500);
	}
	

	public void dismiss() {
		mHandler.removeCallbacks(mRunnable);
	    if (this.mDialog == null)
	      return;
	    try {
	      this.mDialog.dismiss();
	      this.mDialog = null;
	    } catch (Exception localException) {
	      localException.printStackTrace();
	    }
	}
	
	public void setCencelOnListener(View.OnClickListener paramOnClickListener) {
	    this.mCencelButton.setVisibility(0);
	    this.mCencelButton.setOnClickListener(paramOnClickListener);
	}
	
	public void setConfirmOnListener(View.OnClickListener paramOnClickListener) {
	    this.mConfirmButton.setOnClickListener(paramOnClickListener);
	}
	
	public void setDelayCallbackEvent(int paramInt, CallBackEvent paramCallBackEvent) {
	    this.m_s = paramInt;
	    this.mCallBackEvent = paramCallBackEvent;
	}
	
	public void setMsg(String paramString) {
	    this.mMsg.setText(paramString);
	}
	
	public void setTitile(String paramString) {
	    this.mTitle.setText(paramString);
	}
	
	public void show() {
	    if (this.mDialog == null) {
	    	return;
	    }
	    SangNote.this.showConfirmDelayText(m_s);
	    this.mDialog.show();
	    startDelayThread();
	}

}

