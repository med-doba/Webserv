/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   script_php.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-doba <med-doba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 06:25:03 by med-doba          #+#    #+#             */
/*   Updated: 2023/04/23 22:20:58 by med-doba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <fcntl.h>

// std::cout << "Content-Type: text/html\r\n\r\n";
// std::cout << "<html><body>";
// std::cout << "<h1>GET Request Received</h1>";
// std::cout << "<p>Query String: " << query_string << "</p>";
// std::cout << "</body></html>";--GET

// std::cout << "Content-Type: text/html\r\n\r\n";
// std::cout << "<html><body>";
// std::cout << "<h1>POST Request Received</h1>";
// std::cout << "<p>Content Type: " << content_type << "</p>";
// std::cout << "<p>Content Length: " << length << "</p>";
// std::cout << "<p>Request Body: " << request_body << "</p>";
// std::cout << "</body></html>";--POst

//-cookies
// Adding support for cookies and session management in a C++98 HTTP serverParse is a bit more involved than in more modern programming languages and frameworks.
// C++98 does not have built-in support for cookies and sessions,
// so you will need to implement these features yourself.
// Here are some general steps you can follow:

// Parse incoming requests: Your HTTP serverParse will need to parse incoming
// requests to extract any cookies that the client has sent.
// You can do this by examining the "Cookie" header of the incoming request.
// If the header contains cookies,
// you can extract the cookie values and store them in a data structure.

// Set cookies in outgoing responses: When your serverParse needs to set a cookie,
// you can do so by including a "Set-Cookie" header in the outgoing response.
// The header should include the cookie name, value, and any other relevant information,
// such as the cookie's expiration time or path.

// Implement session management: To implement session management,
// you will need to create a data structure that can store session data.
// When a client makes a request, your serverParse can check if the request includes a session ID cookie.
// If it does, your serverParse can use the session ID to look up the session data in your data structure.
// If the session data exists, your serverParse can use it to provide personalized content to the client.

// Create and delete sessions: When a client logs in to your application,
// your serverParse can create a new session ID and
// store the user's session data in your data structure.
// When the user logs out or their session expires, your serverParse can delete the session data.

// Overall, adding support for cookies and session management in a C++98 HTTP serverParse
// requires more manual implementation than in more modern programming languages and
// frameworks. However, with some careful planning and implementation,
// you can provide these features to your users.



Adding support for cookies and session management to an HTTP serverParse using C++98 involves implementing the relevant functionality in your serverParse code. Here's a high-level overview of the steps you would need to take:

    Parse the incoming HTTP request headers to extract any cookies that may have been included in the request. The Cookie header is typically used for this purpose.

    If a session cookie is present, look up the associated session data in your serverParse's session store. If no session cookie is present, generate a new session ID and store it in a new session object in your session store.

    Associate any relevant user data (e.g. user ID, name, etc.) with the session object.

    Generate a new session cookie containing the session ID and add it to the outgoing HTTP response headers.

    Send the response back to the client, including the session cookie if applicable.

    On subsequent requests from the client, repeat steps 1-3 to associate the request with the correct session object.

There are many libraries available that can help with session management and cookie handling in C++98. For example, you might consider using the Boost C++ libraries or the Poco C++ libraries. Alternatively, you could implement the functionality from scratch using the standard C++ libraries.


Sure! Here's a more detailed explanation of how to implement session management in an HTTP serverParse using C++98:

    Define a Session Object: To implement session management, you will need to define a session object that stores session data. This object should contain all the relevant data associated with the session, such as a session ID, user ID, and any other user-specific data that needs to be stored.

    Generate a Session ID: When a new user connects to your serverParse, you will need to generate a unique session ID for them. You can use a random number generator or a hash function to generate the ID.

    Store Session Object: Once you have generated a session ID, you will need to create a new session object and store it in your session store. Your session store could be a database or a simple in-memory data structure like a map or a hash table.

    Associate Session ID with User: Once you have created the session object and stored it in your session store, you will need to associate the session ID with the user. This could be done by storing the session ID in a cookie and sending it to the user's browser, or by embedding it in the URL of each subsequent request.

    Retrieve Session Object: When the user sends a subsequent request to your serverParse, you will need to retrieve the associated session object from your session store. You can do this by extracting the session ID from the cookie or URL, and then looking up the corresponding session object in your session store.

    Update Session Object: As the user interacts with your serverParse, you may need to update the session object with new data. For example, if the user logs in, you may want to update the session object with their user ID. Make sure to save the updated session object back to your session store.

    Expire Session: Finally, you will need to expire the session when it is no longer needed. This could be done by setting an expiration time for each session object and deleting it from the session store when it expires. Alternatively, you could use a garbage collection mechanism to periodically delete expired session objects from the session store.

By implementing these steps, you can create a simple session management system that allows you to associate user-specific data with each user session. This can be useful for implementing features like user authentication, shopping carts, and other personalized content.


Adding support for cookies and session management in a C++98 HTTP serverParse is a bit more involved than in more modern programming languages and frameworks. C++98 does not have built-in support for cookies and sessions, so you will need to implement these features yourself. Here are some general steps you can follow:

    Parse incoming requests: Your HTTP serverParse will need to parse incoming requests to extract any cookies that the client has sent. You can do this by examining the "Cookie" header of the incoming request. If the header contains cookies, you can extract the cookie values and store them in a data structure.

    Set cookies in outgoing responses: When your serverParse needs to set a cookie, you can do so by including a "Set-Cookie" header in the outgoing response. The header should include the cookie name, value, and any other relevant information, such as the cookie's expiration time or path.

    Implement session management: To implement session management, you will need to create a data structure that can store session data. When a client makes a request, your serverParse can check if the request includes a session ID cookie. If it does, your serverParse can use the session ID to look up the session data in your data structure. If the session data exists, your serverParse can use it to provide personalized content to the client.

    Create and delete sessions: When a client logs in to your application, your serverParse can create a new session ID and store the user's session data in your data structure. When the user logs out or their session expires, your serverParse can delete the session data.

Overall, adding support for cookies and session management in a C++98 HTTP serverParse requires more manual implementation than in more modern programming languages and frameworks. However, with some careful planning and implementation, you can provide these features to your users.
