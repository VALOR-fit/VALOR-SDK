#~ cgcc = $(shell readlink -m colorgcc.1.3.2_mod.pl)
cgcc = $(shell perl -e 'use Cwd "abs_path"; print abs_path(shift)."\n";' colorgcc.1.3.2_mod.pl)

build: FORCE clean
	mkdir build
	ln -s "${cgcc}" "build/g++"
	ln -s "${cgcc}" "build/c++"
	ln -s "${cgcc}" "build/gcc"
	ln -s "${cgcc}" "build/cc"

clean:
	[ ! -d build ] || rm -r build ;

FORCE:


