/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-doba <med-doba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 11:29:45 by med-doba          #+#    #+#             */
/*   Updated: 2023/04/19 19:06:19 by med-doba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <stdlib.h>


int	main(void)
{
	MapType	bind_info;
	
	ft_parse_conf("configuration.conf", bind_info);
	return 0;
}
