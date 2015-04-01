#!/bin/sh
echo -n "msg"
read input

git add -A
git commit -am "$input"
git push origin p2
