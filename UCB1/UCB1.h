/*************************************************************************
    > File Name: UCB1.h
    > Author: YinWen
    > Mail: YinWenatBIT@163.com 
    > Created Time: Tue 01 Sep 2015 07:54:50 PM CST
	>Description:实现UCB1算法，提供一个UCB类
 ************************************************************************/

#include <unordered_map>
#include <string>
#include <exception>
#include <vector>

#ifndef _UCB1_H
#define _UCB1_H

using namespace std;


class empty_arm: public runtime_error
{
public:
    explicit empty_arm(const string &str):
        runtime_error(str) {}
};


struct UCBNode
{
	int counts;
	double values;
};

typedef UCBNode UCB;

class UCB1
{
public:
	UCB1();
	~UCB1();
	bool update(string & key, double res);
    bool update(const char *startPos, double res);
    string toString();
    bool readFromString(const string& JString);
	string select_arm();
    std::vector<std::string> & select_arm_N(size_t n);

    std::vector<std::string> keystrs;

private:

    bool insert(string & key, UCB value);
	std::unordered_map<string, UCB> frequencyReward; 
    int totalcount;
};



#endif
