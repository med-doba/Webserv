

// void	ft_show(server &classconfig)
// {
// 	std::cout << "----------------------------\n";
// 	std::vector<std::string>::iterator it1;
// 	for (it1 = classconfig.error_page.begin(); it1 != classconfig.error_page.end(); it1++)
// 		std::cout << *it1 << std::endl;
// 	std::cout << "----------------------------\n";
// 	std::vector<std::string>::iterator it2;
// 	for (it2 = classconfig.index.begin(); it2 != classconfig.index.end(); it2++)
// 		std::cout << *it2 << std::endl;
// 	std::cout << "----------------------------\n";
// 	std::vector<std::string>::iterator it3;
// 	for (it3 = classconfig.root.begin(); it3 != classconfig.root.end(); it3++)
// 		std::cout << *it3 << std::endl;
// 	std::cout << "----------------------------\n";
// 	std::vector<std::string>::iterator it4;
// 	for (it4 = classconfig.server_name.begin(); it4 != classconfig.server_name.end(); it4++)
// 		std::cout << *it4 << std::endl;
// 	std::cout << "----------------------------\n";
// 	std::vector<std::string>::iterator it2;
// 	for (it2 = classconfig.client_max_body_size.begin(); it2 != classconfig.client_max_body_size.end(); it2++)
// 		std::cout << *it2 << std::endl;
// 	std::cout << "----------------------------\n";
// 	std::vector<std::string>::iterator it2;
// 	for (it2 = classconfig.listen.begin(); it2 != classconfig.listen.end(); it2++)
// 		std::cout << *it2 << std::endl;
// 	std::cout << "----------------------------\n";
// 	std::vector<std::string>::iterator it2;
// 	for (it2 = classconfig.error_page.begin(); it2 != classconfig.error_page.end(); it2++)
// 		std::cout << *it2 << std::endl;
// 	std::cout << "----------------------------\n";
// 	std::vector<std::string>::iterator it2;
// 	for (it2 = classconfig.host.begin(); it2 != classconfig.host.end(); it2++)
// 		std::cout << *it2 << std::endl;
// 	std::cout << "----------------------------\n";
// 	std::vector<std::string>::iterator it2;
// 	for (it2 = classconfig.location->allow.begin(); it2 != classconfig.location->allow.end(); it2++)
// 		std::cout << *it2 << std::endl;
// 	std::cout << "----------------------------\n";
// 	std::vector<std::string>::iterator it2;
// 	for (it2 = classconfig.location->autoindex.begin(); it2 != classconfig.location->autoindex.end(); it2++)
// 		std::cout << *it2 << std::endl;
// 	std::cout << "----------------------------\n";
// 	std::vector<std::string>::iterator it2;
// 	for (it2 = classconfig.location->cgi.begin(); it2 != classconfig.location->cgi.end(); it2++)
// 		std::cout << *it2 << std::endl;
// 	std::cout << "----------------------------\n";
// 	std::vector<std::string>::iterator it2;
// 	for (it2 = classconfig.location->rtn.begin(); it2 != classconfig.location->rtn.end(); it2++)
// 		std::cout << *it2 << std::endl;
// 	std::cout << "----------------------------\n";
// }