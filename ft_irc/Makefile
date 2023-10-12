# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zhamdouc <zhamdouc@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/03 13:32:10 by meshahrv          #+#    #+#              #
#    Updated: 2023/09/15 14:35:58 by zhamdouc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

####### COLORS #######

GREEN = "\033[38;5;150m"
LILA = "\033[38;5;140m"
BLUE = "\033[1;36m"
BOLD = "\033[1m"
NC = "\033[0m"

NAME = ircserv

CC = c++
CFLAGS = -Wall -Wextra -Werror -MMD
CPPFLAGS = -std=c++98

OBJ_DIR = objs/
SRC_DIR = src/
INC_DIR = inc/

SRC_SERVER = Server/Server.cpp Server/ServerCmds.cpp Server/ServerClean.cpp
SRC_USER = User/User.cpp
SRC_MSGS = Messages/Messages.cpp
SRC_CHAN = Channel/Channel.cpp Channel/ChannelSet.cpp Channel/ChannelGet.cpp

SRC = main.cpp \
		$(addprefix $(SRC_DIR), $(SRC_SERVER)) \
		$(addprefix $(SRC_DIR), $(SRC_USER)) \
		$(addprefix $(SRC_DIR), $(SRC_MSGS)) \
		$(addprefix $(SRC_DIR), $(SRC_CHAN)) \

OBJ = $(addprefix $(OBJ_DIR), $(SRC:.cpp=.o))

DEPS = $(addprefix $(OBJ_DIR), $(SRC:.cpp=.d))

INC = -I$(INC_DIR)

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJ) -o $(NAME)
	@echo ${LILA}${BOLD}Compiling ${NAME}${NC} [${GREEN}OK${NC}]

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	make --no-print-directory header
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(INC) -o $@ -c $<
	@echo ${BLUE}${BOLD}Building $< ${NC}

$(OBJ_DIR)$(SRC_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(INC) -o $@ -c $<
	@echo ${BLUE}${BOLD}Building $< ${NC}

clean:
	@rm -rf $(OBJ_DIR)
	@echo ${LILA}${BOLD}${NAME}${NC} [${GREEN}OK${NC}] Cleaning Objects : ${BOLD}${NAME}${NC}

fclean: clean
	@rm -f $(NAME)
	@echo ${LILA}${BOLD}${NAME}${NC} [${GREEN}OK${NC}] Cleaning All Objects : ${BOLD}${NAME}${NC}

re: fclean all

-include ${DEPS}

define HEADER
\033[92m
███████╗████████╗     ██╗██████╗  ██████╗        ██╗    ███╗   ███╗███████╗███████╗
██╔════╝╚══██╔══╝     ██║██╔══██╗██╔════╝       ██╔╝    ████╗ ████║╚══███╔╝██╔════╝
█████╗     ██║        ██║██████╔╝██║           ██╔╝     ██╔████╔██║  ███╔╝ █████╗  
██╔══╝     ██║        ██║██╔══██╗██║          ██╔╝      ██║╚██╔╝██║ ███╔╝  ██╔══╝  
██║        ██║███████╗██║██║  ██║╚██████╗    ██╔╝       ██║ ╚═╝ ██║███████╗██║     
╚═╝        ╚═╝╚══════╝╚═╝╚═╝  ╚═╝ ╚═════╝    ╚═╝        ╚═╝     ╚═╝╚══════╝╚═╝     
\033[0m                                                                                                           
endef
export HEADER

header :
		clear
		@echo "$$HEADER"

.PHONY: all clean fclean re