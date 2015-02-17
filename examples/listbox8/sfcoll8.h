/* SFCOLL8.H:  Complete SFCollection class definition

    In this version of the SFCollection class we implement the read()
    and write() member functions to support the added data member 'set'
    used by LISTBOX8.CPP to save the selected object and the selected
    list box item.

    Although read() and write() are the only additional functions re-
    quired by LISTBOX8, we have implemented all member functions defined
    by the base TCollection class such that they behave correctly for
    SFrecord items.

    In this incarnation of SFCollection we have moved the out-of-inline
    functions into a file of their own called SFCOLL8.CPP;  SFCOLL8.CPP
    requires the inclusion of SFCOLL8.H just as the LISTBOX8.CPP
    application does and for the same reason; to supply the definitions
    for the classes, and the function and data member declaration and/or
    definitions.

    Although we could have compiled and linked LISTBOX8 without going
    to this trouble, our SFCollection class would have been limited to
    those applications which reference our SFCollection in only one
    module, i.e., have only one module which refers to an SFCollection.

    Obviously this is a serious limitation for any reasonably sized
    application and one which is overcome by putting any out-of-inline
    functions in a file which is then linked rather than #included.
    This permits the header file containing the class definition and
    inline member functions to be included in as many modules as my be
    required.

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

#if !defined(SFCOLLEC_H)
#define SFCOLLEC_H

#define Uses_TStreamableClass
#define Uses_TCollection
#include <tv.h>

// Modified explict char* title, author, isbn to three element
// char array 'cell' and supplied a enum for accessing array cells
// pnemonically.
struct SFrecord {
  enum { TITLE, AUTHOR, ISBN };
  char *cell[3];
  ~SFrecord();
};

// Masks for separating the focused and selected parts of the 'set' byte
const unsigned short setFocus    = 0xfffc;   // 1111 1111 1111 1100
const unsigned short setSelected = 0x0003;   // 0000 0000 0000 0011

//////////////////////////////////////////// SFCollection ////////////////
class SFCollection : public TCollection {
  public:

    // Low-order 2 bits saves the selected object on closure,
    // rest of bits saves the index to the focused collection item
    // on closure a la:
    //    xxxx xxxx xxxx xx00    =  Title input line selected
    //    xxxx xxxx xxxx xx01    =  Author input line selected
    //    xxxx xxxx xxxx xx10    =  list box selected
    //    ---- ---- ---- --xx    =  index of focused listbox item
    // Means max elements in our collection is 16,383
    unsigned short set;
    static const char *const name;

    SFCollection(ccIndex aLimit, ccIndex aDelta)
        :TCollection(aLimit, aDelta) {}

    SFrecord *at(ccIndex index) {
      return (SFrecord*)TCollection::at(index);
    }

    void atInsert(ccIndex index, SFrecord *item) {
      TCollection::atInsert(index, item);
    }

    void atPut(ccIndex index, SFrecord *item) {
      TCollection::atPut(index, item);
    }

    void atRemove(ccIndex index) {
      TCollection::atRemove(index);
    }

    static void error(ccIndex code, ccIndex info) {
      TCollection::error(code, info);
    }

    SFrecord *firstThat(ccTestFunc Test, void *arg) {
      return (SFrecord*)TCollection::firstThat(Test, arg);
    }

    void forEach(ccAppFunc action, void *arg) {
      TCollection::forEach(action, arg);
    }

    void free(SFrecord *item) {
      TCollection::free(item);
    }

    void freeAll() {
      TCollection::freeAll();
    }

    virtual ccIndex indexOf(SFrecord *item) {
      return TCollection::indexOf(item);
    }

    void insert(SFrecord *item) {
      TCollection::insert(item);
    }

    SFrecord *lastThat(ccTestFunc Test, void *arg) {
      return (SFrecord*)TCollection::lastThat(Test, arg);
    }

    void pack() {
      TCollection::pack();
    }

    void remove(SFrecord *item) {
      TCollection::remove(item);
    }

    void removeAll() {
      TCollection::removeAll();
    }

    virtual void setLimit(ccIndex aLimit) {
      TCollection::setLimit(aLimit);
    }

    short getCount() { return count; }
    static TStreamable *build();

  protected:
    SFCollection(StreamableInit) : TCollection(streamableInit) {}

    // Since we added the 'set' bits to our collection we are now
    // responsible for reading a component of the collection ourselves.
    // This does the job.
    virtual void *read(ipstream& is) {
      is >> set;
      TCollection::read(is);
      return this;
    }

    // Ditto for assuring that the our 'set' bits get written out.
    virtual void write(opstream& os) {
      os << set;
      TCollection::write(os);
    }

  private:
    virtual void freeItem(void *item) { delete (SFrecord*)item; }
    virtual const char *streamableName() const { return name; }
    virtual void *readItem(ipstream&);
    virtual void writeItem(void *, opstream&);
};


//=========================================< SFCollection >===============
inline TStreamable *SFCollection::build() {

  return new SFCollection(streamableInit);
}

//==========================================< SFCollection >==============
inline ipstream& operator >> (ipstream& is, SFCollection& sf)
    { return is >> (TStreamable&)sf; }
inline ipstream& operator >> (ipstream& is, SFCollection*& sf)
    { return is >> (void *&)sf; }

inline opstream& operator << (opstream& os, SFCollection& sf) {
  return os << (TStreamable&)sf;
}
inline opstream& operator << (opstream& os, SFCollection* sf) {
  return os << (TStreamable *)sf;
}

#endif  // SFCOLLEC_H
