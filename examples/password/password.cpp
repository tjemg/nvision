/*--------------------------------------------------------------------*/
/*                                                                    */
/*  Simple password class to allow password checks,                   */
/*  entering new passwords.                                           */
/*                                                                    */
/*  Two default passwords are defined (being a cineast, I chose       */
/*  "ROSEBUD" for one of them, I hope Orson doesn't mind, the other   */
/*  one is "SUPERVISOR").                                             */
/*                                                                    */
/*  Change these as you like in Password::Master().                   */
/*                                                                    */
/*  To avoid passwords being checked out by some wizards who know     */
/*  how to use a binary editor, these two default passwords           */
/*  also are stored using the same simple scrambling algorithm.       */
/*                                                                    */
/*  Additional passwords can be entered by using Password::AddPassword*/
/*  In addition to passwords, user names can be defined.              */
/*                                                                    */
/*  User names are stored together with passwords in a file named     */
/*  either "PASSWORD.PWD" or "<appname>.PWD", depending on the        */
/*  constructor used.                                                 */
/*                                                                    */
/*  Passwords are scrambled using some simple maths and stored        */
/*  in binary format.                                                 */
/*                                                                    */
/*  This will not keep a freak from finding out your passwords        */
/*  but your normal user will not be able to get them with his        */
/*  ASCII editor, either.                                             */
/*                                                                    */
/*                                                                    */
/*  Passwords have to have at least four characters.                  */
/*                                                                    */
/*  Usage:                                                            */
/*                                                                    */
/*  Password *password;                                               */
/*                                                                    */
/*  InputPassword()                                                   */
/*  {                                                                 */
/*     password->AddPassword();                                       */
/*  }                                                                 */
/*                                                                    */
/*  DoSomething()                                                     */
/*  {                                                                 */
/*     if(password->CheckPassword() >= 0)                             */
/*     {                                                              */
/*         ReallyDoIt();                                              */
/*     }                                                              */
/*  }                                                                 */
/*                                                                    */
/*  main(int argc , char **argv)                                      */
/*  {                                                                 */
/*     password = new Password(argv);  OR:                            */
/*     password = new Password();                                     */
/*                                                                    */
/*  }                                                                 */
/*                                                                    */
/*                                                                    */
/*  This code is public domain and can be used freely.                */
/*  There is no guarantee that this code is bug-free!                 */
/*  You use it at your own risk.                                      */
/*                                                                    */
/*  (c)Sagner Software & Systeme                                      */
/*  PO Box 19 05 46                                                   */
/*  D 42705 Solingen , Germany                                        */
/*                                                                    */
/*  CIS 100022,2357                                                   */
/*                                                                    */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <dir.h>

#define Uses_TApplication
#define Uses_TKeys
#include "tv.h"

#include "password.h"
#include "logger.h"

/*--------------------------------------------------------------------*/

class TPwDialog : public TDialog {
    public:
        TPwDialog(TRect &trect , char *title);
        ~TPwDialog();

        void handleEvent(TEvent &event);

        TButton *OKButton;
};

/*--------------------------------------------------------------------*/

TPwDialog::TPwDialog(TRect &trect , char *title)
	: TDialog(trect , title) , TWindowInit(TDialog::initFrame)
{
}

/*--------------------------------------------------------------------*/

TPwDialog::~TPwDialog() {
}

/*--------------------------------------------------------------------*/

void TPwDialog::handleEvent(TEvent &event) {
    switch(event.what) {

        case evKeyDown: {
                            switch(event.keyDown.keyCode) {
                                case kbEnter: {
                                                  if(OKButton->state&sfSelected) {
                                                      event.what = evCommand;
                                                      event.message.command = cmOK;
                                                  } else {
                                                      event.keyDown.keyCode = kbTab;
                                                  }
                                                  break;
                                              }
                            }
                            break;
                        }

        case evCommand:
                        {
                        }

    }

    TDialog::handleEvent(event);
}

/*--------------------------------------------------------------------*/

const char *const TInputPasswd::name = "TInputPasswd";

TStreamable *TInputPasswd::build() {
    return new TInputPasswd( streamableInit );
}

TInputPasswd::TInputPasswd( const TRect& bounds, int aMaxLen )
	     :TInputLine( bounds, aMaxLen )
{
}

