import requests
from requests.auth import HTTPBasicAuth

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def props(cls):   
  return [i for i in cls.__dict__.keys() if i[:1] != '_']
# https://stackoverflow.com/questions/16923898/how-to-get-the-raw-content-of-a-response-in-requests-with-python
# https://stackoverflow.com/questions/9058305/getting-attributes-of-a-class

def printResponse(r):
	# print (props(r))

	print ("-------- REQUEST")
	# print (props(r.request))
	print (r.request.method, "on", r.request.url)
	print ()
	for h in r.request.headers:
		print (h + ": " + r.request.headers[h])
	print ()
	print ("Body:", r.request.body)

	print ()
	print ("-------- RESPONSE")
	print ("Status code:", r.status_code)
	print ()
	for h in r.headers:
		print (h + ": " + r.headers[h])
	print ()
	print ("Body:", r.text)

	# print ("-------- ENCODING")
	# print (r.encoding)
	print ("******************\n")

# PUT -------------------------------------------------------------------------

print (bcolors.OKBLUE + "\n1. Create file 1.txt <=> Update file that doesn't exist:\n" + bcolors.ENDC)
body = "abc"
r = requests.put('http://localhost:8080/putTests/1.txt', data=body)
printResponse(r)

print (bcolors.OKBLUE + "\n2. Create file 1.txt that already exists <=> Update file 1.txt that already exists:\n" + bcolors.ENDC)
body = "def"
r = requests.put('http://localhost:8080/putTests/1.txt', data=body)
printResponse(r)

import os
cwd = os.getcwd()
# print (cwd)
os.remove(cwd + "/www/putTests/1.txt")



















###############################################################################
###############################################################################
###############################################################################
###############################################################################
###############################################################################
###############################################################################

		# CODE ERREUR 200 Ok:

# r = requests.get('http://localhost:8080/')
#r = requests.options('http://localhost:8080/index.html')
#r = requests.get('http://localhost:8080/index.html')
#printResponse(r)
#r = requests.delete('http://localhost:8080/test/test.html')
#r = requests.head('http://localhost:8080/index.html')

 		# CODE ERREUR 405 Methode Not Allowed:

#r = requests.get('http://localhost:8080/test')
#r = requests.delete('http://localhost:8080/test.html')

 		# CODE ERREUR 404 Not found:
 		# CODE ERREUR 202 Accepted:
 		# CODE ERREUR 204 No Content:


# r = requests.get('http://localhost:8080/index.html')
# printResponse(r)

###############################################################################
###############################################################################
###############################################################################
###############################################################################
###############################################################################

