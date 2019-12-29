# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    run.sh                                             :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: osamoile <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/22 17:00:01 by osamoile          #+#    #+#              #
#    Updated: 2019/12/29 15:00:06 by osamoile         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/bin/sh

if [ $(uname -s) = Darwin ]
then
    export DYLD_LIBRARY_PATH=.
    export DYLD_INSERT_LIBRARIES="libft_malloc.so"
	export DYLD_FORCE_FLAT_NAMESPACE=1
	$@
else
    export LD_LIBRARY_PATH=.
    LD_PRELOAD=./libft_malloc.so $@
fi
