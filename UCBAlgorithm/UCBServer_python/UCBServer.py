#!/usr/bin/env python
# coding=utf-8

import redis

import tornado.ioloop
import tornado.web
import UCB1
import simplejson

initStr = '{"default_value": 0.5, "totalcount": 3, "default_count": 10, "element": {"topic9": [10, 0.5, 0], "topic8": [10, 0.5, 0], "topic1": [10, 0.5, 0], "topic3": [10, 0.5, 0], "topic2": [10, 0.5, 0], "topic5": [10, 0.5, 0], "topic4": [10, 0.5, 0], "topic7": [10, 0.5, 0], "topic6": [10, 0.5, 0], "topic14": [10, 0.5, 0], "topic11": [10, 0.5, 0], "topic10": [10, 0.5, 0], "topic13": [10, 0.5, 0], "topic12": [10, 0.5, 0]}}'

print "use http://211.144.146.217:8888/select?uid=123&number=10   for select goods"

print "use http://211.144.146.217:8888/update?uid=123&armkey=topic1&value=0.7 for update"
print "use http://211.144.146.217:8888/blacklist?uid=123&armkey=topic1 for addblacklist"

redisClient=redis.StrictRedis(host='211.144.146.217',port=6379,db=0)


class SelectHandler(tornado.web.RequestHandler):
        def get(self):
            uid = self.get_argument("uid")
            num = self.get_argument("number")
            num = int(num)
            userKey = uid +".updated.ee"
            selectKey = uid+".selected.ee"
            #if uidKey exists, then write the old selectKey,and update the selectKey
            if redisClient.exists(userKey):
                json_selected = redisClient.get(selectKey)
                self.write(json_selected)

                userUCB = UCB1.UCB1()
                UCBJson = redisClient.get(userKey)
                userUCB.readFromString(UCBJson)
                selected = userUCB.select_arm_N(num)
                json_selected = simplejson.dumps(selected)
                #将更新的选择值和用户数据写回数据库
                newUCBJson = userUCB.toString()
                redisClient.set(userKey, newUCBJson)
                redisClient.set(selectKey, json_selected)
            else:
                userUCB = UCB1.UCB1(1,10,0.5)
                userUCB.readFromString(initStr)
                selected = userUCB.select_arm_N(num)
                json_selected = simplejson.dumps(selected)
                #传送初始数据
                self.write(json_selected)
                #将更新的选择值和用户数据写回数据库
                newUCBJson = userUCB.toString()
                redisClient.set(userKey, newUCBJson)
                redisClient.set(selectKey, json_selected)

class UpdateHandler(tornado.web.RequestHandler):
    def get(self):
            uid = self.get_argument("uid")
            armkey = self.get_argument("armkey")
            value = self.get_argument("value")
            value = float(value)
            print value
            userKey = uid +".updated.ee"
            userUCB = UCB1.UCB1()
            UCBJson = redisClient.get(userKey)
            userUCB.readFromString(UCBJson)
            userUCB.update_reset_last(armkey, value)
            #将更新的用户数据写回数据库
            newUCBJson = userUCB.toString()
            redisClient.set(userKey, newUCBJson)

            
class BlackListHandler(tornado.web.RequestHandler):
    def get(self):
            uid = self.get_argument("uid")
            armkey = self.get_argument("armkey")
            userKey = uid +".updated.ee"
            selectKey = uid+".selected.ee"
            
            #将key加入黑名单
            userUCB = UCB1.UCB1()
            UCBJson = redisClient.get(userKey)
            userUCB.readFromString(UCBJson)
            userUCB.addBlackList([armkey])
            #将更新的选择值和用户数据写回数据库
            newUCBJson = userUCB.toString()
            redisClient.set(userKey, newUCBJson)
            json_selected = userUCB.select_arm_N_forcedbyBlackList(10)
            redisClient.set(selectKey, json_selected)

application = tornado.web.Application([
    (r"/select", SelectHandler),
    (r"/update", UpdateHandler),
    (r"/blacklist", BlackListHandler)
])


if __name__ == "__main__":
    application.listen(8888)
    tornado.ioloop.IOLoop.instance().start()
