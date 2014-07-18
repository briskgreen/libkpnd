CC=gcc
all:libkpnd.a libkpnd.so

libkpnd.a:kp.o kp_oauth.o kp_data.o kp_errno.o
	mkdir -p include/kp lib
	cp src/kp.h src/kp_oauth.h src/kp_data.h src/kp_errno.h include/kp/
	ar rcs libkpnd.a kp.o kp_oauth.o kp_data.o kp_errno.o
	cp libkpnd.a lib/

kp.o:src/kp.h src/kp.c 
	$(CC) -c src/kp.c

kp_oauth.o:src/kp_oauth.h src/kp_oauth.c 
	$(CC) -c src/kp_oauth.c

kp_data.o:src/kp_data.h src/kp_data.c 
	$(CC) -c src/kp_data.c

kp_errno.o:src/kp_errno.h src/kp_errno.c 
	$(CC) -c src/kp_errno.c 

libkpnd.so:kp.o kp_oauth.o kp_data.o kp_errno.o
	$(CC) -shared -fPIC -o libkpnd.so kp.o kp_oauth.o kp_data.o kp_errno.o -loauth -ljson
	cp libkpnd.so lib/

clean:
	rm -rfv include lib *.o *.a *.so

install:
	cp -r include/kp /usr/include/ 
	cp lib/libkpnd.a lib/libkpnd.so /usr/lib/

uninstall:
	rm -rfv /usr/include/kp/ /usr/lib/libkpnd.a /usr/lib/libkpnd.so
