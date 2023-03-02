// #include "parse.hpp"
#include <iostream>
#include <fstream>


void	ft_trim(std::string &str)
{
	int	i,j;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	str.erase(0, i);
	i = str.size();
	j = 0;
	while (str[i] == ' ' || str[i--] == '\t')
		j++;
	str.erase(i, j);
}

int	ft_check_double(std:: &str)
{

}

int	main(void)
{
	std::string		lines;
	std::ifstream	file_conf("configuration.conf");

	if (file_conf.is_open())
	{
		while(std::getline(file_conf, lines))
		{
			//skip empty line and commentaires.
			if (lines.empty() || lines[0] == '#')
				continue;
			std::cout << lines << std::endl;
			ft_trim(lines);
			std::cout << lines << std::endl;
			if (lines == "server")
			{
				// if (lines == "listen")
				// if (lines == "client_max_body_size")
				// if (lines == "host")
				// if (lines == "index")
				// if (lines == "sever_name")
				// if (lines == "error_page")
				// if (lines == "location")
				std::cout << "lolo\n";
			}
			else
			{
				std::cerr << "server block not found\n";
				return 1;
			}
		}
	}
	else
	{
		std::cerr << "file not open\n";
		return 1;
	}

	return 0;
}