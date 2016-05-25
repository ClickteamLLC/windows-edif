
// By Anders Riggelsen (Andos)
// http://community.clickteam.com/showthread.php?t=61672

// These files do not match up with the originals - modified for Edif (callbacks inside the extension class, etc..)

#include "Common.h"

Riggs::ObjectSelection::ObjectSelection(LPRH rhPtr)
{
	this->rhPtr = rhPtr;
	this->ObjectList = rhPtr->rhObjectList;		//get a pointer to the mmf object list
	this->OiList = rhPtr->rhOiList;				//get a pointer to the mmf object info list
	this->QualToOiList = rhPtr->rhQualToOiList;	//get a pointer to the mmf qualifier to Oi list
	oiListItemSize = sizeof(objInfoList);

//Only add the sizes to the runtime structures if they weren't compiled directly for those runtimes
#ifndef UNICODE
	if ( rhPtr->rh4.rh4Mv->mvCallFunction(NULL, EF_ISUNICODE, 0, 0, 0) )
		oiListItemSize += 24;
#endif
#ifndef HWABETA
	if ( rhPtr->rh4.rh4Mv->mvCallFunction(NULL, EF_ISHWA, 0, 0, 0) )
		oiListItemSize += sizeof(LPVOID);
#endif
}

//Selects *all* objects of the given object-type
void Riggs::ObjectSelection::SelectAll(short oiList)
{
	const LPOIL pObjectInfo = GetOIL(oiList);
	if(pObjectInfo == NULL)
		return;

	pObjectInfo->oilNumOfSelected = pObjectInfo->oilNObjects;
	pObjectInfo->oilListSelected = pObjectInfo->oilObject;
	pObjectInfo->oilEventCount = rhPtr->rh2.rh2EventCount;

	int i = pObjectInfo->oilObject;
	while(i >= 0)
	{
		LPHO pObject = ObjectList[i].oblOffset;
		pObject->hoNextSelected = pObject->hoNumNext;
		i = pObject->hoNumNext;
	}
}

//Resets all objects of the given object-type
void Riggs::ObjectSelection::SelectNone(short oiList)
{
	LPOIL pObjectInfo = GetOIL(oiList);
	if(pObjectInfo == NULL)
		return;

	pObjectInfo->oilNumOfSelected = 0;
	pObjectInfo->oilListSelected = -1;
	pObjectInfo->oilEventCount = rhPtr->rh2.rh2EventCount;
}

//Resets the SOL and inserts only one given object
void Riggs::ObjectSelection::SelectOneObject(LPRO object)
{
	LPOIL pObjectInfo = GetOIL(object->roHo.hoOi);
	if(pObjectInfo == NULL)
		return;

	pObjectInfo->oilNumOfSelected = 1;
	pObjectInfo->oilEventCount = rhPtr->rh2.rh2EventCount;
	pObjectInfo->oilListSelected = object->roHo.hoNumber;
	ObjectList[object->roHo.hoNumber].oblOffset->hoNextSelected = -1;
}

//Resets the SOL and inserts the given list of objects
void Riggs::ObjectSelection::SelectObjects(short oiList, LPRO* objects, int count)
{
	if(count <= 0)
		return;

	LPOIL pObjectInfo = GetOIL(oiList);
	if(pObjectInfo == NULL)
		return;

	pObjectInfo->oilNumOfSelected = count;
	pObjectInfo->oilEventCount = rhPtr->rh2.rh2EventCount;
	
	short prevNumber = objects[0]->roHo.hoNumber;
	pObjectInfo->oilListSelected = prevNumber;
	
	for(int i=1; i<count; i++)
	{
		short currentNumber = objects[i]->roHo.hoNumber;
		ObjectList[prevNumber].oblOffset->hoNextSelected = currentNumber;
		prevNumber = currentNumber;
	}
	ObjectList[prevNumber].oblOffset->hoNextSelected = -1;
}

//Return the number of selected objects for the given object-type
int Riggs::ObjectSelection::GetNumberOfSelected(short oiList)
{
	LPOIL pObjectInfo = GetOIL(oiList);
	if(pObjectInfo == NULL)
		return 0;

	if(oiList & 0x8000)
	{
		oiList &= 0x7FFF;	//Mask out the qualifier part
		int numberSelected = 0;

		LPQOI CurrentQualToOiStart = (LPQOI)((char*)QualToOiList + oiList);
		LPQOI CurrentQualToOi = CurrentQualToOiStart;

		while(CurrentQualToOi->qoiOiList >= 0)
		{
			LPOIL CurrentOi = GetOIL(CurrentQualToOi->qoiOiList);
			numberSelected += CurrentOi->oilNumOfSelected;
			CurrentQualToOi = (LPQOI)((char*)CurrentQualToOi + 4);
		}
		return numberSelected;
	}
	else
	{
		LPOIL pObjectInfo = GetOIL(oiList);
		return pObjectInfo->oilNumOfSelected;
	}
}

