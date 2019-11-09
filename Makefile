# TODO: libft_malloc_$HOSTTYPE.so
# Your Makefile will have to check the existence of the environment variable $HOSTTYPE. If it is empty or non-existant, to assign the following value:
# ‘uname -m‘_‘uname -s‘
# ifeq ($(HOSTTYPE),)
# HOSTTYPE := $(shell uname -m)_$(shell uname -s)
# endif
# • Your Makefile will have to create a symbolic link libft_malloc.so pointing to
# libft_malloc_$HOSTTYPE.so so for example:
# libft_malloc.so -> libft_malloc_intel-mac.so

# TODO:
# $> export HOSTTYPE = Тестирование
# ln -s libft_malloc_Testing.so libft_malloc.so
# $> ls -l libft_malloc.so
# libft_malloc.so -> libft_malloc_Testing.so
# $>
# Makefile хорошо использует HOSTTYPE, чтобы определить имя продавца книг
# (libft_malloc_ $ HOSSTYPE.so) и создает символическую ссылку libft_malloc.so, указывающую на
# libft_malloc_ $ HOSSTYPE.so?

# cat run.sh #!/bin/sh export DYLD_LIBRARY_PATH=. export
# DYLD_INSERT_LIBRARIES="libft_malloc.so" export DYLD_FORCE_FLAT_NAMESPACE=1 $@


all:
	gcc malloc.c testing.c -o malloc

debug:
	gcc malloc.c testing.c -o malloc -D DEBUG
	
# TODO:
# $> nm libft_malloc.so
# 0000000000000000 T _free
# 0000000000000000 T _malloc
# 0000000000000000 T _realloc
# 0000000000000000 T _show_alloc_mem
# U _mmap
# U _munmap
# U _getpagesize
# U _write
# U dyld_stub_binder
