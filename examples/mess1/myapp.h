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
// myapp.h:  The class definition for TMyApplication.
//
//========================================================================

#ifndef _MYAPP_H
#define _MYAPP_H

#define Uses_TApplication
#define Uses_TMenuBar
#define Uses_TRect
#define Uses_TStatusLine
#define Uses_TEvent
#include <tv.h>

//
// class TMyApplication
//      The application object, derived from the abstract class TApplication
//
// Member functions:
//      TMyApplication() - Constructor.
//

class TMyApplication : public TApplication
{
public:
    TMyApplication();
    static TMenuBar *initMenuBar(TRect);
    static TStatusLine *initStatusLine(TRect);
    void handleEvent(TEvent&);
private:
    void aboutDlg();
    void window();
    void techDlg();
};


#endif
