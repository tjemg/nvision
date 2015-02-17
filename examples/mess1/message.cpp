//========================================================================
//  The following example routines have been provided by the Technical
//  Support staff at Borland International.  They are provided as a
//  courtesy and not as part of a Borland product, and as such, are
//  provided without the assurance of technical support or any specific
//  guarantees.
//========================================================================
//  Turbo Vision - Example of message passing
//
//------------------------------------------------------------------------
//
// message.cpp:  A first look at message passing via evBroadcast.
//
//========================================================================


#include <stdlib.h>
#include "myapp.h"


//
// main:
//      Here we declare an instance of the application object and call the
//      run() member function to start of the program.  Usually, this will be
//      all that is done here.  (Certain other initializations, like getting
//      EMS memory for the overlay manager would also go here.)
//

int main()
{
    //randomize();

    TMyApplication myApp;

    myApp.run();

    return( 0 );
}

