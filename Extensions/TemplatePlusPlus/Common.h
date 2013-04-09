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
//	#define TGFEXT	//TGF2, MMF2, MMF2 Dev
	#define MMFEXT	//MMF2, MMF2 Dev
//	#define PROEXT	//MMF2 Dev only

//Define this if you want legacy comma-less JSON support
//	#define UsingJsonWithoutCommas

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

#include "EditData.hpp"
#include "Extension.hpp"
