all:
	gcc main.c -o out
	gcc -shared -fPIC intercept.c -o malloc.so
	LD_PRELOAD=./malloc.so ./out