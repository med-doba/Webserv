NAME = parseConf.a

SRC = locationParse.cpp parse_conf.cpp serverParse.cpp

OBJ = $(SRC:%.cpp=%.o)

HDR = locationParse.hpp serverParse.hpp

FLAGS = -Wall -Wextra -Werror #-std=c++98 -fsanitize=address

all: $(NAME)

$(NAME): $(OBJ) $(HDR)
	@ar -rc $(NAME) $(OBJ)

%.o: %.cpp $(HDR)
	@c++ $(FLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all
