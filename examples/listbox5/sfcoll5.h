/* SFCOLL5.H:  SFCollection class definition - required by LISTBOX5

    This is the user-implemented TCollection required by LISTBOX5.CPP.
    It represents the minimum implementation of the SFCollection class
    which will support the functionality required by LISTBOX5.CPP.

    Note that there is no attempt to implement streamability at this
    point, even though we are driving from a streamable TCollection
    class.  Indeed, at this point the only reflection of our intension
    to make SFCollection streamable (aside from deriving it from
    TCollection rather than TNSCollection, of course) is the appearance
    of the dummy function member definitions readItem() and writeItem();
    we must supply these to avoid the compiler's complaint that we are
    trying to instantiate an abstract class.

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

// An SFCollection deals in SFrecord structures which we define here.
// The destructor takes care of deallocating memory when we delete a
// SFrecord item and when we kill a containing TMyDialog.
struct SFrecord {
  char *title;
  char *author;
  ~SFrecord() {
    delete title;
    delete author;
  }
};


//////////////////////////////////////////// SFCollection //////////////
class SFCollection : public TCollection {
  public:
    SFCollection(ccIndex aLimit, ccIndex aDelta)
        :TCollection(aLimit, aDelta) {}

    // This member function returns the SFrecord item located at
    // element 'index' in the collection.  This member function allows
    // use to treat a collection like a conventional array. Because it
    // allows me to "sneak up" on a functional application, it is
    // always amongst the first member functions I implement.
    SFrecord *at(ccIndex index) {
      return (SFrecord*)TCollection::at(index);
    }

    // Although not explicitly used by the LISTBOX5 application, this
    // function is called by the internal mechanism of a TListBox
    // object so muct be properly implemented.
    virtual ccIndex indexOf(SFrecord *item) {
      return TCollection::indexOf(item);
    }

    // A public user-specified member function to get a value (i.e.,
    // count) which is otherwise only accessible from classes derived
    // from this class because 'count' is protected.
    short getCount() {
      return count;
    }

  protected:
  private:

    // Required by the LISTBOX5 application to delete an SFrecord.
    // Is directly called by the delete command. It is also called
    // by TNSCollection::freeAll() for each item in the list when
    // we close a TMyDialog.
    virtual void freeItem(void *item) {

      // Invokes the SFrecord destructor to deallocate the title
      // and author.
      delete (SFrecord*)item;
    }

    // These do nothing (yet) but they have to be defined otherwise
    // the compiler will complain that we are trying to instantiate
    // an abstract class.
    virtual void *readItem(ipstream&) { return 0; }
    virtual void writeItem(void *, opstream&) {}
};

#endif  // SFCOLLEC_H
