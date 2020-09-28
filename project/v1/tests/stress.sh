#!/bin/sh

# i=0;
# while (($i < 100)) ;do
# 	curl -H "Keep-Alive: 60" -H "Connection: keep-alive" http://localhost:8080;
# 	i=$(($i+1));
# done

curl -X POST -H "Content-Type: plain/test" --data "BODY IS HERE write something shorter or longer than body limit BODY IS HERE write something shorter or longer than body limit BODY IS HERE write something shorter or longer than body limit " http://localhost:8080/post_body;