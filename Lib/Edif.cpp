
#include "Common.h"

Edif::SDK * SDK;

char Edif::LanguageCode[3];

bool Edif::IsEdittime;
bool Edif::ExternalJSON;

HMENU Edif::ActionMenu, Edif::ConditionMenu, Edif::ExpressionMenu;

short ReadParameterType(const char * Text)
{
    if(!_stricmp(Text, "Text") || !_stricmp(Text, "String"))
        return PARAM_EXPSTRING;

    if(!_stricmp(Text, "Filename") || !_stricmp(Text, "File"))
        return PARAM_FILENAME;

    if(!_stricmp(Text, "Float"))
        return PARAM_EXPRESSION;

    if(!_stricmp(Text, "Integer"))
        return PARAM_EXPRESSION;

    if(!_stricmp(Text, "Object"))
        return PARAM_OBJECT;

	if(!_stricmp(Text, "Position"))
		return PARAM_POSITION;

	if(!_stricmp(Text, "Create"))
		return PARAM_CREATE;

	if(!_stricmp(Text, "SysCreate"))
		return PARAM_SYSCREATE;

	if(!_stricmp(Text, "Animation"))
		return PARAM_ANIMATION;

	if(!_stricmp(Text, "Nop"))
		return PARAM_NOP;

	if(!_stricmp(Text, "Player"))
		return PARAM_PLAYER;

	if(!_stricmp(Text, "Every"))
		return PARAM_EVERY;

	if(!_stricmp(Text, "Key"))
		return PARAM_KEY;

	if(!_stricmp(Text, "Speed"))
		return PARAM_SPEED;

	if(!_stricmp(Text, "JoyDirection"))
		return PARAM_JOYDIRECTION;

	if(!_stricmp(Text, "Shoot"))
		return PARAM_SHOOT;

	if(!_stricmp(Text, "Zone"))
		return PARAM_ZONE;

	if(!_stricmp(Text, "Comparison"))
		return PARAM_COMPARAISON;

	if(!_stricmp(Text, "Colour") || !_stricmp(Text, "Color"))
		return PARAM_COLOUR;

	if(!_stricmp(Text, "Frame"))
		return PARAM_FRAME;

	if(!_stricmp(Text, "SampleLoop"))
		return PARAM_SAMLOOP;

	if(!_stricmp(Text, "MusicLoop"))
		return PARAM_MUSLOOP;

	if(!_stricmp(Text, "NewDirection"))
		return PARAM_NEWDIRECTION;

	if(!_stricmp(Text, "TextNumber"))
		return PARAM_TEXTNUMBER;

	if(!_stricmp(Text, "Click"))
		return PARAM_CLICK;

	if(!_stricmp(Text, "Program"))
		return PARAM_PROGRAM;

    return 0;
}

short ReadExpressionParameterType(const char * Text)
{
    if(!_stricmp(Text, "Text") || !_stricmp(Text, "String"))
        return EXPPARAM_STRING;

    if(!_stricmp(Text, "Float"))
        return EXPPARAM_LONG;

    if(!_stricmp(Text, "Integer"))
        return EXPPARAM_LONG;

    return 0;
}

void Edif::Init(mv * _far mV, LPEDATA edPtr)
{   
    IsEdittime = mV->mvHMainWin != 0;

    mvInvalidateObject(mV, edPtr);
}

void Edif::Free(mv * _far mV)
{   
//    delete ::SDK;
}

void Edif::Free(LPEDATA edPtr)
{   
}

