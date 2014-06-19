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

	    FileTypeUnknown    		  (0),     		// 未知类型
        FileTypeFolder       	  (1000),   	// 文件夹
        FileTypeFile          	  (2000),   	// 普通文件
        FileTypeFilePicture       (2001),       // 图片文件
        FileTypeFileMusic         (2002),       // 音乐文件
        FileTypeFileVideo         (2003),       // 视频文件
        FileTypeFileText          (2004),       // 文本文件
        FileTypeFilePpt           (2005),       // PPT文件
        FileTypeFileApk           (2006),       // apk文件
        FileTypeFileRing          (2007),       // 铃声文件
        FileTypeFilePdf           (2008);       // pdf文件

		private final int value; 

		private FileType(int value) {
			this.value = value;
		}
		public int GetValue() {
			return this.value;
		}
	}

	public enum PhotoActionType {
		PhotoActionUnknown    (0),    // 未知动作
        PhotoActionDisplay    (1),    // 显示
        PhotoActionCache      (2);    // 缓存

		private final int value;

		private PhotoActionType(int value) {
			this.value = value;
		}

		public int GetValue() {
			return this.value;
		}
	}

	public enum PhotoDirectionType {

		PhotoDirectionRight        (1),    // 向右旋转
        PhotoDirectionLeft         (3);    // 向左旋转 

		private final int value;

		private PhotoDirectionType(int value) {
			this.value = value;
		}

		public int GetValue() {
			return this.value;
		}
	}

	public enum ResultCode {
		ResultCodeUnknown         (0),     // 未知结果
        ResultCodeOk              (200),   // 操作成功
        ResultCodeError           (400),   // 普通错误
        ResultCodeErrorParam      (401),   // 参数错误
        ResultCodeErrorTimeout    (402),   // 请求超时
        ResultCodeErrorRefused    (403),   // 拒绝访问
        ResultCodeErrorNotSupport (404),   // 不支持的请求
        ResultCodeErrorConnection (405);   // 网络连接错误


		private final int value;

		private ResultCode(int value) {
			this.value = value;
		}

		public int GetValue() {
			return this.value;
		}
	}
}
