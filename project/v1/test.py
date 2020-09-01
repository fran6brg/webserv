import requests
from requests.auth import HTTPBasicAuth

def printResponse(r):
	print("==STATUSCODE==")
	print(r.status_code)
	print("==HEADERS==")
	print(r.headers)
	print("==BODY==")
	print(r.text)
	print("******************\n")

# req = requests.Request('POST','http://stackoverflow.com',headers={'X-Custom':'Test'},data='a=1&b=2')
# prepared = req.prepare()

# def pretty_print_POST(req):
#     """
#     At this point it is completely built and ready
#     to be fired; it is "prepared".

#     However pay attention at the formatting used in 
#     this function because it is programmed to be pretty 
#     printed and may differ from the actual request.
#     """
#     print('{}\n{}\r\n{}\r\n\r\n{}'.format(
#         '-----------START-----------',
#         req.method + ' ' + req.url,
#         '\r\n'.join('{}: {}'.format(k, v) for k, v in req.headers.items()),
#         req.body,
#     ))

# print("PUT on 8080/test")
# r = requests.put('http://localhost:8080/test')
# printResponse(r)

# print("PUT on 8080/put_test/testfile.txt")
# body = "Some5text5to5test"
# r = requests.put('http://localhost:8080/put_test/testfile.txt', data=body)
# printResponse(r)

# print("same PUT on 8080/put_test/testfile.txt")
# body = "Some4text to test"
# r = requests.put('http://localhost:8080/put_test/testfile.txt', data=body)
# printResponse(r)


# print("GET on 8080")
# r = requests.get('http://localhost:8080')
# printResponse(r)

# print("GET on 4443")
# r = requests.get('http://localhost:4443')
# printResponse(r)

# print("GET on 8080/test")
# r = requests.get('http://localhost:8080/test')
# printResponse(r)
# print(r)

# print("GET on 8080/test/index.html with no language parameter")
# r = requests.get('http://localhost:8080/test/index.html')
# printResponse(r)

# print("GET on 8080/test/index.html with preferred language FR")
# headers = {'Accept-Language': 'fr'}
# r = requests.get('http://localhost:8080/test/index.html', headers=headers)
# printResponse(r)

# print("GET on 8080/test/index.html with preferred language EN")
# headers = {'Accept-Language': 'en'}
# r = requests.get('http://localhost:8080/test/index.html', headers=headers)
# printResponse(r)

# print("GET on 8080/test/index.html with preferred language FR and preferred encoding utf8")
# headers = {'Accept-Language': 'fr', 'Accept-Charset': 'utf8'}
# r = requests.get('http://localhost:8080/test/index.html', headers=headers)
# printResponse(r)

print("GET on 8080/test/index.html with preferred language EN and preferred encoding utf8")
headers = {'Accept-Language': 'en', 'Accept-Charset': 'utf8'}
r = requests.get('http://localhost:8080/test/index.html', headers=headers)
printResponse(r)

# print("GET on 8080/auth/add.html without auth")
# r = requests.get('http://localhost:8080/auth/add.html')
# printResponse(r)

# print("GET on 8080/auth/add.html with auth")
# r = requests.get('http://localhost:8080/auth/add.html', auth=HTTPBasicAuth('test', 'test'))
# printResponse(r)

# print("GET on 80/ with test host header")
# headers = {'Host': 'test'}
# r = requests.get('http://localhost:80/', headers=headers)
# printResponse(r)

# print("GET on 80/ with add host header")
# headers = {'Host': 'add'}
# r = requests.get('http://localhost:80/', headers=headers)
# printResponse(r)