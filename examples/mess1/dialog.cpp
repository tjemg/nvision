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
// dialog.cpp:  The member function definition for TDataEntryDlg.
//
//========================================================================

#define Uses_TInputLine
#define Uses_TStaticText
#define Uses_TRadioButtons
#define Uses_TCheckBoxes
#define Uses_TSItem
#define Uses_TButton
#define Uses_TKeys
#define Uses_TLabel
#define Uses_THistory
#include <tv.h>
#include "dialog.h"
#include "cmds.h"


//
// TDataEntryDlg - Constructor
//

TDataEntryDlg::TDataEntryDlg(TRect bounds,char *aTitle) :
    TDialog(bounds,aTitle),
    TWindowInit(&TDataEntryDlg::initFrame)
{
    TInputLine *pn,*pe;
    TRadioButtons *pr;
    TCheckBoxes *pc;

    // Lesson E.3 //

    isValid = True;


    insert(pn = new TInputLine(TRect(5,2,28,3),21));



    // Lesson E.3 //

    if (!pn)
    {
        isValid = False;
    }


    // Lesson E.2 //
    insert(new TLabel(TRect(14,1,19,2),"~N~ame",pn));
    insert(new THistory(TRect(28,2,31,3),pn,nameHistoryId));

    insert(pe = new TInputLine(TRect(39,2,46,3),5));


    // Lesson E.3 //

    if (!pe)
    {
        isValid = False;
    }


    insert(new TLabel(TRect(40,1,44,2),"~E~xt",pe));
    insert(new THistory(TRect(46,2,49,3),pe,extHistoryId));

    insert(pr = new TRadioButtons(TRect(2, 5, 18, 11),
        new TSItem( "~B~ix",
        new TSItem( "~C~ompuSlave",
        new TSItem( "~G~enie",
        new TSItem( "~I~BBS",
        new TSItem( "~L~etters",
        new TSItem( "~T~raining",
        0))))))));


    if (!pr)
    {
        isValid = False;
    }


    insert(new TLabel(TRect(5,4,15,5),"~B~BS Duties",pr));

    insert(pc = new TCheckBoxes(TRect(22, 5, 54, 11),
        new TSItem("~A~ssembly",
        new TSItem("~B~asic",
        new TSItem("~C~++",
        new TSItem("~D~ebugger",
        new TSItem("D~O~S",
        new TSItem("~E~ngine",
        new TSItem("~L~etters",
        new TSItem("OS~2~",
        new TSItem("O~W~L",
        new TSItem("~P~rofiler",
        new TSItem("~T~V",
        new TSItem("W~i~ndows",
        0))))))))))))));


    if (!pc)
    {
        isValid = False;
    }


    insert(new TLabel(TRect(33,4,43,5),"~I~nterests",pc));

    insert(new TButton(TRect(7, 12, 23, 16), "~C~ancel", cmCancel, bfNormal));
    insert(new TButton(TRect(34, 12, 50, 16), "~S~ave", cmOK, bfDefault));
    selectNext(False);
}


//
// valid - Returns the state of an instance of TDataEntryDlg.
//

Boolean TDataEntryDlg::valid(ushort)
{
    return (isValid);
}
