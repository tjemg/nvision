//-------------------------------------------------------
//
//   tiago_2.h: Header file for tiago_2.cpp
//
//-------------------------------------------------------

#if !defined( __TIAGO_2_H )
#define __TIAGO_2_H

#define Uses_TStreamable
#define Uses_TStreamableClass
#define Uses_TEvent
#define Uses_TRect
#define Uses_TDialog
#define Uses_TButton
#define Uses_TStaticText
#define Uses_TInputLine

#include <tv.h>


struct dataRec  {
  char field1[11];   //TInputLine
  };


class TDialog2 : public TDialog
{

public:

    TDialog2( );
    TDialog2( StreamableInit ) :
           TDialog (streamableInit),
           TWindowInit(TDialog::initFrame) {};
    virtual void handleEvent( TEvent& );
    virtual Boolean valid( ushort );

    TInputLine *mySerial;



private:

    virtual const char *streamableName() const
        { return name; }

protected:

    virtual void write( opstream& );
    virtual void *read( ipstream& );

public:

    static const char * const name;
    static TStreamable *build();

};

#endif  // __TIAGO_2_H

