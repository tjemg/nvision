/* HEAPVIEW.H:  THeapView class def - on-screen heap viewer

  ========================================================================
   Turbo Vision, Copyright (c) 1991 by Borland International
*/

#if !defined( HEAPVIEW_H )
#define HEAPVIEW_H

#define Uses_TEvent
#define Uses_TRect
#define Uses_TView
#include <tv.h>

//////////////////////////////////////////// THeapView ///////////////////
class THeapView : public TView {
  public:
    THeapView( TRect& r );
    virtual void update();
    virtual void draw();
    virtual long heapSize();

  private:
    long oldMem, newMem;
    char heapStr[16];
};

#endif  // HEAPVIEW_H
