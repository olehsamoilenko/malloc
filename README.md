# malloc
This project is about implementing a dynamic memory allocation mechanism<br>
The first implementation was created accordingly to [stepik guide](https://stepik.org/lesson/44328/step/1?unit=22140).<br>
Works on :penguin: Linux and :green_apple: MacOS

Compile library
===
Clone: ```$ git clone --recursive https://github.com/olehsamoilenko/malloc.git```<br>
Compile: ```$ make```<br>
Compile debug version: ```$ make debug```<br>
Again usual version: ```$ make re```<br>

Run tests
===
To run tests please use: ```$ ./run.sh ./test```<br>
To show malloc performance please use:<br>
```$ ./run.sh /usr/bin/time -l ./test``` on :green_apple: MacOS or<br>
```$ ./run.sh /usr/bin/time --verbose ./test``` on :penguin: Linux<br>

Use library
===
```$ gcc program.c [-lft_malloc -L .]```<br>
```$ ./run.sh ./a.out```

Features
===
* memory defragmentation
* unmap-ing unused pages
* memory hexdump with ```show_alloc_mem_ex()```
* debug enviroment variable ```DEBUG```
* test framework: ```$ make debug && sh tests/run_tests.sh```
* integrated with ls: ```$ ./run.sh 'ls -lRa /'``` (sorry, only for :green_apple: MacOS)
* nm cleared of redundant functions
