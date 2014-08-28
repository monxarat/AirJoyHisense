package com.fqx.anyplay.api;


import com.airjoy.airplay.server.AirPlayServer;
import com.airjoy.airplay.server.AirPlayServer.EventAction;
import com.airjoy.airplay.server.AirPlayServer.EventCategory;
import com.fqx.airjoy.server.AirJoy;

public class PublishState {

	private static PublishState instance = null;
	private int mChannel = APPEnum.AirChannel.AirNone.GetValue();
	private String mClientIP = "";
	private boolean mISlidesShowsMode = false;
	private int mMediaType = APPEnum.MediaType.MediaUnknown.GetValue();
	private String mResID = "";
	private String mResName = "";
	private String mResUrl = "";
	private int mState = APPEnum.EventState.EventStateNone.GetValue();

	public static PublishState getInstance() { 
		if (instance == null) { 
			instance = new PublishState(); 
		}
		return instance;
	}

	private String getMediaString(int paramInt) {
		String str;
		if (paramInt == APPEnum.MediaType.MediaPhoto.GetValue()) {
			str = "photo";
		}else if (paramInt == APPEnum.MediaType.MediaVideo.GetValue()) {
		      str = "video";
		}else if (paramInt == APPEnum.MediaType.MediaAudio.GetValue()) {
		      str = "audio";
		}else if (paramInt != APPEnum.MediaType.MediaAudio.GetValue()) {
		    str = "audio";
		}else {
		  str = "none";
	    }
	    
	    return str;
	}

	private String getStateString(int paramInt) {
		String str;
	    if (paramInt == APPEnum.EventState.EventStatePlaying.GetValue()) {
	    	str = "play"; 
	    }else if (paramInt == APPEnum.EventState.EventStatePaused.GetValue()) {
	          str = "pause";
	    }else if (paramInt == APPEnum.EventState.EventStateStopped.GetValue()) {
	          str = "stop";
	    }else if (paramInt != APPEnum.EventState.EventStateInit.GetValue()) {
	        str = "init";
	    }else {
			  str = "none";
		}
		return str;
	}
	
	private EventCategory getMediaType(int type) {
		if (type == APPEnum.MediaType.MediaPhoto.GetValue()) {
			return EventCategory.EventPhoto;
		}else if (type == APPEnum.MediaType.MediaVideo.GetValue()) {
			return EventCategory.EventVideo;
		}else if (type == APPEnum.MediaType.MediaAudio.GetValue()) {
		}else if (type != APPEnum.MediaType.MediaSlideshow.GetValue()) {
			return EventCategory.EventSlideshow;
		}else {
	    }
		return EventCategory.EventUnknown;
	}

	private EventAction getEventAction(int state) {
	    if (state == APPEnum.EventState.EventStatePlaying.GetValue()) {
	    	return EventAction.EventActionPlaying;
	    }else if (state == APPEnum.EventState.EventStatePaused.GetValue()) {
	    	return EventAction.EventActionPaused;
	    }else if (state == APPEnum.EventState.EventStateStopped.GetValue()) {
	    	return EventAction.EventActionStopped;
	    }
	    return EventAction.EventActionNone;
	}
	
	private void AirPlayPublishEvent(int MediaType, int State, String ip) {
		EventCategory eCategory = getMediaType(MediaType);
		EventAction eAction = getEventAction(State);
		AirPlayServer.getInstance().publishEvent(eCategory, eAction, ip);
	}

