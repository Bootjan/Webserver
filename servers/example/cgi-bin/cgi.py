print("Content-Type: text/plain\n")
print("Hallo dit is de output van de cgi met python")

import os

print(os.environ['QUERY_STRING'])