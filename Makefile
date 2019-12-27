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
CC_FLAGS =		-fvisibility=hidden -fPIC # TODO: -Wall -Wextra -Werror
TEST_CC_FLAGS =	-I ./includes # TODO: refactor
ifeq ($(shell uname), Darwin)
	TEST_CC_FLAGS += -flat_namespace
endif
SRC_LIST =		malloc \
				dump \
				mmap
OBJ =			$(addprefix obj/, $(addsuffix .o, $(SRC_LIST)))
TESTS =			test0 \
				test1 \
				test2 \
				test9
#gcc -o test3 tests/test3.c # TODO: uncomment when realloc is done
#gcc -o test3b tests/test3b.c # TODO: uncomment when realloc is done
#gcc -o test4 tests/test4.c # TODO: uncomment when realloc is done
# TODO: test5
# TODO: bonuses

OFF=\033[0m
PURPLE=\033[0;38;2;102;102;255m
PINK=\033[0;35m
PINKBOLD=\033[1;35m
WHITEBOLD=\033[1;37m

ifeq ($(DEBUG), 1)
	DEBUGFLAG=-D DEBUG
endif

all: $(LINK_NAME) $(TESTS)

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
	@gcc $(CC_FLAGS) -c $< -o $@ $(INC) $(DEBUGFLAG)
	@echo "$(PURPLE)Compiling $(WHITEBOLD)$*.c $(PURPLE)done$(OFF)"

%: tests/subject/%.c
	@gcc $(TEST_CC_FLAGS) $< -o $@
	@echo "$(WHITEBOLD)$@$(PURPLE) ready$(OFF)"

%: tests/my/%.c # TODO: get rid of my
	@gcc $< -o $@ $(INC) -lft_malloc -L .
	@echo "$(WHITEBOLD)$@$(PURPLE) ready$(OFF)"

clean:
	@rm -rf obj # TODO: console log

fclean: clean
	@rm -f $(NAME) $(LINK_NAME) $(TESTS)

re: fclean all
