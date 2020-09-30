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
print (bcolors.OKBLUE + "\n" + str(i) + ". Basic test head\n" + bcolors.ENDC)
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.head('http://localhost:8081/')
	printResponse(r, i)

# ---
print (bcolors.OKBLUE + "\n" + str(i) + ". Test head fr\n" + bcolors.ENDC)
i += 1
headers = {'Accept-Language': 'fr'}
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.head('http://localhost:8081/test', headers=headers)
	printResponse(r, i)

# ---
print (bcolors.OKBLUE + "\n" + str(i) + ". Test head en\n" + bcolors.ENDC)
i += 1
headers = {'Accept-Language': 'en'}
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.head('http://localhost:8081/test', headers=headers)
	printResponse(r, i)

# ---
print (bcolors.OKBLUE + "\n" + str(i) + ". Test head fr utf8\n" + bcolors.ENDC)
i += 1
headers = {'Accept-Language': 'fr', 'Accept-Charset': 'utf8'}
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.head('http://localhost:8081/test/yolo', headers=headers)
	printResponse(r, i)

# ---
print (bcolors.OKBLUE + "\n" + str(i) + ". Test head iso-8859-1\n" + bcolors.ENDC)
i += 1
headers = {'Accept-Charset': 'iso-8859-1'}
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.head('http://localhost:8081/test', headers=headers)
	printResponse(r, i)