TStreamableClass RInputPasswd( TInputPasswd::name,
			       TInputPasswd::build,
		       __DELTA(TInputPasswd)
			     );


void TInputPasswd::draw() {
    char  stars[256];
    char *inData = data;
    int len = strlen( inData );

    memset( stars, '*', len );
    stars[len] = EOS;
    data = stars;
    TInputLine::draw();
    data = inData;
}



ushort inputPasswdBoxRect(  TRect &bounds,
			   char *Title,
			   const char *aLabel,
			   const char *uLabel ,
			   char *s,
			   uchar limit )
{
    TPwDialog *dialog;
    TView* control;
    TRect r , u;

    dialog = new TPwDialog(bounds, Title);

    uchar x = 4 + strlen( aLabel );

    u = TRect( x, 2, min(x + limit + 2, dialog->size.x - 3), 3 );

    control = new TInputLine(u , limit);
    dialog->insert( control );

    r = TRect( 2, 2, 3 + strlen(uLabel), 3 );
    dialog->insert( new TLabel( r, uLabel, control ) );


    r = TRect( x, 3, min(x + limit + 2, dialog->size.x - 3), 4 );


    control = new TInputPasswd( r, limit );
    dialog->insert( control );

    r = TRect( 2, 3, 3 + strlen(aLabel), 4 );
    dialog->insert( new TLabel( r, aLabel, control ) );

    dialog->OKButton = new TButton(TRect(40, 5, 52, 7), "O~k~", cmOK, bfNormal);
    //control->options |= ofFramed;
    dialog->insert(dialog->OKButton);

    control = new TButton(TRect(10, 5, 22, 7), "Cancel", cmCancel, bfNormal);
    //control->options |= ofFramed;
    dialog->insert(control);

    dialog->selectNext(False);
    *s = EOS;

    if ( ! TProgram::application->validView(dialog) ) {
        return cmCancel;
    }

    dialog->setData( s );

    ushort result = TProgram::application->deskTop->execView( dialog );

    if( result != cmCancel) {
        dialog->getData( s );
    }

    TObject::destroy( dialog );

    return (result);
}


ushort inputPasswdBox(  char *Title, const char *aLabel, const char *uLabel , char *s, uchar limit ) {
    ushort len = max( strlen(aLabel) + 9 + limit, strlen(Title) + 10 );
    len = min( len, 60 );
    len = max( len , 24 );
    TRect r(0, 0, len, 8);
    r.move((TProgram::deskTop->size.x - r.b.x) / 2,
            (TProgram::deskTop->size.y - r.b.y) / 2);
    return inputPasswdBoxRect(r, Title, aLabel, uLabel , s, limit);
}

/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/


void Password::Master(void) {
    memset(MasterPassword1 , 0 , sizeof(int) * PwLength);
    memset(MasterPassword2 , 0 , sizeof(int) * PwLength);

    MasterPassword1[0] = 'R' Scramble;
    MasterPassword1[1] = 'O' Scramble;
    MasterPassword1[2] = 'S' Scramble;
    MasterPassword1[3] = 'E' Scramble;
    MasterPassword1[4] = 'B' Scramble;
    MasterPassword1[5] = 'U' Scramble;
    MasterPassword1[6] = 'D' Scramble;

    MasterPassword2[0] = 'S' Scramble;
    MasterPassword2[1] = 'U' Scramble;
    MasterPassword2[2] = 'P' Scramble;
    MasterPassword2[3] = 'E' Scramble;
    MasterPassword2[4] = 'R' Scramble;
    MasterPassword2[5] = 'V' Scramble;
    MasterPassword2[6] = 'I' Scramble;
    MasterPassword2[7] = 'S' Scramble;
    MasterPassword2[8] = 'O' Scramble;
    MasterPassword2[9] = 'R' Scramble;
}

/*--------------------------------------------------------------------*/


Password::Password(void) {
    sprintf(PwFileName , "password.pwd");
    Master();
}

/*--------------------------------------------------------------------*/

Password::Password(char **argv) {
    char drive[3] , dir[80] , name[10] , ext[5];

    //fnsplit(argv[0] , drive , dir , name , ext);

    //fnmerge(PwFileName , drive , dir , name , "pwd");

    Master();
}

/*--------------------------------------------------------------------*/

Password::~Password() {
}

/*--------------------------------------------------------------------*/

