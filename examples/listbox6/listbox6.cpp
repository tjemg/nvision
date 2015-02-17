/* LISTBOX6.CPP:  A trivial database browser (rev 2)

   In this example we modify the TListBox so that both the title and
   author are displayed in the list box object. In addition we modify
   MyDialog's eventHandler so an [Enter] with a string in either the
   Title or Author input line will search the respective elements of
   our SFCollection for a match and display the author, title and ISBN
   number if a match is found.

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

#include <stdlib.h>
#include <stdio.h>              // NULL
#include <string.h>             // strlen()

#include <iomanip>
#include <sstream>          // ostrstream()

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
#define Uses_MsgBox
#define Uses_TScreen
#include <tv.h>

// This supplies the user-implemented TCollection SFCollection
#include "sfcoll6.h"

#include "heapview.h"

const int cmAbout   = 100;  // User selected menu item 'About'
const int cmList    = 101;  // User selected menu item 'List'

const int ifWidth  = 35;   // input line width

// NULL terminated array of 'books'.
struct books {
  char *isbn,
       *author,
       *title;
  double rating;
} SFbooks[] = { { "0-451-45132-5", "John Brunner", "Squares of the City, The", 9.9 },
                { "0-441-90871-3", "Thomas A. Easton", "Woodsman", 0.0 },
                { "0-02-017511-6", "Will Bradley", "Ark Liberty", 0.0 },
                { "0-446-36255-7", "Jane E. Fancher", "Uplink", 0.0 },
                { "0-441-69348-2", "Jack L. Chalker", "Run to Chaos Keep", 4.2 },
                { "0-553-29224-2", "Iain M. Banks", "Use of Weapons", 0.0 },
                { "0-553-29362-1", "Robert Reed", "Remarkables, The", 0.0 },
                { "0-451-16014-2", "Jonathan Littell", "Bad Voltage", 2.7 },
                { "0-523-48510-7", "Poul Anderson", "Guardian of Time, The", 5.0 },
                { "0-812-52579-5", "Fred Saberhagen", "Specimens", 5.7 },
                { "0-812-50927-7", "Orson Scott Card", "Worthing Saga, The", 7.3 },
                { "0-671-72080-5", "Michael Flynn", "Nanotech Chronicles, The", 6.1 },
                { NULL, NULL, NULL } };

//////////////////////////////////////////// TMyListBox //////////////////
class TMyListBox : public TListBox {
	public:
    TMyListBox(TRect bounds, ushort num, TScrollBar *bar)
     :TListBox(bounds, num, bar) {}
    void handleEvent(TEvent&);
    void getText(char *, ccIndex , short);
    char *getISBN() { return ((SFCollection*)list())->at(focused)->isbn; }

	protected:
	private:
};


/*========================================== TMyListBox ==================
   handleEvent - because the TListBox class isn't broadcasting item
                 selection, we supply the necessary functionality in our
                 derivitive list box event handler.
*/

void TMyListBox::handleEvent(TEvent& event) {

  if (event.what == evMouseDown)
    if (event.mouse.doubleClick) {
      message(owner, evBroadcast, cmListItemSelected, this);
      clearEvent(event);
    }

  TListBox::handleEvent(event);
} // end of TMyListBox::handleEvent()


/*========================================== MyListBox =================
   getText - composes listbox entry line by concatenting the author and
             title.
*/

void TMyListBox::getText(char *Text, ccIndex item, short maxlen) {

    std::cout.setf(std::ios::right);
    std::stringstream ss;
   ss << std::ios::left
             //<< std::setiosflags(std::ios::left)
             << std::setw((maxlen/2)-3)
             << ((SFCollection*)list())->at(item)->title << " : "
             << ((SFCollection*)list())->at(item)->author << std::ends;
    strcpy(Text,ss.str().c_str());
}


//////////////////////////////////////////// TMyDialog ///////////////////
class TMyDialog : public TDialog {
	public:
    enum TAX { TITLE, AUTHOR };
		TMyDialog(TRect trect, int cols, char *title);//, books **sfb);
    ~TMyDialog() {
      theCollection->freeAll();
      delete theCollection;
    }
    void handleEvent(TEvent &);
    void deleteString();
    void findString(TAX);

