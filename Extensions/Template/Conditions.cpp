
#include "Common.h"

bool Extension::AreTwoNumbersEqual(int First, int Second)
{
    return First == Second;
}

bool ObjectXFilter(Extension* Ext, RunObject* Obj)
{
	return Obj->roHo.hoX == Ext->XPositionCompare;
}

bool Extension::ObjectsXEquals(RunObject* Object, int XPosition)
{
	LPEVP pParam = rdPtr->rHo.hoCurrentParam;
	// pParam points to 1st parameter, which should be of type object
	// Offset it if you have used second or other parameter for object selection using code below
	// LPEVP pParam2nd = (LPEVP)((LPBYTE)pParam + pParam->evpSize);

	short oil = (short)pParam->evp.evpW.evpW0;
	bool negated = ((event*)((LPBYTE)rdPtr->rHo.hoCurrentParam - CND_SIZE))->evtFlags2 & EVFLAG2_NOT;
	XPositionCompare = XPosition;

	return Runtime.ObjectSelection.FilterObjects(this, oil, negated, ObjectXFilter);
}


