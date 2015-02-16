//========================================================================
//  The following example routines have been provided by the Technical
//  Support staff at Borland International.  They are provided as a
//  courtesy and not as part of a Borland product, and as such, are
//  provided without the assurance of technical support or any specific
//  guarantees.
//========================================================================
//  Turbo Vision - Changing text, text color, and sending messages in
//    a dialog box
//
//  - The MyApp::handleEvent function overrides the default handlevent
//  enabling you to respond to user specific commands... in this case
//  cmDrawDialog.
//
//  -TDialog is the base class for a new user defined class MyDialog.
//
//  - In MyDialog the draw member function is overloaded and allows for
//  redifinition of TDialog::draw() function behavior.
//
//  - In the idle loop a counter is incremented and checked for a value
//  via the mod operator.  When the value is found, a broadcast message
//  is sent.
//
//  - The Mydialog::handleEvent function overrides the default handleEvent
//  of TDialog enabling our dialog to respond to the broadcast command.
//
//------------------------------------------------------------------------


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

#include <stdlib.h>      //random
#include <string.h>      //strdup, strlen
#include <tv.h>

#define TIMEOUT       8000          //constant used in idle loop
#define MAX_STRINGS      7          //seven strings in the array
#define MAX_COLORS      16          //colors available

const int cmDrawDialog  = 100;
const int cmDrawLine    = 101;


class TMyApp : public TApplication
{

private:
  unsigned count;      //a count variable...just for fun

public:
	TMyApp();
	static TMenuBar *initMenuBar( TRect );
	static TStatusLine *initStatusLine( TRect );
	void handleEvent(TEvent& event);
protected:
	void DrawDialog(void);
	void idle(void);

};



class myDialog : public TDialog
{

    char *dialogStr;                     //pointer to current string
    char *stringList[MAX_STRINGS];       //array of pointers
    unsigned stringNum;                  //index into array of strings

   public:
      myDialog(const TRect& bounds, const char *aTitle = "Draw Dialog"): TDialog( bounds, aTitle),
	 TWindowInit(&myDialog::initFrame)
	 {
	    stringNum = 0;                 //initialize to first string
	    stringList[0] = strdup("Example of...");
	    stringList[1] = strdup("catching events and...");
	    stringList[2] = strdup("sending messages and...");
	    stringList[3] = strdup("changing text...");
	    stringList[4] = strdup("in a dialog box.");
	    stringList[5] = strdup("Borland International");
	    stringList[6] = strdup("         1992        ");
	 };

      void draw();
      virtual void handleEvent( TEvent& event);

};

//==========================================================================
//        Overlaoded draw:
//           Change the text pointed to by dialogStr, change the
//           color by taking the mod of stringNum.
//==========================================================================
void myDialog::draw()
{

    TDialog::draw();                             //call base class draw()

    char textAttr = getColor(1) & 0xF0;          //set text attributes
    ushort color = stringNum % MAX_COLORS;       //set color

    TDrawBuffer b;
    dialogStr = stringList[ stringNum++ % MAX_STRINGS ] ;
    b.moveStr( 0, dialogStr, color + textAttr );
    writeLine(14,7,strlen(dialogStr),1,b);

}

//==========================================================================
//       If the event is evBroadcast and the command is cmDrawLine...
//       then call the draw member function.
//==========================================================================
void myDialog::handleEvent( TEvent& event)
{

   if ( event.what == evBroadcast )
   {
	  switch( event.message.command )
	  {

	    case cmDrawLine:
	       drawView();
	       break;

	    default:
	       break;
	  }

   }

   TDialog::handleEvent( event );

}


void TMyApp::DrawDialog()
{

    //count = 1;     //initialize the count variable
    myDialog *pd = new myDialog( TRect( 15, 4, 65, 20) );

    if( validView(pd) )
    {
       deskTop->execView( pd );
    }
    destroy( pd );
}


TMyApp::TMyApp() :
	TProgInit( &initStatusLine,
		   &initMenuBar,
		   &initDeskTop
		 )
{
    count = 1;
}


//===========================================================================
//      When count is evenly divisible by TIMEOUT... send a broadcast
//      message to the deskTop.
//===========================================================================
void TMyApp::idle()
{
    TProgram::idle();
    if( ! (count++ % TIMEOUT) )
       message(deskTop, evBroadcast, cmDrawLine, 0);

}



void TMyApp::handleEvent(TEvent& event)
{
   TApplication::handleEvent( event );

   if( event.what == evCommand )
   {
      switch( event.message.command)
      {
   case cmDrawDialog:
		DrawDialog();
		break;

	 default:
		break;
      }
      clearEvent( event );
   }
}



TStatusLine *TMyApp::initStatusLine(TRect r)
{
   r.a.y = r.b.y - 1;

   return new TStatusLine( r,
       *new TStatusDef( 0, 0xFFFF) +
       *new TStatusItem( "~Alt-X~ Exit", kbAltX, cmQuit) +
       *new TStatusItem( "~Alt-D~ Draw Dialog", kbAltD, cmDrawDialog)
       );
}


TMenuBar *TMyApp::initMenuBar( TRect r )
{
	r.b.y = r.a.y + 1;

   return new TMenuBar( r,
	    *new TSubMenu( "~=~", kbAltSpace ) +
	    *new TMenuItem( "~D~raw Dialog", cmDrawDialog, kbAltD, hcNoContext, "Alt-D"));


}


int main()
{
   TMyApp myApp;
   myApp.run();
   return 0;
}
