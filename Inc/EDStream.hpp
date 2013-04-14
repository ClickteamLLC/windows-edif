/* Note:
 * This is not an independent header!
 */

#ifndef __EditDataStream_HeaderPlusPlus__
#define __EditDataStream_HeaderPlusPlus__

#include <istream>
#include <ostream>
#include <streambuf>
#include <vector>
#include <string>

struct EDIStream : std::basic_istream<TCHAR>
{
	struct Buf : std::basic_streambuf<TCHAR>
	{
		Buf(SerializedED const&SED, std::size_t putback = 8) : pb(putback), b(putback)
		{
			b.reserve(pb + SED.Header.extSize);
			for(std::size_t i = 0; i < SED.Header.extSize; ++i)
			{
				b.push_back(SED.data[i]);
			}
			setg(&b.front(), &b.front() + pb, &b.front() + b.size());
		}
		virtual ~Buf()
		{
		}

	private:
		std::size_t pb;
		std::vector<char_type> b;

		virtual int_type underflow()
		{
			if(gptr() < egptr())
			{
				return traits_type::to_int_type(*gptr());
			}
			return traits_type::eof();
		}

		Buf(Buf const&);
		Buf &operator=(Buf const&);
	};

	EDIStream(SerializedED const*SED) : b(*SED), std::basic_istream<TCHAR>(&b)
	{
	}
	virtual ~EDIStream()
	{
	}

	template<typename T>
	T read_value()
	{
		T t = T();
		this->read(reinterpret_cast<TCHAR *>(&t), sizeof(T));
		return t;
	}
	std::basic_string<TCHAR> read_string()
	{
		std::basic_string<TCHAR> s;
		std::getline(*this, s, _T('\0'));
		return s;
	}

private:
	Buf b;

	EDIStream(EDIStream const&);
	EDIStream &operator=(EDIStream const&);
};

struct EDOStream : std::basic_ostream<TCHAR>
{
	struct Buf : std::basic_streambuf<TCHAR>
	{
		Buf(mv *mV, SerializedED *&SED) : mV(mV), SED(SED)
		{
			setp(0, 0);
		}
		virtual ~Buf()
		{
		}

	private:
		mv *mV;
		SerializedED *&SED;
		std::vector<char_type> b;

		virtual int_type overflow(int_type ch)
		{
			if(ch != traits_type::eof())
			{
				b.push_back(char_type(ch));
				setp(&b.front(), &b.back()+1, &b.back()+1);
			}
			return traits_type::eof();
		}
		virtual int sync()
		{
			std::size_t size = (&b.back() + 1) - &b.front();
			SerializedED *t = (SerializedED *)mvReAllocEditData(mV, SED, sizeof(SerializedED)+size);
			if(t)
			{
				SED = t;
				memcpy(&SED->data, &b.front(), size);
				return 0;
			}
			return -1;
		}

		Buf(Buf const&);
		Buf &operator=(Buf const&);
	};

	EDOStream(mv *mV, SerializedED *&SED) : b(mV, SED), std::basic_ostream<TCHAR>(&b)
	{
	}
	virtual ~EDOStream()
	{
	}

	template<typename T>
	EDOStream &write_value(T t)
	{
		this->write(reinterpret_cast<TCHAR const*>(&t), sizeof(T));
		return *this;
	}
	EDOStream &write_string(std::basic_string<TCHAR> const&s)
	{
		*this << s << std::ends;
		return *this;
	}
	template<typename ForwardIterator>
	EDOStream &write_sequence(ForwardIterator first, ForwardIterator pastlast)
	{
		for(; first != pastlast; ++first)
		{
			this->write(reinterpret_cast<TCHAR const*>(&*first), sizeof(*first));
		}
		return *this;
	}

private:
	Buf b;

	EDOStream(EDOStream const&);
	EDOStream &operator=(EDOStream const&);
};

#endif
