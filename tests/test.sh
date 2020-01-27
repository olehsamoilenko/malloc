# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test.sh                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: osamoile <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/22 17:00:01 by osamoile          #+#    #+#              #
#    Updated: 2019/12/29 15:00:06 by osamoile         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/bin/sh

cd $(dirname $0)/..
if [ $(uname -s) = Darwin ]
then
    ./test5
    ./test6
    ./test7
else
    echo 
    sh run.sh ./test5
    sh run.sh ./test6
    sh run.sh ./test7
fi
