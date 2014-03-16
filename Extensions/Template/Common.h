/* Common.h
 * This is the common header file included
 * by almost all other *.cpp files in
 * the project. Anything that gets put or
 * included in here will be available to you
 * everywhere else. You shouldn't need to
 * change anything in this file except
 * to include headers that you want and
 * to change the edition of MMF2 you want
 * to support.
 */

#pragma once

//Only define one of these at once:
//	#define TGFEXT	//TGF2.0, MMF2.0, CTF2.5, MMF2.0 Dev, CTF2.5 Dev
	#define MMFEXT	//        MMF2.0, CTF2.5, MMF2.0 Dev, CTF2.5 Dev
//	#define PROEXT	//                        MMF2.0 Dev, CTF2.5 Dev

#define EDIF_PLAIN_EDITDATA
#include "Edif.h"
#include "Resource.h"

//#include <someheader>
//#include "YourHeader.hpp"

/* stdtstring
 * A std::string that knows if you're using
 * unicode or not. (Protip: strings passed
 * to your extension A/C/Es still need to be
 * const TCHAR *, and the same goes for strings
 * returned from expressions and conditions).
 */
typedef std::basic_string<TCHAR> stdtstring;

/* EDITDATA
 * This is the editdata. Be careful, any data
 * in here is fully serialized, ready to be
 * written to a file on disk, such as the
 * MFA or EXE file. It will also be this way
 * when you get access to it in other
 * runtimes, so it has to be a flat format.
 */
struct EDITDATA
{
	/* Header
	 * This required data member contains
	 * information about your extension for
	 * MMF2, such as the version number
	 * from Extension::Version.
	 */
	extHeader Header;

	/*
	Here you put the flattened data members,
	or you can use a dynamic approach like
	TemplatePlus uses.
	*/
};

#include "Extension.hpp"
