/*
* coding.tom@gmail.com
* 
* AirJoyApi.h
*
* 2011-5-14
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*/

#ifndef __AIRJOY_API_H__
#define __AIRJOY_API_H__

//-----------------------------------------------------------------------------------------------
//
// AIRJOY_API
//
//-----------------------------------------------------------------------------------------------
#ifdef _MSC_VER
#     if (defined AirJoy_shared_EXPORTS) || (defined AirPlay_shared_EXPORTS) || (defined AirStream_shared_EXPORTS) || (defined AirTunes_shared_EXPORTS) || (defined All_shared_EXPORTS)
#		    define DLLEXPORT _declspec(dllexport)
#	  else
#		    define DLLEXPORT _declspec(dllimport)
#	  endif
#	
#	  define DLLLOCAL
#
#else
//#   ifdef __ANDROID__
//#	  define DLLEXPORT
//#	  define DLLLOCAL
//#   else
#	  define DLLEXPORT __attribute__ ((visibility("default")))
#	  define DLLLOCAL __attribute__ ((visibility("hidden")))
//#   endif // __ANDROID__
#endif

#ifdef AIRJOY_STATIC
#   define AIRJOY_API
#else
#   define AIRJOY_API DLLEXPORT
#endif


#endif // __AIRJOY_API_H__
