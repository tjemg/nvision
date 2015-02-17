DIRS =  bhelp     clipboard  cyrillic  demo      desklogo   dlgdraw   \
        dlgdsn    viewers    dyntxt    emodal    fonts      helpdemo  \
        i18n      inplis     listbox   listbox1  listbox2   listbox3  \
        listbox4  listbox5   listbox6  listbox7                       \
        listvi    menunest   menuz     mess1     msgcls     password  \
        progba    sdlg       sdlg2     tcombo    terminal             \
        testhelp  tprogb     tutorial  tvedit    tvhc       videomode


.PHONY: clean all

all:
	@for d in $(DIRS); do         \
	    (cd $$d; make clean all)  \
	done

clean:
	@for d in $(DIRS); do     \
	    (cd $$d; make clean); \
	done;
	@rm -rf *~

