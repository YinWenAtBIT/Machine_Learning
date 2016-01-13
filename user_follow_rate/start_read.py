#!/usr/bin/env python
# coding=utf-8

import datetime
from read_from_clean import read_from_cleandata
from getid_from_mysql import *
from DBsetting import *
import simplejson

def to_txt(today, user_dict):
    save_name = 'save_data/'+today+'.feedback_count'
    f = open(save_name, 'w')
    json_str = simplejson.dumps(user_dict)
    f.write(json_str)
    f.close()

def read_from_txt(date):
    read_name = '/data_ssd/work_data/yinwen/save_data/'+date+'.feedback_count'
    f = open(read_name, 'r')
    json_str = f.read()
    user_dict = simplejson.loads(json_str)
    return user_dict

def date_to_str(datetime_struct):
    year = str(datetime_struct.year)
    month = str(datetime_struct.month)
    if len(month) == 1:
        month = '0'+month

    day = str(datetime_struct.day)
    if len(day) == 1:
        day = '0'+day

    date_str = year+month+day
    return date_str


def update_today(user_follow_dict):

    dir_pre = '/data_ssd/work_data/liqin/user_log/clean/'
    dir_suf = '.user_feedback'

    date_today_struct = datetime.datetime.today()
    today_str = date_to_str(date_today_struct)

    datedelta =  datetime.timedelta(days = 1)
    datedelta_2 =  datetime.timedelta(days = 2)

    date_prev_dict = date_today_struct -datedelta_2
    date_prev_dict_str = date_to_str(date_prev_dict)
    user_follow_dict = read_from_txt(date_prev_dict_str)

    date_yesterday = date_today_struct -datedelta
    date_yesterday_str = date_to_str(date_yesterday)
    read_file = dir_pre + date_yesterday_str+'/'+date_yesterday_str +dir_suf

    conn, cursor = connect_mysql(MYSQL_TABLE)
    print 'read_file_now : ',read_file
    try:
        read_from_cleandata(user_follow_dict, read_file, cursor)
    except IOError:
        pass
    
    to_txt(date_yesterday_str, user_follow_dict)
    close_mysql(conn)
    return user_follow_dict


def read_from_start(date_start):
    dir_pre = '/data_ssd/work_data/liqin/user_log/clean/'
    dir_suf = '.user_feedback'

    #date_start = '20150916'
    date_start_sturct = datetime.datetime.strptime(date_start,'%Y%m%d')
    date_now_struct = date_start_sturct


    date_today_struct = datetime.datetime.today()
    today_str = date_to_str(date_today_struct)

    datedelta =  datetime.timedelta(days = 1)

    date_yesterday = date_today_struct -datedelta
    date_yesterday_str = date_to_str(date_yesterday)

    user_follow_dict = {}

    date_now_read = date_start
    conn, cursor = connect_mysql(MYSQL_TABLE)
    while date_now_read != today_str:
        read_file = dir_pre + date_now_read+'/'+date_now_read +dir_suf
        print 'read_file_now : ',read_file
        try:
            read_from_cleandata(user_follow_dict, read_file, cursor)
        except IOError:
            break
        date_now_struct += datedelta
        date_now_read = date_to_str(date_now_struct)

    to_txt(date_yesterday_str, user_follow_dict)
    close_mysql(conn)
    return user_follow_dict





def usage():  
    print("Usage:%s [-u] [-s] startdate" %sys.argv[0])

if __name__ == '__main__':
    import sys
    import getopt
    try:
        opts,args = getopt.getopt(sys.argv[1:], "us:")
        
        for opt, arg in opts:
            if opt == '-s':
                user_follow_dict = read_from_start(arg)
                break
            if opt == '-u':
                #先读取原有文件
                user_follow_dict ={}
                update_today(user_follow_dict)

    except getopt.GetoptError:
        usage()
