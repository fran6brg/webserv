#!/bin/bash

if [ -z "$1" ]
  then
    echo "Enter a commit message"
    exit 1
fi

echo "make fclean"
make fclean

echo "git add ."
git add .

echo "git commit -m $1"
git commit -m $1

read -p "Push ? \n> " -n 1 -r
echo    # (optional) move to a new line
if [[ $REPLY =~ ^[Yy]$ ]]
then
    echo "git push origin master"
    git push origin master
fi

