CXX = g++
CXXFLAGS = -Wall -Werror -std=c++17

all: loadbalancer

loadbalancer: main.o LoadBalancer.o WebServer.o IPblock.o
	$(CXX) $(CXXFLAGS) -o loadbalancer main.o LoadBalancer.o WebServer.o IPblock.o

main.o: main.cpp LoadBalancer.h
	$(CXX) $(CXXFLAGS) -c main.cpp

LoadBalancer.o: LoadBalancer.cpp LoadBalancer.h Request.h WebServer.h IPblock.h
	$(CXX) $(CXXFLAGS) -c LoadBalancer.cpp

WebServer.o: WebServer.cpp WebServer.h Request.h
	$(CXX) $(CXXFLAGS) -c WebServer.cpp

IPblock.o: IPblock.cpp IPblock.h
	$(CXX) $(CXXFLAGS) -c IPblock.cpp

clean:
	rm -f loadbalancer *.o lb_log.txt