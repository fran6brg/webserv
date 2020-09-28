#!/bin/sh

i=0;
while (($i < 100)) ;do
	curl -H "Keep-Alive: 60" -H "Connection: keep-alive" http://localhost:8080;
	i=$(($i+1));
done
