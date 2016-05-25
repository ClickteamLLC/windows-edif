#if !defined(MinGWSupport_HeaderPlusPlus) && defined(__MINGW32__)
#define      MinGWSupport_HeaderPlusPlus

#include <memory>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iterator>
#include <Windows.h>
#include <tchar.h>

inline int fopen_s(FILE **f, char const *filename, char const *mode)
{
	if(FILE *t = fopen(filename, mode))
	{
		*f = t;
		return 0;
	}
	return -1;
}
inline int _wfopen_s(FILE **f, wchar_t const *filename, wchar_t const *mode)
{
	//unfortunately <codecvt> is not yet supported in libstdc++
	//so we have to use WinAPI
	int flags = WC_COMPOSITECHECK|WC_NO_BEST_FIT_CHARS;
	int fnbs = WideCharToMultiByte(CP_UTF8, flags, filename, -1, 0, 0, 0, 0);
	int mdbs = WideCharToMultiByte(CP_UTF8, flags, mode, -1, 0, 0, 0, 0);

	std::unique_ptr<char[]> fn {new char[fnbs+1]};
	std::unique_ptr<char[]> md {new char[mdbs+1]};

	if(!WideCharToMultiByte(CP_UTF8, flags, filename, -1, fn.get(), fnbs+1, 0, 0)
	|| !WideCharToMultiByte(CP_UTF8, flags, mode, -1, md.get(), mdbs+1, 0, 0))
	{
		return -1;
	}
	return fopen_s(f, fn.get(), md.get());
}
#ifdef UNICODE
	#define _tfopen_s _wfopen_s
#else
	#define _tfopen_s fopen_s
#endif

#define off64_t _off64_t
#define _near
#define _far
#define DLLExport32
#define DLLExport

inline char *_strdup(char const *str)
{
	return strcpy((char *)malloc(strlen(str)), str);
}
inline wchar_t *_wcsdup(wchar_t const *str)
{
	wchar_t *t = (wchar_t *)malloc((std::find(str, str-1, L'\0')-str)*sizeof(wchar_t));
	std::copy(str, std::find(str, str-1, L'\0')+1, t);
	return t;
}
#ifdef UNICODE
	#define _tcsdup _wcsdup
#else
	#define _tcsdup _strdup
#endif

inline int _stricmp(char const *s1, char const *s2)
{
	char const *s1e = s1+strlen(s1);
	char const *s2e = s2+strlen(s2);
	if(std::lexicographical_compare(s1, s1e, s2, s2e, [](char a, char b) -> bool
	{
		std::tolower(a) == std::tolower(b);
	})) return 0;
	if(std::lexicographical_compare(s1, s1e, s2, s2e, [](char a, char b) -> bool
	{
		std::tolower(a) < std::tolower(b);
	})) return -1;
	return 1;
}
inline int _strnicmp(char const *s1, char const *s2, std::size_t n)
{
	char const *s1e = s1+std::min(strlen(s1), n);
	char const *s2e = s2+std::min(strlen(s2), n);
	if(std::lexicographical_compare(s1, s1e, s2, s2e, [](char a, char b) -> bool
	{
		std::tolower(a) == std::tolower(b);
	})) return 0;
	if(std::lexicographical_compare(s1, s1e, s2, s2e, [](char a, char b) -> bool
	{
		std::tolower(a) < std::tolower(b);
	})) return -1;
	return 1;
}

