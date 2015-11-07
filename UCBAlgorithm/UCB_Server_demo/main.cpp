/*************************************************************************
	> File Name: main.cpp
	> Author: YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: Sun 06 Sep 2015 06:10:36 PM CST
 ************************************************************************/

#include<iostream>
#include <served/served.hpp>
#include <served/plugins.hpp>
#include <fstream>
#include "json/json.h"
#include <ctime>
#include <cstdlib>
#include "url_chinese.h"
#include "UCB1.h"

using namespace std;

void addurl(unordered_map<string, vector<string>> &good_url, string &key, string &url);

void initialize(UCB1 &tags_key_goods, UCB1 & topics_key_goods, unordered_map<string, vector<string>> &tag_jason, unordered_map<string, vector<string>> &topic_jason);

void addlike(string &x, string &key, string &count_str, string & value_str, string &totalcount_str, string &jsonstr, string & output);

int main()
{
	UCB1 tags_key_goods(1, 10, 0.5), topics_key_goods(1, 10, 0.5);
	unordered_map<string, vector<string>> tag_jason;
	unordered_map<string, vector<string>> topic_jason;
	
	initialize(tags_key_goods, topics_key_goods, tag_jason, topic_jason);

	string key;
	string number;
	
	served::multiplexer mux;
	mux.use_after(served::plugin::access_log);

	const std::string image_name("served-logo.png");

	mux.handle("/UCB/query")
		.get([&](served::response & res, const served::request & req) {

			
//			res.set_header(std::string("charset"), std::string("gb2312"));
			key = req.query["key"];
			number = req.query["number"];
			
			int chonum = atoi(number.c_str());
			std::vector<std::string>  tohttp;
			string totalcount_str;

			if(key == "tags")
			{
				tohttp = tags_key_goods.select_arm_N(chonum);
				totalcount_str = tags_key_goods.get_totalcount();
			}
			else if (key == "topics")
			{
				tohttp = topics_key_goods.select_arm_N(chonum);
				totalcount_str = topics_key_goods.get_totalcount();

			}
			else
				cout<<"wrong key"<<endl;
				
			std::string totaloutput;
			string select_good;
			string value_str;
			string count_str;


			totaloutput.reserve(1000);
			totaloutput ="<html>";
			totaloutput += "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />";
			totaloutput +="<table border = \"1\"> <tr> <th> 商品名字</th> <th>图片</th><th>属性</th><th>选择</th><th>UCB参数</th></tr>";
			for(auto x: tohttp)
			{
				vector<string> allurl;
				if(key == "tags")
				{
					allurl = tag_jason[x];
					count_str = tags_key_goods.get_count(x);
					value_str = tags_key_goods.get_value(x);
				}
				if(key == "topics")
				{
					allurl = topic_jason[x];
					count_str = topics_key_goods.get_count(x);
					value_str = topics_key_goods.get_value(x);

				}
				int number = rand() % allurl.size();
				select_good = allurl[number];
				

				addlike(x, key, count_str, value_str, totalcount_str, select_good, totaloutput);
			}


			totaloutput +="</html>";
			res.set_body(totaloutput);
		});

	string res_key;
	string res_target;
	string res_choose;

	mux.handle("/UCB/response")
		.get([&](served::response & res, const served::request & req) {
			
			res_key = req.query["key"];
			res_choose = req.query["choose"];
			double updatevalue = atof(res_choose.c_str());
			res_target = Url2Str_gb2312(req.query["target"]);
			cout<<"target: "<<res_target<<"  value :"<<res_choose<<endl;
			if(key == "tags")
			{
				tags_key_goods.update_reset_last(res_target, updatevalue);
				string value_str = tags_key_goods.get_value(res_target);
				cout<<"key: "<<res_target<<"  now value is: "<<value_str<<"updatavalue: "<<updatevalue<<endl;
			}
			if(key == "topics")
			{
				topics_key_goods.update_reset_last(res_target, updatevalue);
				string value_str = topics_key_goods.get_value(res_target);
				cout<<"key: "<<res_target<<"  now value is: "<<value_str<<"updatavalue: "<<updatevalue<<endl;

			}
		});
	
	std::cout << "Try this example by opening http://211.144.146.217:8123/UCB/query?key=tags&number=10 in a browser" << std::endl;

	served::net::server server("0.0.0.0", "8123", mux);
	server.run(10);

	return (EXIT_SUCCESS);

}