void Edif::Init(mv _far * mV)
{   

    strcpy(LanguageCode, "EN");

    do
    {   
        if((!mV) || (!mV->mvGetVersion))
            break;
        
        char KeyName[256];

        if((mV->mvGetVersion() & MMFVERFLAG_MASK) & MMFVERFLAG_HOME)
            strcpy(KeyName, "Software\\Clickteam\\The Games Factory 2\\General");
        
        else if((mV->mvGetVersion() & MMFVERFLAG_MASK) & MMFVERFLAG_PRO)
            strcpy(KeyName, "Software\\Clickteam\\Multimedia Fusion Developer 2\\General");
        
        else
            strcpy(KeyName, "Software\\Clickteam\\Multimedia Fusion 2\\General");

        HKEY Key;
        
        if(RegOpenKeyEx(HKEY_CURRENT_USER, KeyName, 0, KEY_QUERY_VALUE, &Key) != ERROR_SUCCESS)
            break;

        int Version;

        {   long Size = sizeof(int);
            DWORD Type = REG_DWORD;

            RegQueryValueEx(Key, "tbver", 0, &Type, (BYTE *) &Version, (LPDWORD) &Size);
        }

        RegCloseKey(Key);

        if(HIWORD(Version) == 0x40C)
            strcpy(LanguageCode, "FR");

    } while(0);

    char * JSON;
    size_t JSON_Size;

    int result = Edif::GetDependency (JSON, JSON_Size, "json", IDR_EDIF_JSON);
    
    if (result == Edif::DependencyNotFound)
    {
        MessageBox(0, "JSON file not found on disk or in MFX resources", "EDIF SDK - Error", 0);
        exit(0);
    }

    Edif::ExternalJSON = (result == Edif::DependencyWasFile);

    char * copy = (char *) malloc (JSON_Size + 1);
    
    memcpy (copy, JSON, JSON_Size);
    copy [JSON_Size] = 0;

    ::SDK = new Edif::SDK(mV, copy);
}

Edif::SDK::SDK(mv * mV, const char * JSON) : Information(JSON::LoadSource::ObjectString, JSON)
{
    Icon = new cSurface;
	this->mV = mV;

    if(Icon->GetWidth() < 1 && mV->mvImgFilterMgr)
    {
        char * IconData;
        size_t IconSize;

        if (Edif::GetDependency (IconData, IconSize, "png", IDR_EDIF_ICON) != Edif::DependencyNotFound)
        {
            CInputMemFile * File = CInputMemFile::NewInstance ();
            File->Create (IconSize);

            memcpy (File->GetMemBuffer (), IconData, IconSize);

            DWORD PNG = FILTERID_PNG;
            ImportImageFromInputFile(mV->mvImgFilterMgr, File, Icon, &PNG, 0);
                
            File->Delete();

            if(!Icon->HasAlpha())
                Icon->SetTransparentColor(RGB(255, 0, 255));
        }
    }

    JSON::Object &Actions = Information["Actions"];
    JSON::Object &Conditions = Information["Conditions"];
    JSON::Object &Expressions = Information["Expressions"];

    ActionJumps = new void * [Actions.Length + 1];
    ConditionJumps = new void * [Conditions.Length + 1];
    ExpressionJumps = new void * [Expressions.Length + 1];

    ActionJumps [Actions.Length - 1] = 0;
    ConditionJumps [Conditions.Length - 1] = 0;
    ExpressionJumps [Expressions.Length - 1] = 0;
    
    for(int i = 0; i < Actions.Length; ++ i)
    {
        JSON::Object &Action = Actions[i];

        ActionJumps [i] = (void *) Edif::Action;

        ActionFunctions.push_back(0);

        int ActionID = Edif::ActionID(i);

        ActionInfos.push_back(ActionID);  // Menu ID
        ActionInfos.push_back(ActionID);  // String ID
        ActionInfos.push_back(i);         // Code

        ActionInfos.push_back(0);         // Flags

        JSON::Object &Parameters = Action["Parameters"];

        ActionInfos.push_back(Parameters.Length);

        {   short IsFloat = 0;

            for(int i = 0; i < Parameters.Length; ++ i)
            {
                JSON::Object &Parameter = Parameters[i];

                const char * Type;

                if(Parameter.Type == JSON::ObjectType::Map)
                    Type = Parameter["Type"];
                else
                    Type = Parameter[0];

                ActionInfos.push_back(ReadParameterType(Type));
                
                if(!_stricmp(Type, "Float"))
                    IsFloat |= 1 << i;
            }

            ActionFloatFlags.push_back(IsFloat);
        }

        for(int i = 0; i < Parameters.Length; ++ i)
            ActionInfos.push_back(0);
    }

    for(int i = 0; i < Conditions.Length; ++ i)
    {
        JSON::Object &Condition = Conditions[i];

        ConditionJumps [i] = (void *) Edif::Condition;

        ConditionFunctions.push_back(0);

        int ConditionID = Edif::ConditionID(i);

        ConditionInfos.push_back(ConditionID);
        ConditionInfos.push_back(ConditionID);
        ConditionInfos.push_back(i);

        ConditionInfos.push_back(Condition["Triggered"].Boolean != 0 ? 0 : (EVFLAGS_ALWAYS | EVFLAGS_NOTABLE));

        JSON::Object &Parameters = Condition["Parameters"];

        ConditionInfos.push_back(Parameters.Length);

        {   short IsFloat = 0;

            for(int i = 0; i < Parameters.Length; ++ i)
            {
                JSON::Object &Parameter = Parameters[i];

                const char * Type;

                if(Parameter.Type == JSON::ObjectType::Map)
                    Type = Parameter["Type"];
                else
                    Type = Parameter[0];

                ConditionInfos.push_back(ReadParameterType(Type));
                
                if(!_stricmp(Type, "Float"))
                    IsFloat |= 1 << i;
            }

            ConditionFloatFlags.push_back(IsFloat);
        }

        for(int i = 0; i < Parameters.Length; ++ i)
            ConditionInfos.push_back(0);
    }

    for(int i = 0; i < Expressions.Length; ++ i)
    {
        JSON::Object &Expression = Expressions[i];

        ExpressionJumps [i] = (void *) Edif::Expression;

        ExpressionFunctions.push_back(0);

        if(!_stricmp(Expression["Returns"], "Integer"))
            ExpressionTypes.push_back(0);
        else if(!_stricmp(Expression["Returns"], "Float"))
            ExpressionTypes.push_back(1);
        else if(!_stricmp(Expression["Returns"], "Text") || !_stricmp(Expression["Returns"], "String"))
            ExpressionTypes.push_back(2);
        else
            ExpressionTypes.push_back(0);

        int ExpressionID = Edif::ExpressionID(i);

        ExpressionInfos.push_back(ExpressionID);
        ExpressionInfos.push_back(ExpressionID);
        ExpressionInfos.push_back(i);

        {   short FlagsValue = 0;

            if(ExpressionTypes.back() == 1)
                FlagsValue |= EXPFLAG_DOUBLE;

            if(ExpressionTypes.back() == 2)
                FlagsValue |= EXPFLAG_STRING;

            ExpressionInfos.push_back(FlagsValue);
        }

        JSON::Object &Parameters = Expression["Parameters"];

        ExpressionInfos.push_back(Parameters.Length);

        {   short IsFloat = 0;

            for(int i = 0; i < Parameters.Length; ++ i)
            {
                JSON::Object &Parameter = Parameters[i];

                const char * Type;

                if(Parameter.Type == JSON::ObjectType::Map)
                    Type = Parameter["Type"];
                else
                    Type = Parameter[0];

                ExpressionInfos.push_back(ReadExpressionParameterType(Type));
                
                if(!_stricmp(Type, "Float"))
                    IsFloat |= 1 << i;
            }

            ExpressionFloatFlags.push_back(IsFloat);
        }

        for(int i = 0; i < Parameters.Length; ++ i)
            ExpressionInfos.push_back(0);
    }

    ActionMenu = LoadMenuJSON(Edif::ActionID(0), Information["ActionMenu"]);
    ConditionMenu = LoadMenuJSON(Edif::ConditionID(0), Information["ConditionMenu"]);
    ExpressionMenu = LoadMenuJSON(Edif::ExpressionID(0), Information["ExpressionMenu"]);
}

