/* Text.cpp
 * If you define the OEFLAG_TEXT flag,
 * then this file is for you. Here you
 * will deal with text at runtime similarly
 * to how you dealt with it at edittime.
 * By default all functions are commented
 * out, if you implement them (and you
 * should implement them in an all-or-
 * nothing fashion), then you need to
 * uncomment them in Ext.def as well.
 * Functions defined here:
 * GetRunObjectFont
 * SetRunObjectFont
 * GetRunObjectTextColor
 * SetRunObjectTextColor
 */

#include "Common.h"

/* GetRunObjectFont
 * Copy the current font into the Lf parameter.
 */
/*void MMF2Func GetRunObjectFont(RD *rd, LOGFONT *Lf)
{
	//GetObject(rd->pExtension->MyHFont, sizeof(LOGFONT), Lf);
}*/

/* SetRunObjectFont
 * Store the font set by the user. Some
 * action needs to be taken when rect is
 * not null - read the MMF2SDK Help file
 * for more information.
 */
/*void MMF2Func SetRunObjectFont(RD *rd, LOGFONT *Lf, RECT *rect)
{
//	Extension &ext = *(rd->pExtension);
//	HFONT HFont = CreateFontIndirect(Lf);
//	if(hFont != NULL)
//	{
//		if(ext.MyHFont != 0) DeleteObject(ext.MyHFont);
//		ext.MyHFont = HFont;
//		SendMessage(ext.MyHWindow, WM_SETFONT, WPARAM(ext.MyHFont), FALSE);
//	}
}*/

/* GetRunObjectTextColor
 * Plain and simple, return the color
 * of the text.
 */
/*COLORREF MMF2Func GetRunObjectTextColor(RD *rd)
{
	return 0;
}*/

/* SetRunObjectTextColor
 * Store the new color of the text
 * and force and update on your window.
 */
/*void MMF2Func SetRunObjectTextColor(RD *rd, COLORREF color)
{
//	rd->pExtension->MyTextColor = color;
//	InvalidateRect(rd->pExtension->MyHWindow, NULL, TRUE);
}*/
