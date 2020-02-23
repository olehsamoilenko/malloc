# malloc
This project is about implementing a dynamic memory allocation mechanism<br>
The first implementation was created accordingly to [stepik guide](https://stepik.org/lesson/44328/step/1?unit=22140).

Compile library
===
Compile: ```make```<br>
Compile debug version: ```make debug```<br>
Again usual version: ```make re```<br>

Run tests
===
To run tests please use: ```./run.sh ./test```<br>
To show malloc performance please use:<br>
```./run.sh /usr/bin/time -l ./test``` on Mac or<br>
```./run.sh /usr/bin/time --verbose ./test``` on Linux<br>

Use library
===
```gcc program.c```<br>
```./run.sh ./a.out```

Features
===
* memory defragmentation
* unmap-ing unused pages
* memory hexdump with show_alloc_mem_ex()
* debug enviroment variable DEBUG