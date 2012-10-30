// By Anders Riggelsen (Andos)
// http://community.clickteam.com/showthread.php?t=61672

// These files do not match up with the originals - modified for Edif (callbacks inside the extension class, etc..)

#if !defined(OBJECTSELECTION)
#define OBJECTSELECTION

#if !defined(EF_ISHWA)
#define EF_ISHWA		112
#endif
#if !defined(EF_ISUNICODE)
#define EF_ISUNICODE	113
#endif

namespace Riggs
{
	class ObjectSelection  
	{
	public:

		Extension * pExtension;

		ObjectSelection(LPRH rhPtr);

		void SelectAll(short Oi);
		void SelectNone(short Oi);
		void SelectOneObject(LPRO object);
		void SelectObjects(short Oi, LPRO* objects, int count);
		bool ObjectIsOfType(LPRO object, short Oi);
		int GetNumberOfSelected(short Oi);
		
		template<class T> bool FilterObjects(short Oi, bool negate, T filterFunction)
		{
			if(Oi & 0x8000)
				return FilterQualifierObjects(Oi & 0x7FFF, negate, filterFunction) ^ negate;
			else
				return FilterNonQualifierObjects(Oi, negate, filterFunction) ^ negate;
		}

	protected:

		LPRH rhPtr;
		LPOBL ObjectList;
		LPOIL OiList;
		LPQOI QualToOiList;
		int oiListItemSize;

		LPOIL GetOILFromOI(short Oi);

		template<class T> bool DoCallback(void * Class, T Function, LPRO Parameter)
		{
			T _Function = Function;
			void * FunctionPointer = *(void **) &_Function;

			int Result;

			__asm
			{
				pushad

				mov ecx, Class

				push Parameter
					call FunctionPointer
				add esp, 4

				mov Result, eax

				popad
			};

			return (*(char *) &Result) != 0;
		}

		template<class T> bool FilterQualifierObjects(short Oi, bool negate, T filterFunction)
		{
			LPQOI CurrentQualToOiStart = (LPQOI)((char*)QualToOiList + Oi);
			LPQOI CurrentQualToOi = CurrentQualToOiStart;

			bool hasSelected = false;

			while(CurrentQualToOi->qoiOiList >= 0)
			{
				LPOIL CurrentOi = GetOILFromOI(CurrentQualToOi->qoiOiList);
				hasSelected |= FilterNonQualifierObjects(CurrentOi->oilOi, negate, filterFunction);
				CurrentQualToOi = (LPQOI)((char*)CurrentQualToOi + 4);
			}
			return hasSelected;
		}

		template<class T> bool FilterNonQualifierObjects(short Oi, bool negate, T filterFunction)
		{
			LPOIL pObjectInfo = GetOILFromOI(Oi);
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
				bool useObject = DoCallback((void *) pExtension, filterFunction, (LPRO)pObject);

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
	};
}

#endif // !defined(OBJECTSELECTION)