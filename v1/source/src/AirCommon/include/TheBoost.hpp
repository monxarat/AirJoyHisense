/*
* TheBoost.hpp
*
*
* jxfengzi@gmail.com
*
* 2012-2-29
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*/


#ifndef __THE_BOOST_H__
#define __THE_BOOST_H__


//------------------------------------------------------
// boost
//------------------------------------------------------
#define BOOST_ALL_NO_LIB
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>
#include <boost/enable_shared_from_this.hpp>

//------------------------------------------------------
// boost::date_time
//------------------------------------------------------
#define BOOST_DATE_TIME_NO_LIB
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

//------------------------------------------------------
// boost::system
//------------------------------------------------------
#define BOOST_SYSTEM_NO_LIB
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/system/linux_error.hpp>

//------------------------------------------------------
// boost::thread
//------------------------------------------------------
#define BOOST_THREAD_NO_LIB
#include <boost/thread.hpp>

//------------------------------------------------------
// boost::asio
//------------------------------------------------------
#ifdef _MSC_VER
#   ifndef _WIN32_WINNT
#       define _WIN32_WINNT 0x0501
#   endif
#endif

#define BOOST_REGEX_NO_LIB
#include <boost/asio.hpp>

//------------------------------------------------------
// boost::asio
//------------------------------------------------------
#include <boost/signals2.hpp>

//------------------------------------------------------
// boost::uuids
//------------------------------------------------------
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

//------------------------------------------------------
// boost::filesystem
//------------------------------------------------------
#define BOOST_FILESYSTEM_NO_LIB
#define BOOST_FILESYSTEM_VERSION 2
#include <boost/filesystem.hpp>

//------------------------------------------------------
// boost::algorithm
//------------------------------------------------------
#include <boost/algorithm/string.hpp>

//------------------------------------------------------
// typedef
//------------------------------------------------------
typedef boost::shared_ptr<std::string> StringPointer;

#endif // __THE_BOOST_H__
