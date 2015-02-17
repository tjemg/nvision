//========================================================================
//  The following routines have been uploaded to the Borland Forum BPROGB
//  on CompuServe by the Technical Support staff.  They are provided as a
//  courtesy and not as part of a Borland product; and, as such, are
//  provided without the assurance of technical support or any specific
//  guarantees.
//========================================================================
//  Turbo Vision - Building menus
//
//  - This sample code demonstrates a method of creating menus using the
//  an overloaded operator+ to simplify the syntax.
//
//  - A menu is a linked list of TMenuItem, TMenu, and TSubMenu objects.  
//  Normally, the construction of a menu involves one of two techniques.
//  One is nest the constuction of menus inside calls to constuct another
//  menu.  This makes the construction of a menu into one long statement
//  which can be difficult to read and in which it is easy to make a
//  syntax error.  In complex menus, it can also represent one large
//  complex expression that may cause internal compiler tables to be
//  exceeded, resulting in an out of memory error.  The other is to build
//  the menu in reverse order, and pass the pointer to the previous item
//  built as the last parameter (pointer to next menu) to the constructor
//  of the next menu group built.  This forces you to build the menu in
//  reverse order and is not intuitive.
//
//  Another method, is to overload the operator+ for adding two TMenuItem
//  objects.  This makes the menu easier to build.  However, if you use
//  this technique, you may NOT use the TSubMenu class.  Since the
//  operator has been overloaded for TSubMenu + TMenuItem, and TSubMenu is
//  derived from TMenuItem, this could lead to an ambiguity.
//
//  The restriction on using the TSubMenu class is not in the least
//  inconvenient - as this example demonstrates.
//
//  Additional notes:
//  - The reason this works is because of the way menu system is built.
//  Each menu item's next pointer either points to another menu item
//  which is at the same 'level' as that menu item, or it is set to NULL,
//  indicating that it is the last item in that level.  Additionally, each
//  item has an 'action' which is either to case the corresponding cmXXXXX
//  command to be broadcast, or to invoke another menu which will, in
//  turn, consist of a linked list of menu items.  Thus, there is no real
//  difference between the menu items on the 'menu bar' and those listed
//  in one of the sub-menus.  It's just that the top-level list of menu
//  items is displayed across the menu bar, while others, as appear as
//  drop-down menus.
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
const int cmAbout       = 100;
const int cmGreet       = 101;
const int cmDoIt        = 102;
const int cmOne         = 103;
const int cmTwo         = 104;
const int cmThree       = 105;
const int cmSimple      = 106;
const int cmRare        = 107;
const int cmWell        = 108;
const int cmCore        = 109;
const int cmRoll        = 110;
const int cmCafe        = 111;
const int cmTimes       = 112;


//========================================================================
//  Since the objects will always be in a linked list, and the operator+
//  is processd left-to-right, we will define the function as appending
//  menuItem2 to menuItem1, and then return menuItem1.
//------------------------------------------------------------------------
//TMenuItem& operator +( TMenuItem& menuItem1, TMenuItem& menuItem2 )
//{
//    TMenuItem *p = &menuItem1;
//    while( p->next != NULL )
//        p = p->next;
//    p->next = &menuItem2;
//    return menuItem1;
//}

//========================================================================
//  class definitions
//------------------------------------------------------------------------
class TApp : public TApplication {
    //  main application class

public:
    TApp();

    // virtual functions to be locally redefined
    static TMenuBar *initMenuBar( TRect r );
    void handleEvent( TEvent &event );

    // declare new functions
    void AboutBox();
};

//========================================================================
//  implementation of class TApp
//------------------------------------------------------------------------
TApp::TApp() : TProgInit( &TApplication::initStatusLine,
                    &TApp::initMenuBar, &TApplication::initDeskTop )
{
}

