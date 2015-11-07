/*************************************************************************
	> File Name: url_chinese.h
	> Author: YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: 2015年09月29日 星期二 15时58分08秒
 ************************************************************************/
#include <iconv.h>
#include <string>
#include <cstdlib>
#include <cstring>

#ifndef _URL_CHINESE_H
#define _URL_CHINESE_H



char Char2Int(char ch);
char Str2Bin(char *str);
std::string UrlDecode(std::string str);
std::string Url2Str_Utf8(std::string instr);

std::string Url2Str_gb2312(std::string str);

class CodeConverter {
        private:
                        iconv_t cd;
        public:
                        CodeConverter(const char *from_charset,const char *to_charset) {// 构造
                                cd = iconv_open(to_charset,from_charset);
                        }
                
                        ~CodeConverter() {// 析构
                                iconv_close(cd);
                        }
                
                        int convert(char *inbuf,int inlen,char *outbuf,int outlen) {// 转换输出
                                char **pin = &inbuf;
                                char **pout = &outbuf;

                                memset(outbuf,0,outlen);
                                return iconv(cd,pin,(size_t *)&inlen,pout,(size_t *)&outlen);
                        }
};

#endif

