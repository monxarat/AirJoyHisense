

package com.fqx.airjoy.server;
public class AJEnum {
	public enum AirJoyPhotoCmd {
		cachePhoto			(0),
     	displayPhoto		(1),
       	displayCachedPhoto 	(2),
      	stopPhoto 			(3),
		rotatePhoto 		(4),
		zoomPhoto 			(5), 
		movePhoto 			(6); 

		private final int value;

 	   private AirJoyPhotoCmd(int value) {
		   this.value = value;
	   } 

	   public int GetValue() {
		  return this.value;
	   }
	}


	public enum AirJoyStatus {

		DISCONNECT 			(0),
		CONNECTED			(1),
		INITSESSION			(2),
		SESSION				(3),
		CLOSESESSION		(4);

		private final int value; 
		private AirJoyStatus(int value) {
			this.value = value;
		}

		public int GetValue() {
			return this.value;
		}
	}

	public enum DeviceType {
		DeviceTypeUnknown          (0),

        DeviceTypePhone            (100),
        DeviceTypePhoneIphone      (101),
        DeviceTypePhoneAndroid     (102),
        DeviceTypePhoneWindows     (103),

        DeviceTypePad              (200),
        DeviceTypePadIpad          (201),
        DeviceTypePadAndroid       (202),

        DeviceTypePc               (300),
        DeviceTypePcMac            (301),
        DeviceTypePcWindows        (302),
        DeviceTypePcLinux	       (303),
        DeviceTypePcChrome         (304),

        DeviceTypeNotebook         (400),
        DeviceTypeNotebookMac      (401),
        DeviceTypeNotebookWindows  (402),
        DeviceTypeNotebookLinux    (403),
        DeviceTypeNotebookChrome   (404),

        DeviceTypeProjector        (500),

        DeviceTypeiPodTouch        (600), // iPod touch

        DeviceTypeTv               (700);
    
		private final int value;
		private DeviceType(int value) {
			this.value = value;
		}

		public int GetValue() {
			return this.value;
		}
	}

	public enum FileType {

	    FileTypeUnknown    		  (0),     		
        FileTypeFolder       	  (1000),   
        FileTypeFile          	  (2000),   
        FileTypeFilePicture       (2001),   
        FileTypeFileMusic         (2002),   
        FileTypeFileVideo         (2003),   
        FileTypeFileText          (2004),   
        FileTypeFilePpt           (2005),   
        FileTypeFileApk           (2006),   
        FileTypeFileRing          (2007),   
        FileTypeFilePdf           (2008);   

		private final int value; 

		private FileType(int value) {
			this.value = value;
		}
		public int GetValue() {
			return this.value;
		}
	}

	public enum PhotoActionType {
		PhotoActionUnknown    (0),   
        PhotoActionDisplay    (1),   
        PhotoActionCache      (2);   

		private final int value;

		private PhotoActionType(int value) {
			this.value = value;
		}

		public int GetValue() {
			return this.value;
		}
	}

	public enum PhotoDirectionType {

		PhotoDirectionRight        (1), 
        PhotoDirectionLeft         (3); 

		private final int value;

		private PhotoDirectionType(int value) {
			this.value = value;
		}

		public int GetValue() {
			return this.value;
		}
	}

	public enum ResultCode {
		ResultCodeUnknown         (0),   
        ResultCodeOk              (200), 
        ResultCodeError           (400), 
        ResultCodeErrorParam      (401), 
        ResultCodeErrorTimeout    (402), 
        ResultCodeErrorRefused    (403), 
        ResultCodeErrorNotSupport (404), 
        ResultCodeErrorConnection (405); 


		private final int value;

		private ResultCode(int value) {
			this.value = value;
		}

		public int GetValue() {
			return this.value;
		}
	}
}
