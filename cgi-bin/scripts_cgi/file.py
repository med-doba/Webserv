#!/usr/bin/python3

import os
import sys
import cgi, cgitb 
# Set the content type for the CGI response
# print("Content-type: text/html")
# print()

# Access environment variables
# request_method = os.environ.get("REQUEST_METHOD")
# query_string = os.environ.get("QUERY_STRING")
# path_info = os.environ.get("PATH_INFO")

# Check if the request method is GET
# print(query_string)
# if request_method == "GET":
#     # Parse the query string
#     query_params = {}
#     if query_string:
#         param_pairs = query_string.split("&")
#         for param_pair in param_pairs:
#             key, value = param_pair.split("=")
#             query_params[key] = value

#     # Pass the query string parameters as command-line arguments
#     for key, value in query_params.items():
#         sys.argv.append("{}={}".format(key, value))

#     # Print a response with the query string parameters as arguments
#     print("<h1>Query String Parameters:</h1>")
#     for key, value in query_params.items():
#         print("<p>{}: {}</p>".format(key, value))
#     print("<p>PATH_INFO: {}</p>".format(path_info))
# if request_method == "POST":
#     print("hahaah")


# form = cgi.FieldStorage() 

# if form.getvalue('param1'):
#     print(form.getvalue('param1'))

# for key in list(form.keys()):
#     value = form.getvalue(key)
#     print(key + ": " + value)
# if 'param1' in form:
#     print(form['param1'].value)

data = sys.stdin.buffer.read()
print(data)