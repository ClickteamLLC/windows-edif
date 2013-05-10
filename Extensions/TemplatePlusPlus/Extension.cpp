/* Extension.cpp
 * This file contains the definitions for
 * your extension's general runtime functions,
 * such as the constructor and destructor,
 * handling routines, etc.
 * Functions defined here:
 * Extension::Extension <constructor>
 * Extension::~Extension <destructor>
 * Extension::Handle
 * Extension::Display
 * Extension::Pause
 * Extension::Continue
 * Extension::Save
 * Extension::Load
 * Extension::Action		<--|
 * Extension::Condition		<--|- not what you think!
 * Extension::Expression	<--|
 */

#include "Common.h"

/* <constructor>
 * This is your extension's constructor, which
 * is the replacement for the old CreateRunObject
 * function. You don't need to manually call
 * constructors or pointlessly initialize
 * pointers with dynamic memory. Just link
 * your A/C/Es, perform initialization steps, and
 * you're good to go.
 */
Extension::Extension(RD *rd, SerializedED *SED, createObjectInfo *COB) : rd(rd), rh(rd->rHo.hoAdRunHeader), Runtime(rd)
{
	//Link all your action/condition/expression functions
	//to their IDs to match the IDs in the JSON here.
	LinkAction(0, ActionExample);
	LinkAction(1, SecondActionExample);

	LinkCondition(0, AreTwoNumbersEqual);

	LinkExpression(0, Add);
	LinkExpression(1, HelloWorld);


	//This is where you'd do anything you'd do in CreateRunObject in the original SDK.
	//It's the only place you'll get access to the editdata at runtime, so you should
	//transfer anything from the editdata to the extension class here. For example:
//	EditData ed (SED);
//	MyString = ed.MyString;
//	MyInt = ed.MyInt;
//	MyArray = ed.MyArray;

	//
}

/* <destructor>
 * This is your extension's destructor, the
 * replacement for DestroyRunObject. No calling
 * destructors manually or deallocating pointless
 * dynamic memory - in most cases this function
 * won't need any code written.
 */
Extension::~Extension()
{
	//
}

/* Handle
 * MMF2 calls this function to let your extension
 * "live" - if you want, you can have MMF2 call this
 * every frame. This is where you'd, for instance,
 * simulate physics or move an object. This is
 * the analagous function to the old HandleRunObject.
 */
short Extension::Handle()
{
	/*
	   If your extension will draw to the MMF window you should first 
	   check if anything about its display has changed :

		   if (rd->roc.rcChanged) return REFLAG_DISPLAY;
		   else return 0;

	   You will also need to make sure you change this flag yourself 
	   to 1 whenever you want to redraw your object
	 
	   If your extension won't draw to the window, but it still needs 
	   to do something every MMF2 loop use:

			return 0;

	   If you don't need to do something every loop, use :

			return REFLAG_ONESHOT;

	   This doesn't mean this function can never run again. If you want MMF2
	   to handle your object again (causing this code to run) use this function:

			Runtime.Rehandle();

	   At the end of the event loop this code will run.

	*/

	//Will not be called next loop	
	return REFLAG_ONESHOT;
}

/* Display
 * This is the analagous function to
 * DisplayRunObject. If you return
 * REFLAG_DISPLAY in Handle() this
 * routine will run. If you want MMF2
 * to apply ink effects for you, then
 * implement GetRunObjectSurface in
 * Runtime.cpp instead.
 */
short Extension::Display()
{
	return 0;
}

/* Pause
 * If your extension plays sound, for
 * example, then MMF2 calls this to
 * let you know to pause the music,
 * usually by another extension's request
 * or by the player pausing the applcation.
 */
short Extension::Pause()
{
	return 0;
}

/* Continue
 * Opposite to the above, MMF2 lets
 * you know that the silence is over;
 * your extension may live again.
 */
short Extension::Continue()
{
	return 0;
}

/* Save
 * When the user uses the Save
 * Frame Position action, you need
 * so serialize your runtime data to
 * the File given. It is a Windows
 * file handle, but you can use some
 * of MMF2's built-in functions for
 * writing files. Check the MMF2SDK
 * Help file for more information.
 */
bool Extension::Save(HANDLE File)
{
	return true;
}

/* Load
 * As opposed to above, here you need to
 * restore your extension's runtime state
 * from the given file. Only read what you
 * wrote!
 */
bool Extension::Load(HANDLE File)
{
	return true;
}


/* Action, Condition, Expression
 * These are called if there's no function linked
 * to an ID. You may want to put MessageBox calls
 * to let you know that the ID is unlinked, or you
 * may just want to use unlinked A/C/Es as a feature.
 */
void Extension::Action(int ID, RD *rd, long param1, long param2)
{
}

long Extension::Condition(int ID, RD *rd, long param1, long param2)
{
	return false; //hopefully StringComparison (PARAM_CMPSTRING) is not used, or this may crash
}

long Extension::Expression(int ID, RD *rd, long param)
{
	return long(_T("")); //so that unlinked expressions that return strings won't crash
}