#define _MAX_PATH   260
#define _MAX_DRIVE  3
#define _MAX_DIR    256
#define _MAX_FNAME  256
#define _MAX_EXT    256
inline void _splitpath(char const *path, char *drive, char *dir, char *fname, char *ext)
{
	char const *pend = path+strlen(path);
	char const *pd = "\\/";
	if(drive)
	{
		char const *r = std::find(path, pend, ':');
		if(r == pend) drive[0] = '\0';
		else strncpy(drive, path, std::min((r+1)-path, _MAX_DRIVE));
	}
	if(dir)
	{
		char const *r1 = std::find_first_of(path, pend, pd, pd+2);
		if(r1 == pend) dir[0] = '\0';
		else
		{
			char const *r2 = std::find_first_of(std::reverse_iterator<char const *>(pend),
			                                    std::reverse_iterator<char const *>(path),
			                                    pd, pd+2).base();
			if(r2-r1+1 > _MAX_DIR)
			{
				r2 = r1 + _MAX_DIR;
			}
			std::copy(r1, r2+1, dir);
		}
	}
	if(fname)
	{
		char const *r1 = std::find_first_of(std::reverse_iterator<char const *>(pend),
		                                    std::reverse_iterator<char const *>(path),
		                                    pd, pd+2).base();
		if(r1 == path-1) r1 = path;
		char const *r2 = std::find(std::reverse_iterator<char const *>(pend),
		                           std::reverse_iterator<char const *>(path),
		                           '.').base();
		if(r2 == path-1) r2 = pend-1;
		if(r2-r1+1 > _MAX_FNAME)
		{
			r2 = r1 + _MAX_FNAME;
		}
		std::copy(r1, r2+1, fname);
	}
	if(ext)
	{
		char const *r = std::find(std::reverse_iterator<char const *>(pend),
		                          std::reverse_iterator<char const *>(path),
		                          '.').base();
		if(r == path-1) ext[0] = '\0';
		else
		{
			char const *r2 = pend-1;
			if(r2-r+1 > _MAX_EXT)
			{
				r2 = r + _MAX_EXT;
			}
			std::copy(r, r2+1, ext);
		}
	}
}
inline void _wsplitpath(wchar_t const *path, wchar_t *drive, wchar_t *dir, wchar_t *fname, wchar_t *ext)
{
	wchar_t const *pend = std::find(path, path-1, L'\0');
	wchar_t const *pd = L"\\/";
	if(drive)
	{
		wchar_t const *r = std::find(path, pend, L':');
		if(r == pend) drive[0] = L'\0';
		else
		{
			if(r-path+1 > _MAX_DRIVE)
			{
				r = path + _MAX_DRIVE;
			}
			std::copy(path, r+1, drive);
		}
	}
	if(dir)
	{
		wchar_t const *r1 = std::find_first_of(path, pend, pd, pd+2);
		if(r1 == pend) dir[0] = L'\0';
		else
		{
			wchar_t const *r2 = std::find_first_of(std::reverse_iterator<wchar_t const *>(pend),
			                                       std::reverse_iterator<wchar_t const *>(path),
			                                       pd, pd+2).base();
			if(r2-r1+1 > _MAX_DIR)
			{
				r2 = r1 + _MAX_DIR;
			}
			std::copy(r1, r2+1, dir);
		}
	}
	if(fname)
	{
		wchar_t const *r1 = std::find_first_of(std::reverse_iterator<wchar_t const *>(pend),
		                                       std::reverse_iterator<wchar_t const *>(path),
		                                       pd, pd+2).base();
		if(r1 == path-1) r1 = path;
		wchar_t const *r2 = std::find(std::reverse_iterator<wchar_t const *>(pend),
		                              std::reverse_iterator<wchar_t const *>(path),
		                              L'.').base();
		if(r2 == path-1) r2 = pend-1;
		if(r2-r1+1 > _MAX_FNAME)
		{
			r2 = r1 + _MAX_FNAME;
		}
		std::copy(r1, r2+1, fname);
	}
	if(ext)
	{
		wchar_t const *r = std::find(std::reverse_iterator<wchar_t const *>(pend),
		                             std::reverse_iterator<wchar_t const *>(path),
		                             L'.').base();
		if(r == path-1) ext[0] = L'\0';
		else
		{
			wchar_t const *r2 = pend-1;
			if(r2-r+1 > _MAX_EXT)
			{
				r2 = r + _MAX_EXT;
			}
			std::copy(r, r2+1, ext);
		}
	}
}

template<typename... Args>
int sprintf_s(char *buf, std::size_t bs, char const *f, Args... args)
{
	return snprintf(buf, bs, f, args...);
}
#ifdef UNICODE
	#define _stprintf_s swprintf_s
#else
	#define _stprintf_s sprintf_s
#endif

#endif
