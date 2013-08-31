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

	    void SelectAll(short oiList);
	    void SelectNone(short oiList);
	    void SelectOneObject(LPRO object);
	    void SelectObjects(short oiList, LPRO* objects, int count);
	    bool ObjectIsOfType(LPRO object, short oiList);
	    int GetNumberOfSelected(short oiList);
 
	    bool FilterObjects(Extension* extension, short oiList, bool negate, bool (*filterFunction)(Extension*, LPRO));

    protected:

	    LPRH rhPtr;
	    LPOBL ObjectList;
	    LPOIL OiList;
	    LPQOI QualToOiList;
	    int oiListItemSize;

	    LPOIL GetOIL(short oiList);
	    bool FilterQualifierObjects(Extension* extension, short oiList, bool negate, bool (*filterFunction)(Extension*, LPRO));
	    bool FilterNonQualifierObjects(Extension* extension, short oiList, bool negate, bool (*filterFunction)(Extension*, LPRO));
    };
}

#endif // !defined(OBJECTSELECTION)