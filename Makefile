all:
	cd classes;  make
	cd compat;   make
	cd stream;   make
	cd names;    make
	cd old_dep;  make
	ar cr libtv.a classes/*.o compat/*.o stream/*.o names/*.o old_dep/*.o
	cd examples; make

clean:
	cd classes;  make clean
	cd compat;   make clean
	cd stream;   make clean
	cd names;    make clean
	cd old_dep;  make clean
	cd examples; make clean
	rm -rf *.a
	rm -rf *~
