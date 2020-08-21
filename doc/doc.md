# syscalls to create TCP socket:

--- server side
- select: http://manpagesfr.free.fr/man/man2/select.2.html
surveiller plusieurs descripteurs de fichier, attendant qu'au moins l'un des descripteurs de fichier devienne « prêt »
(prêt = s'il est possible d'effectuer l'opération d'entrées-sorties correspondante (par exemple, un read(2)) sans bloquer)

4 macros sont disponibles pour la manipulation des ensembles 
FD_ZERO() efface un ensemble. 
FD_SET() et FD_CLR() ajoutent et suppriment, respectivement, un descripteur de fichier dans un ensemble. 
FD_ISSET() vérifie si un descripteur de fichier est contenu dans un ensemble, principalement utile après le retour de select().

=> pour être prévenu de l'arrivée d'une connexion sur une socket, on peut utiliser select(2) ou poll(2).
Un événement « lecture » sera délivré lorsqu'une tentative de connexion aura lieu, et on pourra alors appeler accept() pour la valider.

RET:

- socket: http://manpagesfr.free.fr/man/man2/socket.2.html
crée un point de communication, et renvoie un descripteur.

RET:

- listen: http://manpagesfr.free.fr/man/man2/listen.2.html
attendre des connexions sur une socket  

if (listen(server_fd, 3) < 0) 
{ 
    perror(“In listen”); 
    exit(EXIT_FAILURE); 
}

RET:

- accept: http://manpagesfr.free.fr/man/man2/accept.2.html
accepter une connexion sur une socket  

S'il n'y a pas de connexion en attente dans la file, et si la socket n'est pas marquée comme non bloquante, accept() se met en attente d'une connexion.
Si la socket est non bloquante, et qu'aucune connexion n'est présente dans la file, accept() échoue avec l'erreur EAGAIN.

if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
{
    perror("In accept");            
    exit(EXIT_FAILURE);        
}

RET: accept() renvoie un entier non négatif, constituant un descripteur pour la nouvelle socket
(nouvelle socket = 'connected socket between a client(when you visit IP address of your server from a web browser) and a server')
S'il échoue, l'appel renvoie -1 et errno contient le code d'erreur.

- recv: http://manpagesfr.free.fr/man/man2/recv.2.html
recevoir un message sur une socket  

RET:

- read:

char buffer[1024] = {0};
int valread = read( new_socket , buffer, 1024); 
printf(“%s\n”,buffer );
if(valread < 0)
{ 
    printf("No bytes are there to read");
}

- write:
char *hello = "Hello from the server";
write(new_socket , hello , strlen(hello));

--- client side
- send: 
envoyer un message sur une socket  

RET:

- close: http://manpagesfr.free.fr/man/man2/close.2.html
fermer un descripteur de fichier  


# HTTP flow

When the request message reaches the server, the server can take either one of these actions:
- map to file:
The server interprets the request received, maps the request into a file under the server's document directory, and returns the file requested to the client.
    
- map to program:
The server interprets the request received, maps the request into a program kept in the server, executes the program, and returns the output of the program to the client.

- error:
The request cannot be satisfied, the server returns an error message.

https://www.ntu.edu.sg/home/ehchua/programming/webprogramming/HTTP_Basics.html


# HTTP request methods

A client can use one of these request methods to send a request message to an HTTP server. The methods are:

GET         request to get a web resource from the server.

HEAD        request to get the header that a GET request would have obtained.

POST        Used to post data up to the web server.

PUT         Ask the server to store the data.

DELETE      Ask the server to delete the data.

TRACE       Ask the server to return a diagnostic trace of the actions it takes.

OPTIONS     Ask the server to return the list of request methods it supports.

CONNECT     Used to tell a proxy to make a connection to another host and simply reply the content, without attempting to parse or cache it.

... Other extension methods.

# HTTP headers to implement
upply its credential (username/password) to access protected resources.

◦ Content-Language		https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Content-Language
						utilisé pour décrire quels langages sont destinés au public, de sorte que cela permette à l'utilisateur de se différencier en fonction de la langue préférée des utilisateurs.
						Par exemple, si "Content-Language: de-DE" est mis en place, cela signifie que la page est destinée à un public parlant l'allemand (par contre, cela n'indique pas que la page est écrite en allemand. Par exemple, elle pourrait être écrite en anglais dans le cadre d'un cours de langue destiné aux allemands).
						Si l'en-tête Content-Language n'est pas spécifié, par défaut, cela signifie que la page est destinée à tout public de langue. 

◦ Content-Length		number-of-bytes - Used by POST request, to inform the server the length of the request body.

◦ Content-Location		"An alternate location for the returned data"
						Content-Location: /index.htm
						https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Location
						indique l'URL directe à utiliser pour accéder à la ressource lorsque la négociation de contenu a eu lieu, sans qu'il soit nécessaire de poursuivre la négociation de contenu.
						https://www.geeksforgeeks.org/http-headers-content-location/
						example: Content-Location: /index.html

◦ Content-Type			mime-type - Used by POST request, to inform the server the media type of the request body.

◦ Date					The date and time at which the message was originated (in "HTTP-date" format as defined by RFC 7231 Date/Time Formats).

◦ Host					The domain name of the server (for virtual hosting), and the TCP port number on which the server is listening. 
						Host: en.wikipedia.org:8080
						The port number may be omitted if the port is the standard port for the service requested. 
						Host: en.wikipedia.org
						Mandatory since HTTP/1.1.[16] If the request is generated directly in HTTP/2, it should not be used.
			
◦ Last-Modified			The last modified date for the requested object (in "HTTP-date" format as defined by RFC 7231)
						Example: Last-Modified: Tue, 15 Nov 1994 12:45:26 GMT

◦ Location				Used in redirection, or when a new resource has been created.
						Example 1: Location: http://www.w3.org/pub/WWW/People.html
						Example 2: Location: /pub/WWW/People.html

◦ Referer				to indicate the referrer of this request. If you click a link from web page 1 to visit web page 2, web page 1 is the referrer for request to web 							page 2. All major browsers set this header, which can be used to track where the request comes from

◦ Retry-After			If an entity is temporarily unavailable, this instructs the client to try again later. Value could be a specified period of time (in seconds) or a HTTP-date
						Example 1: Retry-After: 120
						Example 2: Retry-After: Fri, 07 Nov 2014 23:59:59 GMT

◦ Server				A name for the server

◦ Transfer-Encoding		The form of encoding used to safely transfer the entity to the user. Currently defined methods are: chunked, compress, deflate, gzip, identity.
						Must not be used with HTTP/2.[13]
						Transfer-Encoding: chunked

◦ User-Agent			browser-type - Identify the type of browser used to make the request. Server use this to return different document depending on the type of browsers.
						https://fr.wikipedia.org/wiki/User_agent
						User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:12.0) Gecko/20100101 Firefox/12.0

◦ WWW-Authenticate

# Status code

https://httpstatusdogs.com/


# Pseudo code

// 1 init
parse_nginx_conf();
init_fds(); // FD_ZERO() + new Server() > socket() > bind() > listen() > fcntl() > FD_SET()

// 2 
while (42):
    if (signal):
        break
    readSet = readSetSave
    writeSet = writeSetSave
    select()
    Servers.each(server):
        if FD_ISSET()
           try accept accept() > create new Client() for server ; catch error
		    server.Clients.each(client):
				getRequest() > read() fd generated by accept()
				processResponse() selon la méthode (GET||POST||HEAD||...)
clear()

# NGINX.conf

https://www.youtube.com/watch?v=YD_exb9aPZU&pbjreload=101
 
