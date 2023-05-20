#include "server.hpp"

int main(int ac, char **av)
{
	signal(SIGPIPE, SIG_IGN);
	MapType	bind_info;
	std::string confFile;
	try
	{
		if (ac == 1)
			confFile = "confDir/configuration.conf";
		else if (ac == 2)
			confFile = av[1];
		else
			throw(std::runtime_error("Error: unexcepted arguments"));
		server obj;
		obj.block = ft_parse_conf(confFile.c_str(), bind_info);
		obj.fill(bind_info);
		obj.lunch_servers();
		obj.monitor();
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}
