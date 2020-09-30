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
print (bcolors.OKBLUE + "\n" + ". Options renvoie les METHODS autorisées par la location\n" + bcolors.ENDC)

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". Test Options dans / \n" + bcolors.ENDC)
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.options('http://localhost:8081/')
	printResponse(r, i)

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". Test Options dans /delete \n" + bcolors.ENDC)
headers = {'Accept-Language': 'fr'}
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.options('http://localhost:8081/delete', headers=headers)
	printResponse(r, i)

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". Test Options dans /auto_index \n" + bcolors.ENDC)
headers = {'Accept-Language': 'en'}
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.options('http://localhost:8081/auto_index', headers=headers)
	printResponse(r, i)
