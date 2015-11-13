#!/usr/bin/env python
#coding=utf-8

import re
import sys
from get_final_url import get1

filename = sys.argv[1]
wget_url=sys.argv[2]

write_file = open('list_url.txt', 'a+')

try:
    file_object = open(filename)
    all_the_text = file_object.read()
    
    listName = re.findall(r"(?<=<h1 class=\"article_title\" itemprop=\"name\">).+?(?=<)",all_the_text)
    listName = listName[0]
    listId =re.findall(r"(?<=/p/).+?(?=/)" ,wget_url)
    link_list =re.findall(r"(?<=<div class=\"itemName\"><a href=\").+?(?=\")" ,all_the_text)

    link_final=[]
    for url in link_list:
        url_final=get1(url)
        link_final.append(url_final)


    write_file.write(listId[0])
    write_file.write("\n")
    write_file.write(listName)
    write_file.write("\n")
    for url in link_final:
        write_file.write(url)
        write_file.write("\n")

    write_file.write("\n\n\n")

except IOError:
    pass

finally:
    file_object.close(  )

write_file.close()
