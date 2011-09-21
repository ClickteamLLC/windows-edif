
#include "Common.h"

void WINAPI	DLLExport GetObjInfos (mv _far *mV, void *, LPTSTR ObjName, LPTSTR ObjAuthor,
                                            LPTSTR ObjCopyright, LPTSTR ObjComment, LPTSTR ObjHttp)
{
#ifndef RUN_ONLY
	
    JSON::Object &Properties = SDK->Information["About"];

    strcpy(ObjAuthor,       Edif::Language(Properties["Author"]));
    strcpy(ObjCopyright,    Edif::Language(Properties["Copyright"]));
    strcpy(ObjComment,      Edif::Language(Properties["Comment"]));
    strcpy(ObjHttp,         Edif::Language(Properties["URL"]));
    strcpy(ObjName,         Edif::Language(Properties["Name"]));


#endif // !defined(RUN_ONLY)
}

LPCTSTR WINAPI GetHelpFileName()
{
#ifndef RUN_ONLY
	// Return a file without path if your help file can be loaded by the MMF help file.
//	return "MyExt.chm";

	// Or return the path of your file, relatively to the MMF directory
	// if your file is not loaded by the MMF help file.
	return SDK->Information["About"]["Help"];
#else
	return NULL;
#endif // !defined(RUN_ONLY)
}

void WINAPI DLLExport GetConditionTitle(mv _far *mV, short code, short param, LPTSTR strBuf, short maxLen)
{
#ifndef RUN_ONLY

	if(!IS_COMPATIBLE(mV))
        return;

    JSON::Object &Parameter = SDK->Information["Conditions"][code]["Parameters"][param];

    if(Parameter.Type == JSON::ObjectType::Map)
        strcpy(strBuf, Edif::Language(Parameter["Title"]));
    else
        strcpy(strBuf, Edif::Language(Parameter[1]));

#endif // !defined(RUN_ONLY)
}

void WINAPI DLLExport GetActionTitle(mv _far *mV, short code, short param, LPTSTR strBuf, short maxLen)
{
#ifndef RUN_ONLY

	if(!IS_COMPATIBLE(mV))
        return;

    JSON::Object &Parameter = SDK->Information["Actions"][code]["Parameters"][param];

    if(Parameter.Type == JSON::ObjectType::Map)
        strcpy(strBuf, Edif::Language(Parameter["Title"]));
    else
        strcpy(strBuf, Edif::Language(Parameter[1]));

#endif // !defined(RUN_ONLY)
}


void WINAPI DLLExport GetExpressionParam(mv _far *mV, short code, short param, LPTSTR strBuf, short maxLen)
{
#ifndef RUN_ONLY

	if(!IS_COMPATIBLE(mV))
        return;

    JSON::Object &Parameter = SDK->Information["Expressions"][code]["Parameters"][param];

    if(Parameter.Type == JSON::ObjectType::Map)
        strcpy(strBuf, Edif::Language(Parameter["Title"]));
    else
        strcpy(strBuf, Edif::Language(Parameter[1]));


#endif // !defined(RUN_ONLY)
}


void WINAPI DLLExport GetExpressionTitle(mv _far *mV, short code, LPTSTR strBuf, short maxLen)
{
#ifndef RUN_ONLY

	if(!IS_COMPATIBLE(mV))
        return;

    strcpy(strBuf, Edif::Language(SDK->Information["Expressions"][code]["Title"]));

#endif // !defined(RUN_ONLY)
}

void WINAPI DLLExport GetConditionString(mv _far *mV, short code, LPTSTR strPtr, short maxLen)
{
#ifndef RUN_ONLY

	if(!IS_COMPATIBLE(mV))
        return;

    strcpy(strPtr, Edif::Language(SDK->Information["Conditions"][code]["Title"]));

#endif // !defined(RUN_ONLY)
}

void WINAPI DLLExport GetActionString(mv _far *mV, short code, LPTSTR strPtr, short maxLen)
{
#ifndef RUN_ONLY

	if(!IS_COMPATIBLE(mV))
        return;

    strcpy(strPtr, Edif::Language(SDK->Information["Actions"][code]["Title"]));

#endif // !defined(RUN_ONLY)
}

void WINAPI DLLExport GetExpressionString(mv _far *mV, short code, LPSTR strPtr, short maxLen)
{
#ifndef RUN_ONLY

	if(!IS_COMPATIBLE(mV))
        return;

    strcpy(strPtr, Edif::Language(SDK->Information["Expressions"][code]["Title"]));

#endif // !defined(RUN_ONLY)
}

LPINFOEVENTSV2 WINAPI DLLExport GetConditionInfos(mv _far *mV, short code)
{
#ifndef RUN_ONLY

	if(!IS_COMPATIBLE(mV))
        return NULL;

    return &GetEventInformations((LPEVENTINFOS2) &SDK->ConditionInfos[0], code)->infos;

#endif // !defined(RUN_ONLY)
}

LPINFOEVENTSV2 WINAPI DLLExport GetActionInfos(mv _far *mV, short code)
{
#ifndef RUN_ONLY

	if(!IS_COMPATIBLE(mV))
        return NULL;

    return &GetEventInformations((LPEVENTINFOS2) &SDK->ActionInfos[0], code)->infos;

#endif // !defined(RUN_ONLY)
}

