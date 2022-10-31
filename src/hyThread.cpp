#include "hyThread.h"
#include "hyCurrentThread.h"

namespace hying
{
    struct ThreadData
    {
        typedef Thread::ThreadFunc ThreadFunc;
        ThreadFunc func_;
        std::string name_;
        int* tid_;
        CondCount* latch_;

        ThreadData(ThreadFunc func,
            const std::string& name,
            int* tid,
            CondCount* latch)
            : func_(std::move(func)),
            name_(name),
            tid_(tid),
            latch_(latch)
        { }

        void runInThread()
        {
            *tid_ = CurrentThread::tid();
            tid_ = NULL;
            latch_->countDown();
            latch_ = NULL;

            CurrentThread::t_threadName = name_.empty() ? "muduoThread" : name_.c_str();
            ::prctl(PR_SET_NAME, muduo::CurrentThread::t_threadName);
            try
            {
                func_();
                muduo::CurrentThread::t_threadName = "finished";
            }
            catch (const Exception& ex)
            {
                muduo::CurrentThread::t_threadName = "crashed";
                fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
                fprintf(stderr, "reason: %s\n", ex.what());
                fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
                abort();
            }
            catch (const std::exception& ex)
            {
                muduo::CurrentThread::t_threadName = "crashed";
                fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
                fprintf(stderr, "reason: %s\n", ex.what());
                abort();
            }
            catch (...)
            {
                muduo::CurrentThread::t_threadName = "crashed";
                fprintf(stderr, "unknown exception caught in Thread %s\n", name_.c_str());
                throw; // rethrow
            }
        }
    };
}