#ifndef __EditDataStream_HeaderPlusPlus__
#define __EditDataStream_HeaderPlusPlus__

#include <istream>
#include <ostream>
#include <streambuf>
#include <vector>

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
		std::vector<TCHAR> b;

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

private:
	Buf b;

	EDIStream(EDIStream const&);
	EDIStream &operator=(EDIStream const&);
};

//struct EDOStream : std::basic_ostream<TCHAR>
//{
//	struct Buf : std::basic_streambuf<TCHAR>
//	{
//	};
//};

#endif
