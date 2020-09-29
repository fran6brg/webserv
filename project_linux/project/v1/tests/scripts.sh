#!/bin/bash

# **************************************************************************** #

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

which siege
if [[ $? != 0 ]]; then
	apt-get install siege
fi

# **************************************************************************** #

# siege tests
# also check control + c

i=0;
while (($i < 10000)); do
	../webserv ../conf/tester.conf &
	pid=$!
	sleep 0.05
    # https://www.interserver.net/tips/kb/http-load-testing-siege/
	siege --quiet -t2s http://localhost:8080 &>/dev/null &
	pid_siege=$!
	sleep 0.5
	wait $pid # 1. pour faire des requêtes pendant x secondes
	kill -2 $pid &>/dev/null
	# wait $pid # 2. pour tester des requêtes toutes les x secondes
	if [ $? != 0 ]; then
		exit
	fi
	wait $pid_siege
	i=$(($i+1))
	echo $i
done
