#!/bin/bash

path1="/proc/"
echo $path1

path2=$(ps -a | grep webserv | awk '{print $1, $8}')
path2=$(echo $path2 | xargs)
echo $path2

path3="/status"
echo $path3

path="${path1}${path2}${path3}"
echo $path

# while 1 ; do grep VmPeak /proc/$wpid/status ; sleep 3 ; done
while [ 1 ] ; do 
    grep VmPeak $path
    sleep 3
done
