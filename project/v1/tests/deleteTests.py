import requests
from requests.auth import HTTPBasicAuth
import os
import sys
from printReqAndResp import * 

tests_to_run = []
ac = len(sys.argv)
if ac > 1:
	for av in sys.argv[1:]:
		# print ("to run: test", av)
		tests_to_run.append(int(av))


i = 0

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". Test delete: SUCCESS\n" + bcolors.ENDC)
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.delete('http://localhost:8081/delete/')
	printResponse(r, i)

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". Test delete: no content\n" + bcolors.ENDC)
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.delete('http://localhost:8081/delete/no_content')
	printResponse(r, i)

# DELETE -------------------------------------------------------------------------
