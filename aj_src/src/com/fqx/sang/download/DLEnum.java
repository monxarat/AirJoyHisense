package com.fqx.sang.download;

public class DLEnum {

	public enum DownloadResult{
		ResultCodeProgress          (100),   // ï¿½ï¿½ï¿½ï¿½ï¿½É¹ï¿½
		ResultCodeError        		(101),   // ï¿½ï¿½Í¨ï¿½ï¿½ï¿½ï¿½
		ResultCodeComplete 			(102),   // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
		ResultCodeStop				(103),   // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
		ResultCodeCompleteALL 		(104),   // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
		ResultCodeSize				(105);   // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
		
		private final int value;
		private DownloadResult(int value){
            this.value=value;
		}
		
        public int GetValue(){
             return this.value;
       }
	} 
	
	public enum DownloadCMD {
		C_START 					(0),
		C_CANCEL 					(1);
		
		private int value;
		private DownloadCMD(int value){
            this.value=value;
		}
		
        public int GetValue(){
             return this.value;
       }
	} 
	
	public enum DownloadState{
		S_INIT 						(0),
		S_START						(1),
		S_DOING						(2),
		S_STOP						(3),
		S_COMPLETE					(4),
		S_ERR 						(5),
		S_READY						(6),
		S_CANCEL_BY_SEND 			(7),
		S_SD_FULL_ERR				(8);
		
		private int value;
		private DownloadState(int value){
            this.value=value;
		}
		
        public int GetValue(){
             return this.value;
       }
	} 
	
}
