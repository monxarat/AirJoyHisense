/*
 * Dmap.hpp
 *
 * coding.tom@gmail.com
 *
 * 2013-2-6
 *
 */

#ifndef __DMAP_H__
#define __DMAP_H__

#include "AirJoyTypeDef.hpp"

#include <string>
#include <map>

namespace airjoy
{

    class Dmap
    {
    public:
        Dmap(const std::string &data);
        virtual ~Dmap();

        const std::string & get(const std::string &name);
    };

}

#endif // __DMAP_H__
