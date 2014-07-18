INCLUDE=-I../include/
LIBS=../lib/libkpnd.a -ljson -loauth
all:oauth download get_file_info upload get_user_info

oauth:
	gcc -o oauth oauth.c $(INCLUDE) $(LIBS)

download:
	gcc -o download download.c $(INCLUDE) $(LIBS)

get_file_info:
	gcc -o get_file_info get_file_info.c $(INCLUDE) $(LIBS)

upload:
	gcc -o upload upload.c $(INCLUDE) $(LIBS)

get_user_info:
	gcc -o upload upload.c $(INCLUDE) $(LIBS)

clean:
	rm -rfv oauth download get_file_info upload get_file_info
