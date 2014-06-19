package com.fqx.anyplay.api;

import cn.airjoy.android.service.R;
import android.app.Activity;
import android.content.Context;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.widget.ImageView;
import android.widget.ImageView.ScaleType;

public class VerLeg {
  private Context mContext;
  private ImageView mMarkImageView;
  private LocalInfo mInfo;

  public VerLeg(Context paramContext) {
    this.mContext = paramContext;
    mInfo = LocalInfo.getInstance(paramContext);
  }


  public boolean showTVMark() {
	this.mMarkImageView = ((ImageView)((Activity)this.mContext).findViewById(R.id.tv_mark));
	this.mMarkImageView.setVisibility(View.VISIBLE);
	LayoutParams layoutParams = mMarkImageView.getLayoutParams();
    layoutParams.width = 90;
    layoutParams.height = 64;
	this.mMarkImageView.setLayoutParams(layoutParams);
	this.mMarkImageView.setScaleType(ScaleType.CENTER_INSIDE);
	if(mInfo.g_CustomUI == APPEnum.MDCustomUI.hisense.GetValue()) {
	    this.mMarkImageView.setVisibility(View.GONE);
	}else if(mInfo.g_CustomUI == APPEnum.MDCustomUI.fqx.GetValue()) {
		this.mMarkImageView.setImageResource(R.raw.ajsyssyb);
	}else if(mInfo.g_CustomUI == APPEnum.MDCustomUI.pinjitongda.GetValue()) {
		this.mMarkImageView.setImageResource(R.raw.nosybinit);
	}
    return true;
  }
}

