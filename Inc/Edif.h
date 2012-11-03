#pragma once

#include "json.h"

#include "ccxhdr.h"
#include "CfcFile.h"
#include "ImageFlt.h"
#include "ImgFlt.h"
#include "Surface.h"
#include "Cncf.h"
#include "Patch.h"

#include <vector>
#include <string>

class Extension;

#include "ObjectSelection.h"

#if defined(MMFEXT)
#define	IS_COMPATIBLE(v) (v->mvGetVersion != NULL && (v->mvGetVersion() & MMFBUILD_MASK) >= Extension::MinimumBuild && (v->mvGetVersion() & MMFVERSION_MASK) >= MMFVERSION_20 && ((v->mvGetVersion() & MMFVERFLAG_MASK) & MMFVERFLAG_HOME) == 0)
#elif defined(PROEXT)
#define IS_COMPATIBLE(v) (v->mvGetVersion != NULL && (v->mvGetVersion() & MMFBUILD_MASK) >= Extension::MinimumBuild && (v->mvGetVersion() & MMFVERSION_MASK) >= MMFVERSION_20 && ((v->mvGetVersion() & MMFVERFLAG_MASK) & MMFVERFLAG_PRO) != 0)
#else
#define	IS_COMPATIBLE(v) (v->mvGetVersion != NULL && (v->mvGetVersion() & MMFBUILD_MASK) >= Extension::MinimumBuild && (v->mvGetVersion() & MMFVERSION_MASK) >= MMFVERSION_20)
#endif

#define LinkAction(ID, Function) \
	SDK->ActionFunctions[ID] = Edif::MemberFunctionPointer(&Extension::Function);

#define LinkCondition(ID, Function) \
	SDK->ConditionFunctions[ID] = Edif::MemberFunctionPointer(&Extension::Function);

#define LinkExpression(ID, Function) \
	SDK->ExpressionFunctions[ID] = Edif::MemberFunctionPointer(&Extension::Function);

extern HINSTANCE hInstLib;

struct RD;				typedef			  RD RUNDATA;	typedef RUNDATA * LPRDATA;
struct SerializedED;	typedef	SerializedED EDITDATA;	typedef EDITDATA * LPEDATA;

LPEVENTINFOS2 GetEventInformations(LPEVENTINFOS2 eiPtr, short code);

namespace Edif
{

	class SDK
	{
	public:

		json_value &json;

		SDK (mv * mV, json_value &);
		~SDK ();

		std::vector<short> ActionInfos;
		std::vector<short> ConditionInfos;
		std::vector<short> ExpressionInfos;

		void ** ActionJumps;
		void ** ConditionJumps;
		void ** ExpressionJumps;

		std::vector<void *> ActionFunctions;
		std::vector<void *> ConditionFunctions;
		std::vector<void *> ExpressionFunctions;

		std::vector<char> ExpressionTypes;

		std::vector<short> ActionFloatFlags;
		std::vector<short> ConditionFloatFlags;
		std::vector<short> ExpressionFloatFlags;

		unsigned char * FunctionMemory;

		mv* mV;
		cSurface * Icon;
	};
	
	class Runtime
	{
	protected:

		LPRDATA rdPtr;

	public:

		long param1, param2;

		Runtime(LPRDATA _rdPtr);
		~Runtime();

		void Rehandle();

		void GenerateEvent(int EventID);
		void PushEvent(int EventID);

		void * Allocate(size_t);
		TCHAR * CopyString(const TCHAR *);

		void Pause();
		void Resume();

		void Redisplay();
		void Redraw();
		LPRO LPROFromFixed(int fixedValue);
		int FixedFromLPRO(LPRO object);

		void SetAngleOfObject(LPRO object, float acAngle);

		void SetPosition(int X, int Y);
		CallTables* GetCallTables();
		void CallMovement(int ID, long Parameter);

		void Destroy();

		void GetApplicationDrive(TCHAR * Buffer);
		void GetApplicationDirectory(TCHAR * Buffer);
		void GetApplicationPath(TCHAR * Buffer);
		void GetApplicationName(TCHAR * Buffer);
		void GetApplicationTempPath(TCHAR * Buffer);

