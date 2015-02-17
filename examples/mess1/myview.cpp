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
// myview.cpp:  The member function definition for TMyView.
//
//========================================================================

#define Uses_MsgBox
#include <tv.h>
#include <sstream>
#include <iomanip>
#include <string.h>
#include "myview.h"
#include "cmds.h"

char *BBSType[] = {
    "Bix",
    "CompuSlave",
    "Genie",
    "IBBS",
    "Letters",
    "Training"
};

char interestType[] = { 'I', 'T', 'P', 'W', '2', 'L',
                        'E', 'O', 'D', 'C', 'B', 'A' };


//
// TMyView - Constructor
//

TMyView::TMyView(TRect bounds) :
    TView (bounds)
{
    windowFlag++;

    isValid = True;

    records = new techInfoRec *[size.y];

    if (!records)
    {
        isValid = False;
    }

    maxRecords = size.y;
    numRecords = 0;

    eventMask |= evBroadcast;
    growMode |= gfGrowHiX | gfGrowHiY;
}


//
// ~TMyView - Destructor
//

TMyView::~TMyView()
{
  windowFlag--;

  delete records;
}

//
// draw - The draw function.  Simply uses writeChar and writeStr to display
//        string.
//

void TMyView::draw()
{
    for(int i = 0;i < size.y;i++)
    {
        writeChar(0,i,' ',1,size.x);

        if (i < numRecords)
        {
            char temp[200];

            std::stringstream ss;

            ss << std::setw(20) << records[i]->name
               << "|"
               << std::setw(4) << records[i]->ext
               << "|"
               << std::setw(10) << BBSType[records[i]->bbs];

            unsigned int k = 0x0800;
            for(int j = 0;j < 12;j++)
            {
                ss<< "|";

                if (records[i]->interest & k) {
                    ss << interestType[j];
                } else {
                    ss << ' ';
                }
                k >>= 1;
            }
            ss << std::ends;


            strcpy(temp,ss.str().c_str());
            temp[size.x+1] = 0;
            writeStr(0,i,temp,1);
        }
    }
}


//
// valid - Returns the state of the current instance of TMyView.
//

Boolean TMyView::valid(ushort)
{
    return (isValid);
}


//
// handleEvent - Handle event that tell the view that there is a record.
//

void TMyView::handleEvent(TEvent& event)
{
    TView::handleEvent(event);

    switch(event.what)
    {
        case evBroadcast:
        {
            switch(event.message.command)
            {
                case cmNewRecord:
                {
                    char *data = (char *)event.message.infoPtr;
                    techInfoRec *newRec = new techInfoRec;

                    strcpy(newRec->name,data);
                    strcpy(newRec->ext,&data[21]);
                    newRec->bbs = *(int *)&data[26];
                    newRec->interest = *(int *)&data[28];
                    addRecord(newRec);
                    draw();
                    clearEvent(event);
                    break;
                }
                default:
                {
                    return;
                }
            }
        }
    }
}


//
// addRecord - Add a record to the data base.
//

void TMyView::addRecord(techInfoRec *newRec)
{
    if (numRecords != maxRecords)
    {
        records[numRecords++] = newRec;
    }
    else
    {
        delete newRec;
    }
}
