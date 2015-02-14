all:
	cd classes; make
	cd compat; make
	cd stream; make
	cd names; make
	cd old_dep; make
	ar cr libtv.a classes/*.o compat/*.o stream/*.o names/*.o old_dep/*.o