	public void PublishEvent(int paramInt) {
	    this.mState = paramInt;
	    String str1 = getMediaString(this.mMediaType);
	    String str2 = getStateString(this.mState);
	    if (this.mChannel == APPEnum.AirChannel.AirJoy.GetValue()) {
	      AirJoy.publishMediaEvent(str1, str2, this.mResID, this.mResUrl, this.mResName, this.mClientIP);
	    }else if (this.mChannel == APPEnum.AirChannel.AirPlay.GetValue()) {
	      if (this.mState == APPEnum.EventState.EventStateStopped.GetValue()) {
		      AirPlayPublishEvent(this.mMediaType, this.mState, this.mClientIP);
		      AirPlayPublishEvent(this.mMediaType, APPEnum.EventState.EventStateInit.GetValue(), this.mClientIP);
	      }else{
		      AirPlayPublishEvent(this.mMediaType, this.mState, this.mClientIP);
	      }
	      AirJoy.publishMediaEvent(str1, str2, this.mResID, "", "", this.mClientIP);
	      AnyPlayUtils.LOG_DEBUG("PublishEvent", "Type:" + mMediaType + " State:" + mState + " IP" + mClientIP);
	    }
	}

	public void SendEvent(int paramInt) {
	    AnyPlayUtils.LOG_DEBUG("PublishEvent 2", "Type:" + mMediaType + " State:" + mState + " IP" + mClientIP);
	    AirPlayPublishEvent(this.mMediaType, paramInt, this.mClientIP);
	}


	public void SetChannel(int paramInt, String paramString) {
	    if (this.mChannel == APPEnum.AirChannel.AirNone.GetValue()) {
	    	this.mChannel = paramInt;
			this.mClientIP = paramString;
			return;
	    }
	    
	    if(this.mChannel != paramInt) {
	    	PublishEvent(APPEnum.EventState.EventStateStopped.GetValue());
	    }
	
		if(this.mClientIP.equals(paramString) == false) {
		    PublishEvent(APPEnum.EventState.EventStateStopped.GetValue());
		}
	    
	    this.mChannel = paramInt;
		this.mClientIP = paramString;
	}

	public void SetMediaAudio(int paramInt) {
	}

	public void SetMediaPhotoState(int paramInt) {
	    this.mMediaType = APPEnum.MediaType.MediaPhoto.GetValue();
	    PublishEvent(paramInt);
	}

	public void SetMediaSlideshow(int paramInt) {
	    this.mMediaType = APPEnum.MediaType.MediaSlideshow.GetValue();
	    PublishEvent(paramInt);
	}

	public void SetMediaVideo(int paramInt) {
	    this.mMediaType = APPEnum.MediaType.MediaVideo.GetValue();
	    PublishEvent(paramInt);
	}

	public void SetMediaVideoRate(int paramInt) {
	    if (paramInt == 0) {
	      PublishEvent(APPEnum.EventState.EventStatePaused.GetValue());
	    }else {
	      PublishEvent(APPEnum.EventState.EventStatePlaying.GetValue());
	    }
	}

	public void UpdateResInfo(String paramString1, String paramString2, String paramString3) {
	    this.mResID = paramString1;
	    this.mResUrl = paramString2;
	    this.mResName = paramString3;
	}

	public String getClientIP() {
	    return this.mClientIP;
	}

	public boolean iSlidesShowMode() {
	    return this.mISlidesShowsMode;
	}

	public void init() {
	    PublishEvent(APPEnum.EventState.EventStateInit.GetValue());
	}

	public boolean isMediaVideo() {
	    if (this.mMediaType == APPEnum.MediaType.MediaVideo.GetValue()) {
	    	return true;
	    }
	    return false;
	}

	public void setSlidesShowMode(boolean paramBoolean) {
	    this.mISlidesShowsMode = paramBoolean;
	}

	public void tryInit() {
	    AirPlayPublishEvent(APPEnum.MediaType.MediaVideo.GetValue(), APPEnum.EventState.EventStateStopped.GetValue(), "127.0.0.1");
	}

	public boolean tryStopVideo() {
	    if (this.mMediaType == APPEnum.MediaType.MediaVideo.GetValue()) {
	      PublishEvent(APPEnum.EventState.EventStateStopped.GetValue());
	      return true;
	    }
	    return false;
	}

}
