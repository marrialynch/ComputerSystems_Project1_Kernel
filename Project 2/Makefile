#Makefile for stairs.c

CC = gcc
OUT = output.o
SRC = stairs.c
TXT1 = output1.txt
TXT2 = output2.txt
all: 	as7

as7: stairs.c 
	$(CC) -Wall -pthread -std=c99 -o $(OUT) $(SRC)
	chmod 700 $(OUT)
	 ./$(OUT) >$(TXT1)
	 ./$(OUT) >$(TXT2)

	
clean:
	rm -f $(OUT)
	rm -f $(SOUT)	
