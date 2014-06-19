package com.fqx.anyplay.svideo;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.media.MediaPlayer;
import android.net.Uri;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.MediaController;

import java.io.IOException;
import java.util.Map;

import cn.airjoy.android.service.R;

public class SVideoView extends SurfaceView implements MediaController.MediaPlayerControl {
	private static final int STATE_ERROR = -1;
  private static final int STATE_IDLE = 0;
  private static final int STATE_PAUSED = 4;
  private static final int STATE_PLAYBACK_COMPLETED = 5;
  private static final int STATE_PLAYING = 3;
  private static final int STATE_PREPARED = 2;
  private static final int STATE_PREPARING = 1;
//  private static final int STATE_RESUME = 7;
//  private static final int STATE_SUSPEND = 6;
  private static final int STATE_SUSPEND_UNSUPPORTED = 8;
  private String TAG = "SangVideoView";
  private boolean mCanPause;
  private boolean mCanSeekBack;
  private boolean mCanSeekForward;
  private Context mContext;
  private int mCurrentBufferPercentage;
  private int mCurrentState = 0;
  private int mDuration;
  private MediaController mMediaController;
  private MediaPlayer mMediaPlayer = null;
  private MediaPlayer.OnBufferingUpdateListener mOnBufferingUpdateListener;
  private MediaPlayer.OnCompletionListener mOnCompletionListener;
  private MediaPlayer.OnErrorListener mOnErrorListener;
  private MediaPlayer.OnInfoListener mOnInfoListener;
  private MediaPlayer.OnPreparedListener mOnPreparedListener;
  private MediaPlayer.OnSeekCompleteListener mOnSeekCompleteListener;
  private MediaPlayer.OnVideoSizeChangedListener mOnVideoSizeChangedListener;
  private int mSeekWhenPrepared;
  private int mSurfaceHeight;
  private SurfaceHolder mSurfaceHolder = null;
  private int mSurfaceWidth;
  private int mTargetState = 0;
  private Uri mUri;
  private int mVideoHeight;
  private int mVideoWidth;

  public SVideoView(Context paramContext) {
    super(paramContext);
    initVideoView();
  }

  public SVideoView(Context paramContext, AttributeSet paramAttributeSet) {
    this(paramContext, paramAttributeSet, 0);
    initVideoView();
  }

  public SVideoView(Context paramContext, AttributeSet paramAttributeSet, int paramInt) {
    super(paramContext, paramAttributeSet, paramInt);
    initVideoView();
  }

 
  private void attachMediaController() {
      if (mMediaPlayer != null && mMediaController != null) {
          mMediaController.setMediaPlayer(this);
          View anchorView = this.getParent() instanceof View ?
                  (View)this.getParent() : this;
          mMediaController.setAnchorView(anchorView);
          mMediaController.setEnabled(isInPlaybackState());
      }
  }

  
	@SuppressWarnings("deprecation")
	private void initVideoView() {
	      mVideoWidth = 0;
	      mVideoHeight = 0;
	      getHolder().addCallback(mSHCallback);
	      getHolder().setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
	      setFocusable(true);
	      setFocusableInTouchMode(true);
	      requestFocus();
	      mCurrentState = STATE_IDLE;
	      mTargetState  = STATE_IDLE;
	      this.mContext = getContext();
	 }

	SurfaceHolder.Callback mSHCallback = new SurfaceHolder.Callback() {
	    public void surfaceChanged(SurfaceHolder holder, int format,
	                                int w, int h)
	    {
	        mSurfaceWidth = w;
	        mSurfaceHeight = h;
	        boolean isValidState =  (mTargetState == STATE_PLAYING);
	        boolean hasValidSize = (mVideoWidth == w && mVideoHeight == h);
	        if (mMediaPlayer != null && isValidState && hasValidSize) {
	            if (mSeekWhenPrepared != 0) {
	                seekTo(mSeekWhenPrepared);
	            }
	            start();
	        }
	    }
	
	    public void surfaceCreated(SurfaceHolder holder)
	    {
	        mSurfaceHolder = holder;
	        openVideo();
	    }
	
	    public void surfaceDestroyed(SurfaceHolder holder)
	    {
	        // after we return from this we can't use the surface any more
	        mSurfaceHolder = null;
	        if (mMediaController != null) mMediaController.hide();
	        release(true);
	    }
	};

  
  
