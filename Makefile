# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: osamoile <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/08/01 15:53:40 by osamoile          #+#    #+#              #
#    Updated: 2019/11/09 16:15:32 by osamoile         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME =			libft_malloc_$(HOSTTYPE).so
LINK_NAME =		libft_malloc.so
HEADER =		./includes/malloc.h \
				./includes/block.h
INC =			-I ./includes \
				-I ./libft/includes
CC_FLAGS =		-Wall -Wextra -Werror
SH_LIB_FLAGS =	-fvisibility=hidden -fPIC $(CC_FLAGS)

SRC_LIST =		malloc \
				dump \
				mmap \
				free \
				realloc
OBJ =			$(addprefix obj/, $(addsuffix .o, $(SRC_LIST)))
TESTS =			test0 \
				test1 \
				test2 \
				test3 \
				test4 \
				test5 \
				test6 \
				test7
				# TODO: bonuses test

OFF=\033[0m
PURPLE=\033[0;38;2;102;102;255m
PINK=\033[0;35m
PINKBOLD=\033[1;35m
WHITEBOLD=\033[1;37m
RED=\033[0;31m
REDBOLD=\033[1;31m

all: $(LINK_NAME) tests

$(LINK_NAME): $(NAME)
	@ln -sf $(NAME) $(LINK_NAME)
	@echo "$(PINKBOLD)$(LINK_NAME)$(PINK) linked$(OFF)"

$(NAME): ./libft/libft.a obj $(OBJ)
	@gcc -shared $(OBJ) ./libft/libft.a -o $(NAME)
	@strip -x $(NAME)
	@echo "$(PINKBOLD)$(NAME)$(PINK) ready$(OFF)"

./libft/libft.a:
	@make -C ./libft

obj:
	@mkdir obj

obj/%.o: src/%.c $(HEADER)
	@gcc $(SH_LIB_FLAGS) -c $< -o $@ $(INC)
	@echo "$(PURPLE)Compiling $(WHITEBOLD)$*.c $(PURPLE)done$(OFF)"

tests: $(LINK_NAME) $(TESTS)

%: tests/my/%.c
	@gcc $(CC_FLAGS) $< -o $@ $(INC) -lft -L ./libft -lft_malloc -L .
	@echo "$(WHITEBOLD)$@$(PURPLE) ready$(OFF)"

%: tests/subject/%.c
	@gcc $(CC_FLAGS) $< -o $@
	@echo "$(WHITEBOLD)$@$(PURPLE) ready$(OFF)"

clean:
	@rm -rf obj
	@echo "$(RED)Directory $(REDBOLD)obj$(RED) removed$(OFF)"

fclean: clean
	@rm -f $(LINK_NAME)
	@echo "$(RED)Link $(REDBOLD)$(LINK_NAME)$(RED) removed$(OFF)"
	@rm -f $(NAME)
	@echo "$(RED)Library $(REDBOLD)$(NAME)$(RED) removed$(OFF)"
	@rm -f $(TESTS)
	@echo "$(RED)Tests $(REDBOLD)$(TESTS)$(RED) removed$(OFF)"

re: fclean all

debug: CC_FLAGS += -D DEBUG
debug: re
# debug: tests
