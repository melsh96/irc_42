# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: meshahrv <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/03 13:32:10 by meshahrv          #+#    #+#              #
#    Updated: 2023/07/04 17:41:39 by meshahrv         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

####### COLORS #######

GREEN = "\033[38;5;150m"
CYAN = "\033[38;5;140m"
BOLD = "\033[1m"
NC = "\033[0m"

NAME = ircserv

CC = c++
CFLAGS = -Wall -Wextra -Werror -g3
CPPFLAGS = -std=c++98

OBJ_DIR = objs/
SRC_DIR = src/
INC_DIR = inc/

SRC_SERVER = Server/Server.cpp Server/ServerCmds.cpp Server/ServerClean.cpp
SRC_USER = User/User.cpp
SRC_MSGS = Messages/Messages.cpp

SRC = main.cpp \
		$(addprefix $(SRC_DIR), $(SRC_SERVER)) \
		$(addprefix $(SRC_DIR), $(SRC_USER)) \
		$(addprefix $(SRC_DIR), $(SRC_MSGS)) \

OBJ = $(addprefix $(OBJ_DIR), $(SRC:.cpp=.o))

INC = -I$(INC_DIR)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJ) -o $(NAME)
	@echo ${CYAN}Compiling ${NAME}${NC} [${GREEN}OK${NC}]

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INC) -o $@ -c $<

$(OBJ_DIR)$(SRC_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INC) -o $@ -c $<

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re