Edif::SDK::~SDK()
{

}

int ActionOrCondition(vector<short> &FloatFlags, LPEVENTINFOS2 Info, void * Function, int ID, LPRDATA rdPtr, long param1, long param2)
{
    int * Parameters;
    int ParameterCount;

    {   Info = GetEventInformations(Info, ID);

        ParameterCount = Info->infos.nParams;
        Parameters = (int *) alloca(sizeof(int) * ParameterCount);

        for(int i = 0; i < ParameterCount; ++ i)
        {
            switch(EVINFO2_PARAM(Info, i))
            {
            case PARAM_EXPRESSION:

                if((FloatFlags[ID] & (1 << i)) != 0)
                {
                    Parameters[i] = CNC_GetFloatParameter(rdPtr);
                    break;
                }

                Parameters[i] = CNC_GetIntParameter(rdPtr);
                break;

            case PARAM_EXPSTRING:
            case PARAM_FILENAME:

                Parameters[i] = CNC_GetStringParameter(rdPtr);
                break;

            default:

                Parameters[i] = CNC_GetParameter(rdPtr);
                break;
            }
        }
    }

    void * Extension = rdPtr->pExtension;

    int Result;

    __asm
    {
        pushad

        mov ecx, ParameterCount
        
        cmp ecx, 0
        je CallNow

        mov edx, Parameters

        mov ebx, ecx
        shl ebx, 2

        add edx, ebx
        sub edx, 4

        PushLoop:

            push [edx]
            sub edx, 4

            dec ecx

            cmp ecx, 0
            jne PushLoop

        CallNow:

        mov ecx, Extension
        call Function
            
        mov Result, eax

        popad
    }

    return Result;
}