		void ExecuteProgram(prgParam * Program);

		long EditInteger(EditDebugInfo *);
		long EditText(EditDebugInfo *);

		bool IsHWA();
		bool IsUnicode();

		event &CurrentEvent();

		Riggs::ObjectSelection ObjectSelection;

		void WriteGlobal(const TCHAR * Name, void * Value);
		void * ReadGlobal(const TCHAR * Name);

		#ifdef EdifUseJS
		
			JSContext * GetJSContext();

		#endif
			
	};

	extern bool ExternalJSON;

	void GetSiblingPath (TCHAR * Buffer, const TCHAR * Extension);
	
	const int DependencyNotFound	 = 0;
	const int DependencyWasFile	  = 1;
	const int DependencyWasResource  = 2;

	int GetDependency (char *& Buffer, size_t &Size, const TCHAR * FileExtension, int Resource);

	TCHAR* ConvertString(const char* utf8String);
	TCHAR* ConvertAndCopyString(TCHAR* tstr, const char* utf8String, int maxLength);
	inline void FreeString(TCHAR* s)
	{
		free(s);
	}

	extern TCHAR LanguageCode[3];
	extern bool IsEdittime;

	extern HMENU ActionMenu, ConditionMenu, ExpressionMenu;

	HMENU LoadMenuJSON (int BaseID, const json_value &Source, HMENU Parent = 0);

	int Init(mv _far * mV);
	void Init(mv _far * mV, LPEDATA edPtr);

	void Free(mv _far * mV);
	void Free(LPEDATA edPtr);

	long __stdcall Condition (LPRDATA rdPtr, long param1, long param2);
	short __stdcall Action (LPRDATA rdPtr, long param1, long param2);
	long __stdcall Expression (LPRDATA rdPtr, long param);
   
	inline int ActionID(int ID)
	{
		return 25000 + ID;
	}

	inline int ConditionID(int ID)
	{
		return 26000 + ID;
	}

	inline int ExpressionID(int ID)
	{
		return 27000 + ID;
	}

	template<class T> inline void * MemberFunctionPointer(T Function)
	{
		T _Function = Function;
		return *(void **) &_Function;
	}
}

extern Edif::SDK * SDK;


class Extension;

/* RD
 * This is the extension runtime data.
 * Because all your runtime data is
 * stored in your Extension class, this
 * class is global to all extensions and
 * should not be changed. Some functions
 * will give you a pointer to this class,
 * and from it you can get a pointer to
 * your extension class' instance.
 */
struct RD
{
	headerObject rHo; //Main header

	char r //Placeholder for the optional structures
	[
		sizeof(rCom)+
		sizeof(rMvt)+
		sizeof(rSpr)+
		sizeof(rVal)
	];
	//Pointers to the optional structures, null values mean that that OEFLAG was not specified.
	rCom *rc; // Common structure for movements & animations
	rMvt *rm; // Movements (OEFLAG_MOVEMENTS)
	rSpr *rs; // Sprite (displayable objects) (OEFLAG_SPRITES)
	rVal *rv; // Alterable values (OEFLAG_VALUES)

	Extension * pExtension;
};

/* SerializedED
 * This is the editdata. The class
 * name is a reminder - any data in
 * here is fully serialized, ready
 * to be written to a file on disk,
 * such as the MFA or EXE file. It
 * will also be this way when you
 * get access to it in other runtimes.
 * You should not change this class
 * as the change will affect all other
 * extensions as well. Instead, open
 * EditData.hpp and read about how to
 * implement the editdata there.
 */
struct SerializedED
{
	/* Header
	 * This required data member contains
	 * information about your extension for
	 * MMF2, such as the version number
	 * from Extension::Version.
	 */
	extHeader Header;

	/* data
	 * This is used as a pointer to
	 * the start of the serialized
	 * data. See the EditData class
	 * for more information.
	 */
	char data[1];
};

#define MMF2Func WINAPI DLLExport
