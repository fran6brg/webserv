import requests
from requests.auth import HTTPBasicAuth

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


# GET -------------------------------------------------------------------------

#r = requests.get('http://localhost:4443/index.html', auth=HTTPBasicAuth('test', 'test'))
#printResponse(r)

# r = requests.get('http://localhost:8080/test')
# printResponse(r)

# r = requests.get('http://localhost:8080/CGI/test_cgi_env.cgi')
# printResponse(r)

# headers = {'Accept-Language': 'fr'}
#r = requests.get('http://localhost:8080/index.html', headers=headers)
# printResponse(r)

# headers = {'Accept-Language': 'en'}
# r = requests.get('http://localhost:8080/test/index.html', headers=headers)
# printResponse(r)

# headers = {'Accept-Language': 'fr', 'Accept-Charset': 'utf8'}
# r = requests.get('http://localhost:8080/test/index.html', headers=headers)
# printResponse(r)

# headers = {'Accept-Language': 'en', 'Accept-Charset': 'utf8'}
# r = requests.get('http://localhost:8080/test/index.html', headers=headers)
# printResponse(r)

# r = requests.get('http://localhost:8080/auth/add.html')
# printResponse(r)

# r = requests.get('http://localhost:8080/auth/add.html', auth=HTTPBasicAuth('test', 'test'))
# printResponse(r)

# headers = {'Host': 'test'}
# r = requests.get('http://localhost:80/', headers=headers)
# printResponse(r)

# headers = {'Host': 'add'}
# r = requests.get('http://localhost:80/', headers=headers)
# printResponse(r)

# HEAD ------------------------------------------------------------------------

# POST ------------------------------------------------------------------------

# r = requests.post('http://localhost:8080/im_cgi.bla', auth=HTTPBasicAuth('test', 'test'))
# printResponse(r)

# PUT -------------------------------------------------------------------------

#body = "4.txt"
#r = requests.put('http://localhost:8080/putTests/4.txt', data=body)
#printResponse(r)

# body = "Some5text5to5test"
# r = requests.put('http://localhost:8080/put_test/testfile.txt', data=body)
# printResponse(r)

# DELETE ----------------------------------------------------------------------

# OPTIONS ---------------------------------------------------------------------





















###############################################################################
###############################################################################
###############################################################################
###############################################################################
###############################################################################
###############################################################################

		# CODE ERREUR 200 Ok:

r = requests.get('http://localhost:8080/index.html?bonjour=5')
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
printResponse(r)

###############################################################################
###############################################################################
###############################################################################
###############################################################################
###############################################################################

