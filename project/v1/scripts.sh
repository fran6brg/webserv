#!/bin/bash

# siege tests
# also check control + c

./webserv ./conf/tester.conf &
pid=$!
sleep 0.05
# https://www.interserver.net/tips/kb/http-load-testing-siege/
siege --quiet -bt2s http://localhost:8080/ &>/dev/null &
pid_siege=$!
sleep 1
# wait $pid # 1. pour faire des requêtes pendant x secondes
kill -2 $pid &>/dev/null
wait $pid # 2. pour tester des requêtes toutes les x secondes
if [ $? != 0 ]; then
	exit
fi
wait $pid_siege
