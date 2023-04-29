NAME = parseConf

SRC = main.cpp locationParse.cpp parse_conf.cpp serverParse.cpp

OBJ = $(SRC:%.cpp=%.o)

HDR = locationParse.hpp serverParse.hpp

FLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address

all: $(NAME)

$(NAME): $(OBJ) $(HDR)
	@c++ $(FLAGS) $(SRC) -o $(NAME)

%.o: %.cpp $(HDR)
	@c++ $(FLAGS) -c $< -o $@

clean:
	@rm $(OBJ)

fclean: clean
	@rm $(NAME)

re: fclean all
