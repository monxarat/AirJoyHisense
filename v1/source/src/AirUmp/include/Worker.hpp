/*
 * Worker.hpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-7
 *
 *
 */

#ifndef __WORKER_H__
#define __WORKER_H__

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#define BOOST_ALL_NO_LIB
#include <boost/thread.hpp>

#include "JobQueue.hpp"
#include "AirJoyLog.hpp"

namespace airjoy
{

    template<typename Queue>

    class Worker
    {
    public:
        typedef Queue                               QueueType;
        typedef typename QueueType::JobType         JobType;
        typedef boost::function<bool(JobType &)>    FuncType;
        typedef boost::shared_ptr<QueueType>        QueuePointer;

    private:
        QueuePointer    m_queue;
        FuncType        m_func;

        int                     m_numberOfThread;
        boost::thread_group     m_threads;

    public:
        template<typename Func>
        Worker(const QueuePointer & q, Func func, int n = 1)
            : m_queue(q)
            , m_func(func)
            , m_numberOfThread(n)
        {
            LOGI("Worker::Worker()");

            BOOST_ASSERT(n > 0);
        }

        Worker(const QueuePointer & q, int n = 1)
            : m_queue(q)
            , m_numberOfThread(n)
        {
            BOOST_ASSERT(n > 0);
        }

        void start()
        {
            BOOST_ASSERT(m_func);

            if (m_threads.size() > 0)
                return;

            for (int i = 0; i < m_numberOfThread; ++i) 
                m_threads.create_thread(boost::bind(&Worker::doWork, this));
        }

        template<typename Func>
        void start(Func func)
        {
            m_func = func;
            start();
        }

        void run()
        {
            start();
            m_threads.join_all();
        }

        void stop()
        {
            m_func = 0;
            m_queue->stop();
        }

    private:
        void doWork()
        {
            while (true)
            {
                JobType job = m_queue->pop();

                if (! m_func)
                    break;

                if (! m_func(job))
                    break;
            }
        }
    };

}

#endif // __WORKER_H__
