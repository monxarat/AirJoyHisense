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
// �ỰID
//      == 0�������Ựʧ�ܣ�
//      > 0�������Ự�ɹ���
//
// ע��������Ϣ���豸����ȡ�豸��Ϣ�Ȳ����������ڽ���һ���Ự��
//     �ص������еĻỰID������ĻỰID����һ�¡�
//----------------------------------------------------------------------------
typedef uint32_t            AirJoySessionId;


namespace airjoy
{

    // �ط���Ϣ
    typedef struct _AirJoyPlayBackInfo
    {
        char            id[128 + 1];    // ��Ƶ/��ƵID
        char            url[1024 + 1];  // ��ַ
        char            name[128 + 1];  // ���ƣ���ѡ��
        float           rate;           // 0:��ͣ, 1:����
        float           position;       // ��ǰ����/��ͣʱ��㣬��λΪ�롣
        float           duration;       // 2012-10-31��ӣ���Ƶ�ܳ��ȣ���λ���룻
    } AirJoyPlayBackInfo;

    //----------------------------------------------------------------------------
    //
    // �豸����
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
    // �豸��Ϣ
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
    // ���ش���
    //
    //----------------------------------------------------------------------------
    typedef enum _ResultCode
    {
        ResultCodeUnknown         = 0,     // δ֪���
        ResultCodeOk              = 200,   // �����ɹ�
        ResultCodeError           = 400,   // ��ͨ����
        ResultCodeErrorParam      = 401,   // ��������
        ResultCodeErrorTimeout    = 402,   // ����ʱ
        ResultCodeErrorRefused    = 403,   // �ܾ�����
        ResultCodeErrorNotSupport = 404,   // ��֧�ֵ�����
        ResultCodeErrorConnection = 405,   // �������Ӵ���
    } ResultCode;

    //----------------------------------------------------------------------------
    //
    // Photo
    //
    //----------------------------------------------------------------------------
    typedef enum _PhotoActionType
    {
        PhotoActionUnknown    = 0,    // δ֪����
        PhotoActionDisplay    = 1,    // ��ʾ
        PhotoActionCache      = 2,    // ����
    } PhotoActionType;

    typedef enum _PhotoDirectionType
    {
        PhotoDirectionRight        = 1,    // ������ת
        PhotoDirectionLeft         = 3,    // ������ת
    } PhotoDirectionType;

    //----------------------------------------------------------------------------
    //
    // �ļ���Ϣ
    //
    //----------------------------------------------------------------------------
    typedef enum _FileType
    {
        FileTypeUnknown       = 0,      // δ֪����

        FileTypeFolder        = 1000,   // �ļ���
        FileTypeFile          = 2000,   // ��ͨ�ļ�

        FileTypeFilePicture       = 2001,       // ͼƬ�ļ�
        FileTypeFileMusic         = 2002,       // �����ļ�
        FileTypeFileVideo         = 2003,       // ��Ƶ�ļ�
        FileTypeFileText          = 2004,       // �ı��ļ�
        FileTypeFilePpt           = 2005,       // PPT�ļ�
        FileTypeFileApk           = 2006,       // apk�ļ�
        FileTypeFileRing          = 2007,       // �����ļ�
        FileTypeFilePdf           = 2008,       // pdf�ļ�
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
        uint32_t      index;            // ���ͣ���ǰ�ļ���������0��ʼ��
        uint32_t      total;            // ���ͣ��ļ�����
        FileType      type;             // ����
        std::string   sourceDeviceId;   // �ļ����ڵ�Դ�豸ID
        std::string   name;             // �ļ���
        uint64_t      size;             // �ļ���С
        FileTime      lastModify;       // �ļ�����޸�ʱ�䣨FileTime��ʽ��
        std::string   strLastModify;    // �ļ�����޸�ʱ�䣨�ַ�����ʽ��"2012-12-03 13:02:54"��
        std::string   urlHttp;          // �ɷ��ʵ�http��ַ
        std::string   urlRtsp;          // �ɷ��ʵ�rtsp��ַ(����Ϊ��)
        std::string   urlPreview;       // Ԥ��ͼ��http��ַ(����Ϊ��)
    } FileInfo;

    typedef std::list<FileInfo>    FileInfoList;
}

#endif // __AIRJOY_TYPEDEF_H__
