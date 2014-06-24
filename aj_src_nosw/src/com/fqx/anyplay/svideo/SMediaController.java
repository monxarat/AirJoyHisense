package com.fqx.anyplay.svideo;

import io.vov.vitamio.utils.StringUtils;
import cn.airjoy.android.service.R;
import android.content.Context;
import android.os.Handler;
import android.util.AttributeSet;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.SeekBar;
import android.widget.TextView;

public class SMediaController extends FrameLayout {
  private TextView mCurTimeText;
  private long mCurTimev;
  private TextView mDurTimeText;
  private long mDurTimev;
  private Handler mHandler;
  //private boolean mIShow = false;
  private SeekBar mSeekBar;

  public SMediaController(Context paramContext) {
    super(paramContext);
    init(paramContext);
  }

  public SMediaController(Context paramContext, AttributeSet paramAttributeSet) {
    super(paramContext, paramAttributeSet);
    init(paramContext);
  }

  public SMediaController(Context paramContext, AttributeSet paramAttributeSet, int paramInt) {
    super(paramContext, paramAttributeSet, paramInt);
    init(paramContext);
  }

  private void init(Context paramContext) {
	mHandler = new Handler();
    LayoutInflater.from(paramContext).inflate(R.layout.s_mediacontroller, this, true);
    this.mCurTimeText = ((TextView)findViewById(R.id.mediacontroller_time_current));
    this.mDurTimeText = ((TextView)findViewById(R.id.mediacontroller_time_total));
    this.mSeekBar = ((SeekBar)findViewById(R.id.mediacontroller_seekbar));
    this.mSeekBar.setMax(1000);
//    this.mIShow = false;
  }

  public void hide() {
//    this.mIShow = false;
    setVisibility(View.GONE);
  }

  public void setCurProgress(int paramInt) {
    this.mSeekBar.setProgress(paramInt);
  }

  public void setCurTime(long paramLong) {
    this.mCurTimev = paramLong;
    this.mCurTimeText.setText(StringUtils.generateTime(this.mCurTimev));
  }

  public void setDurTime(long paramLong) {
    try {
      this.mDurTimev = paramLong;
      this.mDurTimeText.setText(StringUtils.generateTime(this.mDurTimev));
      setCurProgress((int)(1000L * this.mCurTimev / this.mDurTimev));
      return;
    } catch (Exception localException) {
      Log.e("setDurTime", localException.toString());
    }
  }

  public void setOnSeekBarChangeListener(SeekBar.OnSeekBarChangeListener paramOnSeekBarChangeListener) {
    this.mSeekBar.setOnSeekBarChangeListener(paramOnSeekBarChangeListener);
  }

  private Runnable showRunnable = new Runnable() {
	@Override
	public void run() {
		hide();
	}
  };

  public void show(int paramInt) {
    setVisibility(0);
    this.mHandler.removeCallbacks(showRunnable);
    this.mHandler.postDelayed(showRunnable, paramInt);
  }
}
