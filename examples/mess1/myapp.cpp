//========================================================================
//  The following example routines have been provided by the Technical
//  Support staff at Borland International.  They are provided as a
//  courtesy and not as part of a Borland product, and as such, are
//  provided without the assurance of technical support or any specific
//  guarantees.
//========================================================================
//  Turbo Vision - Example of message passing
//
//------------------------------------------------------------------------
//
// myapp.cpp:  The member function definition for TMyApplication.
//
//========================================================================

#define Uses_TKeys
#define Uses_TMenuItem
#define Uses_TSubMenu
#define Uses_TStatusLine
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TEvent
#define Uses_TDialog
#define Uses_TStaticText
#define Uses_TButton
#define Uses_TDeskTop
#define Uses_TWindow
#include <tv.h>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include "cmds.h"
#include "myapp.h"
#include "myview.h"
#include "dialog.h"

int windowFlag = 0;

//
// TMyApplication - Constructor.
//

TMyApplication::TMyApplication() :
    TProgInit( &TMyApplication::initStatusLine,
               &TMyApplication::initMenuBar,
               &TMyApplication::initDeskTop )
{
}


//
// initMenuBar - Initialize the menu bar. It will be called by the virtual base
//               TProgInit constructor.
//

TMenuBar *TMyApplication::initMenuBar( TRect bounds )
{
    bounds.b.y = bounds.a.y + 1;

    TMenuBar *mainMenu =
        new TMenuBar (bounds,
            *new TSubMenu ("~=~", kbAltSpace) +
                 *new TMenuItem ("~A~bout...", cmAboutCmd, kbAltA, hcNoContext,
                                 "Alt-A") +
                 newLine() +
                 *new TMenuItem ("~E~xit", cmQuit, kbAltE, hcNoContext,
                                 "Alt-E") +

            *new TSubMenu("~W~indow", kbAltW) +
                 *new TMenuItem("~O~pen", cmOpenCmd, kbF2, hcNoContext,
                                "F2") +
                 *new TMenuItem("~C~lose", cmClose, kbAltF3, hcNoContext,
                                "Alt-F3") +

            *new TSubMenu("~D~ata entry", kbAltD) +
                 *new TMenuItem("~T~ech Info...", cmTechCmd, kbF3, hcNoContext,
                                "F3"));

    return ( mainMenu );
}


//
// initStatusLine - Initialize the status line. It will be called by the
//                  virtual base TProgInit constructor.
//

TStatusLine *TMyApplication::initStatusLine( TRect bounds )
{
    bounds.a.y = bounds.b.y - 1;

    TStatusLine *statusLine =
        new TStatusLine (bounds,
            *new TStatusDef(0, 0xFFFF) +
            *new TStatusItem("~Alt-X~ Exit", kbAltX, cmQuit) +
            *new TStatusItem("~Alt-A~ About", kbAltA, cmAboutCmd) +

            *new TStatusItem("~F2~ Open", kbF2, cmOpenCmd) +
            *new TStatusItem("~Alt-F3~ Close", kbAltF3, cmClose) +

            *new TStatusItem("~F3~ Tech", kbF3, cmTechCmd));

    return (statusLine);
}


//
// handleEvent - Need to handle the event for the menu and status line choice
//               cmAboutCmd.
//

void TMyApplication::handleEvent(TEvent& event)
{
    TApplication::handleEvent(event);

    switch(event.what)
    {
        case evCommand:
        {
            switch(event.message.command)
            {
                case cmAboutCmd:
                {
                    aboutDlg();
                    break;
                }

                case cmOpenCmd:
                {
                    window();
                    break;
                }

                case cmTechCmd:
                {
                    techDlg();
                    break;
                }
                default:
                  return;
            }
            clearEvent(event);
        }
    }
}


//
// aboutDlg - Creates a about dialog box and execute the dialog box.
//

void TMyApplication::aboutDlg()
{
    TDialog *aboutDlgBox = new TDialog(TRect(0, 0, 47, 14),"About");
    char temp[200];
    std::stringstream ss;

    ss << "\003message.cpp\n\003\n"
       << "\003C++ Version 1.00\n\003\n"
       << "\n\003\n"
       << std::ends;

    strcpy(temp,ss.str().c_str());


    if (aboutDlgBox)
    {
        aboutDlgBox->insert(new TStaticText(TRect(2,2,45,9),temp));
        aboutDlgBox->insert(new TButton(TRect(18,10,29,12),"OK",cmOK,bfDefault));

        aboutDlgBox->options |= ofCentered;

        if (validView(aboutDlgBox) != 0)
        {
            deskTop->execView(aboutDlgBox);

            destroy(aboutDlgBox);
        }
    }
}


//
// window() - Open a window with a view for my name in inside.
//

void TMyApplication::window()
{
    TWindow *win = new TWindow(TRect(0,0,80,23), "Data base", wnNoNumber);


    if (win)
    {
        win->eventMask |= evBroadcast;

        TRect bounds = win->getExtent();

        bounds.grow(-1,-1);

        win->insert(new TMyView(bounds));

        if (validView(win) != 0)
        {
            deskTop->insert(win);
        }
    }
}


//
// techDlg - Load the data entry dialog and execute it.
//

void TMyApplication::techDlg()
{
    TDataEntryDlg *entry = new TDataEntryDlg(TRect(0,0,56,17),"Tech Info");

    entry->options |= ofCentered;

    if (validView(entry) != 0)
    {
        if (execView(entry) == cmOK)
        {
          char *data = new char[entry->dataSize()];
          entry->getData(data);

          if (!windowFlag)
          {
              message(deskTop->owner,evCommand,cmOpenCmd,0);
          }
          message(deskTop->owner,evBroadcast,cmNewRecord,data);
        }

        destroy(entry);
    }
}

