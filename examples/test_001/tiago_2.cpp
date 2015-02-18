//-------------------------------------------------------
//                       tiago_2.cpp
//-------------------------------------------------------

#define Uses_TEventQueue
#define Uses_TEvent
#define Uses_TProgram
#define Uses_TApplication
#define Uses_TKeys
#define Uses_TRect
#define Uses_TMenuBar
#define Uses_TSubMenu
#define Uses_TMenuItem
#define Uses_TStatusLine
#define Uses_TStatusItem
#define Uses_TStatusDef
#define Uses_TDeskTop
#define Uses_TView
#define Uses_TWindow
#define Uses_TFrame
#define Uses_TDialog
#define Uses_TButton
#define Uses_TSItem
#define Uses_TMenu

#include <tv.h>

#if !defined( __TIAGO_2_H )
#include "tiago_2.h"
#endif

TDialog2::TDialog2() :
       TDialog(TRect(20, 6, 40, 12), "Verify"),
       TWindowInit(TDialog::initFrame)

{
 TView *control;

 control = new TStaticText(TRect(1, 1, 19, 2), "\x03 Enter Serial");
 insert(control);

 mySerial = new TInputLine(TRect(1, 2, 19, 3), 11);
 control = mySerial;
 insert(control);

 control = new TButton(TRect(1, 3, 19, 5), "O~K~", cmOK, bfDefault);
 insert(control);

 selectNext(False);
}

void TDialog2::handleEvent( TEvent& event)
{
    TDialog::handleEvent(event);
}

Boolean TDialog2::valid(ushort command)
{
   Boolean rslt = TDialog::valid(command);
   if (rslt && (command == cmOK))
     {
     }
   return rslt;
}

const char * const TDialog2::name = "TDialog2";

void TDialog2::write( opstream& os )
{
 TDialog::write( os );
}

void *TDialog2::read( ipstream& is )
{
 TDialog::read( is );
 return this;
}

TStreamable *TDialog2::build()
{
    return new TDialog2( streamableInit );
}

// From here to end of file may be removed if TDialog will not be streamed.

TStreamableClass RDialog( TDialog2::name,
                        TDialog2::build,
                        __DELTA(TDialog2)
                      );

__link(RButton)
__link(RDialog)
__link(RInputLine)
__link(RStaticText)

