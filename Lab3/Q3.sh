#!/bin/bash

function sum_of_digits() {
	local number=$1
	local sod=0
	until [[ $number -eq 0 ]]; do
		let sod+=number%10
		let number/=10
	done
	echo $sod
}

read -p "Enter a number: " number
until [[ "$number" -eq 0 ]]; do
	echo "Sum of digits is $(sum_of_digits $number)"
	echo "Reverse $(echo $number | rev)"

	read -p "Enter a number: " number
done

