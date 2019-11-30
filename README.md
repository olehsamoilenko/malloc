# malloc
This project is about implementing a dynamic memory allocation mechanism<br>
The first implementation was created accordingly to [this Stepik guide](https://stepik.org/lesson/44328/step/1?unit=22140).

1. base implementation
2. avoid memory fragmentation
3. blocks size distribution
4. **mmap** several pages if there is no suitable place

Compile: ```make```<br>
Compile debug version: ```make re DEBUG=1```<br>
Again usual version: ```make re``` OR ```make re DEBUG=0```<br>
