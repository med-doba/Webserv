/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-doba <med-doba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 13:39:48 by med-doba          #+#    #+#             */
/*   Updated: 2023/02/12 16:11:43 by med-doba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "stdio.h"
#include <string>

int	main(int ac, char **av)
{
	int sockfd;
	const int	PORT = 8080;
	struct sockaddr_in address;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		return (std::cerr << "error: socket\n", 0);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(sockfd, (struct sockaddr*)&address, sizeof(address)) == -1)
		return (std::cerr << "error: bind\n", 0);

	
	return 0;
}