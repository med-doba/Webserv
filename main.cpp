// #include "parse.hpp"
#include <iostream>
#include <fstream>
#include <vector>


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
			throw ;//"double element"
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

std::vector<std::string>	ft_split(const std::string str)
{
	std::vector<std::string> rtn;
	int	i = 0,j = 0;

	for (i = 0; str.find(" ", i) != std::string::npos; i++)
	{
		j = str.find(" ", i);
		rtn.push_back(str.substr(i, (j - i)));
		i = j;
	}
	rtn.push_back(str.substr(i, (j - i)));
	return rtn;
}

int	main(void)
{
	std::string		lines;
	std::ifstream	file_conf("configuration.conf");


	bool	InTheServerBlock = false;
	bool	InTheLocationBlock = false;

	if (file_conf.is_open())
	{
		while(std::getline(file_conf, lines))
		{
			ft_trim(lines);
			if (lines.empty() || lines[0] == '#')
				continue;

			if (lines.back() == '{')
			{
				if (lines.find("server"))
					InTheServerBlock = true;
				else if (InTheServerBlock && lines.find("location"))
					InTheLocationBlock = true;
				continue;
			}

			if(InTheServerBlock)
			{
				if (lines.find("listen"))
				{}
				else if (lines.find("client_max_body_size"))
				{}
				else if (lines.find("host"))
				{}
				else if (lines.find("index"))
				{}
				else if (lines.find("sever_name"))
				{}
				else if (lines.find("error_page"))
				{}
			}
			else if (InTheLocationBlock)
			{}

			if (lines == "}")
			{
				if (InTheServerBlock)
					InTheServerBlock = false;
				if (InTheLocationBlock)
					InTheLocationBlock = false;
				//check errors
			}
		}
	}
	return 0;
}