void initialize(UCB1 &tags_key_goods, UCB1 & topics_key_goods, unordered_map<string, vector<string>> &tag_jason, unordered_map<string, vector<string>> &topic_jason)
{

	ifstream data("20150924015910.attri"); 
	string readdata;
		

	Json::Reader reader;
	Json::Value value;
	Json::Value tags;
	Json::Value topic;
	std::string key;
	std::string url;
	int readcount;
	double readfloat;

	Json::FastWriter writer;
	int counts =0;
	
	srand(time(0));

	while(!data.eof())
	{

		getline(data, readdata);

		if(!reader.parse(readdata, value))
       		break;

		topic = value["topics"];
		if(!topic.empty())
		{
			unsigned int j=0;
			key = topic[j].asString();
			topics_key_goods.insert(key);
			addurl(topic_jason, key, readdata);
		}

		/*use tags as key*/
		tags = value["tags"];
		unsigned int num = tags.size();
		for(unsigned int i=0; i< num; i++)
		{
			key = tags[i].asString();
			
			tags_key_goods.insert(key);
			addurl(tag_jason, key, readdata);
		}

	}

	data.close();

	

}


void addlike(string &x, string &key, string &count_str, string & value_str, string &totalcount_str, string &jsonstr, string & output)
{
	Json::Reader reader;
	Json::Value value;
	Json::Value tags;
	std::string topic;
	std::string url;
	std::string imag;
	std::string good_name;

	unsigned int i = 0;

	reader.parse(jsonstr, value);
	
	url = value["goods_channel_origin_url"].asString();
	tags = value["tags"];
	topic = value["topics"][i].asString();
	imag = value["image"][i].asString();
	good_name = value["goods_name"].asString();

	std::string likestring;
	likestring.reserve(100);



	likestring = "<tr><td><a href=\"http://";
	likestring +=url;
	likestring +="\">";
	likestring +=good_name;
	likestring +="</td>";
	output += likestring;

	likestring = "<td><img src=\"";
	likestring += imag;
	likestring += "\" width=\"200\" height=\"100\" /> </td>";
	output += likestring;
	
	
	likestring = "<td>";

	unsigned int num = tags.size();
	for(i=0; i< num; i++)
	{
		likestring += tags[i].asString();
		likestring += "<br>";
	}

	likestring += topic;
	likestring += "</td>";
	output += likestring;

	likestring = "<td><a href=\"http://211.144.146.217:8123/UCB/response?key=";
	likestring += key;
	likestring += "&target=";
	likestring +=x;
	likestring +="&choose=1.0\"> like</a><br>";
	output += likestring;


	likestring = "<a href=\"http://211.144.146.217:8123/UCB/response?key=";
	likestring += key;
	likestring += "&target=";
	likestring +=x;
	likestring +="&choose=0.5\"> soso</a><br>";
	output += likestring;


	likestring = "<a href=\"http://211.144.146.217:8123/UCB/response?key=";
	likestring += key;
	likestring += "&target=";
	likestring +=x;
	likestring +="&choose=0.0\"> dislike</a></td>";
	output += likestring;

	likestring = "<td>count:";
	likestring += count_str;
	likestring += "<br>value:";
	likestring += value_str;
	likestring +="<br>totalcount:";
	likestring +=totalcount_str;
	likestring +="</td></tr>";
	output += likestring;

	
}


void addurl(unordered_map<string, vector<string>> &good_url, string &key, string &url)
{
	auto origin = good_url.find(key);
	if(origin == good_url.end())
   	 {
		vector<string> tag_url;
		tag_url.push_back(url);
 	 	good_url.insert({key, tag_url});	
    	}
	else
	{
		origin->second.push_back(url);
	}
}