LPINFOEVENTSV2 WINAPI DLLExport GetExpressionInfos(mv _far *mV, short code)
{
#ifndef RUN_ONLY

	if(!IS_COMPATIBLE(mV))
        return NULL;

    return &GetEventInformations((LPEVENTINFOS2) &SDK->ExpressionInfos[0], code)->infos;

#endif // !defined(RUN_ONLY)
}

short WINAPI DLLExport GetConditionCodeFromMenu(mv _far *mV, short menuId)
{
#ifndef RUN_ONLY
	
    return menuId - Edif::ConditionID(0);

#endif // !defined(RUN_ONLY)
	return -1;
}

short WINAPI DLLExport GetActionCodeFromMenu(mv _far *mV, short menuId)
{
#ifndef RUN_ONLY

    return menuId - Edif::ActionID(0);

#endif // !defined(RUN_ONLY)
	return -1;
}

short WINAPI DLLExport GetExpressionCodeFromMenu(mv _far *mV, short menuId)
{
#ifndef RUN_ONLY

    return menuId - Edif::ExpressionID(0);

#endif // !defined(RUN_ONLY)
	return -1;
}

void menucpy(HMENU hTargetMenu, HMENU hSourceMenu)
{
	int			n, id, nMn;
	NPSTR		strBuf;
	HMENU		hSubMenu;

	nMn = GetMenuItemCount(hSourceMenu);
	strBuf = (NPSTR)LocalAlloc(LPTR, 80);
	for (n=0; n<nMn; n++)
	{
		if (0 == (id = GetMenuItemID(hSourceMenu, n)))
			AppendMenu(hTargetMenu, MF_SEPARATOR, 0, 0L);
		else
		{
			GetMenuString(hSourceMenu, n, strBuf, 80, MF_BYPOSITION);
			if (id != -1)
				AppendMenu(hTargetMenu, GetMenuState(hSourceMenu, n, MF_BYPOSITION), id, strBuf);
			else
			{
				hSubMenu = CreatePopupMenu();
				AppendMenu(hTargetMenu, MF_POPUP | MF_STRING, (uint)hSubMenu, strBuf);
				menucpy(hSubMenu, GetSubMenu(hSourceMenu, n));
			}
		}
	}
	LocalFree((HLOCAL)strBuf);
}

HMENU WINAPI DLLExport GetConditionMenu(mv _far *mV, fpObjInfo oiPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	// Check compatibility
	if ( IS_COMPATIBLE(mV) )
    {
        HMENU Menu = CreatePopupMenu();
        menucpy(Menu, Edif::ConditionMenu);
		return Menu;
    }
#endif // !defined(RUN_ONLY)
	return NULL;
}

HMENU WINAPI DLLExport GetActionMenu(mv _far *mV, fpObjInfo oiPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	// Check compatibility
	if ( IS_COMPATIBLE(mV) )
    {
        HMENU Menu = CreatePopupMenu();
        menucpy(Menu, Edif::ActionMenu);
        return Menu;
    }
#endif // !defined(RUN_ONLY)
	return NULL;
}

HMENU WINAPI DLLExport GetExpressionMenu(mv _far *mV, fpObjInfo oiPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	// Check compatibility
	if ( IS_COMPATIBLE(mV) )
    {
        HMENU Menu = CreatePopupMenu();
        menucpy(Menu, Edif::ExpressionMenu);
		return Menu;
    }
#endif // !defined(RUN_ONLY)
	return NULL;
}

void AddDirectory(string &From, string &To)
{
	HANDLE FileHandle;
	WIN32_FIND_DATA FileInformation;

	string SearchString;

	SearchString += From;
	SearchString += "*.*";

	FileHandle = FindFirstFile(SearchString.c_str(), &FileInformation);

	if(FileHandle == INVALID_HANDLE_VALUE)
	    return;

	do
	{
        if(*FileInformation.cFileName == '.')
            continue;

		if((FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
		{
			string OldPath;

			OldPath += From;
			OldPath += FileInformation.cFileName;
			OldPath += "\\";
			
			string NewPath;

			NewPath += To;
			NewPath += FileInformation.cFileName;
			NewPath += "\\";
			
			CreateDirectory(NewPath.c_str(), 0);
			AddDirectory(OldPath, NewPath);
			
            continue;
		}

		CopyFile((From + FileInformation.cFileName).c_str(), (To + FileInformation.cFileName).c_str(), FALSE);

	} while(FindNextFile(FileHandle, &FileInformation));

	FindClose(FileHandle);
}

void WINAPI PrepareFlexBuild(LPMV pMV, LPEDATA edPtr, LPCWSTR wTempFolder)
{
#if !defined(RUN_ONLY)

    LPSTR TempFolder;

    {   size_t Length = WideCharToMultiByte(CP_ACP, 0, wTempFolder, -1, 0, 0, 0, 0);
        TempFolder = (LPSTR) alloca(Length + 1);

        WideCharToMultiByte(CP_ACP, 0, wTempFolder, -1, TempFolder, Length, 0, 0);
    }

    char FlashFolderPath[MAX_PATH];
    Edif::GetSiblingPath(FlashFolderPath, "Flash"); 

    if (!*FlashFolderPath)
        return;

    AddDirectory(((string) FlashFolderPath + "\\"), (string) TempFolder);

#endif // !defined(RUN_ONLY)
}