  private boolean isInPlaybackState() {
      return (mMediaPlayer != null &&
              mCurrentState != STATE_ERROR &&
              mCurrentState != STATE_IDLE &&
              mCurrentState != STATE_PREPARING);
  }

  private void openVideo() {
    if ((this.mUri == null) || (this.mSurfaceHolder == null)) {
    	return;
    }
      Intent localIntent = new Intent("com.android.music.musicservicecommand");
      localIntent.putExtra("command", "pause");
      this.mContext.sendBroadcast(localIntent);
      release(false);
      this.mMediaPlayer = new MediaPlayer();
      try {
        this.mMediaPlayer.setOnPreparedListener(this.mPreparedListener);
        this.mMediaPlayer.setOnVideoSizeChangedListener(this.mSizeChangedListener);
        this.mDuration = -1;
        this.mMediaPlayer.setOnCompletionListener(this.mCompletionListener);
        this.mMediaPlayer.setOnErrorListener(this.mErrorListener);
        this.mMediaPlayer.setOnBufferingUpdateListener(this.mBufferingUpdateListener);
        this.mMediaPlayer.setOnInfoListener(this.mInfoListener);
        this.mMediaPlayer.setOnSeekCompleteListener(this.mSeekCompleteListener);
        this.mCurrentBufferPercentage = 0;
        this.mMediaPlayer.setDataSource(this.mContext, this.mUri);
        this.mMediaPlayer.setDisplay(this.mSurfaceHolder);
        this.mMediaPlayer.setAudioStreamType(3);
        this.mMediaPlayer.setScreenOnWhilePlaying(true);
        this.mMediaPlayer.prepareAsync();
        this.mCurrentState = 1;
        attachMediaController();
      } catch (IOException localIOException) {
        Log.e(this.TAG, "Unable to open content: " + this.mUri, localIOException);
        this.mCurrentState = STATE_ERROR;
        this.mTargetState = STATE_ERROR;
        this.mErrorListener.onError(this.mMediaPlayer, 1, 0);
      }
      catch (IllegalArgumentException localIllegalArgumentException)
      {
        Log.e(this.TAG, "Unable to open content: " + this.mUri, localIllegalArgumentException);
        this.mCurrentState = STATE_ERROR;
        this.mTargetState = STATE_ERROR;
        this.mErrorListener.onError(this.mMediaPlayer, 1, 0);
      }
  }

  private void release(boolean paramBoolean)
  {
    if (this.mMediaPlayer == null)
      return;
    this.mMediaPlayer.reset();
    this.mMediaPlayer.release();
    this.mMediaPlayer = null;
    this.mCurrentState = 0;
    if (!paramBoolean)
      return;
    this.mTargetState = 0;
  }

  private void toggleMediaControlsVisiblity() {
      if (mMediaController.isShowing()) {
          mMediaController.hide();
      } else {
          mMediaController.show();
      }
  }

  public boolean canPause()
  {
    return this.mCanPause;
  }

  public boolean canSeekBackward()
  {
    return this.mCanSeekBack;
  }

  public boolean canSeekForward()
  {
    return this.mCanSeekForward;
  }

  public int getBufferPercentage() {
    if (this.mMediaPlayer != null) {
    	return this.mCurrentBufferPercentage;
    }
    
    return 0;
  }

  public int getCurrentPosition() {
    if (isInPlaybackState()) {
    	return mMediaPlayer.getCurrentPosition();
    }
    return 0;
  }

  public int getDuration() {
      if (isInPlaybackState()) {
          if (mDuration > 0) {
              return mDuration;
          }
          mDuration = mMediaPlayer.getDuration();
          return mDuration;
      }
      mDuration = -1;
      return mDuration;
  }

