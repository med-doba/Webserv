#!/usr/bin/python3

import os
import sys

# Set the content type for the CGI response
# print("Content-type: text/html")
# print()

# Access environment variables
request_method = os.environ.get("REQUEST_METHOD")
query_string = os.environ.get("QUERY_STRING")
path_info = os.environ.get("PATH_INFO")

# Check if the request method is GET
# print(query_string)
if request_method != "GET":
    print("<h1>Invalid request method!</h1>")
    exit()

# Parse the query string
query_params = {}
if query_string:
    param_pairs = query_string.split("&")
    for param_pair in param_pairs:
        key, value = param_pair.split("=")
        query_params[key] = value

# Pass the query string parameters as command-line arguments
for key, value in query_params.items():
    sys.argv.append("{}={}".format(key, value))

# Print a response with the query string parameters as arguments
print("<h1>Query String Parameters:</h1>")
for key, value in query_params.items():
    print("<p>{}: {}</p>".format(key, value))
print("<p>PATH_INFO: {}</p>".format(path_info))
