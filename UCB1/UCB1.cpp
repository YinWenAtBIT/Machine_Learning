/*************************************************************************
    > File Name: UCB1.cpp
    > Author: YinWen
    > Mail: YinWenatBIT@163.com 
    > Created Time: Tue 01 Sep 2015 08:32:35 PM CST
 ************************************************************************/

#include "UCB1.h"
#include <cmath>
#include <stdio.h>
#include <cstdlib>
#include "json/json.h"
#include <array>



using namespace std;

UCB1::UCB1():
    totalcount(0)
{

}

UCB1::~UCB1()
{

}

string UCB1::toString()
{
    Json::FastWriter writer;
    Json::Value value;
    std::string key;

    value["totalcount"] = Json::Value(totalcount);

    for(auto it = frequencyReward.begin(); it !=frequencyReward.end(); it++)
    {
        key = it->first;

        Json::Value temp_value;
        temp_value["counts"] = Json::Value(it->second.counts);
        temp_value["keyValue"] = Json::Value(it->second.values);
        value[key] = temp_value;
    }
    return writer.write(value);
}


bool UCB1::readFromString(const string& JString)
{
    Json::Reader reader;
    Json::Value value;
    std::string key;
    int readcount;
    double readfloat;

    if(!reader.parse(JString, value))
        return false;

    auto keymember = value.getMemberNames();
    int num = keymember.size();
    for(int i=0; i<num-1; i++)
    {
        key = keymember[i];
        readcount = value[key]["counts"].asInt();
        readfloat = value[key]["keyValue"].asDouble();

        frequencyReward.insert({key, {readcount, readfloat}});
    }
    totalcount = value["totalcount"].asInt();

    return true;
}

bool UCB1::insert(string& key, UCB value)
{
    auto ret = frequencyReward.insert({key, value});
    return ret.second;
}



bool UCB1::update(const char * start, double res)
{
    string key(start);
    return update(key, res);
}





bool UCB1::update(string& key, double res)
{
	auto origin = frequencyReward.find(key);
	if(origin == frequencyReward.end())
    {
	    auto ret = frequencyReward.insert({ key, {0,0.0} });
        return true;
    }


	double n = ++origin->second.counts;
	origin->second.values = origin->second.values*(n-1)/n + res/n;
    ++this->totalcount;

	return true;
}

string UCB1::select_arm()
{
	string maxkey;

	double bonus;
	double maxvalue = 0.0;
   
    if(frequencyReward.empty())
        throw empty_arm("no arm in the map");

	for(auto it = frequencyReward.begin(); it != frequencyReward.end(); it++ )
	{
		if(it->second.counts == 0)
			return it->first;
		
		bonus = sqrt(2* log((double)totalcount))/it->second.counts;
		if(maxvalue < bonus + it->second.values)
		{
			maxvalue = bonus + it->second.values;
			maxkey = it->first;
		}
	}

	return maxkey;
}


std::vector<std::string> & UCB1::select_arm_N(size_t n)
{
    keystrs.clear();
    int countzero= 0;
    int count =0;

    if(n ==0)
        return keystrs;
    if(n > frequencyReward.size())
        n = frequencyReward.size();

    if(frequencyReward.empty())
        throw empty_arm("no arm in the map");



    vector<std::string> maxkey;
    maxkey.resize(n+1);

	double bonus;
    double valuenow;
    vector<double> maxvalue;
    maxvalue.resize(n+1, 0.0);

	for(auto it = frequencyReward.begin(); it != frequencyReward.end() && countzero <n ; it++ )
	{
		if(it->second.counts == 0)
        {
			keystrs.push_back(it->first);
            ++countzero;
        }
        else
        {

	    	bonus = sqrt(2* log((double)totalcount))/it->second.counts;
            valuenow = bonus +it->second.values;
            int i;
            if(count < n - countzero)
                i = count;
            else
                i = n - countzero;

            while(i>0)
            {
                if(valuenow > maxvalue[i-1])
                {
                    maxvalue[i] = maxvalue[i-1];
                    maxkey[i] = maxkey[i-1];
                    --i;   
                }
                else
                    break;

            }
            maxvalue[i] = valuenow;
            maxkey[i] = it->first;

            ++count;

    	}
    }

    int remain = n - countzero;
    int i=0;
    while(i< remain)
    {
        keystrs.push_back(maxkey[i]);
        i++;
    }


    return keystrs;


}
