#!/usr/bin/python3



import os
import sys

# Set the content type for the CGI response
print("Content-type: text/html")
print()

# Access environment variables
request_method = os.environ.get("REQUEST_METHOD")
query_string = os.environ.get("QUERY_STRING")

# Check if the request method is GET
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


# #!/usr/bin/python

# # Import modules for CGI handling
# import cgi, cgitb

# # Create instance of FieldStorage
# form = cgi.FieldStorage()

# # Get data from fields
# first_name = form.getvalue('first_name')
# last_name  = form.getvalue('last_name')

# print ("Content-type:text/html\r\n\r\n")
# print ("<html>")
# print ("<head>")
# print ("<title>Hello - Second CGI Program</title>")
# print ("</head>")
# print ("<body>")
# print ("<h2>Hello %s %s</h2>" % (first_name, last_name))
# print ("</body>")
# print ("</html>")