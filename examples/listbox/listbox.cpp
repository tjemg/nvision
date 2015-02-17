//========================================================================
//  The following example routines have been provided by the Technical
//  Support staff at Borland International.  They are provided as a
//  courtesy and not as part of a Borland product, and as such, are
//  provided without the assurance of technical support or any specific
//  guarantees.
//========================================================================
//  Turbo Vision - Creating a TListBox
//
//  - This sample code demonstrates one method of creating a simple list
//  box which is inserted into a dialog box.
//
//  - Creating a simple list box is requires an associated scroll bar
//  and a list to display.  This example uses a list of the Turbo Vision
//  classes to display.
//------------------------------------------------------------------------
#define Uses_TApplication
#define Uses_TBackground
#define Uses_TButton
#define Uses_TKeys
#define Uses_TDeskTop
#define Uses_TDialog
#define Uses_TListBox
#define Uses_TMenu
#define Uses_TMenuBar
#define Uses_TMenuItem
#define Uses_TRect
#define Uses_TScrollBar
#define Uses_TStaticText
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TStatusLine
#define Uses_TStringCollection
#include <tv.h>
#include <stdlib.h>

//========================================================================
//  global data
//------------------------------------------------------------------------
const int cmAbout   = 100;  // User selected menu item 'About'
const int cmList    = 101;  // User selected menu item 'List'

// we'll use a list of the TV classes for something
// to display in the list box
char *theList[] = {
    "fpbase",             "fpstream",           "ifpstream",
    "Int11trap",          "iopstream",          "ipstream",
    "MsgBoxText",         "ofpstream",          "opstream",
    "otstream",           "pstream",            "TApplication",
    "TBackground",        "TBufListEntry",      "TButton",
    "TChDirDialog",       "TCheckBoxes",        "TCluster",
    "TCollection",        "TColorDialog",       "TColorDisplay",
    "TColorGroup",        "TColorGroupList",    "TColorItem",
    "TColorItemList",     "TColorSelector",     "TCommandSet",
    "TCrossRef",          "TDeskInit",          "TDeskTop",
    "TDialog",            "TDirCollection",     "TDirEntry",
    "TDirListBox",        "TDisplay",           "TDrawBuffer",
    "TEditor",            "TEditWindow",        "TEventQueue",
    "TFileCollection",    "TFileDialog",        "TFileEditor",
    "TFileInfoPane",      "TFileInputLine",     "TFileList",
    "TFrame",             "TGroup",             "THelpFile",
    "THelpIndex",         "THelpTopic",         "THelpViewer",
    "THelpWindow",        "THistInit",          "THistory",
    "THistoryViewer",     "THistoryWindow",     "THWMouse",
    "TIndicator",         "TInputLine",         "TLabel",
    "TListBox",           "TListViewer",        "TMemo",
    "TMenu",              "TMenuBar",           "TMenuBox",
    "TMenuItem",          "TMenuView",          "TMonoSelector",
    "TMouse",             "TNSCollection",      "TNSSortedCollection",
    "TObject",            "TPalette",           "TParagraph",
    "TParamText",         "TPoint",             "TPReadObjects",
    "TProgInit",          "TProgram",           "TPWObj",
    "TPWrittenObjects",   "TRadioButtons",      "TRect",
    "TResourceCollection","TResourceFile",      "TScreen",
    "TScrollBar",         "TScroller",          "TSItem",
    "TSortedCollection",  "TSortedListBox",     "TStaticText",
    "TStatusDef",         "TStatusItem",        "TStatusLine",
    "TStreamable",        "TStreamableClass",   "TStreamableTypes",
    "TStrIndexRec",       "TStringCollection",  "TStringList",
    "TStrListMaker",      "TSubMenu",           "TSystemError",
    "TTerminal",          "TTextDevice",        "TView",
    "TVMemMgr",           "TWindow",            "TWindowInit"
};

// determine the number of items in the list
// did you know there are 111 classes in Turbo Vision?
int numStrings = sizeof( theList ) / sizeof( theList[0] );

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
    void AboutDialog();
    void ListDialog();
};

//========================================================================
//  implementation of TApp
//------------------------------------------------------------------------
TApp::TApp() : TProgInit( &TApplication::initStatusLine,
                    &TApp::initMenuBar, &TApplication::initDeskTop )
{
    // default status line and desk top
}

//------------------------------------------------------------------------
// define menu bar
//------------------------------------------------------------------------
TMenuBar *TApp::initMenuBar( TRect r )
{
    r.b.y = r.a.y + 1;
    return( new TMenuBar( r, new TMenu(
        *new TMenuItem( "~A~bout", cmAbout, kbAltA, hcNoContext, 0,
        new TMenuItem( "~L~ist", cmList, kbAltL, hcNoContext, 0 )
        ) ) ) );
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
            case cmAbout:       // display the about box
                AboutDialog();
                clearEvent( event );
                break;
            case cmList:        // display our list box
                ListDialog();
                clearEvent( event );
                break;
        }
    }
}

//------------------------------------------------------------------------
// create modal About dialog box
//------------------------------------------------------------------------
void TApp::AboutDialog()
{
    // the About box reminds the user what is being demonstrated
    TDialog *pd = new TDialog( TRect( 0, 0, 35, 12 ), "About" );
    if( pd )
    {
        pd->options |= ofCentered;
        pd->insert( new TStaticText( TRect( 1, 2, 34, 7 ),
                "\003Turbo Vision Example\n\003\n"
                "\003Creating a TListBox\n\003\n"
                "\003Borland Technical Support" ) );
        pd->insert( new TButton( TRect( 3, 9, 32, 11 ), "~O~k",
                                cmOK, bfDefault ) );
        deskTop->execView( pd );
    }
    destroy( pd );
}

//------------------------------------------------------------------------
//  display a dialog box containing a listbox
//------------------------------------------------------------------------
void TApp::ListDialog()
{
    // build a collection to pass to the listbox
    TStringCollection *theCollection = new TStringCollection( 100, 10 );

    for( int i=0; i<numStrings; i++ )
        theCollection->insert( newStr( theList[i] ) );

    TDialog *pd = new TDialog( TRect( 0, 0, 51, 20 ), "TV Classes" );
    pd->options |= ofCentered;

    // create a vertical scroll bar for the list box
    TScrollBar *listScroller = new TScrollBar( TRect( 47, 2, 48, 17 ) );

    // create a list box with 2 columns and associated scroller
    TListBox *listBox = new TListBox( TRect( 3, 2, 47, 17 ),
                                        2, listScroller );
    // associate the collection with the list box
    listBox->newList( theCollection );

    // insert the list box with its scroller into the dialog box
    pd->insert( listBox );
    pd->insert( listScroller );

    // if the object was safely built, exec it as modal
    if( validView( pd ) )
        deskTop->execView( pd );

    // and a little clean up
    destroy( pd );
}

//========================================================================
int main(void)
{
    // now for the tought stuff
    TApp myApp;
    myApp.run();
    return 0;
}
