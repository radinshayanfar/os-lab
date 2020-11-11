#!/bin/bash

# Checking number of arguments
if [[ $# -ne 2 ]]; then
	echo "Number of arguments must be 2"
	exit 1
fi

# Checking whether arguments are numeric
re='^[0-9]+$'
for i in "${@}"; do
	if [[ ! $i =~ $re ]]; then
		echo "\"$i\" is not a numeric value"
		exit 1
	fi
done

# Printing sum of two positional arguments
echo "Sum of two arguments is: $(($1 + $2))"

# Comparing positional arguments
[[ $1 -gt $2 ]] && echo "$1 is greater than $2" || ( [[ $1 -eq $2 ]] && echo "$1 and $2 are equal" || echo "$2 is greater than $1" )

