#PHONY only evokes all if there is a change (gets rid of nothing to do print message)
.PHONY: all 

all: pfind
	@:

pfind: pfind.o 
	@gcc pfind.o -o pfind

pfind.o: pfind.c
	@gcc -c -g pfind.c


clean: 
	@rm -rf *.o pfind
	

