package com.fqx.anyplay.api;


import com.fqx.airjoy.callback.OnCheckNetEvent;

public class CheckVideoNetState { 
	  private final int mTimes = 48;
	  private OnCheckNetEvent mOnCheckNetEvent = null;
	  private boolean mis_check = false;
	  private long ms_now_pos;
	  private int ms_times = 0;
	
	  public CheckVideoNetState() {
		  init();
	  }
	
	  private void doEvent(int paramInt) { 
		  if (this.mOnCheckNetEvent == null) { 
			  return; 
		  } 
		  this.mOnCheckNetEvent.doEvt(paramInt);
	  }
	
		public void checkNetState(long paramLong) {
		    if (!this.mis_check) {
		    	return;
		    }
		    if (this.ms_now_pos != paramLong) {
		        this.ms_now_pos = paramLong;
		        this.ms_times = 0;
		        doEvent(NetCode.OK.GetValue());
		        return;
		    }
		    this.ms_times = (1 + this.ms_times);
		    if (this.ms_times > 3) {
		      doEvent(NetCode.Buff.GetValue());
		    }
		    if(this.ms_times > mTimes) {
			    doEvent(NetCode.Err.GetValue());
			    stop();
		    }
		}
	
	   public void init() { 
		   this.ms_now_pos = 0L;
		   this.ms_times = 0;
		   this.mis_check = false;
	  }
	
	  public void setOnCheckNetEvent(OnCheckNetEvent paramOnCheckNetEvent) { 
		  this.mOnCheckNetEvent = paramOnCheckNetEvent;
	  }
	
	  public void start() { 
		  this.mis_check = true; 
		  this.ms_times = 0; 
		  doEvent(NetCode.OK.GetValue());
	  }
	
	  public void stop() { 
		  doEvent(NetCode.OK.GetValue()); 
		  this.mis_check = false;
	  }
	
	  public static enum NetCode {
	      Init			(0),
	      OK			(1),
	      Buff			(2),
	      Err			(3);
	
	      private int value;
	
	    private NetCode(int v) { 
	    	this.value = v;
	    }
	
	    public int GetValue() { 
	    	return this.value;
	    }
	  }
}
