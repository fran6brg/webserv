import requests
from requests.auth import HTTPBasicAuth
import os
import sys

tests_to_run = []
ac = len(sys.argv)
if ac > 1:
	for av in sys.argv[1:]:
		# print ("to run: test", av)
		tests_to_run.append(int(av))

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

def printResponse(r, i):

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
	# print (props(r.raw))
	print ("Status code:", r.status_code)
	print ("Reason:", r.raw.reason)
	# print ("headers:", r.raw.headers)
	print ()
	for h in r.headers:
		print (h + ": " + r.headers[h])
	print ()
	print ("Body:", r.text)

	# print ("-------- ENCODING")
	# print (r.encoding)
	print ("------------------\n")

# PUT -------------------------------------------------------------------------

i = 0;

i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". Create file 'abc' == Update file 'abc' that doesn't exist:\n" + bcolors.ENDC)
body = "abc"
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.put('http://localhost:8080/putTests/abc', data=body)
	printResponse(r, i)

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". Create file 'abc' that already exists == Update file 'abc':\n" + bcolors.ENDC)
body = "def"
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.put('http://localhost:8080/putTests/abc', data=body)
	printResponse(r, i)

# ---
# if len(tests_to_run) == 0 or (1 in tests_to_run or 2 in tests_to_run):
# 	cwd = os.getcwd()
# 	# print (cwd)
# 	os.remove(cwd + "/www/methods/put/abc")

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". PUT on uri that doesn't exist:\n" + bcolors.ENDC)
body = "this uri doesn't exist (should return 404)"
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.put('http://localhost:8080/doesnotexist/abc', data=body)
	printResponse(r, i)

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". PUT not allowed:\n" + bcolors.ENDC)
body = "not allowed"
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.put('http://localhost:8080/postTests/abc', data=body)
	printResponse(r, i)

# ---
# requests for chunked body doesn't work
# idem for postman https://community.postman.com/t/how-to-send-chunked-request/9010
# i += 1
# print (bcolors.OKBLUE + "\n" + str(i) + ". chunked PUT\n" + bcolors.ENDC)

# def gen():
#     yield 'h'
#     yield 'hi'
#     yield 'there'

# r = requests.put('http://localhost:8080/putTests/chunked.txt', data=gen())
# printResponse(r, i)

# ---
# requests for chunked body doesn't work
# idem for postman https://community.postman.com/t/how-to-send-chunked-request/9010
# i += 1
# print (bcolors.OKBLUE + "\n" + str(i) + ". invalid chunked PUT\n" + bcolors.ENDC)
# body = "invalid chunked PUT"
# r = requests.put('http://localhost:8080/putTests/chunked.txt', data=body)
# printResponse(r, i)

# ---
from requests.auth import HTTPBasicAuth
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". Auth success:\n" + bcolors.ENDC)
body = "auth success"
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.put('http://localhost:8080/auth/file', auth=HTTPBasicAuth('user', 'pass'), data=body)
	printResponse(r, i)

# ---
from requests.auth import HTTPBasicAuth
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". Auth failed:\n" + bcolors.ENDC)
body = "auth failed"
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.put('http://localhost:8080/auth/file', auth=HTTPBasicAuth('user', 'fail'), data=body)
	printResponse(r, i)

# ---
from requests.auth import HTTPBasicAuth
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". max body < content length -> 413:\n" + bcolors.ENDC)
body = "1234"
# headers = {'Max-Body': '3'} -> location
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.put('http://localhost:8080/maxbody/file', data=body)
	printResponse(r, i)
