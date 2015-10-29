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
    totalcount(0), default_count(0), default_value(0.0)
{

}

UCB1::UCB1(int init_totalcount, int init_count, double init_value):
	totalcount(init_totalcount), default_count(init_count), default_value(init_value) 
{

}

UCB1::~UCB1()
{

}

string UCB1::toString()
{
    Json::FastWriter writer;
    Json::Value value;
    Json::Value element;
    std::string key;

    value["totalcount"] = Json::Value(totalcount);
    value["default_count"] = Json::Value(default_count);
    value["default_value"] = Json::Value(default_value);

    for(auto it = frequencyReward.begin(); it !=frequencyReward.end(); it++)
    {
        key = it->first;

        Json::Value temp_value;
        temp_value["counts"] = Json::Value(it->second.counts);
        temp_value["keyValue"] = Json::Value(it->second.values);
        element[key] = temp_value;
    }
    value["element"] = element;
    return writer.write(value);
}


bool UCB1::readFromString(const string& JString)
{
    Json::Reader reader;
    Json::Value value;
    Json::Value element;
    std::string key;

    int readcount;
    double readdouble;

    if(!reader.parse(JString, value))
        return false;

    element = value["element"];
    auto keymember = element.getMemberNames();
    int num = keymember.size();

    for(int i=0; i<num; i++)
    {
        key = keymember[i];
        readcount = element[key]["counts"].asInt();
        readdouble = element[key]["keyValue"].asDouble();

        frequencyReward.insert({key, {readcount, readdouble}});
    }

    totalcount = value["totalcount"].asInt();
    default_count = value["default_count"].asInt();
    default_value = value["default_value"].asDouble();
    return true;
}

bool UCB1::insert(string &key)
{
    auto ret = frequencyReward.insert({key, {default_count, default_value}});
    return ret.second;
}


bool UCB1::insert(string& key, UCB value)
{
    auto ret = frequencyReward.insert({key, value});
    return ret.second;
}

void UCB1::add_totalcount(int num)
{
	totalcount += num;
}

void UCB1::set_totalcount(int number)
{
	totalcount = number;
}


string UCB1::get_totalcount()
{
	char temp[25];
	sprintf(temp,"%d", totalcount);
	string totalcountstr(temp);
	return totalcountstr;
}

string UCB1::get_count(string key)
{
	string countstr;
	auto origin = frequencyReward.find(key);
	if(origin == frequencyReward.end())
		return countstr;

	char temp[25];
	sprintf(temp, "%d", origin->second.counts);
	countstr = temp;
	return countstr;
}

string UCB1::get_value(string key)
{
	string valuestr;
	auto origin = frequencyReward.find(key);
	if(origin == frequencyReward.end())
		return valuestr;

	char temp[25];
	sprintf(temp, "%f", origin->second.values);
	valuestr = temp;
	return valuestr;
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
	    auto ret = frequencyReward.insert({ key, {default_count, default_value} });
        return true;
    }


	double n = ++origin->second.counts;
	origin->second.values = origin->second.values*(n-1)/n + res/n;
//    ++this->totalcount;

	return true;
}

bool UCB1::update_reset_last(string & key, double last, double res)
{
	auto origin = frequencyReward.find(key);
	if(origin == frequencyReward.end())
    {
	    auto ret = frequencyReward.insert({ key, {default_count, default_value} });
        return true;
    }


	double n = origin->second.counts;
	origin->second.values = (origin->second.values*n -last + res)/n;

	return true;

}
bool UCB1::update_reset_last(const char *start, double last, double res)
{
    string key(start);
    return update_reset_last(key, last, res);
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

            int i=n;
            while(i>0)
            {
                if(valuenow >= maxvalue[i-1])
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


    	}
    }

    int remain = n - countzero;
    int i=0;
    while(i< remain)
    {
        keystrs.push_back(maxkey[i]);
        i++;
    }

    for(int i=0; i<keystrs.size(); i++)
        update(keystrs[i], default_count);

    this->add_totalcount(1);

    return keystrs;


}
