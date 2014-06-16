/*
* coding.tom@gmail.com 
*
* AirJoyTypedef.hpp
*
* 2012-3-5
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*/

#ifndef __AIRJOY_TYPEDEF_H__
#define __AIRJOY_TYPEDEF_H__

#include <string>
#include <list>

#if defined(_WIN32) || defined(_WIN64)
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned int      uint32_t;
typedef unsigned __int64  uint64_t;
typedef __int64           int64_t;
#else
#   include <stdint.h>
#endif

//----------------------------------------------------------------------------
// 会话ID
//      == 0：建立会话失败；
//      > 0：建立会话成功；
//
// 注：推送信息到设备，获取设备信息等操作，都属于建立一个会话；
//     回调函数中的会话ID与请求的会话ID保持一致。
//----------------------------------------------------------------------------
typedef uint32_t            AirJoySessionId;


namespace airjoy
{

    // 回放信息
    typedef struct _AirJoyPlayBackInfo
    {
        char            id[128 + 1];    // 视频/音频ID
        char            url[1024 + 1];  // 地址
        char            name[128 + 1];  // 名称（可选）
        float           rate;           // 0:暂停, 1:播放
        float           position;       // 当前播放/暂停时间点，单位为秒。
        float           duration;       // 2012-10-31添加，视频总长度，单位：秒；
    } AirJoyPlayBackInfo;

    //----------------------------------------------------------------------------
    //
    // 设备类型
    //
    //----------------------------------------------------------------------------
    typedef enum _DeviceType
    {
        DeviceTypeUnknown          = 0,

        DeviceTypePhone            = 100,
        DeviceTypePhoneIphone      = 101,
        DeviceTypePhoneAndroid     = 102,
        DeviceTypePhoneWindows     = 103,

        DeviceTypePad              = 200,
        DeviceTypePadIpad          = 201,
        DeviceTypePadAndroid       = 202,

        DeviceTypePc               = 300,
        DeviceTypePcMac            = 301,
        DeviceTypePcWindows        = 302,
        DeviceTypePcLinux	       = 303,
        DeviceTypePcChrome         = 304,

        DeviceTypeNotebook         = 400,
        DeviceTypeNotebookMac      = 401,
        DeviceTypeNotebookWindows  = 402,
        DeviceTypeNotebookLinux    = 403,
        DeviceTypeNotebookChrome   = 404,

        DeviceTypeProjector        = 500,

        DeviceTypeiPodTouch        = 600, // iPod touch

        DeviceTypeTv               = 700,
    } DeviceType;

    //----------------------------------------------------------------------------
    //
    // 设备信息
    //
    //----------------------------------------------------------------------------
    typedef struct _DeviceInfo
    {
        std::string deviceId;
        std::string name;
        DeviceType  type;
    } DeviceInfo;

    //----------------------------------------------------------------------------
    //
    // 返回代码
    //
    //----------------------------------------------------------------------------
    typedef enum _ResultCode
    {
        ResultCodeUnknown         = 0,     // 未知结果
        ResultCodeOk              = 200,   // 操作成功
        ResultCodeError           = 400,   // 普通错误
        ResultCodeErrorParam      = 401,   // 参数错误
        ResultCodeErrorTimeout    = 402,   // 请求超时
        ResultCodeErrorRefused    = 403,   // 拒绝访问
        ResultCodeErrorNotSupport = 404,   // 不支持的请求
        ResultCodeErrorConnection = 405,   // 网络连接错误
    } ResultCode;

    //----------------------------------------------------------------------------
    //
    // Photo
    //
    //----------------------------------------------------------------------------
    typedef enum _PhotoActionType
    {
        PhotoActionUnknown    = 0,    // 未知动作
        PhotoActionDisplay    = 1,    // 显示
        PhotoActionCache      = 2,    // 缓存
    } PhotoActionType;

    typedef enum _PhotoDirectionType
    {
        PhotoDirectionRight        = 1,    // 向右旋转
        PhotoDirectionLeft         = 3,    // 向左旋转
    } PhotoDirectionType;

    //----------------------------------------------------------------------------
    //
    // 文件信息
    //
    //----------------------------------------------------------------------------
    typedef enum _FileType
    {
        FileTypeUnknown       = 0,      // 未知类型

        FileTypeFolder        = 1000,   // 文件夹
        FileTypeFile          = 2000,   // 普通文件

        FileTypeFilePicture       = 2001,       // 图片文件
        FileTypeFileMusic         = 2002,       // 音乐文件
        FileTypeFileVideo         = 2003,       // 视频文件
        FileTypeFileText          = 2004,       // 文本文件
        FileTypeFilePpt           = 2005,       // PPT文件
        FileTypeFileApk           = 2006,       // apk文件
        FileTypeFileRing          = 2007,       // 铃声文件
        FileTypeFilePdf           = 2008,       // pdf文件
    } FileType;
    
    typedef struct _FileTime
    {
        int year;
        int month;
        int day;
        int hour;
        int minute;
        int second;
    } FileTime;

    typedef struct _FileInfo
    {
        uint32_t      index;            // 推送：当前文件索引（从0开始）
        uint32_t      total;            // 推送：文件总数
        FileType      type;             // 类型
        std::string   sourceDeviceId;   // 文件所在的源设备ID
        std::string   name;             // 文件名
        uint64_t      size;             // 文件大小
        FileTime      lastModify;       // 文件最后修改时间（FileTime格式）
        std::string   strLastModify;    // 文件最后修改时间（字符串格式："2012-12-03 13:02:54"）
        std::string   urlHttp;          // 可访问的http地址
        std::string   urlRtsp;          // 可访问的rtsp地址(可能为空)
        std::string   urlPreview;       // 预览图的http地址(可能为空)
    } FileInfo;

    typedef std::list<FileInfo>    FileInfoList;
}

#endif // __AIRJOY_TYPEDEF_H__