//------------------------------------------------------------------------
// define menu bar
//------------------------------------------------------------------------
TMenuBar *TApp::initMenuBar( TRect r )
{
    r.b.y = r.a.y + 1;

    // for ease of reading, we'll build each item of the top level
    // menu separately, then just add them together in the end

    TMenuItem *top1 = new TMenuItem( "~\360~", kbAltSpace, new TMenu(
        // second level pull-down menu
        *new TMenuItem( "~A~bout", cmAbout, kbNoKey, hcNoContext, 0 ) +
        newLine() +
        *new TMenuItem( "~E~xit", kbNoKey, new TMenu(
            // third level pull-down menu
            *new TMenuItem( "~S~ave", cmQuit, kbNoKey, hcNoContext, 0 ) +
            *new TMenuItem( "A~b~andon", cmQuit, kbNoKey, hcNoContext, 0 ) +
            *new TMenuItem( "Just ~Q~uit", cmQuit, kbNoKey, hcNoContext, 0 )
        ) ) +
        *new TMenuItem( "~G~reeting", cmGreet, kbNoKey, hcNoContext, 0 )
    ), hcNoContext );

    TMenuItem *top2 = new TMenuItem( "~D~oIt!", cmDoIt, kbAltD, hcNoContext, 0 );

    TMenuItem *top3 = new TMenuItem( "~O~ptions", kbAltO, new TMenu(
        // second level pull-down menu
        *new TMenuItem( "Option ~1~", cmOne, kbNoKey, hcNoContext, 0 ) +
        *new TMenuItem( "Option ~2~", cmTwo, kbNoKey, hcNoContext, 0 ) +
        *new TMenuItem( "Option ~3~", cmThree, kbNoKey, hcNoContext, 0 )
    ), hcNoContext );

    TMenuItem *top4 = new TMenuItem( "~C~omplex", kbAltC, new TMenu(
        *new TMenuItem( "~S~imple", cmSimple, kbNoKey, hcNoContext, 0 ) +
        *new TMenuItem( "~M~edium", kbNoKey, new TMenu(
            *new TMenuItem( "~R~are", cmRare, kbNoKey, hcNoContext, 0 ) +
            *new TMenuItem( "~W~ell", cmWell, kbNoKey, hcNoContext, 0 )
        ) ) +
        *new TMenuItem( "~H~ard", kbNoKey, new TMenu(
            *new TMenuItem( "~C~ore", cmCore, kbNoKey, hcNoContext, 0 ) +
            *new TMenuItem( "~R~ock", kbNoKey, new TMenu(
                *new TMenuItem( "&~R~oll", cmRoll, kbNoKey, hcNoContext, 0 ) +
                *new TMenuItem( "~C~afe", cmCafe, kbNoKey, hcNoContext, 0 )
            ) ) +
            *new TMenuItem( "~T~imes", cmTimes, kbNoKey, hcNoContext, 0 )
        ) )
    ) );

    return( new TMenuBar ( r, new TMenu( *top1 + *top2 + *top3 + *top4 ) ) );
}


//------------------------------------------------------------------------
// event-handler
//------------------------------------------------------------------------
void TApp::handleEvent( TEvent &event )
{
    TApplication::handleEvent( event );
    if( event.what == evCommand )
    {
        switch( event.message.command )
        {
            case cmAbout:       // 'about' box
                AboutBox();
                clearEvent( event );
                break;
            case cmGreet:
                messageBox( "\003Howdy!", mfInformation | mfOKButton );
                break;
            case cmDoIt:
                messageBox( "\003Just do it!", mfInformation | mfOKButton );
                break;
            case cmOne:
                messageBox( "\003One", mfInformation | mfOKButton );
                break;
            case cmTwo:
                messageBox( "\003Two", mfInformation | mfOKButton );
                break;
            case cmThree:
                messageBox( "\003Three", mfInformation | mfOKButton );
                break;
            case cmSimple:
                messageBox( "\003Simple!", mfInformation | mfOKButton );
                break;
            case cmRare:
                messageBox( "\003Medium Rare", mfInformation | mfOKButton );
                break;
            case cmWell:
                messageBox( "\003Medium Well", mfInformation | mfOKButton );
                break;
            case cmCore:
                messageBox( "\003Hard Core", mfInformation | mfOKButton );
                break;
            case cmRoll:
                messageBox( "\003Hard Rock&Roll", mfInformation | mfOKButton );
                break;
            case cmCafe:
                messageBox( "\003Hard Rock Cafe", mfInformation | mfOKButton );
                break;
            case cmTimes:
                messageBox( "\003Hard Times", mfInformation | mfOKButton );
                break;
            default:
                messageBox( "\003handling error", mfError | mfOKButton );
        }
        clearEvent( event );
    }
}

//------------------------------------------------------------------------
// create modal About dialog box
//------------------------------------------------------------------------
void TApp::AboutBox()
{
    // remind us of the purpose of the example
    TDialog *pd = new TDialog( TRect( 0, 0, 35, 12 ), "About" );
    if( pd )
    {
        pd->insert( new TStaticText( TRect( 1, 2, 34, 7 ),
                "\003Turbo Vision\n\003\n"
                "\003Simplifying complex menus\n\003\n"
                "\003Borland Technical Support"));
        pd->insert( new TButton( TRect( 3, 9, 32, 11), "~O~k",
                cmOK, bfDefault ) );
        pd->options |= ofCentered;
        deskTop->execView( pd );
    }
    destroy( pd );
}

//========================================================================
int main( void )
{
    TApp tv6App;
    tv6App.run();
    return 0;
}

