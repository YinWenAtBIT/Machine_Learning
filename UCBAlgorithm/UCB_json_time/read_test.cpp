/*************************************************************************
	> File Name: read_test.cpp
	> Author: YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: 2015年10月18日 星期日 18时55分55秒
 ************************************************************************/

#include "json/json.h"
#include <cstdlib>
#include <string>
#include <fstream>
#include "UCB1.h"
#include <ctime>
#include <cstdio>
#include <sys/time.h>
#include <unistd.h>


using namespace std;
void addurl(unordered_map<string, vector<string>> &good_url, string &key, string &url);
void initialize(UCB1 &tags_key_goods, UCB1 & topics_key_goods, unordered_map<string, vector<string>> &tag_jason, unordered_map<string, vector<string>> &topic_jason);


int main()
{
	
	std::string tags_str, topics_str;
	
	ifstream tags_json_file("./topics_and_tags/tags_json_file_0.8w.txt");
	ifstream topics_json_file("./topics_and_tags/topics_json_file_0.8w.txt");

	if(!tags_json_file.is_open())
		cout<<"cannot open tags_json_file"<<endl;

	if(!topics_json_file.is_open())
		cout<<"cannot open topics_json_file"<<endl;



	topics_json_file>>topics_str;


	tags_json_file>>tags_str;

	struct timeval timebefore, timenow;	
	long time_tags = 0, time_topics = 0;

	for(int i =0; i<100; i++)
	{
		UCB1 tags_key_goods, topics_key_goods;

		gettimeofday(&timebefore, NULL);
		topics_key_goods.readFromString(topics_str);
//		topics_key_goods.select_arm_N(200);
		gettimeofday(&timenow, NULL);

		time_topics += (timenow.tv_sec - timebefore.tv_sec)*1000000 +timenow.tv_usec - timebefore.tv_usec;
		


		gettimeofday(&timebefore, NULL);
		tags_key_goods.readFromString(tags_str);	
//		tags_key_goods.select_arm_N(200);
		gettimeofday(&timenow, NULL);
	
		time_tags += (timenow.tv_sec - timebefore.tv_sec)*1000000 +timenow.tv_usec - timebefore.tv_usec;
	}

	cout<<"topics_time in ms: "<<time_topics<<endl;
	cout<<"tags_time in ms: "<<time_tags<<endl;
}





