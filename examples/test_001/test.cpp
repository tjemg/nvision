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
#define Uses_MsgBox

#include <tv.h>

#include "tiago_2.h"  // Designed with DLGDSN and adapted by hand

const int cmDrawDialog  = 100;

class TMyApp : public TApplication {

    public:
        TMyApp();
        static TMenuBar *initMenuBar( TRect );
        void handleEvent(TEvent& event);
    protected:
        void DrawDialog(void);
};

void TMyApp::DrawDialog() {

    TDialog2 *pd = new TDialog2( );

    if( validView(pd) ) {
       deskTop->execView( pd );
       messageBox((char *)pd->mySerial->getData(),mfOKButton);
    }
    destroy( pd );
}


TMyApp::TMyApp() : TProgInit( &initStatusLine, &initMenuBar, &initDeskTop) {
}



void TMyApp::handleEvent(TEvent& event) {
    TApplication::handleEvent( event );

    if( event.what == evCommand ) {
        switch( event.message.command) {
            case cmDrawDialog:
                DrawDialog();
                break;

            default:
                break;
        }
        clearEvent( event );
    }
}

TMenuBar *TMyApp::initMenuBar( TRect r ) {
    r.b.y = r.a.y + 1;

    return new TMenuBar( r,
            *new TSubMenu( "~=~", kbAltSpace ) +
            *new TMenuItem( "~D~raw Dialog", cmDrawDialog, kbAltD, hcNoContext, "Alt-D"));


}


int main() {
    TMyApp myApp;
    myApp.run();
    return 0;
}

