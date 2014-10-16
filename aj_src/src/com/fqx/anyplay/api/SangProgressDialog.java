package com.fqx.anyplay.api;

import cn.airjoy.android.service.R;
import android.app.Dialog;
import android.content.Context;
import android.graphics.drawable.AnimationDrawable;
import android.view.Gravity;
import android.widget.ImageView;
import android.widget.TextView;

public class SangProgressDialog extends Dialog {
  private static String MDEFAULEMSG_STRING = "努力加载中，请稍后...  ";
  private static SangProgressDialog customProgressDialog = null;
  private static TextView mProgressTextView;
  private static TextView mTextView;

  public SangProgressDialog(Context paramContext) {
    super(paramContext);
  }

  public SangProgressDialog(Context paramContext, int paramInt) {
    super(paramContext, paramInt);
  }

  public static SangProgressDialog createDialog(Context paramContext) {
	MDEFAULEMSG_STRING = paramContext.getResources().getString(R.string.progress_msg);
    customProgressDialog = new SangProgressDialog(paramContext, R.style.CustomProgressDialog);
    customProgressDialog.setContentView(R.layout.sang_progressdialog);
    customProgressDialog.getWindow().getAttributes().gravity = Gravity.CENTER;
    mProgressTextView = (TextView)customProgressDialog.findViewById(R.id.loading_progress);
    mProgressTextView.setText(" ");
    mTextView = ((TextView)customProgressDialog.findViewById(R.id.id_tv_loadingmsg));
    mTextView.setText(MDEFAULEMSG_STRING);
    return customProgressDialog;
  }

  public void onWindowFocusChanged(boolean paramBoolean) {
    if (customProgressDialog == null) {
    	return;
    }
    ((AnimationDrawable)((ImageView)customProgressDialog.findViewById(R.id.loadingImageView)).getBackground()).start();
  }
  
  public void setOnCannel(OnCancelListener listener) {
    if (customProgressDialog == null) {
    	return;
    }
    this.setOnCancelListener(listener);
    customProgressDialog.setOnCancelListener(listener);
  }

  public SangProgressDialog setMessage(String paramString) {
    TextView localTextView = (TextView)customProgressDialog.findViewById(R.id.id_tv_loadingmsg);
    if (localTextView != null)
      localTextView.setText(paramString);
    return customProgressDialog;
  }

  public void setSpeed(int paramInt) {
    if (mTextView == null) {
    	return;
    }
    if(paramInt < 0) {
	    mTextView.setText(MDEFAULEMSG_STRING);
    }else {
	    mTextView.setText(MDEFAULEMSG_STRING + paramInt + " KB/s");
	}
  }

  public void setProgress(int paramInt) {
    if (mProgressTextView == null) {
    	return;
    }
    mProgressTextView.setText(paramInt + "%");
  }

  
//  public void setMessage(String msg) {
//    if (mProgressTextView == null) {
//    	return;
//    }
//	if (mTextView == null) {
//    	return;
//    }
//    mProgressTextView.setText(" ");
//	mTextView.setText(msg);
//  }

  public SangProgressDialog setTitile(String paramString) {
    return customProgressDialog;
  }
}
