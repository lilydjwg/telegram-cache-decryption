CC=gcc
CFLAGS=-O2 -Wall

.PHONY: all clean

all: ffi_openssl.so

ffi_openssl.so: ffi_openssl.c
	$(CC) $(CFLAGS) -shared ffi_openssl.c -o ffi_openssl.so -lcrypto

clean:
	-rm ffi_openssl.so
