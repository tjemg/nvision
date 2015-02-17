/* LISTBOX8.CPP:  Adding data members to a collection (rev 2)

   In this example it was decided to save the currently selected view
   and the currently selected item in the list box when a browser is
   closed and to restore these when the browser is re-opened. To
   accomplish this we add a data member to our SFCollection class and a
   few lines of code to MyDialog's constructor and event handler to save
   and restore these states. More importantly, the addition of a data
   member to our SFcollection class means that it will be necessary to
   implement the read() and write() SFCollection member functions.

   We also convert the two TInputLine objects 'lauthor' and 'ltitle'
   and the author, title and isbn data members of the SFrecord structure
   into arrays. This will simplify some operations in our code.

   This example requires SFCOLL8.H and SFCOLL8.CPP.
   If using the IDE, it also requires a project file (LISTBOX8.PRJ)
   listing LISTBOX.CPP and SFCOLL8.CPP.

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
#include <io.h>                 // access()
#include <iomanip.h>
#include <strstream.h>          // ostrstream()

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
#define Uses_MsgBox
#define Uses_fpstream
#define Uses_ipstream
#define Uses_TScreen
#include <tv.h>

// SFCollection class with read() and write() functions implemented
#include "sfcoll8.h"

#include "heapview.h"

__link(RFCollection);

const cmAbout   = 100;
const cmList    = 101;

const ifWidth  = 35;

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
    TMyListBox(TRect& bounds, ushort num, TScrollBar *bar)
     :TListBox(bounds, num, bar) {}
    void handleEvent(TEvent&);
    void getText(char *, short, short);
    char *getISBN() { return ((SFCollection*)list())->at(focused)->cell[SFrecord::ISBN]; }

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


/*========================================== TMyListBox ==================
   getText - composes listbox entry line by concatenting the author and
             title; also inserts a double vertical line between them
             in the list box display.
*/

void TMyListBox::getText(char *Text, short item, short maxlen) {

  cout.setf(ios::right);
  ostrstream(Text, maxlen-1)
             << setiosflags(ios::left)
             << setw((maxlen/2)-3)
             << ((SFCollection*)list())->at(item)->cell[SFrecord::TITLE]
             << " º "
             << ((SFCollection*)list())->at(item)->cell[SFrecord::AUTHOR]
             << ends;
}


//////////////////////////////////////////// TMyDialog ///////////////////
class TMyDialog : public TDialog {
	public:
    enum TAX { TITLE, AUTHOR, LIST };
    char *datafile;

		TMyDialog(TRect &trect, int cols, char *title);
    ~TMyDialog() {
      theCollection->freeAll();
      delete theCollection;
    }
    void handleEvent(TEvent &);
    void deleteString();
    void findString(TAX);

  protected:
	private:
    TStaticText *legend[2];         // for title and author legends
    TInputLine *inputLine[2];       // for title and author input lines
    TMyListBox *listBox;
    TScrollBar *listScroller;
    SFCollection *theCollection;
};


