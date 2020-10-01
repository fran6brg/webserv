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
print (bcolors.OKBLUE + "\n" + str(i) + ". http://localhost:8082/text \n" + bcolors.ENDC)
payload = "hello world ! ici le monde"
headers_adds = {
   "Content-Type": "text/plain",
}
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.post('http://localhost:8082/text', payload, headers=headers_adds)
	printResponse(r, i)

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". http://localhost:8082/TEST4\n" + bcolors.ENDC)
payload = "txt=this_is_a_query_string_test"
headers_adds = {
   "Content-Type": "application/x-www-form-urlencoded",
}
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.post('http://localhost:8082/TEST4', payload, headers=headers_adds)
	printResponse(r, i)

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". http://localhost:8082/test.cgi\n" + bcolors.ENDC)
payload = "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
headers_adds = {
    "Content-Type": "text/plain",
}
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.post('http://localhost:8082/test.cgi', payload, headers=headers_adds)
	printResponse(r, i)

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". http://localhost:8082/bj sans cgi \n" + bcolors.ENDC)
body = "bonjour"
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.post('http://localhost:8082/bj', data=body)
	printResponse(r, i)

# ---
from requests.auth import HTTPBasicAuth
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". POST http://localhost:8082/ avec auth \n" + bcolors.ENDC)
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.post('http://localhost:8082/', auth=HTTPBasicAuth('user', 'pass'))
	printResponse(r, i)

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". http://localhost:8082/test.cgi avec body de 1K:\n" + bcolors.ENDC)
body = "x" * 1000
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.post('http://localhost:8082/test.cgi', data=body)
	printResponse(r, i)

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". POST http://localhost:8082/post_body with a size of 200:\n" + bcolors.ENDC)
body = "x" * 200
headers = { 
			"Content-Type": "test/file",
			"Accept-Encoding": "gzip",
			"Connection" : "keep-alive" }
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.post('http://localhost:8082/post_body', data=body, headers=headers)
	printResponse(r, i)
