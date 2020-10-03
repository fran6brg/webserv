#!/bin/bash

./webserv ./conf/tester.conf &
siege --quiet -bt2s http://localhost:8080/ &>/dev/null &