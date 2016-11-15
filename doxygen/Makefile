
all: generate_doxygen_doc symlink_html compile_tex
	
generate_doxygen_doc: clean
	doxygen doxyfile	

symlink_html: FORCE
	ln -s generated_documentation/html/index.html valor.html

compile_tex: FORCE
	cd generated_documentation/latex/; pdflatex refman.tex; cp refman.pdf ../../valor.pdf
	
clean: FORCE
	[ ! -e ./valor.html ] || rm ./valor.html
	[ ! -e ./valor.pdf ]  || rm ./valor.pdf
	[ ! -d ./generated_documentation ] || rm -rf ./generated_documentation

FORCE:


