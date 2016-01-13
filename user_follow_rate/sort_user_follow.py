#!/usr/bin/env python
#coding=utf-8

import simplejson
import redis
import datetime
import time
import math
from getid_from_mysql import *
from DBsetting import *

def read_save_data():
    date_today_struct = datetime.datetime.today()
    datedelta =  datetime.timedelta(days = 1)

    date_yesterday = date_today_struct -datedelta
    date_yesterday_str = date_to_str(date_yesterday)
    user_follow_dict = read_from_txt(date_yesterday_str)
    return user_follow_dict

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



def calc_shop_value(shop_id, delta, shop_dict):
    try:
        shop_count_list = shop_dict[str(shop_id)]
        #print 'find shop_count_list'
        #列表中依次代表：屏蔽，收藏，购买，点击
        value  = -1.0*shop_count_list[0]+0.5*shop_count_list[1]+2.0*shop_count_list[2]+0.1*shop_count_list[3]
        value = value * math.pow(math.e, -delta/86400.0* 0.004530373729)
        return value
    except KeyError:
        #print 'cannot find shop_count_list'
        return 0


def calc_brand_value(brand_id, delta, brand_dict):
    try:
        brand_count_list = brand_dict[str(brand_id)]
        #print 'find brand_count_list'
        #列表中依次代表：屏蔽，收藏，购买，点击
        value  = -1.0*brand_count_list[0]+0.5*brand_count_list[1]+2.0*brand_count_list[2]+0.1*brand_count_list[3]
        value = value * math.pow(math.e, -delta/86400.0* 0.004530373729)
        return value
    except KeyError:
        #print 'cannot find brand_count_list'
        return 0



def calc_user_value(user_id, delta, user_dict):
    try:
        user_count_list = user_dict[str(user_id)]
        #print 'find user_count_list'
        #列表中依次代表：屏蔽，收藏，购买，点击
        value  = -1.0*user_count_list[0]+0.5*user_count_list[1]+2.0*user_count_list[2]+0.1*user_count_list[3]
        value = value * math.pow(math.e, -delta/86400.0* 0.004530373729)
        return value
    except KeyError:
        #print 'cannot find user_count_list'
        return 0


def get_delta_second(create_time):
    now = time.time()
    create_time_stamp = time.mktime(create_time.timetuple())
    delta = int(now - create_time_stamp)
    return delta




def calc_follow_value(user_id, follow_id, type, create_time, user_follow_dict):
    try:
        follow_count_list = user_follow_dict[str(user_id)]
        value = 0.0
    
        delta_time = get_delta_second(create_time)

        if type == 1:
            value = calc_shop_value(follow_id, delta_time, follow_count_list[0])

        if type == 2:
            value = calc_brand_value(follow_id, delta_time, follow_count_list[1])

        if type == 8:
            value = calc_user_value(follow_id, delta_time, follow_count_list[2])

        return value+1.0

    except KeyError:
        #print 'cannot find user_follow_count_list'
        return 1.0




def connect_redis():
    redisClient=redis.StrictRedis(host=REDIS_HOST, port=REDIS_PORT, db=REDIS_DB)
    return redisClient


def aver_value(sorted_list):
    N = len(sorted_list)
    for i in range(N):
        sorted_list[i][2] = (N-i+5.0)/(N+5.0)


def reset_value(sorted_list):
    user_list =  [x for x in sorted_list if x[1]== 8]
    brand_list =  [x for x in sorted_list if x[1]== 2]
    shop_list =  [x for x in sorted_list if x[1]== 1]
    
    user_sorted_list = sorted(user_list, key = lambda x: x[2], reverse = True)
    brand_sorted_list = sorted(brand_list, key = lambda x: x[2], reverse = True)
    shop_sorted_list = sorted(shop_list, key = lambda x: x[2], reverse = True)
    #print 'origin shop_sorted_list:'
    #print shop_sorted_list
    #print '\n\n'

    aver_value(user_sorted_list)
    aver_value(brand_sorted_list)
    aver_value(shop_sorted_list)
    
    #print 'new shop_sorted_list:'
    #print shop_sorted_list
    #print '\n\n'
    
    user_sorted_list.extend(brand_sorted_list)
    user_sorted_list.extend(shop_sorted_list)
    return user_sorted_list 

def set_user_sorted_list(client, user_id, sorted_list):
    user_sorted_dict =  {str(x[0]):x[2] for x in sorted_list if x[1]== 8}
    brand_sorted_dict =  {str(x[0]):x[2] for x in sorted_list if x[1]== 2}
    shop_sorted_dict =  {str(x[0]):x[2] for x in sorted_list if x[1]== 1}

    user_json = simplejson.dumps(user_sorted_dict)
    brand_json = simplejson.dumps(brand_sorted_dict)
    shop_json = simplejson.dumps(shop_sorted_dict)

    key_user = str(user_id)+'.users.user_model'
    key_brand = str(user_id)+'.brands.user_model'
    key_shop = str(user_id)+'.shops.user_model'
    
    client.set(key_user, user_json)
    client.set(key_brand, brand_json)
    client.set(key_shop, shop_json)

    #print key_user, key_brand, key_shop
    #print 'user_sorted_dict : ',user_sorted_dict
    #print 'brand_sorted_dict : ',brand_sorted_dict
    #print 'shop_sorted_dict : ',shop_sorted_dict
    #key = user_id
    #value = simplejson.dumps(sorted_list)
    #client.set(key, value)


if __name__== '__main__':
    user_follow_dict = read_save_data()
    #f = open('check.txt','w')
    #for user_id, list in user_follow_dict.iteritems():
        #f.write('user_id: '+ str(user_id)+'\n')
        #print 'user_id: ',user_id
        #for dict in list:
            #f.write(str(dict)+'\n')
            #print dict
        #f.write('\n\n')
        #print '\n\n'
    redisClient = connect_redis()
    conn, cursor = connect_mysql(MYSQL_TABLE)
    user_id_list = get_distinct_user_id(cursor)
    for user_id in user_id_list:
        follow_id_type_list = get_followIdTypeTime_from_userid(cursor, str(user_id))
        user_follow_list= []
        for follow_id, type, create_time in follow_id_type_list:
            value = calc_follow_value(user_id, follow_id, type, create_time, user_follow_dict)
            user_follow_list.append([follow_id, type, value])

        #user_follow_sorted_list = sorted(user_follow_list, key = lambda x:x[2], reverse = True)
        user_follow_sorted_list = reset_value(user_follow_list)
        set_user_sorted_list(redisClient, user_id, user_follow_sorted_list)

        #f.write('user_id' + str(user_id)+'user_follow_sorted_list: \n')
        #f.write(str(user_follow_sorted_list))
        #f.write('\n\n\n')
        #print 'user_id: ',user_id, 'user_follow_sorted_list:'
        #print user_follow_sorted_list
        #print '\n\n\n'

    #f.close()
    close_mysql(conn)

