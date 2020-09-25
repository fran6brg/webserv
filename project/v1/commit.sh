#!/bin/bash

echo "make fclean" 
make fclean 2> /dev/null || true

echo
echo "git add ."
git add .

echo
echo "Enter a commit message:"
MSG= read -r -p "> " msg
echo "git commit -m $msg"
git commit -m $msg

echo
echo "Push ? [Yy/Nn]:"
read -p "> " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]
then
    echo "git push origin master"
    git push origin master
fi

