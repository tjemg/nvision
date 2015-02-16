//
// Having a button end the modal state...
//
// Written by Borland C++ Tech Support
//

#define Uses_TKeys
#define Uses_TRect
#define Uses_TEvent
#define Uses_TLabel
#define Uses_TButton
#define Uses_TInputLine
#define Uses_TDialog
#define Uses_TMenuBar
#define Uses_TMenu
#define Uses_TMenuItem
#define Uses_TStaticText
#define Uses_TInputLine
#define Uses_TLabel
#define Uses_TDeskTop
#define Uses_TApplication
#include <tv.h>

#include <stdio.h>

const int cmModalBox    = 100;          // Command list.
const int cmEntryBox    = 101;
const int cmOne         = 102;
const int cmTwo         = 103;

class TMode : public TApplication
{

public:

    TMode() : TProgInit( initStatusLine, initMenuBar, initDeskTop) { }
    virtual void handleEvent( TEvent& event );
    static TMenuBar *initMenuBar( TRect r );

    void createModalBox( void );
    void createEntryBox( unsigned cm );

};


class TModalBox : public TDialog
{

public:

    TModalBox();
    virtual void handleEvent( TEvent& event );

};


class TEntryBox : public TDialog
{

public:

    TEntryBox( char *labels[] );

    struct TEBData
    {
        char lineOne[25];
        char lineTwo[25];
    };

};


char *names[][3] = {
    { "Dialog box #1", "First", "Second" },
    { "Dialog box #2", "Velocity", "Acceleration" }
};

//
// Code for main() function.
//
int main(void)
{
    TMode modalApp;
    modalApp.run();
}


//
// Code for the TMode object (Application object.)
//

TMenuBar *TMode::initMenuBar( TRect r )
{
    r.b.y =  r.a.y + 1;

    return (new TMenuBar( r, new TMenu(
      *new TMenuItem( "~M~odal Dialog", cmModalBox, kbAltM )
      )));
}


void TMode::handleEvent( TEvent& event )
{
    TApplication::handleEvent(event);

    if (event.what == evCommand)
    {
        switch (event.message.command)
        {
        case cmModalBox:
            createModalBox();
            clearEvent(event);
            break;

        //
        // This event is created by pressing a button in the previous dialog
        // box.  The infoInt data member will represent the actual button
        // pressed.
        //
        case cmEntryBox:
            createEntryBox( event.message.infoInt );
            clearEvent(event);
            break;
        }
    }
}


void TMode::createModalBox()
{
    TView *t = (TView *) validView( new TModalBox );

    if( t )
    {
        //
        // We don't need to know if Cancel was hit here because there will
        // be a pending event in the system if it was NOT hit.
        //
        deskTop->execView( t );
        destroy( t );
    }
}


void TMode::createEntryBox( unsigned cm )
{
    TEntryBox *u = (TEntryBox *) validView(new TEntryBox(names[cm - cmOne]));
    TEntryBox::TEBData data;

    if( u )
    {
        if(deskTop->execView( u ) == cmOK )
            u->getData( &data );                // Get data entered...
        destroy( u );
    }
}


//
// Code for the TModalBox object.
//

TModalBox::TModalBox() :
    TDialog(TRect(0, 0, 36, 9), "About"),
    TWindowInit( initFrame )
{
    insert(new TStaticText(TRect(3,2,33,3), "\003Modal dialogs & buttons\n \n"));
    insert(new TButton(TRect(6,4,16,6), "~O~ne", cmOne, bfNormal));
    insert(new TButton(TRect(6,6,16,8), "~T~wo", cmTwo, bfNormal));
    insert(new TButton(TRect(18,4,28,6), "~C~ancel", cmCancel, bfDefault));
    options |= ofCentered;
}


void TModalBox::handleEvent( TEvent& event )
{
    TDialog::handleEvent( event );      // call parent handleEvent...

    //
    // If the button was pressed, create a new command event "cmEntryBox"
    // with the infoInt data member representing the actual button pressed.
    //
    if( event.what == evCommand &&
        (event.message.command >= cmOne && event.message.command <= cmTwo )
      )
    {
        event.message.infoInt = event.message.command;
        event.message.command = cmEntryBox;
        putEvent( event );

        if( (state & sfModal) != 0 )
        {
            endModal( cmOK );
            clearEvent( event );
        }
    }
}


TEntryBox::TEntryBox( char *labels[] ) :
    TDialog( TRect(0,0,40,11), labels[0] ),
    TWindowInit( initFrame )
{
    TInputLine *ti = new TInputLine( TRect(2,3,30,4), 25 );
    insert( ti );
    insert( new TLabel( TRect(2,2,30,3), labels[1], ti ) );

    ti = new TInputLine( TRect(2,6,30,7), 25 );
    insert( ti );
    insert( new TLabel( TRect(2,5,30,6), labels[2], ti ) );

    insert( new TButton( TRect(8,8,18,10), "O~K~", cmOK, bfDefault ) );
    insert( new TButton( TRect(22,8,32,10), "~C~ancel", cmCancel, bfNormal ) );

    options |= ofCentered;
}
