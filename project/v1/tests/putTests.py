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
from printReqAndResp import * 

i = 0

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". Create file 'abc' == Update file 'abc' that doesn't exist:\n" + bcolors.ENDC)
body = "abc"
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.put('http://localhost:8081/put_test/abc', data=body)
	printResponse(r, i)

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". Create file 'abc' that already exists == Update file 'abc':\n" + bcolors.ENDC)
body = "def"
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.put('http://localhost:8081/put_test/abc', data=body)
	printResponse(r, i)

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". PUT on uri that doesn't exist:\n" + bcolors.ENDC)
body = "this uri doesn't exist (should return 404)"
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.put('http://localhost:8081/doesnotexist/abc', data=body)
	printResponse(r, i)

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". PUT not allowed:\n" + bcolors.ENDC)
body = "not allowed"
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.put('http://localhost:8081/delete', data=body)
	printResponse(r, i)

# ---
i += 1
from requests.auth import HTTPBasicAuth
print (bcolors.OKBLUE + "\n" + str(i) + ". Auth success:\n" + bcolors.ENDC)
body = "auth success"
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.put('http://localhost:8081/auth/file', auth=HTTPBasicAuth('user', 'pass'), data=body)
	printResponse(r, i)

# ---
i += 1
from requests.auth import HTTPBasicAuth
print (bcolors.OKBLUE + "\n" + str(i) + ". Auth failed:\n" + bcolors.ENDC)
body = "auth failed"
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.put('http://localhost:8081/auth/file', auth=HTTPBasicAuth('user', 'fail'), data=body)
	printResponse(r, i)

# ---
i += 1
from requests.auth import HTTPBasicAuth
print (bcolors.OKBLUE + "\n" + str(i) + ". max body -> 413:\n" + bcolors.ENDC)
body = "413" * 100
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.put('http://localhost:8081/maxbody/index.html', data=body)
	printResponse(r, i)

# ---
i += 1
from requests.auth import HTTPBasicAuth
print (bcolors.OKBLUE + "\n" + str(i) + ". chunked:\n" + bcolors.ENDC)
body = "14\r\nabcdefghijklmnopqrst\r\nA\r\n0123456789\r\n0\r\n\r\n"
headers = {'Transfer-Encoding': 'chunked'}
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.put('http://localhost:8081/put_test/abc', data=body, headers=headers)
	printResponse(r, i)
