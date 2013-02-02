/* Properties.cpp
 * In this file you will define the edittime
 * properties for your extension. By default
 * only a version number is displayed.
 * Functions defined here:
 * GetProperties
 * ReleaseProperties
 * GetPropCreateParam
 * ReleasePropCreateParam
 * GetPropValue
 * SetPropValue
 * GetPropCheck
 * SetPropCheck
 * EditProp
 * IsPropEnabled
 */

#include "Common.h"

#ifndef RUN_ONLY

/* Prop
 * This is the namespace the property identifiers
 * are stored in. It keeps them out of the global
 * namespace and it helps with intellisense.
 */
namespace Prop
{
	/* <enum>
	 * The property identitifers.
	 */
	enum
	{
		zNOT_USED = PROPID_EXTITEM_CUSTOM_FIRST,
		Version,
		//MyString,
		//MyInt,
	};
}

PropData Properties[] = //See the MMF2SDK help file for information on PropData_ macros.
{
	PropData_StaticString(Prop::Version, (UINT_PTR)"Version #", (UINT_PTR)"This is the current version of the EDIF Template object."),
	//PropData_EditMultiLine(Prop::MyString, (UINT_PTR)"My String", (UINT_PTR)"The contents of my string."),
	//PropData_EditNumber(Prop::MyInt, (UINT_PTR)"My Integer", (UINT_PTR)"The value of my integer."),
	PropData_End()
};

#endif

/* GetProperties
 * Here, you choose what proeprties to insert
 * into which tabs. Lots of dynamic things can
 * be done in here, such as dynamically
 * generating the properties based on the
 * number of stored values in the editdata.
 */
BOOL MMF2Func GetProperties(mv *mV, SerializedED *SED, BOOL MasterItem)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//ed.stuff;
	mvInsertProps(mV, SED, Properties, PROPID_TAB_GENERAL, TRUE);
	//if you changed ed:
	//ed.Serialize(mV, SED);
	return TRUE;
#endif
	return FALSE;
}

/* ReleaseProperties
 * If you dynamically allocated memory for
 * property arrays in GetProperties, release
 * that memory in this function.
 */
void MMF2Func ReleaseProperties(mv *mV, SerializedED *SED, BOOL MasterItem)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//ed.stuff;
	//
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
}

/* GetPropCreateParam
 * A convenience function if you use the
 * hard-coded Properties array above. Allows
 * you to dynamically create e.g. combo
 * lists and other things that have
 * additional parameters for their
 * properties. However if you dynamically
 * allocated everything in GetProperties,
 * this function is useless.
 */
LPARAM MMF2Func GetPropCreateParam(mv *mV, SerializedED *SED, UINT PropID)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//ed.stuff;
	//return (LPARAM)TheParameter;
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
	return NULL;
}

/* ReleasePropCreateParam
 * If you found a useful way to use the
 * above function and you dynamically
 * allocated memory, release it here.
 */
void MMF2Func ReleasePropCreateParam(mv *mV, SerializedED *SED, UINT PropID, LPARAM lParam)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//ed.stuff;
	//MyAwesomeMemoryFreeingFunction(lParam);
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
}

/* GetPropValue
 * This is where you return the display
 * values for the properties you defined.
 * Refer to the MMF2SDK Help file for
 * information on what to return for
 * each kind of property.
 */
void *MMF2Func GetPropValue(mv *mV, SerializedED *SED, UINT PropID)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	switch(PropID)
	{
	case Prop::Version:
		{
			return new CPropDataValue("Default.EDIF.Template.0");
		}
	//case Prop::MyString:
	//	{
	//		return new CPropDataValue(ed.MyString.c_str());
	//	}
	//case Prop::MyInt:
	//	{
	//		return new CPropDWordValue(ed.MyInt);
	//	}
	}
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
	return NULL;
}

/* SetPropValue
 * The user has just finished entering their
 * 500 page essay into your MyString field.
 * MMF2 isn't so kind as to save that for them,
 * so it entrusts it with you. Store it!
 */
void MMF2Func SetPropValue(mv *mV, SerializedED *SED, UINT PropID, CPropValue *PropVal)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//switch(PropID)
	//{
	//case Prop::MyString:
	//	{
	//		ed.MyString = (LPSTR)((CPropDataValue*)PropVal)->m_pData;
	//		break;
	//	}
	//case Prop::MyInt:
	//	{
	//		ed.MyInt = (CPropDWordValue*)PropVal)->m_dwValue;
	//		break;
	//	}
	//}
	//since you changed ed:
	//ed.Serialize(mV, SED);

	//You may want to have your object redrawn in the
	//frame editor after the modifications; in this
	//case, just call this function:
	//mvInvalidateObject(mV, SED);
#endif
}

/* GetPropCheck
 * There are checkbox properties, and
 * there are properties that have their
 * own checkbox as an option. This is
 * where you tell MMF2 whether those
 * boxes are ticked or not.
 */
BOOL MMF2Func GetPropCheck(mv *mV, SerializedED *SED, UINT PropID)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//switch(PropID)
	//{
	//case Prop::MyCheckBoxPropertyOrPropertyThatHasTheCheckboxOptionSet:
	//	{
	//		return ed.WhetherOrNotThatPropertyOfMineIsTicked ? TRUE : FALSE;
	//	}
	//}
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
	return FALSE;
}

/* SetPropCheck
 * The user has painstakingly moved the
 * mouse cursor over the checkbox and
 * expended the immense effort required
 * to click the mouse and toggle the
 * state of the tickbox. Don't let their
 * effort be all for naught!
 */
void MMF2Func SetPropCheck(mv *mV, SerializedED *SED, UINT PropID, BOOL Ticked)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//switch(PropID)
	//{
	//case Prop::MyCheckBoxPropertyOrPropertyThatHasTheCheckboxOptionSet:
	//	{
	//		ed.WhetherOrNotThatPropertyOfMineIsTicked = Ticked != FALSE ? true : false;
	//	}
	//}
	//since you changed ed:
	//ed.Serialize(mV, SED);
#endif
}

/* EditProp
 * If you use a button proeprty or a
 * property that has a button, then
 * you would be impolite to ignore
 * the user when they click on that
 * button. Here is where you respond
 * to that button press e.g. with
 * a dialog.
 */
BOOL MMF2Func EditProp(mv *mV, SerializedED *SED, UINT PropID)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//switch(PropID)
	//{
	//case Prop::MyButtonPropertyOrPropertyThatHasAButtonWithIt:
	//	{
	//		//
	//		break;
	//	}
	//}
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
	return FALSE;
}

/* IsPropEnabled
 * Let's say you're tired of having to deal
 * with the user's various 500 page essays,
 * tickbox toggles, and button clicks. Here,
 * you can simply disable a property by
 * returning FALSE for that property.
 */
BOOL MMF2Func IsPropEnabled(mv *mV, SerializedED *SED, UINT PropID)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	switch(PropID)
	{
	case Prop::Version:
		{
			return FALSE; //Makes the version proeprty greyed out
		}
	//case Prop::MyString:	//intentional\\
	//case Prop::MyInt:		//fallthrough\\
	//	{
	//		return TRUE; //allows the user to interact with these proeprties
	//	}
	}
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
	return FALSE;
}
