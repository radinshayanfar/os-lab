#!/bin/bash

read -p "Enter file name: " file
read -p "Enter starting line number: " x
read -p "Enter stopping line number: " y

# count=1
# while IFS= read -r line
# do
# 	[[ $(( count++ )) -ge $x ]] && echo "$line"
# 	[[ $count -gt $y ]] && exit
# done < "$file"

head -n $y $file | tail -n $(($y - $x + 1))

