/*
 * JobQueue.hpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-7
 *
 */

#ifndef __JOB_QUEUE_H__
#define __JOB_QUEUE_H__

#include <queue>

#include <boost/noncopyable.hpp>
#include <boost/utility/value_init.hpp>
#include <boost/assert.hpp>
#include <boost/concept_check.hpp>

#define BOOST_ALL_NO_LIB
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace airjoy
{

    template <typename Job>

    class JobQueue : public boost::noncopyable
    {
    public:
        typedef Job                 JobType;
        typedef std::deque<JobType> QueueType;

        typedef boost::mutex                        MutexType;
        typedef typename MutexType::scoped_lock     LockType;
        typedef boost::condition_variable_any       ConditionType;

        BOOST_CONCEPT_ASSERT((boost::SGIAssignable<JobType>));
        BOOST_CONCEPT_ASSERT((boost::DefaultConstructible<JobType>));

    private:
        QueueType       m_queue;
        MutexType       m_mutex;
        ConditionType   m_hasJob;
        bool            m_isStop;

    public:
        JobQueue() 
            : m_isStop(false) 
        {
        }

        void push(const JobType & j)
        {
            LockType lock(m_mutex);
            m_queue.push_back(j);
            m_hasJob.notify_one();
        }

        JobType pop()
        {
            LockType lock(m_mutex);
            while (m_queue.empty() && ! m_isStop)
                m_hasJob.wait(m_mutex);

            if (m_isStop)
                return boost::initialized_value;

            BOOST_ASSERT(! m_queue.empty());

            JobType tmp = m_queue.front();
            m_queue.pop_front();

            return tmp;
        }

        void stop()
        {
            m_isStop = true;
            m_hasJob.notify_all();
        }
    };

}

#endif // __JOBS_QUEUE_H__