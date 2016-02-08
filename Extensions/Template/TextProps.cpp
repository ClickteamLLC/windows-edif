/* TextProps.cpp
 * If you plan on allowing MMF2 to manage
 * your extension's text display properties
 * for you, this is the file to look into.
 * Functions defined here:
 * GetTextCaps
 * GetTextFont
 * SetTextFont
 * GetTextClr
 * SetTextClr
 * GetTextAlignment
 * SetTextAlignment
 */

#include "Common.h"

/* GetTextCaps
 * Return the text capabilities
 * of your extension at edittime.
 */
DWORD MMF2Func GetTextCaps(mv *mV, SerializedED *SED)
{
#ifndef RUN_ONLY
//	return	TEXT_ALIGN_LEFT|TEXT_ALIGN_HCENTER|TEXT_ALIGN_RIGHT
//			|TEXT_ALIGN_TOP|TEXT_ALIGN_VCENTER|TEXT_ALIGN_BOTTOM
//			|TEXT_FONT|TEXT_COLOR;
#endif
	return 0;
}

/* GetTextFont
 * Return the font used by your extension.
 */
BOOL MMF2Func GetTextFont(mv *mV, SerializedED *SED, LOGFONT *lf, LPTSTR, UINT)
{
#ifndef RUN_ONLY
	//
	return TRUE;
#endif
	return FALSE;
}

/* SetTextFont
 * Changes the font used by your extension.
 */
BOOL MMF2Func SetTextFont(mv *mV, SerializedED *SED, LOGFONT *lf, LPCSTR)
{
#ifndef RUN_ONLY
	//
	return TRUE;
#endif
	return FALSE;
}

/* GetTextClr
 * Return the RGB color value of the text color.
 */
COLORREF MMF2Func GetTextClr(mv *mV, SerializedED *SED)
{
#ifndef RUN_ONLY
	//
	return 200<<8; //green
#endif
	return 0;
}

/* SetTextClr
 * Sets the color of the text.
 */
void MMF2Func SetTextClr(mv *mV, SerializedED *SED, COLORREF Color)
{
#ifndef RUN_ONLY
	//
#endif
}

/* GetTextAlignment
 * Return the alignment of the text.
 */
DWORD MMF2Func GetTextAlignment(mv *mV, SerializedED *SED)
{
#ifndef RUN_ONLY
//	DWORD dw = 0;

//	if((ed.Flags & ALIGN_LEFT)		!= 0) dw |= TEXT_ALIGN_LEFT;
//	if((ed.Flags & ALIGN_HCENTER)	!= 0) dw |= TEXT_ALIGN_HCENTER;
//	if((ed.Flags & ALIGN_RIGHT)		!= 0) dw |= TEXT_ALIGN_RIGHT;

//	if((ed.Flags & ALIGN_TOP)		!= 0) dw |= TEXT_ALIGN_TOP;
//	if((ed.Flags & ALIGN_VCENTER)	!= 0) dw |= TEXT_ALIGN_VCENTER;
//	if((ed.Flags & ALIGN_BOTTOM)	!= 0) dw |= TEXT_ALIGN_BOTTOM;

//	return dw;
#endif
	return 0;
}

/* SetTextAlignment
 * Sets the alignment of the text.
 */
void MMF2Func SetTextAlignment(mv *mV, SerializedED *SED, DWORD AlignFlags)
{
#ifndef RUN_ONLY
//	DWORD dw = ed.Flags;

//	if((AlignFlags & TEXT_ALIGN_LEFT)	!= 0) dw = (dw & ~(ALIGN_LEFT|ALIGN_HCENTER|ALIGN_RIGHT)) | ALIGN_LEFT;
//	if((AlignFlags & TEXT_ALIGN_HCENTER)!= 0) dw = (dw & ~(ALIGN_LEFT|ALIGN_HCENTER|ALIGN_RIGHT)) | ALIGN_HCENTER;
//	if((AlignFlags & TEXT_ALIGN_RIGHT)	!= 0) dw = (dw & ~(ALIGN_LEFT|ALIGN_HCENTER|ALIGN_RIGHT)) | ALIGN_RIGHT;

//	if((AlignFlags & TEXT_ALIGN_TOP)	!= 0) dw = (dw & ~(ALIGN_TOP|ALIGN_VCENTER|ALIGN_BOTTOM)) | ALIGN_TOP;
//	if((AlignFlags & TEXT_ALIGN_VCENTER)!= 0) dw = (dw & ~(ALIGN_TOP|ALIGN_VCENTER|ALIGN_BOTTOM)) | ALIGN_VCENTER;
//	if((AlignFlags & TEXT_ALIGN_BOTTOM)	!= 0) dw = (dw & ~(ALIGN_TOP|ALIGN_VCENTER|ALIGN_BOTTOM)) | ALIGN_BOTTOM;

//	edFlags = dw;
#endif
}
