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
// myview.h:  The class definition for TMyView.
//
//========================================================================



#ifndef _MYVIEW_H
#define _MYVIEW_H

#define Uses_TView
#define Uses_TRect
#define Uses_TEvent
#include <tv.h>


//
// struct techInfoRec - Stores a record for information about a technicion.
//

struct techInfoRec
{
  char name[21];
  char ext[5];
  ushort bbs;
  ushort interest;
};


//
// TMyView - A view to display my name in a view.
//

class TMyView : public TView
{
public:
    TMyView(TRect);
    ~TMyView();
    void handleEvent(TEvent&);
    void draw();
    Boolean valid(ushort);
protected:
    void addRecord(techInfoRec *);
private:
    techInfoRec **records;
    ushort numRecords;
    ushort maxRecords;
    Boolean isValid;
};


#endif