  protected:
	private:
    TStaticText *ltitle;
    TStaticText *lauthor;
    TInputLine *inputLine1;
    TInputLine *inputLine2;
    TMyListBox *listBox;                // for title
    TScrollBar *listScroller;
    SFCollection *theCollection;
};


//==========================================[ MyDialogBox ]===============
TMyDialog::TMyDialog(TRect trect, int cols, char *title)
					:TDialog(trect, title),
					 TWindowInit(TDialog::initFrame) {

  if (!cols) cols = 1;

  theCollection = new SFCollection(100, 10);
  for (short i=0; SFbooks[i].author; i++) {
    SFrecord *sfentry = new SFrecord;
    sfentry->title = newStr(SFbooks[i].title);
    sfentry->author = newStr(SFbooks[i].author);
    sfentry->isbn = newStr(SFbooks[i].isbn);      // *
    theCollection->insert(sfentry);
  }

  options |= ofCentered;

  int lbwidth = ((trect.b.x - trect.a.x) - 3)/2;
  int lbhite = (trect.b.y - trect.a.y) - 2;

  ltitle = new TStaticText(TRect(trect.a.x+6,
                                 trect.a.y+2,
                                 trect.a.x+12,
                                 trect.a.y+3), "Title");

  lauthor = new TStaticText(TRect(trect.a.x+2+ifWidth,
                                  trect.a.y+2,
                                  trect.a.x+2+ifWidth+15,
                                  trect.a.y+3), "Author");

  if (cols > 1)
    listScroller = new TScrollBar(TRect(trect.a.x+3,
                                        trect.a.y+lbhite,
                                        trect.a.x+(2*lbwidth),
                                        trect.a.y+lbhite+1));
  else
    listScroller = new TScrollBar(TRect(trect.a.x+(2*lbwidth),
                                        trect.a.y+4,
                                        trect.a.x+(2*lbwidth)+1,
                                        trect.a.y+lbhite));

  listBox = new TMyListBox(TRect(trect.a.x+3, trect.a.y+4,
                                 trect.a.x+(2*lbwidth),
                                 trect.a.y+lbhite), cols, listScroller);

  listBox->newList(theCollection);

  inputLine1 = new TInputLine(TRect(trect.a.x+3, trect.a.y+3,
                                   trect.a.x+lbwidth+1, trect.a.y+4),
                                   ifWidth);

  inputLine2 = new TInputLine(TRect(trect.a.x+lbwidth+1, trect.a.y+3,
                                   trect.a.x+(2*lbwidth)+1, trect.a.y+4),
                                   ifWidth);

  insert(ltitle);
  insert(lauthor);
  insert(listBox);
  insert(listScroller);
  insert(inputLine2);
  insert(inputLine1);
} // end of MyDialogBox::MyDialogBox()


/*========================================== MyDialogBox =================
	 handleEvent -
*/

void TMyDialog::handleEvent(TEvent &event) {

	switch(event.what) {
    case evKeyDown:
		  switch(event.keyDown.keyCode) {

        // [Enter] with input line focused checks for string in list,
        // reports "not found" if not in list else displays author,
        // title and ISBN of first entry that matches.
			  case kbEnter:
          if (inputLine1->state&sfSelected) {
			      findString(TMyDialog::TITLE);
			      clearEvent(event);
          }
          if (inputLine2->state&sfSelected) {
			      findString(TMyDialog::AUTHOR);
			      clearEvent(event);
          }
			    break;

			  case kbDel:
          if (listBox->state&sfSelected && theCollection->getCount()) {
				    deleteString();
				    clearEvent(event);
          }
				  break;

			  case kbDown:
          if (inputLine1->state&sfSelected ||
              inputLine2->state&sfSelected) {
            listBox->focusItem(0);
            event.keyDown.keyCode = kbTab;
          }
				  break;

			  case kbUp:
          if (listBox->focused == 0 && listBox->state&sfSelected)
            event.keyDown.keyCode = kbTab;
				  break;
      }

      case evBroadcast:
        switch (event.message.command) {
          case cmListItemSelected:
            char message[40];
            std::stringstream msg;

            msg << "\003ISBN: " << std::string(listBox->getISBN()) << std::ends;

            messageBox(msg.str().c_str(), mfOKButton|mfInformation);
            clearEvent(event);
        }
  }

	TDialog::handleEvent(event);
} // end of MyDialogBox::eventHandler()


