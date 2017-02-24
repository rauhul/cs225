#ifdef UTIL_PIPESTREAM_H

template <size_t buffersize>
sizedpipestream<buffersize>::sizedpipestream()
	: wbufferpos(0), rbufferpos(buffersize), rbuffersize(buffersize),
	  maxopsize(-1), is_eof(false), enable_multiline_strings(true)
{
	// Give a compile time error if an invalid buffsize is specified
	// This is necessary for much easier primitive reading/writing
	// The buffer must be either 0 or at least large enough to hold any
	// primitive (probably 8 or 16 bytes)
	char buffer_size_must_be_0_or_over_16
		[(buffersize != 0 && buffersize < sizeof(intmax_t)) ? -1 : 1];
	buffer_size_must_be_0_or_over_16[0] = '\0';

	// Create the pipe
	pipe(fds);
}

template <size_t buffersize>
sizedpipestream<buffersize>::~sizedpipestream()
{
	close();
}

template <size_t buffersize>
void sizedpipestream<buffersize>::close()
{
	close_write();
	close_read();
}

template <size_t buffersize>
void sizedpipestream<buffersize>::close_read()
{
	if (fds[READ_END] == -1) return;
	::close(fds[READ_END]);
	fds[READ_END] = -1;
}

template <size_t buffersize>
void sizedpipestream<buffersize>::close_write()
{
	if (fds[WRITE_END] == -1) return;
	flush();
	::close(fds[WRITE_END]);
	fds[WRITE_END] = -1;
}

template <size_t buffersize>
int sizedpipestream<buffersize>::steal_output(int fd)
{
	::close(fd);
	dup2(fds[WRITE_END], fd);
	return 0;
}

template <size_t buffersize>
int sizedpipestream<buffersize>::steal_input(int fd)
{
	::close(fd);
	dup2(fds[READ_END], fd);
	return 0;
}

template <size_t buffersize>
int sizedpipestream<buffersize>::get_read_fd()
{
	return fds[READ_END];
}

template <size_t buffersize>
int sizedpipestream<buffersize>::get_write_fd()
{
	return fds[WRITE_END];
}


template <size_t buffersize>
void sizedpipestream<buffersize>::flush()
{
	if (buffersize == 0 || wbufferpos == 0 || fds[WRITE_END] == -1)
		return;

	writen(fds[WRITE_END], &wbuffer[0], wbufferpos);
	wbufferpos = 0;
}

template <size_t buffersize>
int sizedpipestream<buffersize>::fill(size_t byte_count)
{
	if (buffersize == 0 || rbufferpos == 0 || fds[READ_END] == -1)
		return -1;

	if (buffersize - rbufferpos < byte_count)
	{
		memmove(&rbuffer[0], &rbuffer[rbufferpos], rbuffersize - rbufferpos);
		rbuffersize -= rbufferpos;
		rbufferpos = 0;
	}

	while (rbuffersize - rbufferpos < byte_count)
	{
		ssize_t read_count = ::read(fds[READ_END],
		                            &rbuffer[rbuffersize],
		                            buffersize - rbuffersize);
		if (read_count == 0) return 0;
		if (read_count == -1)
		{
			if (errno == EINTR) continue;
			return -1;
		}
		rbuffersize += read_count;
	}

	return rbuffersize - rbufferpos;
}

template <size_t buffersize>
sizedpipestream<buffersize> & sizedpipestream<buffersize>::operator<<(const string & str)
{
	if (buffersize == 0)
	{
		writen(fds[WRITE_END], str.c_str(), str.length()+1);
		return *this;
	}

	size_t len = str.length();
	for (size_t i = 0; i < len; i++)
	{
		if (wbufferpos == buffersize) flush();
		wbuffer[wbufferpos++] = str[i];
	}
	if (wbufferpos == buffersize) flush();
	wbuffer[wbufferpos++] = '\0';
	return *this;
}

template <size_t buffersize>
sizedpipestream<buffersize> & sizedpipestream<buffersize>::operator<<(const char * str)
{
	if (buffersize == 0)
	{
		writen(fds[WRITE_END], str, strlen(str)+1);
		return *this;
	}

	for (size_t i = 0; str[i] != '\0'; i++)
	{
		if (wbufferpos == buffersize) flush();
		wbuffer[wbufferpos++] = str[i];
	}
	if (wbufferpos == buffersize) flush();
	wbuffer[wbufferpos++] = '\0';
	return *this;
}

template <size_t buffersize>
sizedpipestream<buffersize> & sizedpipestream<buffersize>::operator>>(char * & str)
{
	size_t capacity = 4;
	str = new char[capacity];
	size_t len = 0;

	int error = 0;
	char c = '\0';
	do
	{
		error = read_primitive<char>(c);
		if (len == maxopsize) continue;
		
		if (len == capacity)
		{
			char * temp = str;
			str  = new char[capacity*2];
			memcpy(str, temp, capacity);
			capacity *= 2;
			delete [] temp;
		}
		str[len++] = c;

	} while (error > 0 && c != '\0' && (enable_multiline_strings || c != '\n'));
	
	str[len-1] = '\0';

	if (!enable_multiline_strings && len > 2 && str[len-2] == '\r')
		str[len-2] = '\0';

	maxopsize = -1;
	return *this;
}

