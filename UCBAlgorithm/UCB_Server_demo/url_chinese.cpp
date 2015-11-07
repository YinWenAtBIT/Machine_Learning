/*************************************************************************
	> File Name: url_chinese.cpp
	> Author: YinWen
	> Mail: yinwenatbit@163.com
	> Created Time: 2015年09月29日 星期二 15时59分31秒
 ************************************************************************/

#include "url_chinese.h"
using namespace std;

char Char2Int(char ch){
        if(ch>='0' && ch<='9')return (char)(ch-'0');
        if(ch>='a' && ch<='f')return (char)(ch-'a'+10);
        if(ch>='A' && ch<='F')return (char)(ch-'A'+10);
        return -1;
}

char Str2Bin(char *str){
        char tempWord[2];
        char chn;

        tempWord[0] = Char2Int(str[0]);                                //make the B to 11 -- 00001011
        tempWord[1] = Char2Int(str[1]);                                //make the 0 to 0  -- 00000000

        chn = (tempWord[0] << 4) | tempWord[1];                //to change the BO to 10110000

        return chn;
}

string UrlDecode(string str){
        string output="";
        char tmp[2];
        int i=0,idx=0,ndx,len=str.length();
        
        while(i<len){
                if(str[i]=='%'){
                        tmp[0]=str[i+1];
                        tmp[1]=str[i+2];
                        output+=Str2Bin(tmp);
                        i=i+3;
                }
                else if(str[i]=='+'){
                        output+=' ';
                        i++;
                }
                else{
                        output+=str[i];
                        i++;
                }
        }
        
        return output;
}

//输入url_Utf-8 ,输出 gb2312
string Url2Str_Utf8(string instr){
        string input;
        input=UrlDecode(instr);

        const int        outlen=instr.length();
        char output[outlen];

        CodeConverter cc = CodeConverter("utf-8","gb2312");
        cc.convert((char *)input.c_str(),strlen(input.c_str()),output,outlen);

        return output;
}

string Url2Str_gb2312(string str){
        return UrlDecode(str);
}
