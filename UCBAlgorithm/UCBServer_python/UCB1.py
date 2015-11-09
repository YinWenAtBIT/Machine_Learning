#!/usr/bin/env python
# coding=utf-8

import math
import simplejson
__metaclass__  = type

class UCB1:
    def __init__(self, init_totalcount = 0, init_count =0, init_value =0.0):
        self.totalcount = init_totalcount
        self.default_count = init_count
        self.default_value = init_value
        self.frequencyReward = {}

    def get_totalcount(self):
        return self.totalcount

    def get_count(self, keystr):
        try:
            return self.frequencyReward[keystr][0]
        except KeyError:
            return None

    def get_value(self, keystr):
        try:
            return self.frequencyReward[keystr][1]
        except KeyError:
            return None

    def insert(self, key, count = None, value = None):
        if key not in self.frequencyReward:
            if count is not None and value is not None:
                self.frequencyReward[key] = [count, value, 0]
            else:
                self.frequencyReward[key] = [self.default_count, self.default_value, 0]
        else:
            pass

    def addBlackList(self, blacklist):
        for key in blacklist:
            self.frequencyReward[key][2] =1

    def set_totalcount(self, num):
        self.totalcount = num

    
    def add_totalcount(self, num):
        self.totalcount += num

    def update(self, key, value):
        if key in self.frequencyReward:
            self.frequencyReward[key][0] += 1
            n = self.frequencyReward[key][0]
            prevalue = self.frequencyReward[key][1]
            self.frequencyReward[key][1] = (prevalue*(n-1) + value)/n
        else:
            self.insert(key)

    def update_reset_last(self, key, value):
            prevalue = self.frequencyReward[key][1]
            n = self.frequencyReward[key][0]
            self.frequencyReward[key][1] = (prevalue*n -self.default_value +value)/n


    def get_all(self):
        return self.frequencyReward

    def select_arm(self):
        maxkey = ''
        maxvalue = 0.0
        for key in self.frequencyReward:
            if self.frequencyReward[key][2] == 1:
                continue

            if self.frequencyReward[key][1] == 0.0 :
                self.add_totalcount(1)
                self.update(key, self.default_value)
                return key

            totalvalue = self.value_bonus(key)
            if totalvalue>maxvalue:
                maxvalue = totalvalue
                maxkey = key
            
        self.add_totalcount(1)
        self.update(maxkey, self.default_value)
        return maxkey
    
    #添加黑名单后，立刻重新选择新的推荐，但是不增加totalcount与counts
    def select_arm_N_forcedbyBlackList(self, N):
        if N == 0:
            return None
        
        allvalue = {x:self.value_bonus(x) for x in self.frequencyReward if self.frequencyReward[x][2] != 1}

        sorted_key = sorted(allvalue.iteritems(), key = lambda asd:asd[1], reverse = True)
        allstr = [x[0] for x in sorted_key]
        return allstr[0:N]


    def select_arm_N(self, N):
        if N == 0:
            return None
        
        allvalue = {x:self.value_bonus(x) for x in self.frequencyReward if self.frequencyReward[x][2] != 1}

        sorted_key = sorted(allvalue.iteritems(), key = lambda asd:asd[1], reverse = True)
        allstr = [x[0] for x in sorted_key]
        for i in range(N):
            self.update(allstr[i], self.default_value)

        self.add_totalcount(1)
        return allstr[0:N]
        

    def value_bonus(self, key):
        if key in self.frequencyReward:
            bonus = math.sqrt(2*math.log(self.totalcount)) /float(self.frequencyReward[key][0])
            return bonus+self.frequencyReward[key][1]


    def select_arm_N_sort(self, N):
        if N == 0:
            return None
        
        allvalue = {x:self.value_bonus(x) for x in self.frequencyReward}
        sorted(allvalue.iteritems(), key = lambda asd:asd[1], reverse = True)
        return allvalue

    def toString(self):
        json = {}
        json["totalcount"] = self.totalcount
        json["default_count"] = self.default_count
        json["default_value"] = self.default_value
        json["element"] = self.frequencyReward
        #element = simplejson.dumps(self.frequencyReward)
        json_str = simplejson.dumps(json)
        return json_str

    def readFromString(self, str):
        try:
            readDict = simplejson.loads(str)
            self.totalcount = readDict["totalcount"]
            self.default_count = readDict["default_count"]
            self.default_value = readDict["default_value"]
            self.frequencyReward = readDict["element"]
        except :
            print "wrong string"
