/* LISTBOX5.CPP:  TListBox and user-implemented collections (rev 2)

   This example demonstrates the use of a list box to access a collect-
   ion of structures. SFCollection is a collection of SFrecord structure
   entries. SFrecord structures have both 'title' and 'author' data
   members so that we can store a book's author as well as it's title.

   To accomdate this user-implemnted-collection we extend the MyListBox
   class introduced in LISTBOX4.CPP by supplying a getText() member
   function that knows how to access the elements of a SFrecord entry
   in our SFCollection. Since it is the getText() member function that
   a TListBox entity uses to display the items in a TListBox object,
   supplying a getText() member function is like putting tires on a
   Porche.

   This example requires SFCOLL5.H.

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

#include <stdio.h>              // NULL
#include <stdlib.h>
#include <string.h>             // strlen()
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
#include "sfcoll5.h"

#include "heapview.h"

const int cmAbout  = 100;  // User selected menu item 'About'
const int cmList   = 101;  // User selected menu item 'List'

const int ifWidth  = 35;   // input line width


// NULL terminated array of 'books'. This might represent an outside
// database (such as Paradox) which is being accessed through a library
// of C functions (such as the Paradox Engine) from a Turbo Vision app.
// In this example we are only going to use the author and title.
struct books {
    char *isbn,
         *author,
         *title;
    double rating;
} SFbooks[] = {
    { "0-451-45132-5", "John Brunner", "Squares of the City, The", 9.9 },
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
    { NULL, NULL, NULL }
};

//////////////////////////////////////////// TMyListBox //////////////////
class TMyListBox : public TListBox {
    public:
        enum TAX { TITLE, AUTHOR };

        // Minor modification to initialize new TITLE/AUTHOR display switch
        TMyListBox(TRect bounds, ushort num, TScrollBar *bar)
            :TListBox(bounds, num, bar), tax(TITLE) {};
        void handleEvent(TEvent&);
            void getText(char *, ccIndex, short);

        // This function permits us to change displayed field of record.
        void setax(TAX taxit) { tax = taxit; }

    protected:
    private:
        int tax;                            // title/author switch
};


/*========================================== TMyListBox ==================
  handleEvent - because the TListBox class isn't broadcasting item
  selection, we supply the necessary functionality in our
  derivitive list box event handler.
  */

void TMyListBox::handleEvent(TEvent& event) {

    if ((event.what == evMouseDown) && (event.mouse.doubleClick)) {
        message(owner, evBroadcast, cmListItemSelected, this);
        clearEvent(event);
    }

    TListBox::handleEvent(event);
} // end of TMyListBox::handleEvent()


/*========================================== TMyListBox ==================
  getText - gets author or title from currently focused SFrecord entry;
  if 'tax' data member is non-zero gets title else gets
  author.

  getText() is a virtual TListBox member function which must always be
  supplied when we choose to deviate from handling simple collections
  of strings.

  Note that we had to cast the return from the TListBox list() member
  function to our collection type so that the compiler will know how
  we have structured the collection's entries.
  */

void TMyListBox::getText(char *Text, ccIndex item, short) {
    if (TITLE==tax) {
        strcpy(Text, ((SFCollection*)list())->at(item)->title);
    } else {
        strcpy(Text, ((SFCollection*)list())->at(item)->author);
    }
}


//////////////////////////////////////////// TMyDialog ///////////////////
class TMyDialog : public TDialog {
    public:
        TMyDialog(TRect trect, int cols, char *title);

        // We are now using dynamically allocated memory to hold a copy of
        // the title and author (via calls to newStr() in the TMyDialog
        // constructor; consequently, we need to add a call to
        // 'theCollection's freeAll() to release those allocations before
        // we release the container itself. Note that we are actually calling
        // TNSCollection's freeAll() since we didn't implement one in
        // SFCOLL5.H... however, TNSCollection ends up calling the VIRTUAL
        // member function freeItem() of the invoking instance if it is
        // implemented... and it is; SFCollection:freeItem() does a
        //                delete (SFrecord*)item;
        //                             which invokes the destructor of each
        // SFrecord which, in turn, deletes the title and author allocations.
        ~TMyDialog() {
            theCollection->freeAll();
            delete theCollection;
        }
        void handleEvent(TEvent &);
        void deleteString();
        void addString();