  public boolean isPlaying() {
      return isInPlaybackState() && mMediaPlayer.isPlaying();
  }

  
  @Override
  public boolean onKeyDown(int keyCode, KeyEvent event)
  {
      boolean isKeyCodeSupported = keyCode != KeyEvent.KEYCODE_BACK &&
                                   keyCode != KeyEvent.KEYCODE_VOLUME_UP &&
                                   keyCode != KeyEvent.KEYCODE_VOLUME_DOWN &&
                                   keyCode != KeyEvent.KEYCODE_VOLUME_MUTE &&
                                   keyCode != KeyEvent.KEYCODE_MENU &&
                                   keyCode != KeyEvent.KEYCODE_CALL &&
                                   keyCode != KeyEvent.KEYCODE_ENDCALL;
      if (isInPlaybackState() && isKeyCodeSupported && mMediaController != null) {
          if (keyCode == KeyEvent.KEYCODE_HEADSETHOOK ||
                  keyCode == KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE) {
              if (mMediaPlayer.isPlaying()) {
                  pause();
                  mMediaController.show();
              } else {
                  start();
                  mMediaController.hide();
              }
              return true;
          } else if (keyCode == KeyEvent.KEYCODE_MEDIA_PLAY) {
              if (!mMediaPlayer.isPlaying()) {
                  start();
                  mMediaController.hide();
              }
              return true;
          } else if (keyCode == KeyEvent.KEYCODE_MEDIA_STOP
                  || keyCode == KeyEvent.KEYCODE_MEDIA_PAUSE) {
              if (mMediaPlayer.isPlaying()) {
                  pause();
                  mMediaController.show();
              }
              return true;
          } else {
              toggleMediaControlsVisiblity();
          }
      }

      return super.onKeyDown(keyCode, event);
  }

  @Override
  protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
      int width = getDefaultSize(mVideoWidth, widthMeasureSpec);
      int height = getDefaultSize(mVideoHeight, heightMeasureSpec);
      if (mVideoWidth > 0 && mVideoHeight > 0) {
          if ( mVideoWidth * height  > width * mVideoHeight ) {
              height = width * mVideoHeight / mVideoWidth;
          } else if ( mVideoWidth * height  < width * mVideoHeight ) {
              width = height * mVideoWidth / mVideoHeight;
          } 
      }
      setMeasuredDimension(width, height);
  }

  public boolean onTouchEvent(MotionEvent paramMotionEvent)
  {
    if ((isInPlaybackState()) && (this.mMediaController != null))
      toggleMediaControlsVisiblity();
    return false;
  }

  public boolean onTrackballEvent(MotionEvent paramMotionEvent)
  {
    if ((isInPlaybackState()) && (this.mMediaController != null))
      toggleMediaControlsVisiblity();
    return false;
  }

  
	public void pause() {
		if (isInPlaybackState()) {
			if (mMediaPlayer.isPlaying()) {
				mMediaPlayer.pause();
	            mCurrentState = STATE_PAUSED;
	        }
	    }
	    mTargetState = STATE_PAUSED;
	}
