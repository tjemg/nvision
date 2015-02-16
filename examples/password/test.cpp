/*
    Test Program
    Author: Tiago Gasiba
            2015
*/

#include <stdio.h>

#define Uses_string
#define Uses_TProgram
#define Uses_TApplication
#define Uses_TRect
#define Uses_TMenuBar
#define Uses_TSubMenu
#define Uses_TMenuItem
#define Uses_TStatusLine
#define Uses_TStatusItem
#define Uses_TStatusDef
#define Uses_TDeskTop
#define Uses_TKeys

#include "password.h"

const int cmTest1  = 100;
const int cmTest2  = 110;

class TMyApp : public TApplication {
    public:
        TMyApp();
        static TStatusLine *initStatusLine( TRect r );
        static TMenuBar *initMenuBar( TRect r );
        virtual void handleEvent( TEvent& event);
        void testFunction1();
        void testFunction2();
};


TMyApp::TMyApp() :
    TProgInit( &TMyApp::initStatusLine,
            &TMyApp::initMenuBar,
            &TMyApp::initDeskTop
            ) {
}

void TMyApp::handleEvent(TEvent& event) {
    TApplication::handleEvent(event);

    if( event.what == evCommand ) {
        switch( event.message.command ) {
            case cmTest1:
                testFunction1();
                break;

            case cmTest2:
                testFunction2();
                break;

            default:
                return;
        }
        clearEvent( event );
    }
}

TMenuBar *TMyApp::initMenuBar( TRect r ) {
    r.b.y = r.a.y + 1;
    return new TMenuBar( r,
            *new TSubMenu( "~D~emo", kbAltD ) +
            *new TMenuItem( "~T~est - Check", cmTest1, kbNoKey, hcNoContext, "" )+
            *new TMenuItem( "~T~est - Add", cmTest2, kbNoKey, hcNoContext, "" )+
            *new TMenuItem( "E~x~it", cmQuit, kbNoKey, hcNoContext, "Alt-X" )
            );
}

TStatusLine *TMyApp::initStatusLine( TRect r ) {
    r.a.y = r.b.y - 1;
    return new TStatusLine( r,
            *new TStatusDef( 0, 0xFFFF ) +
            *new TStatusItem( 0, kbF10, cmMenu ) +
            *new TStatusItem( "~Alt-X~ Exit", kbAltX, cmQuit )
            );
}

void TMyApp::testFunction1() {
    Password *password;

    password = new Password();
    password->CheckPassword();
}

void TMyApp::testFunction2() {
    Password *password;

    password = new Password();
    password->AddPassword();
}

int main() {
    TMyApp myApp;
    myApp.run();
    return 0;
}

