package com.fqx.audio.player;

import io.vov.vitamio.utils.StringUtils;
import cn.airjoy.android.service.R;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.AttributeSet;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.TextView;

public class AudioCover extends FrameLayout {
  private TextView mAudioAlbum;
  private TextView mAudioArt;
  private ImageView mAudioImg;
  private TextView mAudioName;
  private ImageView mAudioPauseImg;
  private TextView mCurTimeText;
  private long mCurTimev;
  private TextView mDurTimeText;
  private long mDurTimev;
  private boolean mIShow = false;
  private SeekBar mSeekBar;

	public AudioCover(Context paramContext) {
	    super(paramContext);
	    init(paramContext);
	}
	
	public AudioCover(Context paramContext, AttributeSet paramAttributeSet) {
	    super(paramContext, paramAttributeSet);
	    init(paramContext);
	}
	
	public AudioCover(Context paramContext, AttributeSet paramAttributeSet, int paramInt) {
	    super(paramContext, paramAttributeSet, paramInt);
	    init(paramContext);
	}
	
	private void init(Context paramContext) {
	    LayoutInflater.from(paramContext).inflate(R.layout.audio_cover, this, true);
	    this.mCurTimeText = ((TextView)findViewById(R.id.mediacontroller_time_current));
	    this.mDurTimeText = ((TextView)findViewById(R.id.mediacontroller_time_total));
	    this.mAudioImg = ((ImageView)findViewById(R.id.iv_audio_img));
	    this.mAudioPauseImg = ((ImageView)findViewById(R.id.iv_audio_pause));
	    this.mAudioName = ((TextView)findViewById(R.id.tv_audio_title));
	    this.mAudioArt = ((TextView)findViewById(R.id.tv_audio_info));
	    this.mAudioAlbum = ((TextView)findViewById(R.id.tv_audio_info2));
	    this.mSeekBar = ((SeekBar)findViewById(R.id.mediacontroller_seekbar));
	    this.mSeekBar.setMax(1000);
	    this.mIShow = false;
	}
	
	public boolean iShow() {
	    return this.mIShow;
	}
	
	public void setAudioInfo(AudioInfo paramAudioInfo) {
	    if (paramAudioInfo == null) {
	    	return;
	    }
	      this.mAudioName.setText(paramAudioInfo.nameString);
	      if (paramAudioInfo.artString.trim().equals("")) {
	    	  this.mAudioArt.setText(" ");
	      }else{
	    	  this.mAudioArt.setText("ÑÝ³ªÕß:  " + paramAudioInfo.artString);
	      }
	
	      if (paramAudioInfo.albumString.trim().equals("")) {
	    	  this.mAudioAlbum.setText(" ");
	       }else{
	    	   this.mAudioAlbum.setText("×¨¼­Ãû:  " + paramAudioInfo.albumString);
	       }
	      if(paramAudioInfo.imgData == null) {
	    	  this.mAudioImg.setImageResource(R.drawable.audio_img);
	    	  this.mAudioArt.setText(" ");
	    	  this.mAudioAlbum.setText(" ");
	    	  return;
	      }
	      if (paramAudioInfo.imgData.length == 0) {
	        this.mAudioImg.setImageResource(R.drawable.audio_img);
	      }else{
	    	  Bitmap localBitmap = BitmapFactory.decodeByteArray(paramAudioInfo.imgData, 0, paramAudioInfo.imgData.length);
	    	  this.mAudioImg.setImageBitmap(localBitmap);
	      }
	}
	
	public void setCurProgress(int paramInt) {
	    this.mSeekBar.setProgress(paramInt);
	}
	
	public void setCurTime(long paramLong) {
	    this.mCurTimev = paramLong;
	    this.mCurTimeText.setText(StringUtils.generateTime(this.mCurTimev));
	}
	
	public void setDurTime(long paramLong) {
	    try
	    {
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
	
	public void setPauseShow(boolean paramBoolean) {
	    if (paramBoolean) {
	      this.mAudioPauseImg.setVisibility(View.VISIBLE);
	    }else {
	      this.mAudioPauseImg.setVisibility(View.GONE);
	    }
	}
	
	public void show(int paramInt) {
	    this.mIShow = true;
	    setVisibility(View.VISIBLE);
	}

}
