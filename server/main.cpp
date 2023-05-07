#include "server.hpp"

int main(int ac, char **av)
{
	// signal(SIGPIPE, SIG_IGN);
	MapType	bind_info;

	try
	{
		if (ac != 2)
			throw(std::runtime_error("Wrong Number Of Arguments"));
		server obj;
		obj.block = ft_parse_conf(av[1], bind_info);
		obj.fill(bind_info);
		obj.lunch_servers();
		obj.monitor();
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}
