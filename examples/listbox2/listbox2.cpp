/* LISTBOX2.CPP: Deriving a new Dialog Box containing a TListBox

   This sample code demonstrates the use of class derivation to create
   a new dialog box class which contains a Tlistbox. It is identical to
   LISTBOX1.CPP in functionality but is quite different in the method
   used to realize that functionality;  note that the listBox creation
   machinery been moved into the derived dialog box's constructor; only
   the code required to create, run and remove the dialog box remains in
   TApp's ListDialog() function.

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

#include "heapview.h"

//  global data
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
int numStrings = sizeof(theList) / sizeof(theList[0]);


//////////////////////////////////////////// TMyDialog ///////////////////
class TMyDialog : public TDialog {
	public:
		TMyDialog(TRect trect, char *title, char *sarray[], int ns);

    // Since we are now creating 'theCollection' in the constructor,
    // we will uncreate (i.e., delete) it in the destructor.
    ~TMyDialog() {
      delete theCollection;
    }

	private:
    TListBox *listBox;
    TScrollBar *listScroller;
    TStringCollection *theCollection;
};

//==========================================[ MyDialogBox ]===============
TMyDialog::TMyDialog(TRect trect, char *title, char *sarray[], int ns)
					:TDialog(trect, title),
					 TWindowInit(TDialog::initFrame) {

  // build a collection to pass to the listbox
  theCollection = new TStringCollection(100, 10);
  for (int i=0; i<ns; i++) theCollection->insert(sarray[i]);

  options |= ofCentered;

  // Note that we have modified the sizing of the listbox to be
  // relative to the size of the enclosing dialog box... this makes it
  // possible to change the TRect arguments to the TMyDialog constructor
  // without requiring a rewrite of the constructor. Note, however,
  // that we do not do anything to constrain the dialog or list box
  // coordinates to within valid desktop ranges... not very robust but
  // maybe we'll do something about it in a later example.
  int lbwidth = (trect.b.x - trect.a.x) - 3;
  int lbhite = (trect.b.y - trect.a.y) - 2;

  // create a scroll bar for the list box
  listScroller = new TScrollBar(TRect(trect.a.x+3, trect.a.y+lbhite,
                                      trect.a.x+lbwidth, trect.a.y+lbhite+1));

  // create a list box with 2 columns and associated scroller
  listBox = new TListBox(TRect(trect.a.x+3, trect.a.y+2,
                               trect.a.x+lbwidth, trect.a.y+lbhite),
                               2, listScroller);

  // associate the collection with the list box
  listBox->newList(theCollection);

  // insert the list box with its scroller into the dialog box
  insert(listBox);
  insert(listScroller);
} // end of MyDialogBox::MyDialogBox()


//////////////////////////////////////////// TApp ////////////////////////
class TApp : public TApplication {
  public:
    TApp();

    // virtual functions to be locally redefined
    static TMenuBar *initMenuBar(TRect r);
    void handleEvent(TEvent &event);

    // declare new functions
    void AboutDialog();
    void ListDialog();
    void idle();

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
               "\003TListBox Example 2\n"
               "\003Using a Dialog Box constructor\n"
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

  TMyDialog *pd = new TMyDialog(TRect(0, 0, 60, 15), "TV Classes", theList, numStrings);

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

  // now for the tough stuff
  TApp myApp;
  myApp.run();
  return 0;
}
