#include "JSON.h"

#ifdef _WIN32
#define strcasecmp _stricmp
#endif

bool LoadNext(JSON::Object &Object, char *& Iterator);

void CommentCheck(char * &Iterator)
{
    if(Iterator[0] == '/')
    {
        if(Iterator[1] == '/')
        {
            while(*Iterator && *Iterator != '\n' && *Iterator != '\r')
                ++ Iterator;
            
            return;
        }
        
        if(Iterator[1] == '*')
        {
            while(*Iterator && (Iterator[0] != '*' || Iterator[1] != '/'))
                ++ Iterator;
            
            return;
        }
    }
}

bool StringCheck(char * &Iterator)
{        
    if(*Iterator != '"')
        return false;
        
    ++ Iterator;
    
    while(*Iterator != '"')
    {
        if(*Iterator == '\\')
            ++ Iterator;
            
        ++ Iterator;
    }
    
    return true;      
}

bool Seek(char * &Iterator, char What)
{
	while(*Iterator != What)
	{
        if(*Iterator == '\\')
        {
            Iterator += 2;
            continue;
        }
            
        if(What != '"')
            CommentCheck(Iterator);

		if(!*Iterator)
			return false;

		++ Iterator;
	}

	*Iterator = 0;

	++ Iterator;
	return true;
}

bool KillClosing(char * &Iterator, char Opening, char Closing)
{
	int Depth = 0;

	for (;*Iterator; ++ Iterator)
	{
        CommentCheck(Iterator);

		if(*Iterator == Opening)
		{
			++ Depth;
			continue;
		}

		if(*Iterator == Closing)
		{
			if(!Depth)
			{
				*Iterator = 0;
				++ Iterator;

				return true;
			}

			-- Depth;
			continue;
		}
	}

	return false;
}

int Count(char * Iterator)
{
    int Count = 0;
	int Depth = 0;

	for(; *Iterator; ++ Iterator)
	{
        CommentCheck(Iterator);
        
        if(StringCheck(Iterator))
        {
            if(!Depth)
                ++ Count;
            
            continue;
        }
        
        if(*Iterator == ':' && !Depth)
        {
            -- Count;
            continue;
        }

        if(*Iterator >= '0' && *Iterator <= '9')
        {
		    if(!Depth)
                ++ Count;
                
            while(*Iterator >= '0' && *Iterator <= '9')
                ++ Iterator;
                
            -- Iterator;
            continue;
        }

        if(*Iterator == 't' || *Iterator == 'f')
        {
		    if(!Depth)
                ++ Count;

            while(*Iterator >= 'a' && *Iterator <= 'z')
                ++ Iterator;

            -- Iterator;
            continue;
        }

        if(*Iterator == '[' || *Iterator == '{')
		{
		    if(!Depth)
                ++ Count;
                
			++ Depth;
			continue;
		}

        if(*Iterator == ']' || *Iterator == '}')
		{
			-- Depth;

			if(Depth < 0)
                break;
		}
	}
    
	return Count;
}

bool ParseObject(JSON::Object &Object, char * String)
{
	char * Iterator = String;

	Object.Type = JSON::ObjectType::Map;
    Object.Length = Count(Iterator);

    if(Object.Length == -1)
        return false;

    Object.MapNames = new const char * [Object.Length];
    Object.MapObjects = new JSON::Object[Object.Length];

	for(int i = 0; i < Object.Length; ++ i)
	{
		if(!Seek(Iterator, '"'))
			return false;

		Object.MapNames[i] = Iterator;

		if(!Seek(Iterator, '"'))
			return false;

		if(!Seek(Iterator, ':'))
			return false;

		if(!LoadNext(Object.MapObjects[i], Iterator))
			break;
	}

	return true;
}

bool ParseArray(JSON::Object &Object, char * String)
{
	char * Iterator = String;

	Object.Type = JSON::ObjectType::List;

    Object.Length = Count(Iterator);

    if(Object.Length == -1)
        return false;

    Object.List = new JSON::Object[Object.Length];

	for(int i = 0; i < Object.Length; ++ i)
		if(!LoadNext(Object.List[i], Iterator))
			break;

	return true;
}

