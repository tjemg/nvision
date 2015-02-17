/* LISTBOX9.CPP:  Taking off the trainer wheels (rev 2)

   In this example we remove our browser from its ready-made dialog box
   and derive it from the more flexible TWindow class. In addition, we
   convert from the modal operation of previous examples to non-modal
   operation. We will try to do this in a stepwise fashion so that we
   can see the process clearly (at least I hope we will be able to!).

   Note that the changes required for each step have been noted with a
   "STEP x" annotation in the vicinity of the change(s).

   STEP 1:  Change TDialog to TWindow, change execView() to insert()
            and compile. Correct all compiler errors. Recompile and run.

   STEP 2:  Our listBox has a little problem; the flashing red color
            scheme doesn't do much for the esthetic and we can't close
            the window with the frame's close button. We need to supply
            a getPallete() for our listBox; the flashing white on red
            is Turbo Vision's way of telling us we have supplied one or
            more invalid color indici.  The close failure is occuring
            because our new TWindow based eventHandler() isn't respond-
            ing to broadcast messages. The TDialog base class did this
            for us; a TWindow class requires us instruct it to respond
            to broadcast messages.

   STEP 3:  Maybe not really a step but a true example of OOP's
            programming (pun intended)... we got the colors right for
            the list box but it looks like we will need to do the same
            thing for the two TInputline objects. This will require that
            we derive a minimal TInputLine class of our own: MyInputLine
            will provide our getPalette() function.

            NOTE:  If you can find it, there is a example source file
            called PALETTE.CPP (or AUTOPAL.CPP) which shows how to
            derive classes which are able to map their palette into
            any owner class palette automatically.

   STEP 4:  The parts of our browser aren't moving properly relative to
            each other... some adjustments to get the browser and its
            objects to adjust correctly to the new variable size browser
            window are are obviously necessary.

   Required files: LISTBOX9.CPP
                   HEAPVIEW.H, HEAPVIEW.CPP,
                   SFCOLL8.H, SFCOLL8.CPP

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

#include "sfcoll8.h"

#include "heapview.h"

__link(RFCollection);

const cmAbout   = 100;
const cmList    = 101;

const ifWidth  = 35;

// We have bumped up the number of records in our fake database so
// we can more easily see the limits of our browser.
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
                { "0-553-29664-7", "George Alec Effinger", "Exile Kiss, The", 0.0 },
                { "0-380-76284-6", "Tim Sullivan", "Lords of Creation, The", 4.9 },
                { "0-671-72080-5", "Fred Saberhagen", "Berserker Man, The", 4.7 },
                { "0-671-72080-5", "Michael Flynn", "Nanotech Chronicles, The", 6.1 },
                { "0-441-58616-3", "David Drake", "Northworld Justice", 4.7 },
                { "0-345-37574-2", "Alan Dean Foster", "Call to Arms, A", 4.8 },
                { "0-380-70911-2", "Lisa Mason", "Arachne", 5.3 },
                { "0-553-29342-7", "Issac Asimov", "Pebble in the Sky", 5.7 },
                { "0-441-02257-X", "S.N.Lewitt", "Cybernetic Jungle", 5.2 },
                { "0-812-54584-2", "L.E.Modesitt,Jr.", "Ecologic Envoy, The", 6.1 },
                { NULL, NULL, NULL } };


// The MyInputLine class and its getPallete() member function are part
// of STEP 3.

//////////////////////////////////////////// MyInputLine /////////////////
class MyInputLine : public TInputLine {
	public:
		MyInputLine(const TRect& bounds, int aMaxLen):
        TInputLine(bounds, aMaxLen) {}
    virtual TPalette& getPalette() const;
	protected:
	private:
};


/*========================================== TInputLine ==================
   getPalette -
*/

#define cpMyInputLine "\x07\x07\x07\x07"
TPalette& MyInputLine::getPalette() const {

  static TPalette palette(cpMyInputLine, sizeof(cpMyInputLine)-1);
  return palette;
}


//////////////////////////////////////////// TMyListBox //////////////////
class TMyListBox : public TListBox {
	public:
    TMyListBox(TRect& bounds, ushort num, TScrollBar *bar)
     :TListBox(bounds, num, bar) {}
    void handleEvent(TEvent&);
    void getText(char *, short, short);
    TPalette& TMyListBox::getPalette() const;             // STEP 2
    char *getISBN() { return ((SFCollection*)list())->at(focused)->cell[SFrecord::ISBN]; }

	protected:
	private:
};


//========================================== TMyListBox ==================
void TMyListBox::handleEvent(TEvent& event) {

  if (event.what == evMouseDown)
    if (event.mouse.doubleClick) {
      message(owner, evBroadcast, cmListItemSelected, this);
      clearEvent(event);
    }

  TListBox::handleEvent(event);
} // end of TMyListBox::handleEvent()


//========================================== TMyListBox ==================
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

                                                          // STEP 2
/*========================================== TMyListBox ==================
   getPalette -  remaps the 5 color entries of a TListBox from the 26th
                 thru 29th entries of the owner (which works OK for a
                 TDialog class object) into the 3rd thru 6th entries of
                 the owner (which works correctly for a TWindow class
                 object).
*/

#define cpMyListBox "\x03\x03\x04\x05\x06"
TPalette& TMyListBox::getPalette() const {

  static TPalette palette(cpMyListBox, sizeof(cpMyListBox)-1);
  return palette;
}


