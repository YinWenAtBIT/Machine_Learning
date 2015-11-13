#!/bin/bash

rm list_url.txt
touch list_url.txt


i=0
prefix=./html/
for str in $(cat list_unique.txt)
do
    if [ $i -eq 3  ]
    then
        :#break
    fi
    i=$((i+1))
    name=${prefix}${i}.html
    echo  $name 
    wget   --output-document=$name $str
    python get_url_name2.py $name $str
done
