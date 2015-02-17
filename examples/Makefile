DIRS = bhelp     clipboard  cyrillic  demo      desklogo   dlgdraw   \
       dlgdsn    dumb       dyntxt    emodal    fonts      helpdemo  \
       i18n      inplis     listbox   listbox1  listbox2   listbox3  \
       listbox4  listbox5   listbox6  listbox7  listbox8   listbox9  \
       listvi    menunest   menuz     mess1     msgcls     password  \
       progba    sdlg       sdlg2     tcombo    terminal   test1     \
       testhelp  tprogb     tutorial  tvedit    tvhc       videomode \
       viewers

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

