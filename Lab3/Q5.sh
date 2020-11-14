#!/bin/bash

read -r -d '' txt1 << EOM
1
22
333
4444
55555
EOM

read -r -d '' txt2 << EOM
​     *
    * *
   * * *
  * * * *
 * * * * *
* * * * * *
* * * * * *
 * * * * *
  * * * *
   * * *
    * *
     *
EOM

read -r -d '' txt3 << EOM
|
---
|  |
   ---
|  |  |
      ---
|  |  |  |
         ---
|  |  |  |  |
            ---
EOM

texts=("$txt1" "$txt2" "$txt3")

read -p "Enter a number between 1-3: " num
((num--))
echo "${texts[num]}"

