 #!/bin/sh
# date.cgi
echo "Content-type: text/html"
echo
#Creation du corps du document
echo "<html><head><title>date.cgi</title></head>" echo "<body>"
echo "<h1>Date sur le serveur</h1>"
echo -n "On est le `date +%D`, il est "
echo "`date +%H`h `date +%M`m"
echo "</body></html>"
