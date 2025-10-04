default:
	cd makeprocs; make
	cd injector_1; make
	cd injector_2; make
	cd reaction_1; make
	cd reaction_2; make
	cd reaction_3; make

clean:
	cd makeprocs; make clean
	cd injector_1; make clean
	cd injector_2; make clean
	cd reaction_1; make clean
	cd reaction_2; make clean
	cd reaction_3; make clean

run:
	cd ../../bin; dlxsim -x os.dlx.obj -a -u makeprocs.dlx.obj 3 5 ; ee469_fixterminal
