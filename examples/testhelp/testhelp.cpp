//=============================================================
//  The following routines have been uploaded to the Borland
//  Forum BPROGB on CompuServe by the Technical Support staff.
//  They are provided as a courtesy and not as part of a Borland
//  product; and, as such, are provided without the assurance of
//  technical support or any specific guarantees.
//==============================================================
//  Turbo Vision - Context Sensitive Help
//
//  - This sample code demonstrates creating context sensitive
//    help.
//
//  - The Step to follow are:
//
//     1) Overload TApplication::getPalette to return a new
//        palette which contains palette entries for the help
//        system.
//
//     2) Overload TApplication::getEvent to handle the cmMyHelp
//        command.  Overloading getEvent allows the application
//        to help requests comming from any view.
//
//     3) Inherit from TStatusLine to create THintStatusline.
//
//     4) Create a  XXXX.txt file (see testhelp.txt).
//          a) compile XXXX.txt with tvhc.exe in the
//             borlandc\tvision\help subdirectory.  The
//             resulting file XXXX.hlp is the binary
//             file which Turbo Vision uses to access help
//             at runtime.
//
//==============================================================

#define Uses_TWindow
#define Uses_TMenuBox
#define Uses_TMenu
#define Uses_TKeys
#define Uses_TApplication
#define Uses_TEvent
#define Uses_TRect
#define Uses_TDeskTop
#define Uses_TMenuBar
#define Uses_TMenuItem
#define Uses_TSubMenu
#define Uses_TStatusLine
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_MsgBox
#define Uses_fpstream
#define Uses_TDialog
#define Uses_TButton
#define Uses_TStaticText
#define Uses_TRadioButtons
#define Uses_TSItem
#define Uses_TPalette
#define Uses_THelpFile
#define Uses_THelpWindow


#include <tv.h>
//#include <help.h>

//
//these are the constants created by TVHC
//

#include "testhelp.h"

const int cmAbout     = 100;
const int cmMyHelp    = 101;
const int cmMySave    = 102;
const int cmMyOpen    = 103;

//
//This is the help file generated from testhelp.txt by TVHC
//
const char * HELPFILE = "testhelp.hlp";

//
//Statusline hints help give the user information
//
class THintStatusLine: public TStatusLine
{

public:

     THintStatusLine(TRect& r, TStatusDef& d)
          : TStatusLine(r, d) {}
     const char* hint(ushort);
};


//
//Here we return a hint depending on the current help context
//
const char* THintStatusLine::hint(ushort context)
{
     switch (context)
     {
       case hcNoContext:
         return "";

       case hcExit:
         return "Alt-X to Exit";

       case hcSave:
         return "Save a file";

       case hcEdit:
         return "Edit file";

       case hcClose:
         return "Close the current file";

       case hcOpen:
         return "Open a file";

       case hcAbout:
	 return "View the About Box";

       case hcHello:
         return "Hello World";

       default:
         return "";
     }
}


class TMyApplication : public TApplication
{
 public:
     TMyApplication();
//     TPalette& getPalette() const;
     void getEvent(TEvent&);
     virtual void handleEvent(TEvent&);
     static TMenuBar* initMenuBar(TRect);
     static TStatusLine* initStatusLine(TRect);
};


TMyApplication::TMyApplication()
     : TProgInit(initStatusLine,initMenuBar,initDeskTop)
{
}


//TPalette& TMyApplication::getPalette() const
//{
//     //
//     //We need to add palette entries for the help screen
//     //
//     static TPalette
//       color(cHelpColor,
//          sizeof(cHelpColor - 1);
//
//     static TPalette
//       blackwhite(cpBlackWhite cHelpBlackWhite,
//	       sizeof(cHelpBlackWhite) - 1);
//     static TPalette
//
//       monochrome(cpMonochrome cHelpMonochrome,
//               sizeof(cHelpMonochrome) - 1);
//
//     static TPalette *palettes[] =
//     {
//       &color,
//       &blackwhite,
//       &monochrome
//     };
//
//     return *(palettes [appPalette] );
//}


//
//We use the Application getEvent to access help requests
//from any view.
//
void TMyApplication::getEvent(TEvent& event) {
    static Boolean helpInUse = False;

    TProgram::getEvent(event);
    if (event.what == evCommand) {
        switch (event.message.command) {
            case cmMyHelp:
                if (helpInUse == True)
                    return;

                fpstream& helpStream = *new fpstream(HELPFILE,
                        std::ios::in | std::ios::binary);
                THelpFile* helpFile = new THelpFile(helpStream);

                if (!helpStream) {
                    messageBox("Could not open help file NEWHELP.HLP", mfError | mfOKButton);
                    delete helpFile;
                } else {
                    helpInUse = True;
                    TWindow* window = (TWindow*)validView(new THelpWindow(helpFile, getHelpCtx() ) );
                    if (window) {
                        execView(window);
                        destroy(window);
                        helpInUse = False;
                    }
                    clearEvent(event);
                }
                break;
        }
    }

}


//
//Application handleEvent
//
void TMyApplication::handleEvent(TEvent &event)
{
     TApplication::handleEvent(event);

     if (event.what == evCommand)
     {
       switch (event.message.command)
       {
         case cmAbout:
          TDialog *pd =
             new TDialog(TRect(20,4,60,20), "About");
          if( validView(pd) )
          {
             //
             //assign help context to this view
             //
             pd->helpCtx = hcAboutBox;
             //
             //insert Statictext
             //
             pd->insert( new TStaticText(TRect(5,3,35,6),
                 "\003TestHelp About Box\n\n\003Provided by"
                 "\n\n\003Borland Technical Support"));

             TRadioButtons *rb =
                    new TRadioButtons(TRect(7,7,32,9),
                         new TSItem("One",
                         new TSItem("Two",
                         new TSItem("Three",
                         new TSItem("Four", 0) ))));
             //
             //assign help context to the RadioButtons
             //
             rb->helpCtx = hcAboutRadio;
             pd->insert(rb);

             pd->insert(new TButton(TRect(15,10,25,12),
                     "~O~k", cmOK, bfDefault) );
             execView(pd);
             destroy(pd);
          }
       }
       clearEvent (event);
     }
}


//
//Application MenuBar - this method of building a menu will
//      never produce the 'out of memory' problem generated
//      by nested menus.
//
TMenuBar* TMyApplication::initMenuBar(TRect r)
{
     r.b.y = r.a.y + 1;

    TSubMenu& sub1 =
        *new TSubMenu("~=~", kbAltSpace, hcNoContext ) +
        *new TMenuItem("~A~bout...", cmAbout, 0, hcAbout, 0, 0)+
        *new TMenuItem("E~x~it", cmQuit, kbAltX, hcExit, 0, 0);

    TSubMenu& sub2 = *new TSubMenu( "~T~est", kbAltT, hcTest) +
        *new TMenuItem( "~H~ello", cmOK, kbAltH, hcHello, 0, 0);

    TSubMenu& sub3 = *new TSubMenu( "~F~ile", kbNoKey, hcFile)+
        *new TMenuItem( "~S~ave", cmOK, kbAltS, hcSave, 0, 0) +
        *new TMenuItem( "~E~dit", cmOK, kbAltE, hcEdit, 0, 0) +
        *new TMenuItem( "~O~pen", cmOK, kbAltO, hcOpen, 0, 0);

    TMenuBar * menuBar =
        new TMenuBar( r, new TMenu((TMenuItem &)
          (sub1 + sub2 + sub3)));

    return menuBar;
}


//
//Application statusline
//
TStatusLine* TMyApplication::initStatusLine(TRect r)
{
     r.a.y = r.b.y - 1;

     //standard statusline help
     return new THintStatusLine( r,
       *new TStatusDef(hcNoContext, hcEndStandard ) +
         *new TStatusItem("~F1~ Help", kbF1, cmMyHelp) +
         *new TStatusItem("~Alt+X~ Exit", kbAltX, cmQuit ) +

     //statusline help for your about box
       *new TStatusDef(hcAboutBox, hcAboutBox) +
         *new TStatusItem("~F1~ Help", kbF1, cmMyHelp) +
         *new TStatusItem("~Esc~ to Close Dialog", kbEsc,
              cmClose));

}

int main()
{
     TMyApplication *Myapp = new TMyApplication();
     Myapp->deskTop->helpCtx = hcNoContext;
     Myapp->run();
     TProgram::destroy(Myapp);
     return 0;
}
