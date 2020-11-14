#!/bin/bash

operands_re='^[+-]?[0-9]+(.[0-9]+)?$'
operator_re='^[\+\-\*\/]$'

# Checking number of arguments
if [[ $# -ne 3 ]]; then
	echo "Number of arguments must be 3"
	exit 1
fi

# Checking operator
if [[ ! "$2" =~ $operator_re ]]; then
	echo "Operator is invalid."
	exit 1
fi

# Checking whether arguments are numeric
for i in "$1" "$3"; do
	if [[ ! "$i" =~ $operands_re ]]; then
		echo "\"$i\" is not a real value."
		exit 1
	fi
done

