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

    headers name        meaning from https://www.ntu.edu.sg/home/ehchua/programming/webprogramming/HTTP_Basics.html

◦ Accept-Charsets       tell the server which character sets (ISO-8859-1, ISO-8859-2, ISO-8859-5, BIG5, UCS2, UCS4, UTF8...) it can handle or it prefers.

◦ Accept-Language       tell the server what languages it can handle or it prefers. If the server has multiple versions of the requested document (e.g., in English, Chinese,                        French), it can check this header to decide which version to return. This process is called language negotiation.

◦ Allow

◦ Authorization			Used by the client to supply its credential (username/password) to access protected resources.

◦ Content-Language

◦ Content-Length		number-of-bytes - Used by POST request, to inform the server the length of the request body.

◦ Content-Location

◦ Content-Type			mime-type - Used by POST request, to inform the server the media type of the request body.
◦ Date
◦ Host
◦ Last-Modified
◦ Location

◦ Referer				to indicate the referrer of this request. If you click a link from web page 1 to visit web page 2, web page 1 is the referrer for request to web 							page 2. All major browsers set this header, which can be used to track where the request comes from

◦ Retry-After
◦ Server
◦ Transfer-Encoding

◦ User-Agent			browser-type - Identify the type of browser used to make the request. Server use this to return different document depending on the type of browsers.

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
 
# todo
- comprendre l'objectif de GET/PUT etc ...
- comprendre les CGI ...
- ?