HMENU Edif::LoadMenuJSON(int BaseID, JSON::Object &Source, HMENU Parent)
{
    if(!Parent)
        Parent = CreateMenu();

    for(int i = 0; i < Source.Length; ++ i)
    {
        JSON::Object &MenuItem = Source[i];

        if(MenuItem.Type == JSON::ObjectType::String)
        {
            if(!_stricmp(MenuItem, "Separator"))
            {
                AppendMenu(Parent, MF_BYPOSITION | MF_SEPARATOR, 0, 0);
                continue;
            }

            continue;
        }

        if(MenuItem[0].Type == JSON::ObjectType::String && MenuItem[1].Type == JSON::ObjectType::List)
        {
            HMENU SubMenu = CreatePopupMenu();
            LoadMenuJSON(BaseID, MenuItem, SubMenu);

            AppendMenu(Parent, MF_BYPOSITION | MF_STRING | MF_POPUP, (UINT) SubMenu, Edif::Language(MenuItem[0]));

            continue;
        }

        int ItemOffset = 0;

        int ID = BaseID + (int) MenuItem[ItemOffset].Number;
        const char * Text = Edif::Language(MenuItem[ItemOffset + 1]);
        bool Disabled = MenuItem.Length > (ItemOffset + 2) ? MenuItem[ItemOffset + 2].Boolean != 0 : false;

        AppendMenu(Parent, Disabled ? MF_GRAYED | MF_UNCHECKED | MF_BYPOSITION | MF_STRING
                                : MF_BYPOSITION | MF_STRING, ID, Text);

    }

    return Parent;
}

long __stdcall Edif::Condition(LPRDATA rdPtr, long param1, long param2)
{
    int ID = rdPtr->rHo.hoEventNumber;

    rdPtr->pExtension->Runtime.param1 = param1;
    rdPtr->pExtension->Runtime.param2 = param2;

    void * Function = ::SDK->ConditionFunctions[ID];

    if(!Function)
        return rdPtr->pExtension->Condition(ID, rdPtr, param1, param2);

    int Result = ActionOrCondition(::SDK->ConditionFloatFlags, (LPEVENTINFOS2) &::SDK->ConditionInfos[0],
                                Function, ID, rdPtr, param1, param2);

    return *(char *) &Result;
}

short __stdcall Edif::Action(LPRDATA rdPtr, long param1, long param2)
{
    /* int ID = rdPtr->rHo.hoAdRunHeader->rh4.rh4ActionStart->evtNum; */
    int ID = rdPtr->rHo.hoEventNumber;

    rdPtr->pExtension->Runtime.param1 = param1;
    rdPtr->pExtension->Runtime.param2 = param2;

    void * Function = ::SDK->ActionFunctions[ID];

    if(!Function)
    {
        rdPtr->pExtension->Action(ID, rdPtr, param1, param2);
        return 0;
    }

    ActionOrCondition(::SDK->ActionFloatFlags, (LPEVENTINFOS2) &::SDK->ActionInfos[0],
                                Function, ID, rdPtr, param1, param2);

    return 0;
}


