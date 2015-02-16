/* LISTBOX3.CPP:  Interactive update of a list box

   This sample code derives a Dialog box containing a TListBox and a
   TInputLine and has an eventHandler() that demonstrates a method for
   overriding and/or supplementing the standard interface behavior of
   a Turbo Vision application:

      - Pressing [Enter] with the input line focused puts the string
        in the input line (if any) into the list

      - Pressing [downarrow] with the input line focused moves to the
        first (upper-most) item in the list box and gives it focus.

      - Pressing [uparrow] with the first (upper-most) list box item
        focused moves focus to the input line.

      - Pressing [Del]ete with a list box item focused deletes the
        focused list box item.

   All other control behavior is the behavior defined for the standard
   Turbo Vision classes.

   08-24-92:  Corrected coding error which caused assertion error on
              delete of list item. EAS
	05-26-92:  Fixed a mem leak in TMyDialog. BBK
  ------------------------------------------------------------------------
   The following example routines have been provided by the Technical
   Support staff at Borland International.  They are provided as a
   courtesy and not as part of a Borland product, and as such, are
   provided without the assurance of technical support or any specific
   guarantees.

  ========================================================================
    Copyright (c) 1991 by Borland International
    All Rights Reserved.
*/

#include <string.h>             // strlen()
#include <stdlib.h>

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
#define Uses_TInputLine
#define Uses_TScrollBar
#define Uses_TStaticText
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TStatusLine
#define Uses_TStringCollection
#include <tv.h>

#include "heapview.h"

// A NULL terminated list of strings is now required by TMyDialog
char *theList[] = { "Ecologic Envoy, The",
                    "Squares of the City, The",
                    "Worthing Saga, The",
                    "Arachne", NULL };

const int cmAbout   = 100;  // User selected menu item 'About'
const int cmList    = 101;  // User selected menu item 'List'

int ifWidth     = 35;   // input line width


//////////////////////////////////////////// TMyDialog ///////////////////
class TMyDialog : public TDialog {
	public:
		TMyDialog(TRect trect, int cols, char *title, char *sarray[]);
    ~TMyDialog() {
      destroy(theCollection);
    }
    void handleEvent(TEvent &);
    void deleteString();
    void addString();

	private:
    TInputLine *inputLine;
    TListBox *listBox;
    TScrollBar *listScroller;
    TStringCollection *theCollection;
};


//==========================================[ MyDialogBox ]===============
TMyDialog::TMyDialog(TRect trect, int cols, char *title, char *sarray[])
					:TDialog(trect, title),
					 TWindowInit(TDialog::initFrame) {

  // Must have at least one column!
  if (!cols) cols = 1;

  // Build a collection to pass to the listbox. Note that in this
  // example our constructor expects a NULL terminated array. This
  // change allowed us to eliminate the need to pass the number of
  // elements in the array to the constructor as was required in the
  // previous list box examples.
  theCollection = new TStringCollection(100, 10);
  char *listItem;
  for (int i=0; sarray[i]; i++) {
    listItem = newStr(sarray[i]);
    theCollection->insert(listItem);
  }

  options |= ofCentered;

  int lbwidth = (trect.b.x - trect.a.x) - 3;
  int lbhite = (trect.b.y - trect.a.y) - 2;

  // If more than 1 column requested, create a horizontal scroll bar
  // else create a vertical scroll bar for the list box. This change
  // permits us a reasonably natural way to use TMyDialog to create
  // dialog boxes with either a horizontal or vertical scroll bar
  // without editing the constructor.
  if (cols > 1)
    listScroller = new TScrollBar(TRect(trect.a.x+3, trect.a.y+lbhite,
                                  trect.a.x+lbwidth, trect.a.y+lbhite+1));
  else
    listScroller = new TScrollBar(TRect(trect.a.x+lbwidth, trect.a.y+3,
                                  trect.a.x+lbwidth+1, trect.a.y+lbhite));

  // Create a list box with 'cols' columns and an associated scroller.
  listBox = new TListBox(TRect(trect.a.x+3, trect.a.y+3,
                               trect.a.x+lbwidth, trect.a.y+lbhite),
                               cols, listScroller);

  // Associate the collection with the list box.
  listBox->newList(theCollection);

  // Create input line.
  inputLine = new TInputLine(TRect(trect.a.x+3, trect.a.y+2,
                                   trect.a.x+lbwidth+1, trect.a.y+3),
                                   ifWidth);

  // Insert list box, its scroller and the input line into dialog box.
  insert(listBox);
  insert(listScroller);
  insert(inputLine);
} // end of MyDialogBox::MyDialogBox()


/*========================================== MyDialogBox =================
	 handleEvent -
*/

