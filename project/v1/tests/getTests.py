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
print (bcolors.OKBLUE + "\n" + str(i) + ". BASIC Test Get\n" + bcolors.ENDC)
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.get('http://localhost:8081/')
	printResponse(r, i)

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". Test Get with Accept-Language header: fr \n" + bcolors.ENDC)
headers = {'Accept-Language': 'fr'}
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.get('http://localhost:8081/test', headers=headers)
	printResponse(r, i)

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". Test Get with Accept-Language header: en \n" + bcolors.ENDC)
headers = {'Accept-Language': 'en'}
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.get('http://localhost:8081/test', headers=headers)
	printResponse(r, i)

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". Test Get with Accept-Language: fr && Accept-Charset: utf8 \n" + bcolors.ENDC)
headers = {'Accept-Language': 'fr', 'Accept-Charset': 'utf8'}
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.get('http://localhost:8081/test', headers=headers)
	printResponse(r, i)

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". Test Get with Accept-Charset: iso-8859-1\n" + bcolors.ENDC)
headers = {'Accept-Charset': 'iso-8859-1'}
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.get('http://localhost:8081/test', headers=headers)
	printResponse(r, i)

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". Test Get with cgi\n" + bcolors.ENDC)
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.get('http://localhost:8081/index.cgi')
	printResponse(r, i)

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". Test Get with auto_index\n" + bcolors.ENDC)
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.get('http://localhost:8081/auto_index')
	printResponse(r, i)

# ---
i += 1
print (bcolors.OKBLUE + "\n" + str(i) + ". Test Get without auto_index\n" + bcolors.ENDC)
if len(tests_to_run) == 0 or i in tests_to_run:
	r = requests.get('http://localhost:8081/no_auto_index')
	printResponse(r, i)
