
build: FORCE clean
	mkdir build
	ln -s $(shell readlink -m colorgcc.1.3.2_mod.pl) build/g++
	ln -s $(shell readlink -m colorgcc.1.3.2_mod.pl) build/c++
	ln -s $(shell readlink -m colorgcc.1.3.2_mod.pl) build/gcc
	ln -s $(shell readlink -m colorgcc.1.3.2_mod.pl) build/cc

clean:
	[ ! -d build ] || rm -r build ;

FORCE:


