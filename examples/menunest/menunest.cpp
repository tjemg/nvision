//========================================================================
//  The following routines have been uploaded to the Borland Forum BPROGB
//  on CompuServe by the Technical Support staff.  They are provided as a
//  courtesy and not as part of a Borland product; and, as such, are
//  provided without the assurance of technical support or any specific
//  guarantees.
//========================================================================
//  Turbo Vision - nested menus
//
//  - This sample code demonstrates creating nested menus using the
//  TSubMenu class.
//
//  - There are two constructors for TMenuItem.  Control of what selecting
//  the menu item does is controlled by use of these two constructors.
//
//      TMenuItem( const char *aName,
//                 ushort aCommand,
//                 ushort aKeyCode,
//                 ushort aHelpCtx = hcNoContext,
//                 char *p = 0;
//                 TMenuItem *aNext = 0
//               );
//      TMenuItem( const char *aName,
//                 ushort aKeyCode,
//                 TMenu *aSubMenu,
//                 ushort aHelpCtx = hcNoContext,
//                 TMenuItem *aNext = 0
//               );
//
//  If the first of the above constructors is used, then selecting the
//  menu item will cause an event to be generated with aCommand.
//  If the second constructor is used, then selecting the menu item will
//  cause the menu aSubMenu to be created.
//========================================================================
#define Uses_MsgBox
#define Uses_TApplication
#define Uses_TButton
#define Uses_TDeskTop
#define Uses_TDialog
#define Uses_TKeys
#define Uses_TMenu
#define Uses_TMenuBar
#define Uses_TMenuItem
#define Uses_TStaticText
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TStatusLine
#define Uses_TSubMenu
#include <tv.h>

//========================================================================
//  global data
//------------------------------------------------------------------------
const int cmAbout       = 100;  // user request: about box

//========================================================================
//  class definitions
//------------------------------------------------------------------------
class TV6 : public TApplication {
    //  main application class

public:
    TV6();

    // virtual functions to be locally redefined
    static TMenuBar *initMenuBar( TRect r );
    void handleEvent( TEvent &event );

    // declare new functions
    void AboutBox();

};

//========================================================================
//  implementation of class TV6
//------------------------------------------------------------------------
TV6::TV6() : TProgInit( &TApplication::initStatusLine,
                    &TV6::initMenuBar, &TApplication::initDeskTop )
{
}

//------------------------------------------------------------------------
//  define menu bar
//------------------------------------------------------------------------
TMenuBar *TV6::initMenuBar(TRect r)
{
    r.b.y = r.a.y + 1;
    TMenuBar *newMenu = new TMenuBar( r,
        *new TSubMenu( "~=~", kbAltSpace ) +
        *new TMenuItem( "~A~bout", cmAbout, kbAltA, hcNoContext ) +
        newLine() +
        // this TMenuItem will bring up another menu
        *new TMenuItem( "Exit", 0, new TMenu(
            *new TMenuItem( "Exit & ~S~ave", cmQuit, kbAltX, hcNoContext, 0,
            new TMenuItem( "Exit & ~A~bandon", cmQuit, kbAltY, hcNoContext, 0,
            new TMenuItem( "Just ~Q~uit", cmQuit, kbAltZ, hcNoContext, 0,
            new TMenuItem( "~N~ext Level", 0, new TMenu(
               *new TMenuItem( "~O~ne", cmQuit, kbAltX, hcNoContext, 0, 
               new TMenuItem( "~T~wo", cmQuit, kbAltX ))))                                          
        )))), hcNoContext)
        );
    return newMenu;
}

//------------------------------------------------------------------------
// event-handler
//------------------------------------------------------------------------
void TV6::handleEvent( TEvent &event )
{
    TApplication::handleEvent( event );
    if( event.what == evCommand )
        switch( event.message.command )
        {
            case cmAbout:       // 'about' box
                AboutBox();
                clearEvent( event );
                break;
        }
}

//------------------------------------------------------------------------
// create modal About dialog box
//------------------------------------------------------------------------
void TV6::AboutBox()
{
    // remind us of the purpose of the example
    TDialog *pd = new TDialog( TRect( 0, 0, 35, 12 ), "About" );
    if( pd )
    {
        pd->insert( new TStaticText( TRect( 1, 2, 34, 7 ),
                "\003Turbo Vision\n\003\n"
                "\003Creating a nested menu\n\003\n"
                "\003Borland Technical Support"));
        pd->insert( new TButton( TRect( 3, 9, 32, 11), "~O~key-d~O~key",
                cmOK, bfDefault ) );
        pd->options |= ofCentered;
        deskTop->execView( pd );
    }
    destroy( pd );
}

//========================================================================
int main( void )
{
    TV6 tv6App;
    tv6App.run();
    return 0;
}

