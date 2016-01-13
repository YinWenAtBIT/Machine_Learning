#!/usr/bin/env python
#coding=utf-8

import MySQLdb
from DBsetting import *

def connect_mysql(database):
	conn=MySQLdb.connect(host=MYSQL_HOST, user=MYSQL_USER, passwd=MYSQL_PASSWORD, db=database ,charset="utf8")
	cursor = conn.cursor()
	return conn, cursor



def fetch_userid_from_listid(cursor, list_id):
	query = 'select user_id from user_info_list where list_id =' +list_id
	n = cursor.execute(query)
	userid = None
	if n !=0:
		id_int = cursor.fetchone()[0]
		userid = str(id_int)
	return userid

def fetch_shopid_brandid_from_goodsid(cursor, good_id):
	query = 'select shop_id, brand_id from goods_info_ext where goods_id='+good_id
	n = cursor.execute(query)
	shopid = None
	brandid = None
	if n !=0:
		shop_int, brand_int = cursor.fetchone()
		shopid = str(shop_int)
		brandid = str(brand_int)
	return shopid, brandid

def close_mysql(conn):
	conn.close()


def get_distinct_user_id(cursor):
    query='select distinct user_id from user_info_follow'
    n = cursor.execute(query)
    user_id_list = [x[0] for x in cursor.fetchall()]
    return user_id_list

def get_followIdTypeTime_from_userid(cursor, user_id):
    query='select follow_object_id, follow_object_type_id, follow_create_time from user_info_follow where user_id='+user_id
    cursor.execute(query)
    follow_id_type_list = [x for x in cursor.fetchall()]
    return follow_id_type_list



if __name__ == '__main__':
	conn, cursor = connect_mysql('api_wave87_com')
	listid = '132'
	userid = fetch_userid_from_listid(cursor, listid)
	print 'userid get from list id: ', userid
	goodid = '15365'
	shopid, brandid = fetch_shopid_brandid_from_goodsid(cursor, goodid)
	print 'shopid and brandid get from goods id: ', shopid, brandid
	close_mysql(conn)
