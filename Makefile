

CC = c++

CFLAGS = -Wall -Werror -Wextra -std=c++98

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

CGI = cgi-bin/cgi.a

INC = server/*.hpp

NAME = webserv

$(NAME) : $(LIB) $(CGI) $(OBJ_DIR) $(OFILES)
	@$(CC) $(CFLAGS) $(OFILES) $(LIB) $(CGI) -o $(NAME)
	@echo "done for webserv"

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

$(LIB) : force
	make -C parsingConf

$(CGI) : force
	make -C cgi-bin

$(OFILES): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp $(INC)
	@$(CC) -c $(CFLAGS) $< -o $@

clean :
	make clean -C parsingConf
	make clean -C cgi-bin
	@rm -rf $(OBJ_DIR)

fclean : clean
	make fclean -C parsingConf
	make fclean -C cgi-bin
	@rm -rf $(NAME)

force:

re : fclean all

all : $(NAME)

.PHONY : re all clean fclean