bool LoadNext(JSON::Object &Object, char *& Iterator)
{
	do
	{
        CommentCheck(Iterator);

		if(*Iterator == '[')
		{
			char * Begin = ++ Iterator;

			KillClosing(Iterator, '[', ']');
			return ParseArray(Object, Begin);	
		} 

		if(*Iterator == '\"')
		{
			char * Value = ++ Iterator;

			if(!Seek(Iterator, '\"'))
				return 0;

            char * End = Value + strlen(Value) -1;
            
            for(char * it = Value; *it; ++ it)
            {
                if(*it == '\\')
                {
                    memmove(it, it + 1, (End - it) + 1);
                    
                    if(*it == 'n')
                    {
                        *it = '\n';
                        continue;
                    }
                }
            }

			Object.String = Value;
			Object.Type = JSON::ObjectType::String;

			return true;
		} 

		if(*Iterator == '{')
		{
			char * Begin = ++ Iterator;

			KillClosing(Iterator, '{', '}');
			return ParseObject(Object, Begin);
		}

		if(*Iterator >= '0' && *Iterator <= '9')
		{
			char * Begin = Iterator;

			while((*Iterator >= '0' && *Iterator <= '9') || *Iterator == '.')
				++ Iterator;

			char Backup = *Iterator;
			*Iterator = 0;

			Object.Number = (float) atof(Begin);
			*Iterator = Backup;

			Object.Type = JSON::ObjectType::Number;
			return true;
		}

		if(*Iterator == 't')
		{
			if(memcmp(Iterator, "true", 4))
				return false;

			Iterator += 4;

			Object.Boolean = 1;
			Object.Type = JSON::ObjectType::Boolean;

			return true;
		}

		if(*Iterator == 'f')
		{
			if(memcmp(Iterator, "false", 5))
				return false;

			Iterator += 5;

			Object.Boolean = 0;
			Object.Type = JSON::ObjectType::Boolean;

			return true;
		}
	}
	while(* ++ Iterator);

	return 0;
}

JSON::Object::Object()
{
    Buffer = 0;
    Type = ObjectType::List;
    String = 0;
    MapNames = 0;
    Length = 0;
}

JSON::Object::Object(JSON::LoadSource::Type Source, const char * Data)
{
	switch(Source)
	{
		case JSON::LoadSource::StringReadOnly:
		{
			Buffer = _strdup(Data);

			char * Iterator = Buffer;
			LoadNext(*this, Iterator);

			break;
		}

        case JSON::LoadSource::ObjectString:
		case JSON::LoadSource::String:
		{
			Buffer = 0;

            char * Iterator = (char *) Data;

            if(Source == JSON::LoadSource::String)
			    LoadNext(*this, Iterator);
            else
                ParseObject(*this, Iterator);

			break;
		}

        case JSON::LoadSource::ObjectFile:
		case JSON::LoadSource::File:
		{

			FILE * File = fopen(Data, "rb");

			if(!File)
				break;

			fseek(File, 0, SEEK_END);
			int Length = ftell(File);
			fseek(File, 0, SEEK_SET);

			Buffer = (char *) malloc(Length + 1);
			
			if(fread(Buffer, 1, Length, File) != Length)
				break;

            if(Source == JSON::LoadSource::File)
			    LoadNext(*this, Buffer);
            else
            {
                Buffer[Length] = 0;
                ParseObject(*this, Buffer);
            }

			break;
		}

		case JSON::LoadSource::None:
		{
			Buffer = 0;
			break;
		}
	};
}

const JSON::Object &JSON::Object::operator [] (const char * Name) const
{
	if ( MapNames == NULL )
	    return JSON::Empty;

    for(int i = 0; i < Length; ++ i)
    {
        if(!strcasecmp(MapNames[i], Name))
            return MapObjects[i];
    }

    return JSON::Empty;
}

const JSON::Object &JSON::Object::operator [] (int Index) const
{
    if(Index > (Length - 1) || Index < 0)
        return JSON::Empty;

    return List[Index];
}

JSON::Object &JSON::Object::operator [] (const char * Name)
{
    for(int i = 0; i < Length; ++ i)
    {
        if(!strcasecmp(MapNames[i], Name))
            return MapObjects[i];
    }

    return (JSON::Object &) JSON::Empty;
}

JSON::Object &JSON::Object::operator [] (int Index)
{
    if(Index > (Length - 1) || Index < 0)
        return (JSON::Object &) JSON::Empty;

    return List[Index];
}

JSON::Object::~Object()
{
	free(Buffer);
}

JSON::Object::operator const char * () const
{
    switch(Type)
    {
        case JSON::ObjectType::String:
            return String;

        default:
            return "";
    }
}
