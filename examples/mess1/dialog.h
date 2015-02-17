//========================================================================
//  The following example routines have been provided by the Technical
//  Support staff at Borland International.  They are provided as a
//  courtesy and not as part of a Borland product, and as such, are
//  provided without the assurance of technical support or any specific
//  guarantees.
//========================================================================
//  Turbo Vision - Example of message passing
//
//------------------------------------------------------------------------
//
// dialog.h:  The class definition for TDataEntryDlg.
//
//========================================================================



#ifndef _DIALOG_H
#define _DIALOG_H

#define Uses_TDialog
#define Uses_TRect
#include <tv.h>


//
// TDataEntryDlg - A complex dialog box to demostrate various objects designed
//                 for the TDialog class.
//

class TDataEntryDlg : public TDialog
{
public:
    TDataEntryDlg(TRect,char *);
    Boolean valid(ushort);
private:
    Boolean isValid;
};


#endif
