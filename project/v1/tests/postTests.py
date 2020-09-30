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

# payload = {'some': 'data'}
# headers_adds = {
#    "Content-Type": "application/json",
# }
# r = requests.post('http://localhost:80/', data=json.dumps(payload), headers=headers_adds)
# printResponse(r, 0)

# payload = "hello world ! icici le monde"
# headers_adds = {
#    "Content-Type": "text/plain",
# }
# r = requests.post('http://localhost:80/', payload, headers=headers_adds)
# printResponse(r, 0)

# files = {'file': open('babouin.png', 'rb')}
# headers_adds = {
#    "Content-Type": "image/png",
# 	"Transfer-Encoding": "chunked"
# }
# r = requests.post('http://localhost:80/photo', files=files,  headers=headers_adds)
# printResponse(r, 0) 

# payload = "txt=this_is_a_query_string_test"
# headers_adds = {
#    "Content-Type": "application/x-www-form-urlencoded",
# }

# r = requests.post('http://localhost:80/', payload, headers=headers_adds)
# printResponse(r, 0)

# payload = "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
# headers_adds = {
#     "Content-Type": "text/plain",
# }
# r = requests.post('http://localhost:8081/test.cgi', payload, headers=headers_adds)
# printResponse(r, 0)

# # TEST with your own content length
# s = Session()
# headers = { "Content-Type": "text/plain" }
# data = "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
# req = Request('POST', 'http://localhost:80/test.cgi', data=data, headers=headers)
# prepped = req.prepare()
# prepped.headers['Content-Length'] = 10
# r = s.send(prepped)
# print(r)

# import base64
# i += 1
# if len(tests_to_run) == 0 or i in tests_to_run:
# 	usrPass = "userid:password"
# 	b64Val = base64.b64encode(usrPass)
# 	r = requests.post(api_URL,
# 					headers={"Authorization": "Basic %s" % b64Val},
# 					data=payload)
# 	printResponse(r, i)

# ---
from requests.auth import HTTPBasicAuth
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". POST http://localhost:8081/test/bonjour sans cgi \n" + bcolors.ENDC)
body = "bonjour"
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.post('http://localhost:8081/test/bonjour', data=body)
	printResponse(r, i)

# # ---
# from requests.auth import HTTPBasicAuth
# i += 1
# print (bcolors.OKBLUE + "\n" + str(i) + ". POST http://localhost:8081/ avec auth \n" + bcolors.ENDC)
# if len(tests_to_run) == 0 or i in tests_to_run:
# 	r = requests.post('http://localhost:8081/', auth=HTTPBasicAuth('user', 'pass'))
# 	printResponse(r, i)

# # ---
# i += 1
# print (bcolors.OKBLUE + "\n" + str(i) + ". 1K:\n" + bcolors.ENDC)
# body = "x" * 1000
# if len(tests_to_run) == 0 or i in tests_to_run:
# 	r = requests.post('http://localhost:8081/directory/youpi.bla', data=body)
# 	printResponse(r, i)

# ---
# i += 1
# print (bcolors.OKBLUE + "\n" + str(i) + ". POST http://localhost:8081/post_body with a size of 200:\n" + bcolors.ENDC)
# body = "x" * 200
# headers = { "Transfer-Encoding": "chunked",
# 			"Content-Type": "test/file",
# 			"Accept-Encoding": "gzip",
# 			"Connection" : "keep-alive" }
# if len(tests_to_run) == 0 or i in tests_to_run:
# 	r = requests.post('http://localhost:8081/post_body', data=body, headers=headers)
# 	printResponse(r, i)
