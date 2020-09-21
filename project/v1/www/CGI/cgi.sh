#! /bin/sh
# get_post.cgi
echo 'status code 200'

echo 'Content-type: text/plain' echo
echo "QS=$QUERY_STRING"
read DATA
echo "Data=$DATA"
