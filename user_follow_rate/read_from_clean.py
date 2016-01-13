#!/usr/bin/env python
#coding=utf-8

def read_from_cleandata(user_follow_dict, file_name, cursor):
    from add_coperation_count import *


#dir_pre= '/home/yw/github/wanwuhuakai/follow_rate/'

#date_start = 20150916

#dir_suf = '.user_feedback'



    f_feedback = open(file_name, 'r')
    first_line = f_feedback.readline()

    while True:
        current_line = f_feedback.readline()
        current_line_list = current_line.split()
        try:
            current_user_id = current_line_list[1]
            try:
                user_list = user_follow_dict[current_user_id]
            except KeyError:
                user_follow_dict[current_user_id] = [{},{},{}]
                user_list = user_follow_dict[current_user_id]

            add_op_count(current_line_list, user_list, cursor)
        except IndexError:
            break


