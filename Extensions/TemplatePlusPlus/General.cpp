/* General.cpp
 * Contains functions common to both edittime and runtime.
 * Functions defined here:
 * DllMain
 * InitExtension
 * FreeExtension
 * LoadObject
 * UnloadObject
 * UpdateEditStrucrure
 * UpdateFileNames
 * EnumElts
 */

#include "Common.h"

/* hInstLib
 * This is the HINSTANCE for your extension's DLL.
 * It is set by DllMain and used later for anything
 * that requires it, such as some Windows API
 * functions and some internal EDIF code. You
 * shouldn't change its value, ever.
 */
HINSTANCE hInstLib;

/* DllMain
 * This is a required function for Windows DLLs. You
 * should not need to edit this function - if you do,
 * refer to MSDN for information.
 */
BOOL WINAPI DllMain(HINSTANCE DLL, DWORD Reason, LPVOID)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH: //DLL is attaching to the address space of the current process.
		{
			hInstLib = DLL; //Store HINSTANCE
			break;
		}
	case DLL_THREAD_ATTACH: //A new thread is being created in the current process.
		{
			//
			break;
		}
	case DLL_THREAD_DETACH: //A thread is exiting cleanly.
		{
			//
			break;
		}
	case DLL_PROCESS_DETACH: //The calling process is detaching the DLL from its address space.
		{
			//
			break;
		}
	}
	return TRUE;
}



/* InitExtension
 * MMF2 calls this before anything else when it loads your
 * extension. This includes during the splash screen, in which
 * case "Quiet" is true. This is where you should initialize
 * global information for either edittime or runtime. Be aware,
 * though, that at edittime, multiple open MFAs with your
 * extension will share the same global information. You can
 * use mV->mvEditApp to tell apart different MFAs from each
 * other. If an error occurs during this function, return
 * -1 and MMF2 will not load your extension.
 */
int MMF2Func InitExtension(mv *mV, int Quiet)
{
	return Edif::Init(mV);
}

/* FreeExtension
 * Called just before MMF2 unloads your extension. You
 * should release any memory you allocated above in
 * the InitializeExtension function. You should always
 * return a value of 0.
 */
int MMF2Func FreeExtension(mv *mV)
{
	//
	Edif::Free(mV);
	return 0;
}


/* LoadObject
 * This is called at both edittime and runtime when MMF2
 * loads each unique instance of your object. (Each one
 * with a different name, not each instance of the same
 * one). Not much needs to be done here.
 */
int	MMF2Func LoadObject(mv *mV, LPCSTR FileName, SerializedED *SED, int)
{
	Edif::Init(mV, SED);
	return 0;
}

/* UnloadObject
 * new is to delete as LoadObject is to this functon.
 * If you initialized stuff above, deinitialize it here.
 */
void MMF2Func UnloadObject(mv *mV, SerializedED *SED, int)
{
	//
}

/* UpdateEditStructure (DEPRECATED)
 * This is called when MMF2 notices that your
 * extension has a newer version number than
 * the one in the header of the SerializedED.
 * Thankfully, however, we don't need this
 * function at all - the EditData class is
 * as smart as you programmed it to be and
 * it changes the size dynamically rather
 * than statically. So, you can safely
 * ignore this function all together.
 * (It would be a pain to write anyway -
 * see the MMF2SDK help for an example)
 */
HGLOBAL MMF2Func UpdateEditStructure(mv *mV, SerializedED *OldSED)
{
	return 0;
}

/* UpdateFileNames
 * When the application is moved to a new directory,
 * MMF2 asks your extension to ensure that its file
 * paths are moving with it. Just call Update for
 * each file path - you will need to create buffers
 * of size MAX_PATH if you use C++ strings for
 * the paths.
 */
void MMF2Func UpdateFileNames(mv *mV, LPSTR appName, SerializedED *SED, void (WINAPI *Update)(LPSTR, LPSTR))
{
	//
}

/* EnumElts
 * That whacky function that MMF2 uses to enumerate all the
 * animation frames associated with your object, because MMF2
 * is nice enough to deal with them for you so you can't-er,
 * don't have to. Refer to the MMF2SDK Help file for
 * information on how not to misuse this function.
 * You must also uncomment the entry in the Ext.def
 * file if you uncomment this function.
 */
/*int MMF2Func EnumElts (mv *mV, SerializedED *SED, ENUMELTPROC enumProc, ENUMELTPROC undoProc, LPARAM lp1, LPARAM lp2)
{  
	int error = 0;

	//Replace wImgIdx with the name of the WORD variable you create within the edit structure
  
	//Enum images  
	if((error = enumProc(&edPtr->wImgIdx, IMG_TAB, lp1, lp2)) != 0)
	{
		//Undo enum images	  
		undoProc(&edPtr->wImgIdx, IMG_TAB, lp1, lp2);
	}  

	return error;
}*/