void Password::scramble(void) {
    char *p = PwBuf.UserPassword;

    memset(&PwScrambleBuf.UserPassword , 0 , PwLength * sizeof(int));
    for(int i = 0 ; i < min(PwLength , strlen(PwBuf.UserPassword)) ; i ++) {
        PwScrambleBuf.UserPassword[i] = (*p * 3) + 100;
        p ++;
    }
    strcpy(PwScrambleBuf.UserName , PwBuf.UserName);
}

/*--------------------------------------------------------------------*/

void Password::unscramble(void) {
    char *p = PwBuf.UserPassword;

    memset(PwBuf.UserPassword , 0 , PwLength + 1);

    for(int i = 0 ; i < PwLength ; i ++) {
        if(PwScrambleBuf.UserPassword[i] != 0) {
            *p = (PwScrambleBuf.UserPassword[i] - 100) / 3;
            p ++;
        } else {
            break;
        }
    }
}

/*--------------------------------------------------------------------*/

int Password::CheckPassword(void) {
    int res = -1;

    memset(&PwInBuf , 0 , sizeof(PwStruct));

    if (inputPasswdBox("Password Check" , "Please Enter your Password: " , "Please Enter your Username: " ,
                (char *)&PwInBuf , PwLength + 1) != cmCancel) {
        LOG("CHECK UserName :" + std::string(PwInBuf.UserName) );
        LOG("CHECK Password :" + std::string(PwInBuf.UserPassword) );
        memcpy(PwScrambleBuf.UserPassword , MasterPassword1 , PwLength * sizeof(int));
        unscramble();
        if(strcmp(PwInBuf.UserPassword , PwBuf.UserPassword) == 0) {
            strcpy(UserName , "Xanadu");
            res = 0;
        } else {
            memcpy(PwScrambleBuf.UserPassword , MasterPassword2 , PwLength * sizeof(int));

            unscramble();

            if(strcmp(PwInBuf.UserPassword , PwBuf.UserPassword) == 0) {
                strcpy(UserName , "SuperUser");
                res = 1;
            } else {
                int i = 1;

                PwFilePtr = fopen(PwFileName , "r+b");

                if(PwFilePtr != NULL) {
                    while(fread(&PwScrambleBuf , sizeof(ScrambleStruct) , 1 , PwFilePtr) == 1) {
                        unscramble();

                        i ++;

                        if(strcmp(PwInBuf.UserPassword , PwBuf.UserPassword) == 0) {
                            strcpy(UserName , PwScrambleBuf.UserName);
                            res = i;
                            break;
                        }
                    }
                    fclose(PwFilePtr);
                }
            }
        }
        if(res < 0) {
            WrongPassword();
        }
    }

    return(res);
}

/*--------------------------------------------------------------------*/

void Password::AddPassword(void) {
    memset(&PwInBuf , 0 , sizeof(PwStruct));
    if(inputPasswdBox("Enter New Password" , "Please enter the password: " , "Please enter the Username: " , (char *)&PwInBuf , PwLength + 1) != cmCancel) {
        LOG("ADD UserName :" + std::string(PwInBuf.UserName) );
        LOG("ADD Password :" + std::string(PwInBuf.UserPassword) );
        memset(&PwBuf , 0 , sizeof(PwStruct));

        if(inputPasswdBox("Check Password" , "Please enter the password: " , "Please enter the Username: " , (char *)&PwBuf , PwLength + 1) != cmCancel) {
            LOG("ADD_UserName :" + std::string(PwInBuf.UserName) );
            LOG("ADD_Password :" + std::string(PwInBuf.UserPassword) );

            if((strcmp(PwInBuf.UserPassword , PwBuf.UserPassword) == 0)
                    && (strcmp(PwInBuf.UserName , PwBuf.UserName) == 0)
                    && (strlen(PwInBuf.UserPassword) > 3)) {
                scramble();
                PwFilePtr = fopen(PwFileName , "a+b");

                if(PwFilePtr != NULL) {
                    fwrite(&PwScrambleBuf , sizeof(ScrambleStruct) , 1 , PwFilePtr);
                    fclose(PwFilePtr);
                }
            } else {
                WrongPassword();
            }
        }
    }
}

/*--------------------------------------------------------------------*/

void Password::WrongPassword(void) {
    messageBox(mfError | mfOKButton , "Wrong Password");
}

//main(int argc , char **argv) {
//    Password *password = new Password(argv);
//}
