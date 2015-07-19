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

struct EDIStream : std::basic_istream<char>
{
	struct Buf : std::basic_streambuf<char>
	{
		Buf(SerializedED const&SED, std::size_t putback = 8) : pb(putback), b(putback)
		{
			std::size_t const edsize = SED.Header.extSize - sizeof(SerializedED);
			b.reserve(pb + edsize);
			for(std::size_t i = 0; i < edsize; ++i)
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

	EDIStream(SerializedED const*SED) : b(*SED), std::basic_istream<char>(&b)
	{
	}
	virtual ~EDIStream()
	{
	}

	template<typename T>
	T read_value()
	{
		T t = T();
		this->read(reinterpret_cast<char *>(&t), sizeof(T));
		return t;
	}
	std::basic_string<TCHAR> read_string()
	{
		std::basic_string<TCHAR> s;
		while(TCHAR c = read_value<TCHAR>())
		{
			s += c;
		}
		return s;
	}

private:
	Buf b;

	EDIStream(EDIStream const&);
	EDIStream &operator=(EDIStream const&);
};

struct EDOStream : std::basic_ostream<char>
{
	struct Buf : std::basic_streambuf<char>
	{
		Buf(mv *mV, SerializedED *&SED) : mV(mV), SED(SED)
		{
			setp(0, 0);
		}
		virtual ~Buf()
		{
			if(b.empty())
			{
				return;
			}
			std::size_t size = (&b.back() + 1) - &b.front();
			SerializedED *t = (SerializedED *)mvReAllocEditData(mV, SED, sizeof(SerializedED)+size);
			if(t)
			{
				SED = t;
				memcpy(&(SED->data[0]), &b.front(), size);
			}
			else
			{
				MessageBox(NULL, _T("There isn't enough from to store the editdata!"), _T("EDIF Extension"), MB_ICONWARNING);
			}
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
			}
			return 0;
		}

		Buf(Buf const&);
		Buf &operator=(Buf const&);
	};

	EDOStream(mv *mV, SerializedED *&SED) : b(mV, SED), std::basic_ostream<char>(&b)
	{
	}
	virtual ~EDOStream()
	{
	}

	template<typename T>
	EDOStream &write_value(T t)
	{
		this->write(reinterpret_cast<char const*>(&t), sizeof(T));
		return *this;
	}
	EDOStream &write_string(std::basic_string<TCHAR> const&s)
	{
		for(std::basic_string<TCHAR>::const_iterator it = s.begin(); it != s.end(); ++it)
		{
			write_value(*it);
		}
		write_value(TCHAR());
		return *this;
	}
	template<typename ForwardIterator>
	EDOStream &write_sequence(ForwardIterator first, ForwardIterator pastlast)
	{
		for(; first != pastlast; ++first)
		{
			this->write(reinterpret_cast<char const*>(&*first), sizeof(*first));
		}
		return *this;
	}

private:
	Buf b;

	EDOStream(EDOStream const&);
	EDOStream &operator=(EDOStream const&);
};

#endif
