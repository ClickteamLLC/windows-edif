
#pragma once

#include "JSON.h"

#include "ccxhdr.h"
#include "CfcFile.h"
#include "ImageFlt.h"
#include "ImgFlt.h"
#include "Surface.h"
#include "Cncf.h"
#include "Patch.h"

#include <vector>
#include <list>
#include <string>
using namespace std;

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

struct RUNDATA;
struct EDITDATA;

typedef RUNDATA * LPRDATA;
typedef EDITDATA * LPEDATA;

LPEVENTINFOS2 GetEventInformations(LPEVENTINFOS2 eiPtr, short code);

namespace Edif
{

    class SDK
    {
    public:

        JSON::Object Information;

        SDK (mv * mV, const char * JSON);
        ~SDK ();

        vector<short> ActionInfos;
        vector<short> ConditionInfos;
        vector<short> ExpressionInfos;

        void ** ActionJumps;
        void ** ConditionJumps;
        void ** ExpressionJumps;

        vector<void *> ActionFunctions;
        vector<void *> ConditionFunctions;
        vector<void *> ExpressionFunctions;

        vector<char> ExpressionTypes;

        vector<short> ActionFloatFlags;
        vector<short> ConditionFloatFlags;
        vector<short> ExpressionFloatFlags;

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
        char * CopyString(const char *);

        void Pause();
        void Resume();

        void Redisplay();
        void Redraw();
		LPRO LPROFromFixed(int fixedValue);
		int FixedFromLPRO(LPRO object);

		void SetPosition(int X, int Y);
		CallTables* GetCallTables();
        void CallMovement(int ID, long Parameter);

        void Destroy();

        void GetApplicationDrive(char * Buffer);
        void GetApplicationDirectory(char * Buffer);
        void GetApplicationPath(char * Buffer);
        void GetApplicationName(char * Buffer);
        void GetApplicationTempPath(char * Buffer);

        void ExecuteProgram(prgParam * Program);

        long EditInteger(EditDebugInfo *);
        long EditText(EditDebugInfo *);

        bool IsHWA();
        bool IsUnicode();

        event &CurrentEvent();

        Riggs::ObjectSelection ObjectSelection;

        void WriteGlobal(const char * Name, void * Value);
        void * ReadGlobal(const char * Name);

        #ifdef EdifUseJS
        
            JSContext * GetJSContext();

        #endif
            
    };

    extern bool ExternalJSON;

    void GetSiblingPath (char * Buffer, const char * Extension);
    
    const int DependencyNotFound     = 0;
    const int DependencyWasFile      = 1;
    const int DependencyWasResource  = 2;

    int GetDependency (char *& Buffer, size_t &Size, const char * FileExtension, int Resource);

    extern char LanguageCode[3];
    extern bool IsEdittime;

    extern HMENU ActionMenu, ConditionMenu, ExpressionMenu;

    HMENU LoadMenuJSON (int BaseID, JSON::Object &Source, HMENU Parent = 0);

    void Init(mv _far * mV);
    void Init(mv _far * mV, LPEDATA edPtr);

    void Free(mv _far * mV);
    void Free(LPEDATA edPtr);

    long __stdcall Condition (LPRDATA rdPtr, long param1, long param2);
    short __stdcall Action (LPRDATA rdPtr, long param1, long param2);
    long __stdcall Expression (LPRDATA rdPtr, long param);
   
    inline const char * Language(const JSON::Object &Source)
    {
        if(Source.Type == JSON::ObjectType::String)
            return Source;
 
        return Source[LanguageCode];
    }

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