#!/usr/bin/env python
# coding=utf-8


def add_op_count(line_list, user_list, cursor):
    object_type = line_list[3]
    object_id = line_list[4]
    object_op = line_list[2]

    if object_type == '5':
        add_list_cout(user_list, object_id, object_op, cursor)

    if object_type == '3':
        add_goods_count(user_list, object_id, object_op, cursor)
        


def add_list_cout(user_list, list_id, list_op, cursor):
    #寻找清单对应的用户id
    from getid_from_mysql import fetch_userid_from_listid
    follow_user_id = fetch_userid_from_listid(cursor, list_id)

    #对该用户id进行的操作进行统计
    follow_user_dict = user_list[2]
    try:
        user_count_list = follow_user_dict[follow_user_id]
    except KeyError:
        follow_user_dict[follow_user_id] = [0,0,0,0]
        user_count_list = follow_user_dict[follow_user_id]
    count_add(user_count_list, list_op)


def add_goods_count(user_list, goods_id, goods_op, cursor):
    #寻找商品对应的品牌和店铺id
    from getid_from_mysql import fetch_shopid_brandid_from_goodsid
    follow_shop_id, follow_brand_id = fetch_shopid_brandid_from_goodsid(cursor, goods_id) #暂时设定一个，应该从mysql中取出

    #对该用户id进行的操作进行统计
    follow_shop_dict = user_list[0]
    follow_brand_dict = user_list[1]
    try:
        shop_count_list = follow_shop_dict[follow_shop_id]
    except KeyError:
        follow_shop_dict[follow_shop_id] = [0,0,0,0]
        shop_count_list = follow_shop_dict[follow_shop_id]
    count_add(shop_count_list, goods_op)

    try:
        brand_count_list = follow_brand_dict[follow_brand_id]
    except KeyError:
        follow_brand_dict[follow_brand_id] = [0,0,0,0]
        brand_count_list = follow_brand_dict[follow_brand_id]
    count_add(brand_count_list, goods_op)
    

def count_add(count_list, list_op):
    if list_op == '13':
        count_list[0] += 1
    if list_op == '6':
        count_list[1] += 1
    if list_op == '14':
        count_list[2] += 1
    if list_op == '20':
        count_list[3] += 1