bool Riggs::ObjectSelection::ObjectIsOfType(LPRO object, short oiList)
{
	LPOIL pObjectInfo = GetOIL(oiList);
	if(pObjectInfo == NULL)
		return false;
	short Oi = pObjectInfo->oilOi;

	if(Oi & 0x8000)
	{
		Oi &= 0x7FFF;	//Mask out the qualifier part
		LPQOI CurrentQualToOiStart = (LPQOI)((char*)QualToOiList + Oi);
		LPQOI CurrentQualToOi = CurrentQualToOiStart;

		while(CurrentQualToOi->qoiOiList >= 0)
		{
			LPOIL CurrentOi = GetOIL(CurrentQualToOi->qoiOiList);
			if(CurrentOi->oilOi == object->roHo.hoOi)
				return true;
			CurrentQualToOi = (LPQOI)((char*)CurrentQualToOi + 4);
		}
		return false;
	}
	else
		return (object->roHo.hoOi == Oi);
}


//Returns the object-info structure from a given object-type
LPOIL Riggs::ObjectSelection::GetOIL(short oiList)
{
	return (LPOIL)((char*)OiList + oiList*oiListItemSize);
}




bool Riggs::ObjectSelection::FilterObjects(Extension* extension, short oiList, bool negate, bool (*filterFunction)(Extension*, LPRO))
{
	LPOIL pObjectInfo = GetOIL(oiList);
	if(pObjectInfo == NULL)
		return false;

    if(oiList & 0x8000)
        return FilterQualifierObjects(extension, oiList & 0x7FFF, negate, filterFunction) ^ negate;
    else
        return FilterNonQualifierObjects(extension, oiList, negate, filterFunction) ^ negate;
}

bool Riggs::ObjectSelection::FilterQualifierObjects(Extension* extension, short oiList, bool negate, bool (*filterFunction)(Extension*, LPRO))
{
	LPOIL pObjectInfo = GetOIL(oiList);
	if(pObjectInfo == NULL)
		return false;
	short Oi = pObjectInfo->oilOi;

    LPQOI CurrentQualToOiStart = (LPQOI)((char*)QualToOiList + Oi);
    LPQOI CurrentQualToOi = CurrentQualToOiStart;

    bool hasSelected = false;
	int i=0;

	LPSHORT ptr=&CurrentQualToOi->qoiOi;
	/*while( ptr != -1 )
    {
        short CurrentOi = GetOIL(CurrentQualToOi->qoiOiList);
        hasSelected |= FilterNonQualifierObjects(extension, CurrentOi->oilOi, negate, filterFunction);
        CurrentQualToOi = (LPQOI)((char*)CurrentQualToOi + 4);
    }*/
    return hasSelected;
}

bool Riggs::ObjectSelection::FilterNonQualifierObjects(Extension* extension, short oiList, bool negate, bool (*filterFunction)(Extension*, LPRO))
{
    LPOIL pObjectInfo = GetOIL(oiList);
	if(pObjectInfo == NULL)
		return false;
	short Oi = pObjectInfo->oilOi;

    bool hasSelected = false;

    if(pObjectInfo->oilEventCount != rhPtr->rh2.rh2EventCount)
        SelectAll(Oi);	//The SOL is invalid, must reset.

    //If SOL is empty
    if(pObjectInfo->oilNumOfSelected <= 0)
        return false;

    int firstSelected = -1;
    int count = 0;
    int current = pObjectInfo->oilListSelected;
    LPHO previous = NULL;

    while(current >= 0)
    {
        LPHO pObject = ObjectList[current].oblOffset;
        bool useObject = filterFunction(extension, (LPRO)pObject);

        if(negate)
            useObject = !useObject;

        hasSelected |= useObject;

        if(useObject)
        {
	        if(firstSelected == -1)
		        firstSelected = current;

	        if(previous != NULL)
		        previous->hoNextSelected = current;
			
	        previous = pObject;
	        count++;
        }
        current = pObject->hoNextSelected;
    }
    if(previous != NULL)
        previous->hoNextSelected = -1;

    pObjectInfo->oilListSelected = firstSelected;
    pObjectInfo->oilNumOfSelected = count;

    return hasSelected;
}
