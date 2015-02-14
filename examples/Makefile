DIRS = bhelp    clipboard demo      desklogo \
       dlgdsn   dyntxt    fonts    helpdemo  i18n     \
       listvi   msgcls    progba    \
       sdlg     sdlg2     tcombo   terminal  tprogb   \
       tutorial tvedit    tvhc     videomode inplis

.PHONY: clean all

all:
	@for d in $(DIRS); do         \
	    (cd $$d; make clean all)  \
	done

clean:
	@for d in $(DIRS); do     \
	    (cd $$d; make clean); \
	done;
