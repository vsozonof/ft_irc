# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vsozonof <vsozonof@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/05 14:09:59 by vsozonof          #+#    #+#              #
#    Updated: 2024/07/23 15:22:30 by vsozonof         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CC = c++
CPPFLAGS = -Wall -Werror -Wextra -std=c++98

SRCS = main.cpp \
	   srcs/Parsing.cpp \
	   srcs/Server/Server.cpp \
	   
	   
DEPS = includes/inc.hpp \
	   srcs/Server/Server.hpp \
	   

OBJS = $(SRCS:.cpp=.o)

RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp $(DEPS)
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
		$(RM) $(OBJS)

fclean: 
		$(RM) $(NAME) $(OBJS)

re: fclean all

.PHONY: all clean fclean re