//  public void pause() {
//    if ((isInPlaybackState()) && (this.mMediaPlayer.isPlaying())) {
//      this.mMediaPlayer.pause();
//      this.mCurrentState = 4;
//    }
//    this.mTargetState = 4;
//  }

  public int resolveAdjustedSize(int desiredSize, int measureSpec) {
      int result = desiredSize;
      int specMode = MeasureSpec.getMode(measureSpec);
      int specSize =  MeasureSpec.getSize(measureSpec);

      switch (specMode) {
          case MeasureSpec.UNSPECIFIED:
              /* Parent says we can be as big as we want. Just don't be larger
               * than max size imposed on ourselves.
               */
              result = desiredSize;
              break;

          case MeasureSpec.AT_MOST:
              /* Parent says we can be as big as we want, up to specSize.
               * Don't be larger than specSize, and don't be larger than
               * the max size imposed on ourselves.
               */
              result = Math.min(desiredSize, specSize);
              break;

          case MeasureSpec.EXACTLY:
              // No choice. Do what we are told.
              result = specSize;
              break;
      }
      return result;
	}

  public void resume() {
	    if (this.mCurrentState == STATE_SUSPEND_UNSUPPORTED) {
		    openVideo();
	    }
  }

  public void seekTo(int msec) {
      if (isInPlaybackState()) {
          mMediaPlayer.seekTo(msec);
          mSeekWhenPrepared = 0;
      } else {
          mSeekWhenPrepared = msec;
      }
  }

  public void setMediaController(MediaController paramMediaController)
  {
    if (this.mMediaController != null)
      this.mMediaController.hide();
    this.mMediaController = paramMediaController;
    attachMediaController();
  }

  public void setOnBufferingUpdateListener(MediaPlayer.OnBufferingUpdateListener paramOnBufferingUpdateListener)
  {
    this.mOnBufferingUpdateListener = paramOnBufferingUpdateListener;
  }

  public void setOnCompletionListener(MediaPlayer.OnCompletionListener paramOnCompletionListener)
  {
    this.mOnCompletionListener = paramOnCompletionListener;
  }

  public void setOnErrorListener(MediaPlayer.OnErrorListener paramOnErrorListener)
  {
    this.mOnErrorListener = paramOnErrorListener;
  }

  public void setOnInfoListener(MediaPlayer.OnInfoListener paramOnInfoListener)
  {
    this.mOnInfoListener = paramOnInfoListener;
  }

  public void setOnPreparedListener(MediaPlayer.OnPreparedListener paramOnPreparedListener)
  {
    this.mOnPreparedListener = paramOnPreparedListener;
  }

  public void setOnSeekCompleteListener(MediaPlayer.OnSeekCompleteListener paramOnSeekCompleteListener)
  {
    this.mOnSeekCompleteListener = paramOnSeekCompleteListener;
  }

  public void setOnVideoSizeChangedListener(MediaPlayer.OnVideoSizeChangedListener paramOnVideoSizeChangedListener)
  {
    this.mOnVideoSizeChangedListener = paramOnVideoSizeChangedListener;
  }

  public void setVideoPath(String paramString)
  {
    setVideoURI(Uri.parse(paramString));
  }

  public void setVideoURI(Uri paramUri)
  {
    setVideoURI(paramUri, null);
  }

  public void setVideoURI(Uri paramUri, Map<String, String> paramMap) {
    this.mUri = paramUri;
    this.mSeekWhenPrepared = 0;
    openVideo();
    requestLayout();
    invalidate();
  }

  public void start() {
    if (isInPlaybackState())
    {
      this.mMediaPlayer.start();
      this.mCurrentState = 3;
    }
    this.mTargetState = 3;
  }

  public void stopPlayback() {
    if (this.mMediaPlayer == null)
      return;
    this.mMediaPlayer.stop();
    this.mMediaPlayer.release();
    this.mMediaPlayer = null;
    this.mCurrentState = 0;
    this.mTargetState = 0;
  }
  

  
  	MediaPlayer.OnVideoSizeChangedListener mSizeChangedListener = new MediaPlayer.OnVideoSizeChangedListener() {
	            public void onVideoSizeChanged(MediaPlayer mp, int width, int height) {
	                mVideoWidth = mp.getVideoWidth();
	                mVideoHeight = mp.getVideoHeight();
	                if (mVideoWidth != 0 && mVideoHeight != 0) {
	                    getHolder().setFixedSize(mVideoWidth, mVideoHeight);
	                    requestLayout();
	                }
	                if(mOnVideoSizeChangedListener !=null) {
	                	mOnVideoSizeChangedListener.onVideoSizeChanged(mp, width, height);
	                }
	            }
	    };

    MediaPlayer.OnPreparedListener mPreparedListener = new MediaPlayer.OnPreparedListener() {
        public void onPrepared(MediaPlayer mp) {
            mCurrentState = STATE_PREPARED;
            if (mOnPreparedListener != null) {
                mOnPreparedListener.onPrepared(mMediaPlayer);
            }
            if (mMediaController != null) {
                mMediaController.setEnabled(true);
            }
            mVideoWidth = mp.getVideoWidth();
            mVideoHeight = mp.getVideoHeight();

            int seekToPosition = mSeekWhenPrepared;  // mSeekWhenPrepared may be changed after seekTo() call
            if (seekToPosition != 0) {
                seekTo(seekToPosition);
            }
            if (mVideoWidth != 0 && mVideoHeight != 0) {
                //Log.i("@@@@", "video size: " + mVideoWidth +"/"+ mVideoHeight);
                getHolder().setFixedSize(mVideoWidth, mVideoHeight);
                if (mSurfaceWidth == mVideoWidth && mSurfaceHeight == mVideoHeight) {
                    // We didn't actually change the size (it was already at the size
                    // we need), so we won't get a "surface changed" callback, so
                    // start the video here instead of in the callback.
                    if (mTargetState == STATE_PLAYING) {
                        start();
                        if (mMediaController != null) {
                            mMediaController.show();
                        }
                    } else if (!isPlaying() &&
                               (seekToPosition != 0 || getCurrentPosition() > 0)) {
                       if (mMediaController != null) {
                           // Show the media controls when we're paused into a video and make 'em stick.
                           mMediaController.show(0);
                       }
                   }
                }
            } else {
                // We don't know the video size yet, but should start anyway.
                // The video size might be reported to us later.
                if (mTargetState == STATE_PLAYING) {
                    start();
                }
            }
        }
    };

	private MediaPlayer.OnCompletionListener mCompletionListener = new MediaPlayer.OnCompletionListener() {
        public void onCompletion(MediaPlayer mp) {
            mCurrentState = STATE_PLAYBACK_COMPLETED;
            mTargetState = STATE_PLAYBACK_COMPLETED;
            if (mMediaController != null) {
                mMediaController.hide();
            }
            if (mOnCompletionListener != null) {
                mOnCompletionListener.onCompletion(mMediaPlayer);
            }
        }
    };

    private MediaPlayer.OnErrorListener mErrorListener = new MediaPlayer.OnErrorListener() {
        public boolean onError(MediaPlayer mp, int framework_err, int impl_err) {
            Log.d(TAG, "Error: " + framework_err + "," + impl_err);
            mCurrentState = STATE_ERROR;
            mTargetState = STATE_ERROR;
            if (mMediaController != null) {
                mMediaController.hide();
            }

            /* If an error handler has been supplied, use it and finish. */
            if (mOnErrorListener != null) {
                if (mOnErrorListener.onError(mMediaPlayer, framework_err, impl_err)) {
                    return true;
                }
            }

            /* Otherwise, pop up an error dialog so the user knows that
             * something bad has happened. Only try and pop up the dialog
             * if we're attached to a window. When we're going away and no
             * longer have a window, don't bother showing the user an error.
             */
            if (getWindowToken() != null) {
                new AlertDialog.Builder(mContext)
                        .setMessage(R.string.VideoView_error_text_unknown)
                        .setPositiveButton(R.string.VideoView_error_button, new DialogInterface.OnClickListener() {
                        	public void onClick(DialogInterface dialog, int whichButton) {
                        		if (mOnCompletionListener != null) {
                        			mOnCompletionListener.onCompletion(mMediaPlayer);
                                }
                        	}
                        })
                        .setCancelable(false)
                        .show();
            }
            return true;
        }
    };

    private MediaPlayer.OnBufferingUpdateListener mBufferingUpdateListener = new MediaPlayer.OnBufferingUpdateListener() {
        public void onBufferingUpdate(MediaPlayer mp, int percent) {
            mCurrentBufferPercentage = percent;
            if(mOnBufferingUpdateListener != null) {
            	mOnBufferingUpdateListener.onBufferingUpdate(mp, percent);
            }
        }
    };

    private MediaPlayer.OnInfoListener mInfoListener = new MediaPlayer.OnInfoListener() {
		@Override
		public boolean onInfo(MediaPlayer mp, int what, int extra) {
			if(mOnInfoListener != null) {
				mOnInfoListener.onInfo(mp, what, extra);
			}
			return false;
		}
	};

    private MediaPlayer.OnSeekCompleteListener mSeekCompleteListener = new MediaPlayer.OnSeekCompleteListener() {
		@Override
		public void onSeekComplete(MediaPlayer mp) {
			if(mOnSeekCompleteListener != null) {
				mOnSeekCompleteListener.onSeekComplete(mp);
			}
			
		}
	};
}