
ufs: UFS.o main_ufs.o disque.o
	gcc main_ufs.o UFS.o disque.o  -g -o ufs

ufsTest: UFS.o mainTest.o disque.o
	gcc mainTest.o UFS.o disque.o -o ufsTest

TestEtudiant: UFS.o mainScriptTest.o disque.o
	gcc mainScriptTest.o UFS.o disque.o -o TestEtudiant

mainScriptTest.o: mainScriptTest.c
	gcc -c -g mainScriptTest.c

mainTest.o: mainTest.c
	gcc -c -g mainTest.c

prompt.o: prompt.c
	gcc -c -g prompt.c

UFS.o: UFS.c
	gcc -c -g UFS.c

main_ufs.o: main_ufs.c
	gcc -c -g main_ufs.c

disque.o: disque.c
	gcc -c -g disque.c
	
clean: 
	rm -f *.o ufs ufsTest glofs prompt *.*~
