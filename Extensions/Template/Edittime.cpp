/* Edittime.cpp
 * This file contains functions for the
 * New Object dialog and the Frame Editor.
 * For properties, see Properties.cpp. For
 * custom parameters, see CustomParams.cpp.
 * For text properties, see TextProps.cpp.
 * Functions defined here:
 * MakeIconEx
 * UsesFile
 * CreateFromFile
 * CreateObject
 * EditObject
 * SetEditSize
 * PutObject
 * RemoveObject
 * CloneObject
 * GetObjectRect
 * EditorDisplay
 * IsTransparent
 * PrepareToWriteObject
 * GetFilters
 */

#include "Common.h"

/* CreateObject
 * This is the first time you have
 * access to the editdata, so it
 * needs to be initialized with
 * default values. Just be aware
 * that if the user chooses to
 * create your object from a file,
 * CreateFromFile will be called
 * instead of this function.
 */
int MMF2Func CreateObject(mv *mV, LO *lo, SerializedED *SED)
{
#ifndef RUN_ONLY
	if(IS_COMPATIBLE(mV)) //Make sure MMF2 hasn't changed in a way that makes your extension incompatible
	{
		Edif::Init(mV, SED);

		EditData().Serialize(mV, SED); //create & store the default editdata

		return 0;
	}
#endif
	return -1;
}

/* MakeIconEx
 * Lets you draw the icon dynamically
 * by drawing into the Icon surface. The
 * current example just uses the
 * Icon.png from the resources.
 */
int MMF2Func MakeIconEx(mv *mV, cSurface *Icon, LPTSTR Name, OI *oi, SerializedED *SED)
{
#ifndef RUN_ONLY
	Icon->Delete();
	Icon->Clone(*SDK->Icon);
	Icon->SetTransparentColor(RGB(255, 0, 255));
	return 0;
#endif
	return -1;
}

/* UsesFile
 * When the user chooses "Create From
 * File", MMF2 asks each extension if
 * it supports being created from that
 * file. Here you should investigate
 * the file and see if your extension
 * can be created from it in the
 * CreateFromFile function below. You
 * can simply check the file extension,
 * for example.
 */
BOOL MMF2Func UsesFile(mv *mV, LPTSTR FileName)
{
#ifndef RUN_ONLY
	if(IS_COMPATIBLE(mV)) //check for compatibility, since you can't return an error from CreateFromFile
	{
//		char ext[_MAX_EXT];
//		_tsplitpath(FileName, 0, 0, 0, ext);
//		if(stdtstring(".iherebydeclarethatthisfilecontainsmyintmystringandmyarray") == ext)
//		{
//			return TRUE;
//		}
	}
#endif
	return FALSE;
}

/* CreateFromFile
 * If the user decides to create your object
 * from the file you said you were OK with
 * above, this is where you take that file
 * and actually create your object from it.
 * The CreateObject function up above will
 * not have executed, so you need to initialize
 * the editdata just as you would in CreateObject.
 */
void MMF2Func CreateFromFile(mv *mV, LPTSTR FileName, SerializedED *SED)
{
#ifndef RUN_ONLY
	Edif::Init(mV, SED);

	EditData ed; //default EditData
//	std::ifstream in (FileName);
//	EditData::MyArray_t::size_type MyArray_size;
//	in >> ed.MyString >> ed.MyInt >> MyArray_size;
//	ed.MyArray.clear();
//	for(EditData::MyArray_t::size_type i = 0; i < MyArray_size; ++i)
//	{
//		ed.MyArray.push_back(0);
//		in >> ed.MyArray.back();
//	}
	ed.Serialize(mV, SED);
#endif
}

/* PutObject
 * Each time a duplicate is made of your
 * object, MMF2 calls this function to
 * let you know. You still share the same
 * editdata, the difference is the
 * LevelObject pointer and the number of
 * duplicates.
 */
void MMF2Func PutObject(mv *mV, LO *lo, SerializedED *SED, ushort NDup)
{
#ifndef RUN_ONLY
	if(NDup == 1) //is this the first object being created?
	{
		//You can put common code to both
		//CreateObject and CreateFromFile
		//as this function will execute
		//after either one.
	}
#endif
}

/* RemoveObject
 * Just the opposite of the above, when
 * the user removes an instance of your
 * object. This also functions as a
 * DestroyObject function of sorts when
 * NDup == 1.
 */
void MMF2Func RemoveObject(mv *mV, LO *lo, SerializedED *SED, ushort NDup)
{
#ifndef RUN_ONLY
	if(NDup == 1) //is this the last object being removed?
	{
		Edif::Free(SED);
		//
	}
#endif
}