long __stdcall Edif::Expression(LPRDATA rdPtr, long param)
{
    int ID = rdPtr->rHo.hoEventNumber;

    rdPtr->pExtension->Runtime.param1 = param;
    rdPtr->pExtension->Runtime.param2 = 0;

    int * Parameters;
    int ParameterCount;

    void * Function = ::SDK->ExpressionFunctions[ID];

    if(!Function)
        return rdPtr->pExtension->Expression(ID, rdPtr, param);

    {   LPEVENTINFOS2 Infos = GetEventInformations((LPEVENTINFOS2) &::SDK->ExpressionInfos[0], ID);

        ParameterCount = Infos->infos.nParams;
        Parameters = (int *) alloca(sizeof(int) * ParameterCount);

        for(int i = 0; i < ParameterCount; ++ i)
        {
            switch(EVINFO2_PARAM(Infos, i))
            {
            case EXPPARAM_STRING:

                if(!i)
                {
                    Parameters[i] = CNC_GetFirstExpressionParameter(rdPtr, param, TYPE_STRING);
                    break;
                }

                Parameters[i] = CNC_GetNextExpressionParameter(rdPtr, param, TYPE_STRING);
                break;  

            case EXPPARAM_LONG:

                int Type = ((::SDK->ExpressionFloatFlags[ID] & (1 << i)) != 0) ? TYPE_FLOAT : TYPE_LONG;

                if(!i)
                {
                    Parameters[i] = CNC_GetFirstExpressionParameter(rdPtr, param, Type);
                    break;
                }

                Parameters[i] = CNC_GetNextExpressionParameter(rdPtr, param, Type);
                break;
            }
        }
    }

    void * Extension = rdPtr->pExtension;

    int Result;
    int ExpressionType = ::SDK->ExpressionTypes[ID];
    
    __asm
    {
        pushad

        mov ecx, ParameterCount
        
        cmp ecx, 0
        je CallNow

        mov edx, Parameters

        mov ebx, ecx
        shl ebx, 2

        add edx, ebx
        sub edx, 4

    PushLoop:

        push [edx]
        sub edx, 4

        dec ecx

        cmp ecx, 0
        jne PushLoop

    CallNow:

        mov ecx, Extension
        call Function

        mov ecx, ExpressionType;

        cmp ecx, 1
        jne NotFloat

        fstp Result
        jmp End

    NotFloat:
        
        mov Result, eax
        
    End:

        popad
    }
    
    switch(ExpressionType)
    {
        case 1: // Float
        {
            rdPtr->rHo.hoFlags |= HOF_FLOAT;
            break;
        }

        case 2: // String
        {
            rdPtr->rHo.hoFlags |= HOF_STRING;
            break;
        }
    };
    
    return Result;
}

int Edif::GetDependency (char *& Buffer, size_t &Size, const char * FileExtension, int Resource)
{
    char Filename [MAX_PATH];
    GetSiblingPath (Filename, FileExtension);

    if (*Filename)
    {
        FILE * File = fopen (Filename, "rb");

        if (!File)
            return DependencyNotFound;

        fseek (File, 0, SEEK_END);
        Size = ftell (File);
        fseek (File, 0, SEEK_SET);

        Buffer = (char *) malloc (Size + 1);
        Buffer [Size] = 0;

        fread (Buffer, 1, Size, File);
        
        fclose (File);

        return DependencyWasFile;
    }

    if (!Resource)
        return DependencyNotFound;

    HRSRC res = FindResource (hInstLib, MAKEINTRESOURCE (Resource), "EDIF");

    if (!res)
        return DependencyNotFound;

    Size = SizeofResource (hInstLib, res);
    Buffer = (char *) LockResource (LoadResource (hInstLib, res));

    return DependencyWasResource;
}

void Edif::GetSiblingPath(char * Buffer, const char * FileExtension)
{
    char temp [MAX_PATH];

    GetModuleFileName (hInstLib, temp, sizeof(temp));

    char * Filename = temp + strlen(temp) - 1;

    while(*Filename != '.')
        -- Filename;

    strcpy(++ Filename, FileExtension);

    Filename = Filename + strlen(Filename) - 1;

    while(*Filename != '\\' && *Filename != '/')
        -- Filename;

    ++ Filename;



    char ExecutablePath [MAX_PATH];
    GetModuleFileName (GetModuleHandle (0), ExecutablePath, sizeof(ExecutablePath));

    {   char * Iterator = ExecutablePath + strlen(ExecutablePath) - 1;

        while(*Iterator != '\\' && *Iterator != '/')
            -- Iterator;

        *Iterator = 0;
    }


    /* Same folder as the EXE - standard edrt */

    char FullFilename [MAX_PATH];
    sprintf(FullFilename, "%s/%s", ExecutablePath, Filename);

    if(GetFileAttributes(FullFilename) == 0xFFFFFFFF)
    {
        /* Data/Runtime - editor */

        sprintf(FullFilename, "%s/Data/Runtime/%s", ExecutablePath, Filename);

        if(GetFileAttributes(FullFilename) == 0xFFFFFFFF)
        {
            /* Parent folder - HWA edrt */

            sprintf(FullFilename, "%s/../%s", ExecutablePath, Filename);

            if(GetFileAttributes(FullFilename) == 0xFFFFFFFF)
            {
                /* TODO : Check MFX dir too, for when "compress the runtime" isn't checked */

                *Buffer = 0;
                return;
            }
        }
    }

    strcpy(Buffer, FullFilename);
}
