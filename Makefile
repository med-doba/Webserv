

CC = c++

CFLAGS = -Wall -Werror -Wextra -g -fsanitize=address,undefined,integer -std=c++98

CFILES = main.cpp\
		client.cpp\
		miniserver.cpp\
		server.cpp\
		parssingOfBody.cpp\
		parssingOfHeader.cpp\
		response.cpp\


OFILES = $(addprefix $(OBJ_DIR)/,$(CFILES:.cpp=.o))

OBJ_DIR = ./server/OBJ

SRC_DIR = ./server

LIB = parsingConf/parseConf.a

INC = server/*.hpp

NAME = webserv

$(NAME) : $(LIB) $(OBJ_DIR) $(OFILES)
	@$(CC) $(CFLAGS) $(OFILES) $(LIB) -o $(NAME)
	@echo "done for webserv"

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

$(LIB) : force
	make -C parsingConf

$(OFILES): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp $(INC)
	@$(CC) -c $(CFLAGS) $< -o $@

clean :
	make clean -C parsingConf
	@rm -rf $(OBJ_DIR)

fclean : clean
	make fclean -C parsingConf
	@rm -rf $(NAME)

force:

re : fclean all

all : $(NAME)

.PHONY : re all clean fclean
