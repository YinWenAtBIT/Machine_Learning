#########################################################################
# Created Time: 2015年12月09日 星期三 20时37分06秒
#########################################################################
#!/bin/bash
source ~/.bashrc

if [ -z $USER_FOLLOW_RATE_HOME ]
then
    echo "USER_FOLLOW_RATE_HOME is not set."
    exit
fi

cd $USER_FOLLOW_RATE_HOME

python start_read.py 
python sort_user_follow.py
