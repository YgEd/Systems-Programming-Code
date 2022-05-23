#PHONY only evokes all if there is a change (gets rid of nothing to do print message)
.PHONY: all 

all: spfind
	@:

spfind: finds.o 
	@gcc finds.o -o spfind

finds.o: finds.c
	@gcc -c -g finds.c


clean: 
	@rm -rf *.o spfind
	

