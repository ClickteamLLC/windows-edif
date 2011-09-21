
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace JSON
{
	namespace LoadSource
	{
		enum Type
		{
			String,
            ObjectString,
			StringReadOnly,
			File,
            ObjectFile,

			None
		};
	};

	namespace ObjectType
	{
		enum Type
		{
			String,
			Number,
			Boolean,
			Map,
			List
		};
	};

	class Object
	{
	private:
		
		char * Buffer;

	public:

        Object();
		Object(LoadSource::Type Source, const char *);

		~Object();

		ObjectType::Type Type;

		union
		{
			const char * String;
			float Number;
			int Boolean;

			Object * List;
			Object * MapObjects;
		};

		const char ** MapNames;
		int Length;

        const JSON::Object &operator [] (const char * Name) const;
        const JSON::Object &operator [] (int Index) const;

        JSON::Object &operator [] (const char * Name);
        JSON::Object &operator [] (int Index);

        operator const char * () const;
	};

    const JSON::Object Empty;
};

