#!/bin/sh
echo -n
read input

git add -A
git commit -am "$input"
git push origin p3
