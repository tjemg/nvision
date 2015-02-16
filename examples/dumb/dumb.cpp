#include <iostream>

#define Uses_otstream
#define Uses_TApplication
#define Uses_TDeskTop
#define Uses_TRect
#define Uses_TTerminal
#define Uses_TWindow

#include <tv.h>

//------------------------------------------------------------------------
class TOutputWindow : public TWindow
{
    TTerminal *term;

    public:
    TOutputWindow( TRect bounds, const char *title,
            std::ostream& ostr, ushort bufsize );
    void attach( std::ostream& ostr );
};

//------------------------------------------------------------------------
class TOutErr : public TApplication
{
    std::ostream  oldCout;
    std::ostream  oldCerr;
    std::ostream  oldClog;

    public:
    TOutErr( TRect& outbounds, ushort outBufSize,
            TRect& errBounds, ushort errBufSize );
    ~TOutErr();
    virtual void run();
};

TOutputWindow::TOutputWindow( TRect bounds, const char *title,
        std::ostream& ostr, ushort bufsize ) :
    TWindowInit( TOutputWindow::initFrame ),
    TWindow( bounds, title, wnNoNumber )
{
    bounds = getExtent();
    term = new TTerminal( bounds,
            standardScrollBar( sbHorizontal | sbHandleKeyboard ),
            standardScrollBar( sbVertical | sbHandleKeyboard ),
            bufsize );
    insert( term );
    otstream ot( term );
    ot << "Terminal window \"" << title << "\" created. " << std::endl;
    ostr = ot;
}

//------------------------------------------------------------------------
void TOutputWindow::attach( std::ostream& ostr )
{
    ostr = term;
}

//------------------------------------------------------------------------
TOutErr::TOutErr( TRect& outbounds, ushort outBufSize,
        TRect& errBounds, ushort errBufSize ) :
    TProgInit( TOutErr::initStatusLine, TOutErr::initMenuBar,
            TOutErr::initDeskTop )
{
    oldCout = std::cout;
    oldCerr = std::cerr;
    oldClog = std::clog;
    TOutputWindow *tow;
    tow = new TOutputWindow( outbounds, "Standard Output", std::cout, outBufSize );
    deskTop->insert( tow );
    tow = new TOutputWindow( errBounds, "Standard Error", std::cerr, errBufSize );
    tow->attach( clog );
    deskTop->insert( tow );
}

//------------------------------------------------------------------------
TOutErr::~TOutErr()
{
    std::cout = oldCout;
    std::cerr = oldCerr;
    std::clog = oldClog;
}

//------------------------------------------------------------------------
void TOutErr::run()
{
    std::cout << "Testing standard output:";
    for( int i=0; i<10; i++ )
        std::cout << ' ' << i;
    std::cout << std::endl;

    cerr << "Testing standard error:";
    for( int i=0; i<20; i++ )
        cerr << ' ' << i;
    cerr << std::endl;

    clog << "Testing log:";
    for( int i=0; i<30; i++ )
        clog << ' ' << i;
    clog << std::endl;

    TApplication::run();
}

//------------------------------------------------------------------------
int main( int argc, char **argv )
{
    std::cout << "Testing " << argv[0] << "\n\n   Press any key...";
    getch();
    TOutErr outerr( TRect( 0, 0, 80, 15 ),
            8192, TRect( 0, 15, 80, 23 ), 2048 );
    outerr.run();
    return argc;
}

