# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rostrub <rostrub@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/05 14:09:59 by vsozonof          #+#    #+#              #
#    Updated: 2025/04/21 16:21:25 by rostrub          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CC = c++
CPPFLAGS = -Wall -Werror -Wextra -std=c++98

BL=\033[1;34m
GR=\033[1;32m
OR=\033[1;33m
RD=\033[1;31m
WH=\033[0m

SRCS = main.cpp \
	   srcs/Parsing.cpp \
	   srcs/Server/Server.cpp \
	   srcs/Server/client_handler.cpp \
	   srcs/Server/msg_handler.cpp \
	   srcs/Server/registrationUtils.cpp \
	   srcs/Client/Client.cpp \
	   srcs/Salon/Salon.cpp \
	   srcs/commands/command.cpp \


DEPS = includes/inc.hpp \
	   srcs/Server/Server.hpp \
	   srcs/Client/Client.hpp \
	   srcs/Salon/Salon.hpp \
	   srcs/commands/command.hpp \


OBJS = $(SRCS:.cpp=.o)

RM = rm -f

all: $(NAME) ready

$(NAME): $(OBJS)
	@echo "\n$(RD)FT_IRC -\t$(WH)$(BL)Linking $(OR)($<)$(WH)\r"
	@$(CC) $(CPPFLAGS) $(OBJS) -o $(NAME)
	@echo "                                   \r\c"

%.o: %.cpp $(DEPS)
	@echo "$(RD)FT_IRC -\t$(WH)$(BL)Compiling $(OR)($<)$(WH)\c\r"
	@$(CC) $(CPPFLAGS) -c $< -o $@
	@echo "                                   \r\c"

clean:
	@echo "$(RD)FT_IRC -\t$(WH)$(BL)CLEANING $(OR)$(WH)"
	@$(RM) $(OBJS)
	@echo "$(RD)FT_IRC -\t$(WH)$(BL)CLEANED $(OR)$(WH)"

fclean:
	@echo "$(RD)FT_IRC -\t$(WH)$(BL)FCLEANING $(OR)$(WH)"
	@$(RM) $(NAME) $(OBJS)
	@echo "$(RD)FT_IRC -\t$(WH)$(BL)FCLEANED $(OR)$(WH)"


ready:
	@echo "$(OR)Welcome to ...\n"
	@echo "  ▄████████     ███                ▄█     ▄████████  ▄████████"
	@echo "  ███    ███ ▀█████████▄           ███    ███    ███ ███    ███"
	@echo "  ███    █▀     ▀███▀▀██           ███▌   ███    ███ ███    █▀ "
	@echo " ▄███▄▄▄         ███   ▀           ███▌  ▄███▄▄▄▄██▀ ███       "
	@echo "▀▀███▀▀▀         ███               ███▌ ▀▀███▀▀▀▀▀   ███       "
	@echo "  ███            ███               ███  ▀███████████ ███    █▄ "
	@echo "  ███            ███               ███    ███    ███ ███    ███"
	@echo "  ███           ▄████▀             █▀     ███    ███ ████████▀ "
	@echo "                                          ███    ███           $(WH)"
	@echo "\n  $(OR)Usage:$(WH) ./ircserv [port] [password]"
	@echo "  $(OR)- port$(WH) - port number to listen on"
	@echo "  $(OR)- password$(WH) - server password"
	@echo "  $(OR)- Example:$(WH) ./ircserv 6666 prout\n"

re: fclean all

.PHONY: all clean fclean re