    protected:
    private:
        TInputLine   *inputLine;
        TMyListBox   *listBox;
        TScrollBar   *listScroller;
        SFCollection *theCollection;        // Use our collection
};


//==========================================[ TMyDialog ]=================
TMyDialog::TMyDialog(TRect trect, int cols, char *title)
    :TDialog(trect, title),
    TWindowInit(TDialog::initFrame) {

        if (!cols) cols = 1;

        theCollection = new SFCollection(100, 10);

        // We are now dynamically allocating memory for copies of the
        // data from the books array. Pointers to these dynamically allocated
        // SFrecord entries are then inserted into the SFCollection rather
        // than pointers to the source data.
        for (short i=0; SFbooks[i].author; i++) {
            SFrecord *sfentry = new SFrecord;
            sfentry->title  = newStr(SFbooks[i].title);
            sfentry->author = newStr(SFbooks[i].author);
            theCollection->insert(sfentry);
        }

        options |= ofCentered;

        int lbwidth = (trect.b.x - trect.a.x) - 3;
        int lbhite = (trect.b.y - trect.a.y) - 2;

        if (cols > 1)
            listScroller = new TScrollBar(TRect(trect.a.x+3, trect.a.y+lbhite, trect.a.x+lbwidth, trect.a.y+lbhite+1));
        else
            listScroller = new TScrollBar(TRect(trect.a.x+lbwidth, trect.a.y+3, trect.a.x+lbwidth+1, trect.a.y+lbhite));

        listBox = new TMyListBox(TRect(trect.a.x+3, trect.a.y+3, trect.a.x+lbwidth, trect.a.y+lbhite), cols, listScroller);

        listBox->newList(theCollection);

        inputLine = new TInputLine(TRect(trect.a.x+3, trect.a.y+2, trect.a.x+lbwidth+1, trect.a.y+3), ifWidth);

        insert(listBox);
        insert(listScroller);
        insert(inputLine);
} // end of TMyDialog::TMyDialog()


/*========================================== TMyDialog ===================
  handleEvent -
 */

void TMyDialog::handleEvent(TEvent &event) {

    switch(event.what) {
        case evKeyDown:
            switch(event.keyDown.keyCode) {

                case kbEnter:
                    if (inputLine->state&sfSelected) {
                        addString();
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
                    if (inputLine->state&sfSelected) {
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
                    char author[ifWidth];
                    char message[81];

                    // Switch to author access,...
                    listBox->setax(TMyListBox::AUTHOR);
                    // get author and...
                    listBox->getText(author, listBox->focused, ifWidth);
                    // restore title access.
                    listBox->setax(TMyListBox::TITLE);

                    std::stringstream ss;

                    ss << "\003AUTHOR: " << author << std::ends;

                    messageBox(ss.str().c_str(), mfOKButton|mfInformation);
                    clearEvent(event);
            }
    }

    TDialog::handleEvent(event);
} // end of MyDialogBox::eventHandler()


/*========================================== TMyDialog ===================
  deleteString - Pressing [Delete] while a list box item is focused
  deletes that item from the list.
  */

void TMyDialog::deleteString()
{
    if (theCollection->getCount()) {
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

    if (strlen(temp)) {
        // Allocate memory and copy title from input field.
        SFrecord *sfentry = new SFrecord;
        sfentry->title = newStr(temp);

        // We've made no provision for author's name... NULL is safe!
        sfentry->author = NULL;

        theCollection->insert(sfentry);
        listBox->setRange(theCollection->getCount());
        listBox->drawView();

        *inputLine->data = EOS;
        inputLine->draw();
        inputLine->selectAll(True);
    }

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

    protected:
    private:
        THeapView *heap;
};


//==========================================[ TApp ]======================
TApp::TApp() : TProgInit(TApplication::initStatusLine,
        TApp::initMenuBar,
        TApplication::initDeskTop)
{

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
                    "\003TListBox Example 5\n"
                    "\003User Collections Part 1\n\n"
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
            1, "SF List"); //, SFbooks);

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
