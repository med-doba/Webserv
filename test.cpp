/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-doba <med-doba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 11:26:58 by med-doba          #+#    #+#             */
/*   Updated: 2023/03/03 11:30:34 by med-doba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::ifstream file("nginx.conf");

	if (!file.is_open())
	{
		std::cerr << "Failed to open configuration file" << std::endl;
		return 1;
	}

	bool in_http_block = false;
	bool in_server_block = false;
	bool in_location_block = false;
	std::string current_location;

	std::string server_name, listen, root, index;

	std::string line;
	while (std::getline(file, line))
	{
		line.erase(0, line.find_first_not_of(" \t"));
		line.erase(line.find_last_not_of(" \t") + 1);

		if (line.empty() || line[0] == '#')
		{
			continue;
		}

		if (line[line.length() - 1] == '{')
		{
			if (line == "http {") {
				in_http_block = true;
			} else if (in_http_block && line == "server {") {
				in_server_block = true;
			} else if (in_server_block && line.substr(0, 9) == "location ") {
				in_location_block = true;
				current_location = line.substr(9, line.length() - 10);
			}
			continue;
		}

		if (line == "}")
		{
			if (in_location_block) {
				in_location_block = false;
			} else if (in_server_block) {
				in_server_block = false;
			} else if (in_http_block) {
				in_http_block = false;
			}
			continue;
		}

		if (in_location_block)
		{
			if (line == "root /var/www/html;") {
				root = "/var/www/html";
			} else if (line == "index index.html;") {
				index = "index.html";
			} else if (line == "root /var/www/images;") {
				root = "/var/www/images";
			} else if (line == "index index.html;") {
				index = "index.html";
			}
        }
		else if (in_server_block)
		{
            if (line.substr(0, 14) == "server_name ")
			{
                server_name = line.substr(14, line.length() - 15);
            }
			else if (line.substr(0, 7) == "listen ")
			{
                listen = line.substr(7, line.length() - 8);
			}
		}
	}
}
