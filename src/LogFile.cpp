#include "LogFile.h"
#include "Utils.h"
#include "ProcessInfo.h"
#include <chrono>

namespace hying
{
	AppendFile::AppendFile(StringArg filename)
		: m_fp(fopen(filename.c_str(), "a")),
		  m_writtenBytes(0)
	{
		assert(m_fp);
		// m_fp流先存在m_buf缓存区，当m_buf满了再输出到文件
		setvbuf(m_fp, m_buffer, m_buffer?_IOFBF:_IONBF, sizeof m_buffer);
	}

	AppendFile::~AppendFile()
	{
		fclose(m_fp);
	}

	void AppendFile::append(const char* logline, const size_t len)
	{
		size_t written = 0;
		while (written != len)
		{
			size_t remain = len - written;
			size_t n = write(logline + written, remain);
			if (remain != n)
			{
				int err = ferror(m_fp);
				if (err)
				{
					fprintf(stderr, "AppendFile::append() failed %s\n", strerror_tl(err));
					break;
				}
			}
			written += n;
		}
		m_writtenBytes += written;
	}

	void AppendFile::flush()
	{
		fflush(m_fp);
	}

	size_t AppendFile::write(const char* logline, size_t len)
	{
		// 非线程安全的写函数，这样速度会快一点
		return _fwrite_nolock(logline, 1, len, m_fp);
	}

	LogFile::LogFile(const std::string& basename,
		off_t rollSize,
		bool threadSafe,
		int flushInterval,
		int checkEveryN)
		: m_basename(basename),
		m_rollSize(rollSize),
		m_flushInterval(flushInterval),
		m_checkEveryN(checkEveryN),
		m_count(0),
		m_mutex(threadSafe ? new std::mutex : NULL),
		m_startOfPeriod(0),
		m_lastRoll(0),
		m_lastFlush(0)
	{
		assert(basename.find('/') == std::string::npos);
		rollFile();
	}

	LogFile::~LogFile() = default;

	void LogFile::append(const char* logline, int len)
	{
		if (m_mutex)
		{
			std::unique_lock<std::mutex> lock(*m_mutex);
			append_unlocked(logline, len);
		}
		else
		{
			append_unlocked(logline, len);
		}
	}

	void LogFile::flush()
	{
		if (m_mutex)
		{
			std::unique_lock<std::mutex> lock(*m_mutex);
			m_file->flush();
		}
		else
		{
			m_file->flush();
		}
	}

	void LogFile::append_unlocked(const char* logline, int len)
	{
		m_file->append(logline, len);

		if (m_file->writtenBytes() > m_rollSize)
		{
			rollFile();
		}
		else
		{
			++m_count;
			if (m_count >= m_checkEveryN)
			{
				m_count = 0;
				time_t now = ::time(NULL);
				time_t thisPeriod = now / kRollPerSeconds * kRollPerSeconds;
				if (thisPeriod != m_startOfPeriod)
				{
					rollFile();
				}
				else if (now - m_lastFlush > m_flushInterval)
				{
					m_lastFlush = now;
					m_file->flush();
				}
			}
		}
	}

	bool LogFile::rollFile()
	{
		time_t now = 0;
		std::string filename = getLogFileName(m_basename, &now);
		time_t start = now / kRollPerSeconds * kRollPerSeconds;

		if (now > m_lastRoll)
		{
			m_lastRoll = now;
			m_lastFlush = now;
			m_startOfPeriod = start;
			m_file.reset(new AppendFile(filename));
			return true;
		}
		return false;
	}

	std::string LogFile::getLogFileName(const std::string& basename, time_t* now)
	{
		std::string filename;
		filename.reserve(basename.size() + 64);
		filename = basename;

		char timebuf[32];
		struct tm tm;
		*now = time(NULL);
		gmtime_r(now, &tm);
		strftime(timebuf, sizeof timebuf, ".%Y%m%d-%H%M%S.", &tm);
		filename += timebuf;
		filename += hostname();

		char pidbuf[32];
		snprintf(pidbuf, sizeof pidbuf, ".%d", pid());
		filename += pidbuf;
		filename += ".log";
		return filename;
	}

}