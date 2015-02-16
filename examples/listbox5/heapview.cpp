/* HEAPVIEW.CPP:  THeapView member functions - On-screen heap viewer

  ========================================================================
    Turbo Vision, Copyright (c) 1991 by Borland International
*/

#define Uses_TRect
#define Uses_TView
#define Uses_TDrawBuffer
#include <tv.h>

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sstream>
#include <iomanip>
#include <time.h>

#include "heapview.h"

//=========================================[ THeapView ]==================
THeapView::THeapView(TRect& r) : TView(r) {

  oldMem = 0;
  newMem = heapSize();
}

/*========================================== THeapView ===================
   draw -

     REQUIRES:
     RETURNS:
*/

void THeapView::draw() {

  TDrawBuffer buf;
  char c = getColor(2);

  buf.moveChar(0, ' ', c, size.x);
  buf.moveStr(0, heapStr, c);
  writeLine(0, 0, size.x, 1, buf);
}


/*========================================== THeapView ===================
   update -

     REQUIRES:
     RETURNS:
*/

void THeapView::update() {

  if ((newMem = heapSize()) != oldMem) {
    oldMem = newMem;
    drawView();
  }
}


/*========================================== THeapView ===================
   heapSize -

     REQUIRES:
     RETURNS:
*/

long THeapView::heapSize() {
 long total;
  strcpy(heapStr, "     No heap");
  total = -1;
  return(total);
}


