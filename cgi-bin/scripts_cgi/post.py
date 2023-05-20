#!/usr/bin/python3

import cgi
import sys
import os
import urllib.parse
import select


# Check if stdin has any input
if select.select([sys.stdin,], [], [], 0.0)[0]:
    data=sys.stdin.read()

    parsed_data = urllib.parse.parse_qs(data)

    # Get the values of first_name and last_name
    first_name = parsed_data.get('first_name', [''])[0]
    last_name = parsed_data.get('last_name', [''])[0]


    print("<html>")
    print("<head>")
    print("<title>Hello - Second CGI Program</title>")
    print("</head>")
    print("<body>")
    print("<h2>Hello %s %s</h2>" % (first_name, last_name))
    print("</body>")
    print("</html>")
else:
    print("nothing to read")
