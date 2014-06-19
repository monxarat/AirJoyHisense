package com.fqx.anyplay.api;

import android.nfc.tech.IsoDep;

public class APPEnum {

	public enum AirAudioCmd {

		didStartPlayAudio 	(0),
		didPause			(1),
		didStop				(2),
		didSetVolume		(3),
		didSetImage			(4),
		didSetAudioInfo		(5);

		private int value;
		private AirAudioCmd(int v) {
			this.value = v;
		}

		public int GetValue() {
			return this.value;
		}
	}

	public enum AirChannel {
		AirNone 		(0),
		AirPlay			(1),
		AirJoy			(2),
		AirTunes		(3);

		private int value;
		private AirChannel(int v) {
			this.value = v;
		}

		public int GetValue() {
			return this.value;
		}
	}

	public enum AirImgCmd {

		didPutPhote						(0),
		didPutPhotoCacheOnly			(1),
		didStopPhotoOrSlideshow			(2),
		didDisplayCachedPhoto			(3),
		willPutPhoto					(4),
		willPutPhotoCacheOnly			(5),
		didGetSlideshowsPicture			(6),
		didSlidePictureStart			(7),
		didSlidePictureStop				(8),
		didSlidePictureOver				(9);

		private int value;
		private AirImgCmd(int v) {
			this.value = v;
		}
		

		public int GetValue() {
			return this.value;
		}
	}

	public enum AirVideoCmd {
		
		didStartPlayVideo				(0),
		didSetPlaybackRate				(1),
		didStopPlayback					(2),
		setCurrentPlaybackProgress		(3),
		didCreateEventSession			(4),
		didSetVolume					(5),
		didLifeVale						(6),
		didTryPlay						(7);

		private int value;
		private AirVideoCmd(int v) {
			this.value = v;
		}

		public int GetValue() {
			return this.value;
		}
	}

	public enum EventState {

		EventStateNone				(-1),
		EventStatePlaying			(0),
		EventStatePaused			(1),
		EventStateLoading			(2),
		EventStateStopped			(3),
		EventStateInit				(100);

		private int value;
		
		private EventState(int v) {
			this.value = v;
		}

		public int GetValue() {
			return this.value;
		}
	}

	public enum GalleryCallBackCmd {

		SINGLE_CLICKED			(0),
		DOUBLE_CLICKED			(1),
		SELECTED				(2);
		private int value;

		private GalleryCallBackCmd(int v) {
			this.value = v;
		}

		public int GetValue() {
			return this.value;
		}
	}

	public enum MediaType {

		MediaUnknown     (-1),
		MediaPhoto       (0),    // Õº∆¨
		MediaVideo       (1),    //  ”∆µ
		MediaSlideshow   (2),    // ª√µ∆∆¨
		MediaAudio		 (3);

		private int value;

		private MediaType(int v) {
			this.value = v;
		}

		public int GetValue() {
			return this.value;
		}
	}

	public enum ServerState {
	
		S_NONE			(0),
		S_NET_ERR		(1);
		private int value;

		private ServerState(int v) {
			this.value = v;
		}

		public int GetValue() {
			return this.value;
		}
	}

	public enum ServiceCmd {
		SessionEvent  		(0);

		private int value;
		private ServiceCmd(int v) {
			this.value = v;
		}

		public int GetValue() {
			return this.value;
		}
	}

	public enum ServiceState {

		Init			(0),
		OK				(1),
		Fail			(2);

		private int value;
		private ServiceState(int v) {
			this.value = v;
		}

		public int GetValue() {
			return this.value;
		}
	}

	public enum SlidesShowsEvent {
		Start			(0),
		Stop			(1),
		Err				(2);

		private int value;
		private SlidesShowsEvent(int v) {
			this.value = v;
		}

		public int GetValue() {
			return this.value;
		}
	}
	
	
	public enum MDCustomUI{
		fqx 					(100),
		hisense 				(101),
		pinjitongda 			(102);
		private int value;

		private MDCustomUI(int v) {
			this.value = v;
		}

		public int GetValue() {
			return this.value;
		}
	}

	
	public enum MDVideoM3U8{
		LocalPlayer				(200),
		APVideo 				(201);

		private int value;

		private MDVideoM3U8(int v) {
			this.value = v;
		}

		public int GetValue() {
			return this.value;
		}
	}
}
