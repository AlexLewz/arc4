#
# test/Makefile
#

CC=		gcc
CFLAGS=	-std=c99
MESG=	usage.c
MTMP=	usage.out.c
MOUT=	usage.out
KTMP=	usage.key


EXE=	arc4
OBJ=	usage.o rw_stack.o ar4ive2.o ar4_256.o
SRC=	usage.c rw_stack.c ar4ive2.c ar4_256.c


$(EXE): $(OBJ)
	@$(CC) $(OBJ) -o $(EXE)

$(OBJ):
	@$(CC) -c $(SRC) $(CFLAGS)

clean:
	@rm -f $(EXE) *.o *.obj

cleanall:
	@rm -f $(EXE) *.o *.out *.key *.obj $(MTMP) $(MOUT) $(KTMP)

rebuild: cleanall $(EXE)

encrypt:
	./$(EXE) $(MESG) > $(MOUT) 2> $(KTMP)
decrypt:
	./$(EXE) -dk $(KTMP) $(MOUT) > $(MTMP)

encryptn:
	./$(EXE) -r 128 -n $(MESG) > $(MOUT) 2> $(KTMP)
decryptn:
	./$(EXE) -r 128 -n -d -k $(KTMP) $(MOUT) > $(MTMP)


