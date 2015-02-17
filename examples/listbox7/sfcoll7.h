/* SFCOLL7.H:  SFCollection class definition & inline member functions

    This is the user-implemented TCollection required by LISTBOX7.CPP.
    It adds the functionality to our SFCollection class which permits
    the SFrecord entries to be streamed to and from a file.

  ----------------------------------------------------------------------
   The following example routines have been provided by the Technical
   Support staff at Borland International.  They are provided as a
   courtesy and not as part of a Borland product, and as such, are
   provided without the assurance of technical support or any specific
   guarantees.
*/

#if !defined(SFCOLLEC_H)
#define SFCOLLEC_H

#define Uses_TStreamableClass
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
    // A data member required by the stream manager. It is returned
    // to the stream manager via the member function 'streamableName()'.
    // Since it is static, it must be initialized with a pointer to a
    // string ("SFCollection" in our case) outside the class and at
    // global level.
    static const char *const name;

    SFCollection(ccIndex aLimit, ccIndex aDelta)
        :TCollection(aLimit, aDelta) {}

    SFrecord *at(ccIndex index) {
      return (SFrecord*)TCollection::at(index);
    }

    virtual ccIndex indexOf(SFrecord *item) {
      return TCollection::indexOf(item);
    }

    short getCount() { return count; }

    // Makes an empty SFrecord.
    static TStreamable *build();

  protected:

    // This constuctor is required to allocate the correct memory for
    // an SFCollection together with the initialized vtable pointers.
    SFCollection(StreamableInit) : TCollection(streamableInit) {}

    void write(opstream& os) { TCollection::write(os); }

  private:
    virtual void freeItem(void *item) { delete (SFrecord*)item; }

    // Required by the stream manager; returns the SFCollection 'name'
    // data member when called by the stream manager.
    virtual const char *streamableName() const { return name; }

    // These have now been defined so that an SFrecord entry can be
    // read from and written to the input and output streams 'is' and
    // 'os', respectively.
    virtual void *readItem(ipstream&);
    virtual void writeItem(void *, opstream&);
};


const char *const SFCollection::name = "SFCollection";
//=========================================< SFCollection >===============
inline TStreamable *SFCollection::build() {

  return new SFCollection(streamableInit);
}

//=========================================< SFCollection >===============
inline void SFCollection::writeItem(void *obj, opstream& os) {

  SFrecord *sfentry = (SFrecord*)obj;
  os.writeString(sfentry->title);
  os.writeString(sfentry->author);
  os.writeString(sfentry->isbn);
}

//==========================================< SFCollection >==============
inline void *SFCollection::readItem(ipstream& is) {

  SFrecord *sfentry = new SFrecord;
  sfentry->title = is.readString();
  sfentry->author = is.readString();
  sfentry->isbn = is.readString();
  return sfentry;
}

//==========================================< SFCollection >==============
TStreamableClass RFCollection(SFCollection::name, SFCollection::build, __DELTA(SFCollection));

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
