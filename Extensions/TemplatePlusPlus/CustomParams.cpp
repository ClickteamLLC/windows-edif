/* CustomParams.cpp
 * Just as with all the other SDKs, EDIF
 * supports the creation and use of custom,
 * you-defined parameters. Your own dialog,
 * your own display string, your own sense
 * of control. Read the MMF2SDK Help file
 * for more information. To use custom
 * parameters with EDIF, the parameter type
 * must be "Custom" or "Custom#" where # is
 * the number of the custom parameter.
 * Functions defined here:
 * InitParameter
 * EditParameter
 * GetParameterString
 */

#include "Common.h"

/* InitParameter
 * Like with the editdata, this data will
 * be written as-is to disk and later read
 * back. The most you can store is defined
 * by PARAM_EXTMAXSIZE, which currently is
 * 512 bytes. Use them wisely!
 */
void MMF2Func InitParameter(mv *mV, short ID, paramExt *ParamData)
{
#ifndef RUN_ONLY
	//store data in ParamData->pextData
	//store the size of the data in ParamData->pextSize
#endif
}

// Example of custom parameter setup proc
// --------------------------------------
/*
#ifndef RUN_ONLY
BOOL CALLBACK SetupProc(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam)
{
	paramExt*			pExt;

	switch (msgType)
	{
		case WM_INITDIALOG: // Init dialog

			// Save edptr
			SetWindowLong(hDlg, DWL_USER, lParam);
			pExt=(paramExt*)lParam;

			SetDlgItemText(hDlg, IDC_EDIT, pExt->pextData);
			return TRUE;

		case WM_COMMAND: // Command

			// Retrieve edptr
			pExt = (paramExt *)GetWindowLong(hDlg, DWL_USER);

			switch (wmCommandID)
			{
			case IDOK:	// Exit
				GetDlgItemText(hDlg, IDC_EDIT, pExt->pextData, 500);
				pExt->pextSize=sizeof(paramExt)+strlen(pExt->pextData)+1;
				EndDialog(hDlg, TRUE);
				return TRUE;

				default:
					break;
			}
			break;

		default:
			break;
	}
	return FALSE;
}
#endif
*/

/* EditParameter
 * This is where you actually bring up
 * the dialog and take input from
 * the user and store it.
 */
void MMF2Func EditParameter(mv _far *mV, short code, paramExt* pExt)
{
#ifndef RUN_ONLY
	// Example
	// -------
	// DialogBoxParam(hInstLib, MAKEINTRESOURCE(DB_TRYPARAM), mV->mvHEditWin, SetupProc, (LPARAM)(LPBYTE)pExt);
#endif
}

/* GetParameterString
 * This is where you give MMF2 the display
 * string for your custom parameter. Remember,
 * this is the display for ONE PARAMETER. It
 * will appear amongst all the other parameter
 * displays depending on the A/C's display
 * string.
 */
void MMF2Func GetParameterString(mv _far *mV, short code, paramExt* pExt, LPSTR pDest, short size)
{
#ifndef RUN_ONLY
	// Example
	// -------
	// wsprintf(pDest, "Super parameter %s", pExt->pextData);
#endif
}