//==========================================[ TMyDialog ]=================
TMyDialog::TMyDialog(TRect &trect, int cols, char *title)
					:TDialog(trect, title),
					 TWindowInit(TDialog::initFrame) {

  if (!cols) cols = 1;
  datafile = "LISTBOX8.DAT";

  if (access(datafile, 0)) {
    theCollection = new SFCollection(100, 10);
    for (short i=0; SFbooks[i].author; i++) {
      SFrecord *sfentry = new SFrecord;
      sfentry->cell[SFrecord::TITLE] = newStr(SFbooks[i].title);
      sfentry->cell[SFrecord::AUTHOR] = newStr(SFbooks[i].author);
      sfentry->cell[SFrecord::ISBN] = newStr(SFbooks[i].isbn);
      theCollection->insert(sfentry);
    }

    // Select title input line, collection item 0 focused
    theCollection->set = TITLE;
  }
  else {
    fpstream *lbs = new fpstream(datafile, ios::in|
                                           ios::binary|
                                           ios::nocreate);
    if (lbs->good()) {
      *lbs >> theCollection;
      delete lbs;
    }
  }

  options |= ofCentered;

  int lbwidth = ((trect.b.x - trect.a.x) - 3)/2;
  int lbhite = (trect.b.y - trect.a.y) - 2;

  legend[TITLE] = new TStaticText(TRect(trect.a.x+6,
                                        trect.a.y+2,
                                        trect.a.x+12,
                                        trect.a.y+3), "Title");

  legend[AUTHOR] = new TStaticText(TRect(trect.a.x+2+ifWidth,
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

  // To get the focused listbox item on the last closure we...
  short theFocus = theCollection->set;

  // ...strip low-order 2 bits (which contain the selected object
  // on last closure) and...
  theFocus = theFocus&setFocus;

  // ...shift the upper 14 bits left two bits.
  theFocus = theFocus>>2;

  // Make this the focused item in the listbox.
  listBox->focusItem(theFocus);

  inputLine[TITLE] = new TInputLine(TRect(trect.a.x+3,
                                          trect.a.y+3,
                                          trect.a.x+lbwidth+1,
                                          trect.a.y+4), ifWidth);

  inputLine[AUTHOR] = new TInputLine(TRect(trect.a.x+lbwidth+1,
                                           trect.a.y+3,
                                           trect.a.x+(2*lbwidth)+1,
                                           trect.a.y+4), ifWidth);

  insert(listBox);
  insert(listScroller);

  // Insert title and author input fields.
  for (short i=0; i<2; i++) {
    insert(legend[i]);
    insert(inputLine[i]);
  }

  // To get the object that was selected on last closure we strip
  // high-order 14 bits containing the focused list box item on
  // last closure.
  short lastSelected = theCollection->set&setSelected;

  // Restore selected (or Title input line if no last selected)
  // view on last closure.

  // If list box was selected make it selected now...
  if (lastSelected >= LIST)
    listBox->makeFirst();
  else

    // ...else must have been an input line.
    inputLine[lastSelected]->makeFirst();
} // end of MyDialogBox::MyDialogBox()


/*========================================== TMyDialog ===================
	 handleEvent -
*/

void TMyDialog::handleEvent(TEvent &event) {

	switch(event.what) {
    case evKeyDown:
		  switch(event.keyDown.keyCode) {
			  case kbEnter:
          if (inputLine[TITLE]->state&sfSelected) {
			      findString(TITLE);
			      clearEvent(event);
          }
          if (inputLine[AUTHOR]->state&sfSelected) {
			      findString(AUTHOR);
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
          if (inputLine[TITLE]->state&sfSelected ||
              inputLine[AUTHOR]->state&sfSelected) {
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

            ostrstream(message, 40) << "\003ISBN: "
                                     << listBox->getISBN() << ends;

            messageBox(message, mfOKButton|mfInformation);
            clearEvent(event);
            break;
        }

      case evCommand:
        switch(event.message.command) {
          case cmClose:
            if (theCollection->getCount()) {
              fpstream *lbs = new fpstream(datafile, ios::out|
                                                     ios::binary|
                                                     ios::trunc);
              if (!lbs) {
                messageBox(mfOKButton|mfError, "%s open failure",
                                               datafile);
                ::delete(datafile);
              }
              else {

                // Compose the 'set' value.  Upper 14 bits is the focused
                // item in the listbox.
                short exitSet = listBox->focused;
                exitSet = exitSet<<2;

                // Lower 2 bits is the selected object (title input line
                // = 00, author input line = 01, listbox = 11);
                if (listBox->getState(sfSelected))
                  exitSet |= LIST;
                else
                  if (inputLine[TITLE]->getState(sfSelected))
                    exitSet |= TITLE;
                  else
                    exitSet |= AUTHOR;

                // Load set data member and...
                theCollection->set = exitSet;

                // ...save sf collection.
                *lbs << theCollection;
              }

              delete lbs;
            }
            break;
        }
  }

	TDialog::handleEvent(event);
	clearEvent(event);
} // end of TMyDialog::eventHandler()


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
                authors are searched. If no string appears, we just
                return.
*/

void TMyDialog::findString(TAX x) {
	char temp[ifWidth+1];

  inputLine[x]->getData(temp);
  *inputLine[x]->data='\0';
  inputLine[x]->draw();
  inputLine[x]->selectAll(True);

  if (!strlen(temp)) return;

  for (short i=0; i < theCollection->getCount(); i++)
    if (!strcmpi(((SFCollection*)listBox->list())->at(i)->cell[x], temp))
      break;

  char message[133];
  if (i < theCollection->getCount()) {
    ostrstream(message, 133)
         << " AUTHOR: "
         << ((SFCollection*)listBox->list())->at(i)->cell[SFrecord::AUTHOR]
         << endl
         << " TITLE:  "
         << ((SFCollection*)listBox->list())->at(i)->cell[SFrecord::TITLE]
         << endl
         << " ISBN:   "
         << ((SFCollection*)listBox->list())->at(i)->cell[SFrecord::ISBN]
         << ends;
    listBox->focusItem(i);
  }
  else
    ostrstream(message, 133) << "\"" << temp << "\" not found." << ends;

  messageBox(message, mfOKButton|mfInformation);
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

  TDialog *pd = new TDialog(TRect(0, 0, 35, 12), "About");
  if (pd) {
    pd->options |= ofCentered;
    pd->insert(new TStaticText(TRect(1, 2, 34, 7),
               "\003TListBox Example 8\n"
               "\003Adding collection data members\n"
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

  TMyDialog *pd = new TMyDialog(TRect(0, 0, 70, 17), 1, "SF Browser");

  if (validView(pd)) deskTop->execView(pd);
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

  TApp myApp;
  myApp.run();
  return 0;
}
