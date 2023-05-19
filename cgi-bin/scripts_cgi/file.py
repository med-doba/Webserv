#!/usr/bin/python3

import os

print("Content-type: text/html\r\n\r\n")
print("<font size=+1>Environment</font><br>")
for param, value in os.environ.items():
    print(f"<b>{param}</b>: {value}<br>\n")