//////////////////////////////////////////// TMyDialog ///////////////////
class TMyDialog : public TWindow {                        // STEP1
	public:
    enum TAX { TITLE, AUTHOR, LIST };
    char *datafile;

		TMyDialog(TRect &trect, int cols, char *title);
    ~TMyDialog() {
      theCollection->freeAll();
      delete theCollection;
    }
    void handleEvent(TEvent &);
    virtual void sizeLimits(TPoint& min, TPoint& max);
    void deleteString();
    void findString(TAX);

  protected:
	private:
    short maxWinWidth;                                    // STEP 4
    short minWinHite;                                     // STEP 4
    TStaticText *legend[2];
    MyInputLine *inputLine[2];                            // STEP 3
    TMyListBox *listBox;
    TScrollBar *listScroller;
    SFCollection *theCollection;
};


//==========================================[ MyDialogBox ]===============
TMyDialog::TMyDialog(TRect &trect, int cols, char *title)
					:TWindow(trect, title, 1),                      // STEP 1
					 TWindowInit(TDialog::initFrame) {

  // The 'maxWinWidth' data member preserves the initial  // STEP 4
  // width of the window and the setLimit() member
  // function assures that the window can't vary from this width.
  // 'maxWinHite' is set to assure that a uparrow and downarrow are
  // always visible so we can always scroll the list.
  TRect r = getExtent();
  maxWinWidth = r.b.x-r.a.x;
  minWinHite = 8;

  if (!cols) cols = 1;
  datafile = "LISTBOX9.DAT";

  if (access(datafile, 0)) {
    theCollection = new SFCollection(100, 10);
    for (short i=0; SFbooks[i].author; i++) {
      SFrecord *sfentry = new SFrecord;
      sfentry->cell[SFrecord::TITLE] = newStr(SFbooks[i].title);
      sfentry->cell[SFrecord::AUTHOR] = newStr(SFbooks[i].author);
      sfentry->cell[SFrecord::ISBN] = newStr(SFbooks[i].isbn);
      theCollection->insert(sfentry);
    }
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

  // Keeps the the browser within the desktop border.
  dragMode = dmLimitLoY|dmLimitHiY|dmLimitHiX|dmLimitHiY; // STEP 4

  int lbwidth = ((trect.b.x - trect.a.x) - 3)/2;
  int lbhite = (trect.b.y - trect.a.y) - 1;

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

  // Permit the listbox and its scroller to grow and      // STEP 4
  // shrink vertically from the bottom and anchor them
  // at the top.
  listBox->growMode = gfGrowHiY|gfGrowRel;
  listScroller->growMode = gfGrowHiY|gfGrowRel;

  listBox->newList(theCollection);

  short theFocus = theCollection->set;
  theFocus = theFocus&setFocus;
  theFocus = theFocus>>2;
  listBox->focusItem(theFocus);

  // Now using the derived TInputLine demanded by color   // STEP 3
  // indexing.
  inputLine[TITLE] = new MyInputLine(TRect(trect.a.x+3,
                                          trect.a.y+3,
                                          trect.a.x+lbwidth+1,
                                          trect.a.y+4), ifWidth);
  inputLine[AUTHOR] = new MyInputLine(TRect(trect.a.x+lbwidth+1,
                                           trect.a.y+3,
                                           trect.a.x+(2*lbwidth)+1,
                                           trect.a.y+4), ifWidth);

  insert(listBox);
  insert(listScroller);

  for (short i=0; i<2; i++) {
    insert(legend[i]);
    insert(inputLine[i]);
  }

  short lastSelected = theCollection->set&setSelected;
  if (lastSelected >= LIST)
    listBox->makeFirst();
  else
    inputLine[lastSelected]->makeFirst();
} // end of MyDialogBox::MyDialogBox()


                                                         // STEP 4
//========================================== TMyDialog ===================
void TMyDialog::sizeLimits(TPoint& min, TPoint& max) {
  TWindow::sizeLimits(min, max);
  min.x = max.x = maxWinWidth;
  min.y = minWinHite;
}

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
                short exitSet = listBox->focused;
                exitSet = exitSet<<2;
                if (listBox->getState(sfSelected))
                  exitSet |= LIST;
                else
                  if (inputLine[TITLE]->getState(sfSelected))
                    exitSet |= TITLE;
                  else
                    exitSet |= AUTHOR;

                theCollection->set = exitSet;
                *lbs << theCollection;
              }

              delete lbs;
            }
            break;
        }
  }

	TWindow::handleEvent(event);                            // STEP 1
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
      case cmAbout:
        AboutDialog();
        clearEvent(event);
        break;
      case cmList:
        ListDialog();
        clearEvent(event);
        break;
    }
  }
} // end of TApp::handleEvent()


/*=========================================== TApp =======================
   AboutDialog - create modal About dialog box
*/

void TApp::AboutDialog() {

  TDialog *pd = new TDialog(TRect(0, 0, 35, 12), "About");
  if (pd) {
    pd->options |= ofCentered;
    pd->insert(new TStaticText(TRect(1, 2, 34, 7),
               "\003TListBox Example 9\n"
               "\003Taking of the trainer wheels\n"
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

  if (pd) {

    // Make our TWindow based class respond to broadcast messages.
    pd->eventMask |= evBroadcast;                         //  STEP 2

    // We are now modeless!
    if (validView(pd)) deskTop->insert(pd);               //  STEP 1
  }
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
