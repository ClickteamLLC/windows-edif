/* Runtime.cpp
 * This file contains general runtime functions
 * that aren't part of the Extension class.
 * Functions defined here:
 * GetRunObjectSurface
 * GetRunObjectCollisionMask
 * StartApp
 * EndApp
 * StartFrame
 * EndFrame
 * WindowProc
 */

#include "Common.h"

/* GetRunObjectSurface
 * Implement this function instead of
 * Extension::Display if your extension
 * supports ink effects and transitions.
 * You can support ink effects in
 * DisplayRunObject too, but this is
 * done automatically if you implement
 * GetRunObjectSurface (MMF2 applies the
 * ink effects to the surface). Don't
 * forget to enable the function in the
 * .def file if you uncomment & implement
 * this function.
 */
/*cSurface* MMF2Func GetRunObjectSurface(RD *rd)
{
	return NULL;
}*/

/* GetRunObjectCollisionMask
 * Implement this function if your extension
 * supports fine collision detection
 * (OEPREFS_FINECOLLISIONS), rr if it's a
 * background object and you want Obstacle
 * properties for this object. You should
 * return NULL if the object is not transparent.
 * Don't forget to enable the function in the
 * .def file if you uncomment & implement this
 * function.
 */
/*LPSMASK MMF2Func GetRunObjectCollisionMask(RD *rd, LPARAM lParam)
{
	// Typical example for active objects
	// ----------------------------------
	// Opaque? collide with box
	if ( (rdPtr->rs.rsEffect & EFFECTFLAG_TRANSPARENT) == 0 )	// Note: only if your object has the OEPREFS_INKEFFECTS option
		return NULL;

	// Transparent? Create mask
	LPSMASK pMask = rdPtr->m_pColMask;
	if ( pMask == NULL )
	{
		if ( rdPtr->m_pSurface != NULL )
		{
			DWORD dwMaskSize = rdPtr->m_pSurface->CreateMask(NULL, lParam);
			if ( dwMaskSize != 0 )
			{
				pMask = (LPSMASK)calloc(dwMaskSize, 1);
				if ( pMask != NULL )
				{
					rdPtr->m_pSurface->CreateMask(pMask, lParam);
					rdPtr->m_pColMask = pMask;
				}
			}
		}
	}

	//Note: for active objects, lParam is always the same.
	//For background objects (OEFLAG_BACKGROUND), lParam maybe be different if the user uses your object
	//as obstacle and as platform. In this case, you should store 2 collision masks
	//in your data: one if lParam is 0 and another one if lParam is different from 0.

	return pMask;
}*/

/* StartApp
 * Called when the application initially
 * starts or when it restarts (EndApp will
 * NOT be called beforehand in the case
 * of a restart). This is useful if you
 * need to store global data across
 * frames.
 */
void MMF2Func StartApp(mv *mV, CRunApp *App)
{
//	delete (MyGlobalData *)Edif::Runtime::ReadGlobal(mV, App, "My Global Data");
//	Edif::Runtime::WriteGlobal(mV, App, "My Global Data", new MyGlobalData());
}

/* EndApp
 * Called when the application is ending
 * and about to close. This is NOT called
 * in the event of a Restart Application
 * action. You can release any memory you
 * aqquired above in StartApp.
 */
void MMF2Func EndApp(mv *mV, CRunApp *App)
{
//	delete (MyGlobalData *)Edif::Runtime::ReadGlobal(mV, App, "My Global Data");
//	Edif::Runtime:WriteGlobal(mV, App, "My Global Data", 0); //clear dangling pointer
}

/* StartFrame
 * Called at the start of a frame. If you
 * store global data above, you should make
 * a copy of it so that it may be restored
 * when the frame restarts. This function is
 * also called when the frame restarts.
 * EndFrame is NOT called when the frame
 * restarts.
 */
void MMF2Func StartFrame(mv *mV, CRunApp *App, int FrameIndex)
{
//	if(!Edif::Runtime::ReadGlobal(mV, App, "My Global Data Backup")) //first time on this frame
//	{
//		Edif::Runtime::WriteGlobal(mV, App, "My Global Data Backup",
//			new MyGlobalData(*(MyGlobalData *)Edif::Runtime::ReadGlobal(mV, App, "My Global Data"))); //copy
//	}
//	else //frame restarting, restore initial global data
//	{
//		delete (MyGlobalData *)Edif::Runtime::ReadGlobal(mV, App, "My Global Data");
//		Edif::Runtime::WriteGlobal(mV, App, "My Global Data",
//			new MyGlobalData(*(MyGlobalData *)Edif::Runtime::ReadGlobal(mV, App, "My Global Data Backup"))); //copy
//	}
}

/* EndFrame
 * Called when the frame ends. MMF2
 * does NOT call this function in the
 * event of a "Restart Frame" action.
 */
void MMF2Func EndFrame(mv *mV, CRunApp *App, int FrameIndex)
{
//	delete (MyGlobalData *)Edif::Runtime::ReadGlobal(mV, App, "My Global Data Backup");
//	Edif::Runtime:WriteGlobal(mV, App, "My Global Data Backup", 0); //clear dangling pointer
}


// ============================================================================
//
// WINDOWPROC (interception of messages sent to hMainWin and hEditWin)
//
// Do not forget to enable the WindowProc function in the .def file if you implement it
// 
// ============================================================================
/*
// Get the pointer to the object's data from its window handle
// Note: the object's window must have been subclassed with a
// callRunTimeFunction(rdPtr, RFUNCTION_SUBCLASSWINDOW, 0, 0);
// See the documentation and the Simple Control example for more info.
//
LPRDATA GetRdPtr(HWND hwnd, LPRH rhPtr)
{
	return (LPRDATA)GetProp(hwnd, (LPCSTR)rhPtr->rh4.rh4AtomRd);
}

// Called from the window proc of hMainWin and hEditWin.
// You can intercept the messages and/or tell the main proc to ignore them.
//
LRESULT CALLBACK DLLExport WindowProc(LPRH rhPtr, HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	LPRDATA rdPtr = NULL;

	switch (nMsg) {

	// Example
	case WM_CTLCOLORSTATIC:
		{
			// Get the handle of the control
			HWND hWndControl = (HWND)lParam;

			// Get a pointer to the RUNDATA structure (see GetRdptr function above for more info)
			rdPtr = GetRdPtr(hWndControl, rhPtr);

			// Check if the rdPtr pointer is valid and points to an object created with this extension
			if ( rdPtr == NULL || rdPtr->rHo.hoIdentifier != IDENTIFIER )
				break;

			// OK, intercept the message
			HDC hDC = (HDC)wParam;
			SetBkColor(hDC, rdPtr->backColor);
			SetTextColor(hDC, rdPtr->fontColor);
			rhPtr->rh4.rh4KpxReturn = (long)rdPtr->hBackBrush;
			return REFLAG_MSGRETURNVALUE;
		}
		break;
	}

	return 0;
}
*/
