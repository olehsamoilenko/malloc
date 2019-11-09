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

NAME =			malloc
HEADER =		malloc.h#./includes/asm.h
INC =			-I .#-I ./includes \
				-I ./libft/includes
LIB =			-lft -L libft
FLAGS =			#-Wall -Wextra -Werror
LIST =			malloc \
				testing
OBJ =			$(addprefix obj/, $(addsuffix .o, $(LIST)))

OFF=\033[0m
PURPLE=\033[0;35m
PURPLEBOLD=\033[1;35m
WHITE=\033[1;37m
PURPLELIGHT=\033[38;2;102;102;255m

all: $(NAME)

obj/%.o: src/%.c $(HEADER)
	@gcc $(FLAGS) -c $< -o $@ $(INC)
	@echo "$(PURPLELIGHT)Compiling $(WHITE)$< $(PURPLELIGHT)done$(OFF)"

$(NAME): ./libft/libft.a obj $(OBJ)
	@gcc $(OBJ) -o $(NAME) $(LIB)
	@echo "$(PURPLEBOLD)$(NAME)$(PURPLE) is ready$(OFF)"

./libft/libft.a:
	@make -C ./libft

obj:
	@mkdir obj

clean:
	@make -C ./libft clean
	@rm -rf obj
	
fclean: clean
	@make -C ./libft fclean
	@rm -f $(NAME)

re: fclean all
