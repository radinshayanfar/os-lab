#!/bin/bash

read -p "Enter first operator: " op1
read -p "Enter operand: " operand
read -p "Enter second operator: " op2

echo -n "Result: "
case $operand in
	+)
		expr "$op1" + "$op2";;
	-)
		expr "$op1" - "$op2";;
	\*)
		expr "$op1" \* "$op2";;
	/)
		expr "$op1" / "$op2";;
esac

