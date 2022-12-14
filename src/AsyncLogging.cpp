#include "AsyncLogging.h"
#include "TimeStamp.h"

using namespace hying;

AsyncLogging::AsyncLogging(const std::string& basename,
    off_t rollSize,
    int flushInterval)
    : m_flushInterval(flushInterval),
    m_basename(basename),
    m_rollSize(rollSize),
    m_thread(this, "Logging"),
    m_latch(1),
    m_mutex(),
    m_cond(),
    m_currentBuffer(new Buffer),
    m_nextBuffer(new Buffer),
    m_buffers()
{
    m_currentBuffer->bzero();
    m_nextBuffer->bzero();
    m_buffers.reserve(16);
}

void AsyncLogging::append(const char* logline, int len)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    if (m_currentBuffer->avail() > len)
    {
        m_currentBuffer->append(logline, len);
    }
    else
    {
        m_buffers.push_back(std::move(m_currentBuffer));

        if (m_nextBuffer)
        {
            m_currentBuffer = std::move(m_nextBuffer);
        }
        else
        {
            m_currentBuffer.reset(new Buffer); // Rarely happens
        }
        m_currentBuffer->append(logline, len);
        m_cond.notify_one();
    }
}

void AsyncLogging::OnActive()
{
    m_latch.countDown();
    LogFile output(m_basename, m_rollSize, false);
    BufferPtr newBuffer1(new Buffer);
    BufferPtr newBuffer2(new Buffer);
    newBuffer1->bzero();
    newBuffer2->bzero();
    BufferVector buffersToWrite;
    buffersToWrite.reserve(16);
    while (m_thread.isRunning())
    {
        assert(newBuffer1 && newBuffer1->length() == 0);
        assert(newBuffer2 && newBuffer2->length() == 0);
        assert(buffersToWrite.empty());

        {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (m_buffers.empty())  // unusual usage!
            {
                m_cond.wait_for(lock, std::chrono::seconds(m_flushInterval));
            }
            m_buffers.push_back(std::move(m_currentBuffer));
            m_currentBuffer = std::move(newBuffer1);
            buffersToWrite.swap(m_buffers);
            if (!m_nextBuffer)
            {
                m_nextBuffer = std::move(newBuffer2);
            }
        }

        assert(!buffersToWrite.empty());

        if (buffersToWrite.size() > 25)
        {
            char buf[256];
            snprintf(buf, sizeof buf, "Dropped log messages at %s, %zd larger buffers\n",
                Timestamp::now().toFormattedString().c_str(),
                buffersToWrite.size() - 2);
            fputs(buf, stderr);
            output.append(buf, static_cast<int>(strlen(buf)));
            buffersToWrite.erase(buffersToWrite.begin() + 2, buffersToWrite.end());
        }

        for (const auto& buffer : buffersToWrite)
        {
            // FIXME: use unbuffered stdio FILE ? or use ::writev ?
            output.append(buffer->data(), buffer->length());
        }

        if (buffersToWrite.size() > 2)
        {
            // drop non-bzero-ed buffers, avoid trashing
            buffersToWrite.resize(2);
        }

        if (!newBuffer1)
        {
            assert(!buffersToWrite.empty());
            newBuffer1 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newBuffer1->reset();
        }

        if (!newBuffer2)
        {
            assert(!buffersToWrite.empty());
            newBuffer2 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newBuffer2->reset();
        }

        buffersToWrite.clear();
        output.flush();
    }
    output.flush();
}

