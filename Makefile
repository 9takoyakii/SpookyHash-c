CC = gcc
CFLAGS = -O3 -Wall

all: test
test: test.o tinytest.o SpookyV1.o SpookyV2.o SpookyV1.h SpookyV2.h
test.o: test.c
tinytest.o: tinytest.c tinytest.h
SpookyV1.o: SpookyV1.c SpookyV1.h
SpookyV2.o: SpookyV2.c SpookyV2.h

clean:
	rm -f test tinytest.o test.o SpookyV1.o SpookyV2.o

.PHONY: clean