void TMyDialog::handleEvent(TEvent &event) {

	switch(event.what) {
    case evKeyDown:
		  switch(event.keyDown.keyCode) {

        // [Enter] with input line focused puts string in list.
			  case kbEnter:
          if (inputLine->state&sfSelected) {
			      addString();
			      clearEvent(event);
          }
			    break;

        // [Delete] deletes focused data
			  case kbDel:
          if (listBox->state&sfSelected) {
				    deleteString();
				    clearEvent(event);
          }
				  break;

        // Down arrow while in line input editor moves into listbox.
			  case kbDown:
          if (inputLine->state&sfSelected) {
            listBox->focusItem(0);
            event.keyDown.keyCode = kbTab;
          }
				  break;

        // Up arrow when top entry of listbox has focus moves into
        // line input editor.
			  case kbUp:
          if (listBox->focused == 0 && listBox->state&sfSelected)
            event.keyDown.keyCode = kbTab;
				  break;
      }
	}

	// Let TDialog handler do it's thing with any remaining events.
	TDialog::handleEvent(event);
	clearEvent(event);								// Dood it so dump command.
} // end of MyDialogBox::eventHandler()


/*========================================== TMyDialog ===================
	 deleteString - Pressing [Delete] while a list box item is focused
                  deletes that item from the list.
*/

void TMyDialog::deleteString() {

  // If there are any items in collection...
	if (theCollection->getCount()) {

    // delete the currently focused item.
		theCollection->atFree(listBox->focused);
		listBox->setRange(theCollection->getCount());
		listBox->drawView();
	}
}


/*========================================== TMyDialog ===================
	 addString - Pressing Enter with the input line focused adds the
               string (if any) on the input line to the list box. The
               input line is then cleared.
*/

void TMyDialog::addString() {

	char *temp = new char[ifWidth+1];
	inputLine->getData(temp);

  // If there's anything to transfer to theCollection...
	if (strlen(temp)) {

    // ...transfer it.
		theCollection->insert(temp);
		listBox->setRange(theCollection->getCount());
		listBox->drawView();

    // Clear input line buffer, display & re-select.
		*inputLine->data='\0';
    inputLine->draw();
		inputLine->selectAll(True);
	}
  else

    // If nothing to insert, throw away the allocation.
    delete temp;
} // end of TMyDialog::addString()


//////////////////////////////////////////// TApp ////////////////////////
class TApp : public TApplication {
  public:
    TApp();
    static TMenuBar *initMenuBar(TRect r);
    void handleEvent(TEvent &event);
    void idle();

    void AboutDialog();
    void ListDialog();

    THeapView *heap;
};


//==========================================[ TApp ]======================
TApp::TApp() : TProgInit(TApplication::initStatusLine,
                         TApp::initMenuBar,
                         TApplication::initDeskTop) {

  // Create heap monitor and place on status line
  TRect r = getExtent();
  r.a.x = r.b.x - 13;
  r.a.y = r.b.y - 1;
  heap = new THeapView(r);
  insert(heap);
}


//=========================================== TApp =======================
TMenuBar *TApp::initMenuBar(TRect r) {

  r.b.y = r.a.y + 1;
  return(new TMenuBar(r, new TMenu(
    *new TMenuItem("~A~bout", cmAbout, kbAltA, hcNoContext, 0,
      new TMenuItem("~L~ist", cmList, kbAltL, hcNoContext, 0)))));
}


//=========================================== TApp =======================
void TApp::handleEvent(TEvent &event) {

  TApplication::handleEvent(event);
  if (event.what == evCommand) {
    switch(event.message.command) {
      case cmAbout:                     // display the about box
        AboutDialog();
        clearEvent(event);
        break;
      case cmList:                      // display our list box
        ListDialog();
        clearEvent(event);
        break;
    }
  }
}


/*=========================================== TApp =======================
   AboutDialog - create modal About dialog box
*/

void TApp::AboutDialog() {

  // the About box reminds the user what is being demonstrated
  TDialog *pd = new TDialog(TRect(0, 0, 35, 12), "About");
  if (pd) {
    pd->options |= ofCentered;
    pd->insert(new TStaticText(TRect(1, 2, 34, 7),
               "\003TListbox Example 3\n"
               "\003Interactive update\n"
               "\003Borland Technical Support"));
    pd->insert(new TButton(TRect(3, 9, 32, 11), "~O~k", cmOK, bfDefault));
        deskTop->execView(pd);
  }
  destroy(pd);
}


/*=========================================== TApp =======================
   ListDialog - display a dialog box containing a listbox
*/

void TApp::ListDialog() {

  TMyDialog *pd = new TMyDialog(TRect(0, 0, 35, 15),
                                1, "SF List", theList);

  // if the object was safely built, exec it as modal
  if (validView(pd)) deskTop->execView(pd);

  // and a little clean up
  destroy(pd);
}


/*========================================== TApp ====================
   idle - handles updating the heap monitor when the application
          is not doing anything else.

     REQUIRES:
     RETURNS:
*/

void TApp::idle() {

  TProgram::idle();
  heap->update();
}


//************************************************************************
int main(void) {

  // Your standard Turbo Vision main().
  TApp myApp;
  myApp.run();
  return 0;
}
