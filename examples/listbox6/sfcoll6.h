/* SFCOLL6.H:  SFCollection class definition & inline member functions

    This is a user-implemented TCollection required by LISTBOX6.CPP.
    Since all members defined so far are inline, the header file
    completely defines the collection, i.e., no SFCOLL6.CPP exists
    or is needed.

  ----------------------------------------------------------------------
   The following example routines have been provided by the Technical
   Support staff at Borland International.  They are provided as a
   courtesy and not as part of a Borland product, and as such, are
   provided without the assurance of technical support or any specific
   guarantees.
*/

#if !defined(SFCOLLEC_H)
#define SFCOLLEC_H

#define Uses_TCollection
#include <tv.h>

struct SFrecord {
  char *title;
  char *author;
  char *isbn;
  ~SFrecord() {
    delete title;
    delete author;
    delete isbn;
  }
};


//////////////////////////////////////////// SFCollection //////////////
class SFCollection : public TCollection {
  public:
    SFCollection(ccIndex aLimit, ccIndex aDelta)
        :TCollection(aLimit, aDelta) {}

    SFrecord *at(ccIndex index) {
      return (SFrecord*)TCollection::at(index);
    }

    virtual ccIndex indexOf(SFrecord *item) {
      return TCollection::indexOf(item);
    }

    short getCount() {
      return count;
    }

  protected:
  private:
    virtual void freeItem(void *item) {
      delete (SFrecord*)item;
    }

    // These do nothing (yet) but they have to be defined otherwise
    // the compiler will complain that we are trying to instantiate
    // an abstract class.
    virtual void *readItem(ipstream&) { return 0; }
    virtual void writeItem(void *, opstream&) {}
};

#endif  // SFCOLLEC_H
