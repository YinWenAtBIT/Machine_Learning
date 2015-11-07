C=gcc
CXX=g++
CFLAGS= -g -D_DEBUG -fPIC -Wshadow -Wcast-qual -Wcast-align -Wwrite-strings -Wsign-compare -Winvalid-pch -fms-extensions -Wall -MMD
CPPFLAGS=$(CFLAGS) -Woverloaded-virtual -Wsign-promo -fno-gnu-keywords -std=c++11 

DEPS=-I/home/yinwen/jsoncpp-src-0.5.0/include  -I/home/yinwen/served/src  /home/yinwen/jsoncpp-src-0.5.0/libs/linux-gcc-4.8.2/libjson_linux-gcc-4.8.2_libmt.a 

LIBPATH=/home/yinwen/served/lib

LIB=-lserved 
#-lboost_system-mt -lstdc++

OBJS=main.o UCB1.o url_chinese.o

TARGET=unittest_UCB1


ALL: $(TARGET)

$(TARGET):$(OBJS)
	$(CXX) $(CPPFLAGS) -o unittest_UCB1 $^  $(DEPS) -L$(LIBPATH) $(LIB)

UCB1.o: UCB1.cpp
	$(CXX) $(CPPFLAGS) -c $<  $(DEPS)

url_chinese.o:url_chinese.cpp
	$(CXX) $(CPPFLAGS) -c $<  $(DEPS)

main.o:main.cpp
	$(CXX) $(CPPFLAGS) -c $<  $(DEPS)

