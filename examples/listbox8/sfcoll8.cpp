/* SFCOLL8.CPP:  Adding data members to a collection

    SFCOLL8.CPP contains the non-inline functions required by the
    SFCollection class. We were forced to place these functions into
    this file when we chose to use a "for" statement in the definition
    of these functions; in Borland C++, member functions which use
    "for/else" statements cannot be inline and non-line functions cannot
    be #included in a project more than once without sponsoring a
    "symbol xyz defined in module abc is duplicated in efg" error.

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

#include "sfcoll8.h"

//==========================================[ SFrecord ]==================
SFrecord::~SFrecord() {

  for (int i=0; i<3; i++)
    delete cell[i];
}

const char *const SFCollection::name = "SFCollection";
//==========================================< SFCollection >==============
TStreamableClass RFCollection(SFCollection::name,
											        SFCollection::build,
											        __DELTA(SFCollection));

//=========================================< SFCollection >===============
void SFCollection::writeItem(void *obj, opstream& os) {

  SFrecord *sfentry = (SFrecord*)obj;
  for (short i=0; i<3; i++) os.writeString((const char*)sfentry->cell[i]);
}

//==========================================< SFCollection >==============
void *SFCollection::readItem(ipstream& is) {

  SFrecord *sfentry = new SFrecord;
  for (short i=0; i<3; i++) sfentry->cell[i] = is.readString();
  return sfentry;
}

