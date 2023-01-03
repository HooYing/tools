#ifndef __ASYNCLOGGING_H__
#define __ASYNCLOGGING_H__

#include "pch.h"
#include "Thread.h"
#include "LogFile.h"
#include "LogStream.h"
#include <vector>
#include <condition_variable>

namespace hying
{
	class AsyncLogging : public IThreadCallBack
	{
	public:
        AsyncLogging(const std::string& basename,
            off_t rollSize,
            int flushInterval = 3);

        ~AsyncLogging()
        {
            if (m_thread.isRunning())
            {
                stop();
            }
        }

        void append(const char* logline, int len);

        void start()
        {
            m_thread.start();
            m_latch.wait();
        }

        void stop()
        {
            m_thread.stop();
            m_cond.notify_one();
            m_thread.join();
        }

    private:

        virtual void OnActive() override;

        typedef FixedBuffer<kLargeBuffer> Buffer;
        typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
        typedef BufferVector::value_type BufferPtr;

        const int m_flushInterval;
        const std::string m_basename;
        const off_t m_rollSize;
        Thread m_thread;
        CondCount m_latch;
        std::mutex m_mutex;
        std::condition_variable m_cond;
        BufferPtr m_currentBuffer;
        BufferPtr m_nextBuffer;
        BufferVector m_buffers;
	};
}

#endif