/* CloneObject
 * When the user makes a clone of your object
 * (not just another instance), MMF2 copies the
 * contents of the editdata and then lets you
 * know that the editdata in question is in
 * new hands. Here you can 'fix' anything that
 * might be caused by directly copying the
 * editdata, though you should never have
 * any problems like that in the first place.
 */
void MMF2Func CloneObject(mv *mV, OI *oi, SerializedED *SED)
{
#ifndef RUN_ONLY
	//
#endif
}

/* EditObject
 * If the user double-clicks your object icon
 * or chooses the Edit option from a context
 * menu, this function is called. This is useful
 * as a que to bring up an animation editor for
 * animations in your object. Return TRUE if
 * changes were made to the editdata, and FALSE
 * otherwise.
 */
BOOL MMF2Func EditObject (mv *mV, OI *oi, LO *lo, SerializedED *SED)
{
#ifndef RUN_ONLY
	//
#endif
	return FALSE;
}

/* SetEditSize
 * If your object is resizeable, MMF2 calls this
 * to let you know that the user has just requested
 * a resize of the object. You can take the new
 * size as is, or if it reminds you too much of
 * 500 page essays you can limit the size to whatever
 * you want. If you uncomment this function, make
 * sure you also uncomment it in Ext.def. The
 * presence of this function tells MMF2 that your
 * object can be resized; otherwise it can't.
 */
/*BOOL MMF2Func SetEditSize(mv *mV, SerializedED *SED, int x, int y)
{
#ifndef RUN_ONLY
	EditData ed (SED);
	ed.Width = x;
	ed.Height = y;
	ed.Serialize(mV, SED);
	return TRUE;
#endif
	return FALSE;
}*/

/* GetObjectRect
 * MMF2 wants to know from time to time how much
 * space your object is taking up on the frame
 * editor. Currently this just gives the size of
 * your Icon.png, but you can change it to parallel
 * with the SetEditSize function above.
 */
void MMF2Func GetObjectRect(mv *mV, RECT *rect, LO *lo, SerializedED *SED)
{
#ifndef RUN_ONLY
	rect->right = rect->left + SDK->Icon->GetWidth();
	rect->bottom = rect->top + SDK->Icon->GetHeight();
#endif
}


/* EditorDisplay
 * This function does the work of drawing your
 * object on the frame editor. In this example,
 * the Icon.png file is simply drawn to the
 * frame, but you can change this to draw
 * anything you want.
 */
void MMF2Func EditorDisplay(mv *mV, OI *oi, LO *lo, SerializedED *SED, RECT *rect)
{
#ifndef RUN_ONLY
	cSurface *Surface = WinGetSurface(int(mV->mvIdEditWin)); //get access to the frame editor surface
	if(!Surface) return; //failure
	SDK->Icon->Blit(*Surface, rect->left, rect->top, BMODE_TRANSP, BOP_COPY, 0); //copy the icon onto the frame editor surface
#endif
}


/* IsTransparent
 * MMF2 calls this to ask if the mouse pointer
 * is over a transparent part of your object.
 * Don't ask why this isn't called "IsOpaque",
 * just accept that it isn't and move on. If the
 * given coordinates are over an opaque part of
 * your object, return TRUE, otherwise return
 * FALSE. (Protip: MMF2 calls this function a
 * LOT. Don't put a MessageBox function in here
 * or any other kind of debug function.)
 */
extern "C" BOOL MMF2Func IsTransparent(mv *mV, LO *lo, SerializedED *SED, int x, int y)
{
#ifndef RUN_ONLY
	//
#endif
	return FALSE;
}

/* PrepareToWriteObject (DEPRECATED)
 * Just before MMF2 writes the editdata to the MFA,
 * it calls this function to let you clean up
 * a temporary copy of the editdata. Because you
 * intelligently designed your EditData::Serialize
 * function to always save cleaned data anyway,
 * this function is useless.
 */
void MMF2Func PrepareToWriteObject(mv *, SerializedED *, OI *){}

/* GetFilters
 * When the MFA filters are set to Automatic, MMF2
 * asks your extension if it uses filters or not.
 * If you use any of either kind of filters, return
 * TRUE for that kind.
 */
BOOL MMF2Func GetFilters(mv *mV, SerializedED *SED, DWORD Flags, void *)
{
#ifndef RUN_ONLY
	//If your extension uses image filters
//	if((dwFlags & GETFILTERS_IMAGES) != 0) return TRUE;

	//If your extension uses sound filters
//	if((dwFlags & GETFILTERS_SOUNDS) != 0) return TRUE;
#endif
	return FALSE;
}
