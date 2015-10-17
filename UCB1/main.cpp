/*************************************************************************
	> File Name: main.cpp
	> Author: YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: Sun 06 Sep 2015 06:10:36 PM CST
 ************************************************************************/

#include<iostream>
using namespace std;

#include "UCB1.h"



int main()
{
    UCB1 goods;
    goods.update("a", 0.0);
    goods.update("b", 0.0);
    goods.update("c", 0.0);
    goods.update("d", 0.0);
    goods.update("e", 0.0);
    goods.update("f", 0.0);
   
    cout<<"select three with all zero\n"<<endl;

    auto member = goods.select_arm_N(3);
    for(auto x: member)
        cout<<x<<endl;

    cout<<"select_arm from cool start: "<<goods.select_arm()<<endl;
    cout<<goods.toString()<<endl;

    goods.update("a", 0.123);
    goods.update("d", 0.33);
    goods.update("f", 0.23);
    goods.update("e", 0.42);

    cout<<"select three with b,c zero\n"<<endl;

    member = goods.select_arm_N(3);
    for(auto x: member)
        cout<<x<<endl;

    goods.update("c", 0.5);
    goods.update("b", 0.1);

    cout<<"select_arm from first insert: "<<goods.select_arm()<<endl;
    cout<<goods.toString()<<endl;
    cout<<endl<<endl<<endl;

    goods.update("c", 0.8);
    goods.update("c", 0.8);
    goods.update("c", 0.8);
    goods.update("c", 0.8);
    goods.update("c", 0.8);
    goods.update("c", 0.8);
    goods.update("c", 0.8);
    goods.update("c", 0.8);
    goods.update("c", 0.8);
    goods.update("c", 0.8);

    cout<<"select_arm after add another 10 c: "<<goods.select_arm()<<endl;

    cout<<goods.toString()<<endl;

    cout<<"select three after add 10 c \n"<<endl;
    member = goods.select_arm_N(3);
    for(auto x: member)
        cout<<x<<endl;
    
    cout<<"select 10 with only 6 key \n"<<endl;
    member = goods.select_arm_N(10);
    for(auto x: member)
        cout<<x<<endl;

    cout<<"select 0 \n"<<endl;
    member = goods.select_arm_N(0);
    for(auto x: member)
        cout<<x<<endl;

    /*ouput the data and read it to another one*/
    string output = goods.toString();

    UCB1 another_one;
    another_one.readFromString(output);

    cout<<"another_one readfrom str and now is \n"<<endl;
    cout<<another_one.toString();
}
