/*
* BoostThreadPrebuild.cpp
*
*
* coding.tom@gmail.com
*
* 2012-5-23
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*/


#define BOOST_DATE_TIME_SOURCE
#define BOOST_THREAD_NO_LIB

#ifdef _MSC_VER
namespace boost
{
    extern "C" void tss_cleanup_implemented(void) {}
}
#include <libs/thread/src/win32/thread.cpp>
#include <libs/thread/src/win32/tss_dll.cpp>
#include <libs/thread/src/win32/tss_pe.cpp>
#else
#include <libs/thread/src/pthread/thread.cpp>
#include <libs/thread/src/pthread/once.cpp>
#endif