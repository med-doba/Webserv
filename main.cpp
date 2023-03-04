#include "parse.hpp"


void	ft_ft(std::string str)
{
	std::cout << str << std::endl;
}

bool	ft_check_extention(std::string str)
{
	std::string	sub;
	int	i = -1;

	std::string	tmp[3] = {".py", ".php", ".html"};
	sub = str.substr(str.find_last_of("."));

	while(++i < 3)
	{
		if (sub == tmp[i])
			return true;
	}
	return false;
}

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

void	ft_check_double(std::vector<std::string> &container)
{
	std::vector<std::string>::iterator	it1, it2;
	int	count;

	for(it1 = container.begin(); it1 != container.end(); it1++)
	{
		count = 0;
		for (it2 = container.begin(); it2 != container.end(); it2++)
		{
			if(*it1 == *it2)
				count++;
		}
		if(count > 1)
		{
			std::cout << "error double\n";
			exit(1);
		}
			// throw ;//"double element"
	}
}

int	ft_occurrences_of_char(std::string &line, char c)
{
	int	count = 0;

	for(int i = 0; line.find(c, i) != std::string::npos; i++)
	{
		count++;
		if (count > 1)
			return -1;
		i = line.find(c, i);
	}
	return count;
}

std::vector<std::string>	ft_split(const std::string str, std::string split)
{
	std::vector<std::string> rtn;
	int	i = 0,j = 0;

	for (i = 0; str.find_first_of(split, i) != std::string::npos; i++)
	{
		j = str.find_first_of(split, i);
		//i need to prevent add empty line here
		rtn.push_back(str.substr(i, (j - i)));
		i = j;
	}
	if (i < str.length())
		rtn.push_back(str.substr(i, (j - i)));
	return rtn;
}

bool	ft_isDigit(std::string &str)
{
	int	i = -1;

	while (++i < str.size())
		if (!std::isdigit(str[i]))
			return false;
	return true;
}

void	ft_error(std::string msg)
{
	std::cerr << msg << std::endl;
}

bool	ft_check_cmbsize(std::string &str)
{
	std::string	tmp = str;

	if (!str.empty())
	{
		if (str.back() == 'M')
		{
			str.pop_back();
			if (ft_isDigit(str))
				return true;
			else
				str = tmp;
		}
	}
	return false;
}

bool	ft_checkRang_nbr(std::string str)
{
	int	index = 0;
	std::vector<std::string>	array;
	std::vector<std::string>::iterator	it;

	array = ft_split(str, ".");
	for (it = array.begin(); it != array.end(); it++)
	{
		if (!ft_isDigit(*it) || !(std::stoll(*it) >= 0 && std::stoll(*it) <= 255) || array.size() != 4)
			return false;
		// std::cout <<"it=" <<  *it << "$"<<std::endl;
	}
	return true;
}

int	main(void)
{
	parse	classconfig;
	std::string		lines;
	std::ifstream	file_conf("filetest.conf");


	bool	InTheServerBlock = false;
	bool	InTheLocationBlock = false;

	if (file_conf.is_open())
	{
		while(std::getline(file_conf, lines))
		{
			if (lines.empty())
				continue;
			ft_trim(lines);
			if (lines[0] == '#')
				continue;
			if (!lines.empty() && lines.back() == '{')
			{
				if (lines.find("server") != std::string::npos)
					InTheServerBlock = true;
				else if (InTheServerBlock && (lines.find("location") != std::string::npos))
					InTheLocationBlock = true;
				continue;
			}

			if(InTheServerBlock)
			{
				if (ft_occurrences_of_char(lines, ';') == -1)
					return (ft_error("error: occurrences_of_char"), 1);
				// ft_ft(lines);
				lines.pop_back();
				if (lines.find("listen") != std::string::npos)
				{
					classconfig.listen = ft_split(lines, " \t");
					classconfig.listen.back().pop_back();
					std::vector<std::string>::iterator it;
					for (it = (classconfig.listen.begin() + 1); it != classconfig.listen.end(); it++)
					{
						if (!ft_isDigit(*it))
							return (ft_error("listen: error is didgit"), 1);
					}
					ft_check_double(classconfig.listen);
				}
				else if (lines.find("client_max_body_size") != std::string::npos)
				{
					classconfig.client_max_body_size = ft_split(lines, " \t");
					if (classconfig.client_max_body_size.size() > 2)
						return (ft_error("ClMaxBodySize: error is didgit"), 1);
					if (!ft_check_cmbsize(classconfig.client_max_body_size[1]))
						return (ft_error("ClMaxBodySize: error cmbs"), 1);
				}
				else if (lines.substr(0, 4) == "host")
				{
					classconfig.host = ft_split(lines, " \t");
					std::vector<std::string>::iterator it;
					for (it = classconfig.host.begin(); it != classconfig.host.end(); it++)
					{
						if (!ft_checkRang_nbr(*(it +1)))
							return (ft_error("host: error rang"), 1);
					}
				}
				else if (lines.substr(0, 11) == "server_name")
				{
					classconfig.server_name = ft_split(lines, " \t");
					// std::vector<std::string>::iterator it;
					// for (it = classconfig.server_name.begin(); it != classconfig.server_name.end(); it++)
					// {
					// 	std::cout << "serve " << *it << "\n";
					// }
				}
				else if (lines.substr(0, 5) == "index")
				{
					classconfig.index = ft_split(lines, " \t");
					 std::vector<std::string>::iterator it;
					for (it = classconfig.index.begin() + 1 ; it != classconfig.index.end(); it++)
					{
						if (ft_check_extention(*it) == false)
							return (ft_error("index: error extention"), 1);
					}
				}
				else if (lines.find("error_page") != std::string::npos)
				{
					classconfig.error_page = ft_split(lines, " \t");
				}
				else if (lines.find("index") != std::string::npos)
				{
					classconfig.index = ft_split(lines, " \t");
				}
			}
			if (InTheLocationBlock)
			{
			}

			if (lines == "}")
			{
				if (InTheServerBlock)
					InTheServerBlock = false;
				if (InTheLocationBlock)
					InTheLocationBlock = false;
				//check errors
			}
			// std::cout << "----------------------------\n";
		}
		file_conf.close();
		// std::cout << "----------------------------\n";
		// std::vector<std::string>::iterator it1;
		// for (it1 = classconfig.listen.begin(); it1 != classconfig.listen.end(); it1++)
		// {
		// 	std::cout << *it1 << std::endl;
		// }
		// std::cout << "----------------------------\n";
		// std::vector<std::string>::iterator it2;
		// for (it2 = classconfig.client_max_body_size.begin(); it2 != classconfig.client_max_body_size.end(); it2++)
		// {
		// 	std::cout << *it2 << std::endl;
		// }
		// std::cout << "----------------------------\n";
	}
	return 0;
}