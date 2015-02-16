
#define Uses_MsgBox
#define Uses_TButton
#define Uses_TDeskTop
#define Uses_TDialog
#define Uses_TInputLine
#define Uses_TLabel
#define Uses_TProgram
#define Uses_TRect
#define Uses_TStaticText
#define Uses_TStreamable
#define Uses_TStreamableClass
#define Uses_TStreamableTypes
#define Uses_TView
#define Uses_TWindow
#define Uses_fpbase
#define Uses_fpstream
#define Uses_ifpstream
#define Uses_iopstream
#define Uses_ipstream
#define Uses_ofpstream
#define Uses_opstream
#define Uses_pstream
#define Uses_otstream

#include <tv.h>

/*--------------------------------------------------------------------*/

#define PwLength 16

/*--------------------------------------------------------------------*/

#define Scramble * 3 + 100

/*--------------------------------------------------------------------*/

struct PwStruct
{
	char UserName[PwLength + 1];
	char UserPassword[PwLength + 1];
};

struct ScrambleStruct
{
	char UserName[PwLength + 1];
	int  UserPassword[PwLength + 1];
};

/*--------------------------------------------------------------------*/


class Password
{
	public:
		void scramble (void);
		void unscramble(void);

	private:
		PwStruct PwBuf;
		PwStruct PwInBuf;

	public:
		Password(char **argv);
		Password(void);

		~Password();

		int CheckPassword(void);

		void AddPassword(void);

		void WrongPassword(void);

		char UserName[PwLength + 1];

	private:
		FILE *PwFilePtr;

		char PwFileName[80];

		ScrambleStruct PwScrambleBuf;

		int MasterPassword1[PwLength];

		int MasterPassword2[PwLength];

		void Master(void);
};

/*--------------------------------------------------------------------*/

class TInputPasswd : public TInputLine
{

public:

    TInputPasswd( const TRect&, int len );
    TInputPasswd( int x, int y, int len );
    virtual void draw( void );

protected:

    TInputPasswd( StreamableInit ) : TInputLine( streamableInit ) {};

private:

    virtual const char *streamableName() const
	{ return name; }

public:

    static const char * const name;
    static TStreamable *build();

};


inline ipstream& operator >> ( ipstream& is, TInputPasswd& cl )
    { return is >> (TStreamable&)cl; }
inline ipstream& operator >> ( ipstream& is, TInputPasswd*& cl )
    { return is >> (void *&)cl; }
inline opstream& operator << ( opstream& os, TInputPasswd& cl )
    { return os << (TStreamable&)cl; }
inline opstream& operator << ( opstream& os, TInputPasswd* cl )
    { return os << (TStreamable *)cl; }


ushort inputPasswdBoxRect( const TRect &bounds,
			   const char *Title,
			   const char *aLabel,
			   char *s,
			   uchar limit );


ushort inputPasswdBox( const char *Title, const char *aLabel, char *s, uchar limit );