template <size_t buffersize>
sizedpipestream<buffersize> & sizedpipestream<buffersize>::operator>>(string & str)
{
	char * cstr = NULL;
	operator>>(cstr);
	str = cstr;
	delete [] cstr;
	return *this;
}

template <size_t buffersize>
sizedpipestream<buffersize> & sizedpipestream<buffersize>::operator<<
	(pipestream_manip::enable_multiline_strings_t value)
{
	enable_multiline_strings = value.value;
	return *this;
}

template <size_t buffersize>
sizedpipestream<buffersize> & sizedpipestream<buffersize>::operator>>
	(pipestream_manip::setmax_t max)
{
	maxopsize = max.max;
	return *this;
}

template <size_t buffersize>
inline bool sizedpipestream<buffersize>::eof()
{ return is_eof; }

template <size_t buffersize>
inline sizedpipestream<buffersize> & sizedpipestream<buffersize>::operator<<(const signed char & value)
{ return write_primitive(value); }

template <size_t buffersize>
inline sizedpipestream<buffersize> & sizedpipestream<buffersize>::operator<<(const signed short & value)
{ return write_primitive(value); }

template <size_t buffersize>
inline sizedpipestream<buffersize> & sizedpipestream<buffersize>::operator<<(const signed int & value)
{ return write_primitive(value); }

template <size_t buffersize>
inline sizedpipestream<buffersize> & sizedpipestream<buffersize>::operator<<(const signed long & value)
{ return write_primitive(value); }

template <size_t buffersize>
inline sizedpipestream<buffersize> & sizedpipestream<buffersize>::operator<<(const unsigned char & value)
{ return write_primitive(value); }

template <size_t buffersize>
inline sizedpipestream<buffersize> & sizedpipestream<buffersize>::operator<<(const unsigned short & value)
{ return write_primitive(value); }

template <size_t buffersize>
inline sizedpipestream<buffersize> & sizedpipestream<buffersize>::operator<<(const unsigned int & value)
{ return write_primitive(value); }

template <size_t buffersize>
inline sizedpipestream<buffersize> & sizedpipestream<buffersize>::operator<<(const unsigned long & value)
{ return write_primitive(value); }

template <size_t buffersize>
inline sizedpipestream<buffersize> & sizedpipestream<buffersize>::operator>>(signed char & value)
{ read_primitive(value); return *this; }

template <size_t buffersize>
inline sizedpipestream<buffersize> & sizedpipestream<buffersize>::operator>>(signed short & value)
{ read_primitive(value); return *this; }

template <size_t buffersize>
inline sizedpipestream<buffersize> & sizedpipestream<buffersize>::operator>>(signed int & value)
{ read_primitive(value); return *this; }

template <size_t buffersize>
inline sizedpipestream<buffersize> & sizedpipestream<buffersize>::operator>>(signed long & value)
{ read_primitive(value); return *this; }

template <size_t buffersize>
inline sizedpipestream<buffersize> & sizedpipestream<buffersize>::operator>>(unsigned char & value)
{ read_primitive(value); return *this; }

template <size_t buffersize>
inline sizedpipestream<buffersize> & sizedpipestream<buffersize>::operator>>(unsigned short & value)
{ read_primitive(value); return *this; }

template <size_t buffersize>
inline sizedpipestream<buffersize> & sizedpipestream<buffersize>::operator>>(unsigned int & value)
{ read_primitive(value); return *this; }

template <size_t buffersize>
inline sizedpipestream<buffersize> & sizedpipestream<buffersize>::operator>>(unsigned long & value)
{ read_primitive(value); return *this; }

template <size_t buffersize>
template <typename T>
sizedpipestream<buffersize> & sizedpipestream<buffersize>::write_primitive(T value)
{
	if (fds[WRITE_END] == -1) return *this;

	if (buffersize == 0)
	{
		writen(fds[WRITE_END], &value, sizeof value);
		return *this;
	}

	if (buffersize - wbufferpos < sizeof value)
		flush();

	*reinterpret_cast<T*>(&wbuffer[wbufferpos]) = value;
	wbufferpos += sizeof value;
	return *this;
}

template <size_t buffersize>
template <typename T>
int sizedpipestream<buffersize>::read_primitive(T & value)
{
	if (fds[READ_END] == -1) return -1;

	if (buffersize == 0)
		return readn(fds[READ_END], &value, sizeof value);

	if (rbuffersize - rbufferpos < sizeof value)
	{
		int error = fill(sizeof value);
		if (error == 0) is_eof = true;
		if (error <= 0) return error;
	}

	value = *reinterpret_cast<T*>(&rbuffer[rbufferpos]);
	rbufferpos += sizeof value;
	return sizeof value;
}

#endif

