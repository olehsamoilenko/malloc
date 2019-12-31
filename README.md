# malloc
This project is about implementing a dynamic memory allocation mechanism<br>
The first implementation was created accordingly to [stepik guide](https://stepik.org/lesson/44328/step/1?unit=22140).

Compile library
===
Compile: ```make```<br>
Compile debug version: ```make re DEBUG=1```<br>
Again usual version: ```make re``` or ```make re DEBUG=0```<br>

Run tests
===
To run tests please use: ```./run.sh test```<br>
To show malloc performance please use:<br>
```./run.sh /usr/bin/time -l test``` on Mac or<br>
```./run.sh /usr/bin/time --verbose test``` on Linux<br>

Use library
===
Usage 1.<br>
```gcc -flat_namespace program.c```<br>
```./run.sh ./a.out```
# TODO: check usages

Usage 2.<br>
```gcc -lft_malloc -L . program.c```<br>
```./a.out```