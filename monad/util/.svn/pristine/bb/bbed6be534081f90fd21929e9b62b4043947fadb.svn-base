#ifndef UTIL_PIPESTREAM_H
#define UTIL_PIPESTREAM_H
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "util.h"

namespace util
{

namespace pipestream_manip {
	struct setmax_t {
		size_t max;
	};

	struct enable_multiline_strings_t {
		bool value;
	};
}
inline pipestream_manip::setmax_t setmax(size_t max)
{
	pipestream_manip::setmax_t retval = { max };
	return retval;
}

const pipestream_manip::enable_multiline_strings_t enable_multiline_strings  = { true };
const pipestream_manip::enable_multiline_strings_t disable_multiline_strings = { false };

template <size_t buffersize>
class sizedpipestream
{
	protected:
	uint8_t wbuffer[buffersize+1]; // +1 to avoid 0 sized arrays
	uint8_t rbuffer[buffersize+1];
	size_t wbufferpos;
	size_t rbufferpos;
	size_t rbuffersize;
	size_t maxopsize;
	int fds[2];
	bool is_eof;
	bool enable_multiline_strings;
	static const int READ_END  = 0;
	static const int WRITE_END = 1;

	public:
	sizedpipestream();
	
	~sizedpipestream();
	void close();
	void close_read();
	void close_write();
	
	bool eof();

	void flush();
	int fill(size_t byte_count);

	int steal_output(int fd);
	int steal_input(int fd);
	
	int get_read_fd();
	int get_write_fd();

	sizedpipestream<buffersize> & operator<<(const string & str);
	sizedpipestream<buffersize> & operator<<(const char * str);
	sizedpipestream<buffersize> & operator<<(const   signed char  & value);
	sizedpipestream<buffersize> & operator<<(const   signed short & value);
	sizedpipestream<buffersize> & operator<<(const   signed int   & value);
	sizedpipestream<buffersize> & operator<<(const   signed long  & value);
	sizedpipestream<buffersize> & operator<<(const unsigned char  & value);
	sizedpipestream<buffersize> & operator<<(const unsigned short & value);
	sizedpipestream<buffersize> & operator<<(const unsigned int   & value);
	sizedpipestream<buffersize> & operator<<(const unsigned long  & value);

	sizedpipestream<buffersize> & operator>>(string & str);
	sizedpipestream<buffersize> & operator>>(char * & str);
	sizedpipestream<buffersize> & operator>>(  signed char  & value);
	sizedpipestream<buffersize> & operator>>(  signed short & value);
	sizedpipestream<buffersize> & operator>>(  signed int   & value);
	sizedpipestream<buffersize> & operator>>(  signed long  & value);
	sizedpipestream<buffersize> & operator>>(unsigned char  & value);
	sizedpipestream<buffersize> & operator>>(unsigned short & value);
	sizedpipestream<buffersize> & operator>>(unsigned int   & value);
	sizedpipestream<buffersize> & operator>>(unsigned long  & value);

	sizedpipestream<buffersize> & operator<<(pipestream_manip::enable_multiline_strings_t value);
	sizedpipestream<buffersize> & operator>>(pipestream_manip::setmax_t max);

	protected:
	template <typename T>
	sizedpipestream<buffersize> & write_primitive(T value);
	template <typename T>
	int read_primitive(T & value);

	private:
	sizedpipestream(sizedpipestream<buffersize> & other);
	sizedpipestream<buffersize> & operator=(sizedpipestream<buffersize> & other);
};

typedef sizedpipestream<512> pipestream;

#include "pipestream.cpp"

} // namespace util
#endif
