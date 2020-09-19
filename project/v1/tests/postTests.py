import requests
from requests.auth import HTTPBasicAuth
import os
import sys
import json

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
	print ("Status code:", r.status_code)
	print ()
	for h in r.headers:
		print (h + ": " + r.headers[h])
	print ()
	print ("Body:", r.text)

	# print ("-------- ENCODING")
	# print (r.encoding)
	print ("------------------\n")

# PUT -------------------------------------------------------------------------

#print (bcolors.OKBLUE + "\n" + str(i) + ".1 requete post avec json \n" + bcolors.ENDC)
#
#payload = {'some': 'data2'}
#headers_adds = {
#    "Content-Type": "application/json",
#}
#r = requests.post('http://localhost:80/cgi_tester', data=json.dumps(payload), headers=headers_adds)
#printResponse(r, 0)

#payload = "hello world ! icici le monde"
#headers_adds = {
#    "Content-Type": "sqfqsfsq",
#}
#
#r = requests.post('http://localhost:80/postTests/rzq', payload, headers=headers_adds)
#printResponse(r, 0)

#files = {'file': open('babouin.png', 'rb')}
#headers_adds = {
#    "Content-Type": "image/png",
#}
#r = requests.post('http://localhost:80/postTests/photo', files=files,  headers=headers_adds)
#printResponse(r, 0) 


payload = "txt=this_is_a_query_string_test"
headers_adds = {
    "Content-Type": "application/x-www-form-urlencoded",
}

r = requests.post('http://localhost:80/doritos', payload, headers=headers_adds)
printResponse(r, 0)