/*========================================== TMyDialog ===================
	 deleteString - Pressing [Delete] while a list box item is focused
                  deletes that item from the list.
*/

void TMyDialog::deleteString() {

	if (theCollection->getCount()) {
		theCollection->atFree(listBox->focused);
		listBox->setRange(theCollection->getCount());
		listBox->drawView();
	}
}


/*========================================== TMyDialog ===================
	 findString - Pressing Enter with an input line focused searches list
                for matching author or title and reports associated ISBN
                author and title information in an information box. If
                [Enter] pressed in title input line, titles are searched
                for string; if [Enter] pressed while in author input line
                authors are searched. On successful find, sets focus to
                found list box item.
*/

void TMyDialog::findString(TAX x) {
	char temp[ifWidth+1];
  short i;

  // If doing a title search, get title string then clear line and
  // refocus else do same for author string.
  if (x) {
      inputLine2->getData(temp);
      inputLine2->setData((void*)"");
      inputLine2->draw();
      inputLine2->selectAll(True);
  }
  else {
      inputLine1->getData(temp);
      inputLine1->setData((void*)"");
      inputLine1->draw();
      inputLine1->selectAll(True);
  }

  // If there's no string just return immediately.
  if (!strlen(temp)) return;

  // If string from title input line search titles for match...
  if (x) {
    for (i=0; i < theCollection->getCount(); i++)
      if (0== (strcmp(((SFCollection*)listBox->list())->at(i)->author, temp)))
        break;
  }
  else {

    // ...else search authors.
    for (i=0; i < theCollection->getCount(); i++)
      if (0== (strcmp(((SFCollection*)listBox->list())->at(i)->title, temp)))
        break;
  }

  // Make a composition buffer for search results message.
  char message[133];
  std::stringstream ss;

  // If we found a match...
  if (i < theCollection->getCount()) {

      // ...compose entry message else...
      ss << " AUTHOR: "
          << ((SFCollection*)listBox->list())->at(i)->author
          << std::endl
          << " TITLE:  "
          << ((SFCollection*)listBox->list())->at(i)->title
          << std::endl
          << " ISBN:   "
          << ((SFCollection*)listBox->list())->at(i)->isbn
          << std::ends;
      listBox->focusItem(i);
  }
  else {
      // ...indicate that entry not found.
      ss << "\"" << temp << "\" not found." << std::ends;
  }

  // Now say what's true.
  messageBox(ss.str().c_str(), mfOKButton|mfInformation);
} // end of TMyDialog::findString()


//////////////////////////////////////////// TApp ////////////////////////
class TApp : public TApplication {
  public:
    TApp();
    static TMenuBar *initMenuBar(TRect r);
    void handleEvent(TEvent &event);
    void idle();

    void AboutDialog();
    void ListDialog();

  protected:
    THeapView *heap;
  private:
};


//==========================================[ TApp ]======================
TApp::TApp() : TProgInit(TApplication::initStatusLine,
                         TApp::initMenuBar,
                         TApplication::initDeskTop) {

  TScreen::checkSnow = False;

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
               "\003TListBox Example 6\n"
               "\003User Collections Part 2\n"
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

  TMyDialog *pd = new TMyDialog(TRect(0, 0, 70, 17),
                                1, "SF List"); //, SFbooks);

  // If the object was safely built, so modal execution.
  if (validView(pd)) deskTop->execView(pd);

  // Weeere baaaack... kill the dialog box and all its components.
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
