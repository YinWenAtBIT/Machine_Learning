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
	UCB1(int init_totalcount, int init_count, double init_value);
	~UCB1();
	bool update(string & key, double res);
    bool update(const char *start, double res);

	bool update_reset_last(string & key, double res);
    bool update_reset_last(const char *start, double res);

    string toString();
    bool readFromString(const string& JString);
	string select_arm();
    std::vector<std::string> & select_arm_N(size_t n);

    std::vector<std::string> keystrs;
	bool insert(string & key, UCB value);
	bool insert(string &key);
	void set_totalcount(int number);
	void add_totalcount(int num);
	string get_totalcount();
	string get_count(string key);
	string get_value(string key);
private:

	std::unordered_map<string, UCB> frequencyReward; 
    int totalcount;
	int default_count;
	double default_value;
};



#endif
