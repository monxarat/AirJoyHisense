/*
* AirTimer.hpp
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

#ifndef __AIR_TIMER_H__
#define __AIR_TIMER_H__

#include <boost/date_time/posix_time/posix_time.hpp>

namespace airjoy
{

    class AirTimer
    {
    public:
        AirTimer()
            : start_time_(boost::posix_time::microsec_clock::local_time())
        {
        }

        ~AirTimer()
        {
        }

        void restart() 
        {
            start_time_ = boost::posix_time::microsec_clock::local_time();
        }

        double elapsed()
        {
            boost::posix_time::time_duration d = boost::posix_time::microsec_clock::local_time() - start_time_;
            double result = (double)d.ticks();
            result /= d.ticks_per_second();

            return result;
        }

    private:
        boost::posix_time::ptime start_time_;
    };

}

#endif // __AIR_TIMER_H__
