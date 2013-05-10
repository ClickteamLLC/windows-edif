/* Expressions.cpp
 * This is where you should define the
 * behavior of your expressions. Make sure
 * the parameters and return types match those
 * in the JSON exactly! Double check Extension.h
 * as well.
 */

#include "Common.h"

int Extension::Add(int First, int Second)
{
	return First + Second;
}

const TCHAR *Extension::HelloWorld()
{
	return _T("Hello world!");
	//return Runtime.CopyString(MyString.c_str()); //for stdtstrings
}
