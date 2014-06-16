/*
* BoostFileSystemPrebuild.cpp
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


#define BOOST_SYSTEM_NO_LIB
#define BOOST_FILESYSTEM_NO_LIB
#define BOOST_FILESYSTEM_VERSION 2
#include <boost/filesystem.hpp>

#include <libs/filesystem/v2/src/v2_operations.cpp>
#include <libs/filesystem/v2/src/v2_path.cpp>
#include <libs/filesystem/v2/src/v2_portability.cpp>

#include <libs/filesystem/v3/src/utf8_codecvt_facet.cpp>
