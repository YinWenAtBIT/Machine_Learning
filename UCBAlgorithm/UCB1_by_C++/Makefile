C=gcc
CXX=g++
CFLAGS= -g -D_DEBUG -fPIC -Wshadow -Wcast-qual -Wcast-align -Wwrite-strings -Wsign-compare -Winvalid-pch -fms-extensions -Wall -MMD
CPPFLAGS=$(CFLAGS) -Woverloaded-virtual -Wsign-promo -fno-gnu-keywords -std=c++11 

DEPS=/home/yw/jsoncpp-src-0.5.0/include  /home/yw/jsoncpp-src-0.5.0/libs/linux-gcc-4.8.3/libjson_linux-gcc-4.8.3_libmt.a 
OBJS=main.o UCB1.o

TARGET=unittest_UCB1


ALL: $(TARGET)

$(TARGET):main.o UCB1.o 
	$(CXX) $(CPPFLAGS) -o unittest_UCB1  main.o UCB1.o -I$(DEPS)

UCB1.o: UCB1.cpp
	$(CXX) $(CPPFLAGS) -c $<  -I$(DEPS)

main.o:main.cpp
	$(CXX) $(CPPFLAGS) -c $< -I$(DEPS)
