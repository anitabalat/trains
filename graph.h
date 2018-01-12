#ifndef GRAPH_MATRIX_H
#define GRAPH_MATRIX_H

#include <cstddef>
#include <iostream>
#include <iomanip>
#include <stack>
#include <queue>
#include <climits>
#include <sstream>
#include <locale>
#include <cstdio>
using namespace std;
#define INFINITY INT_MAX

struct node {

	bool edge;                   // True or false if edge exists
	string arr;					 // Strings to hold arrival and departure times to be converted
	string dept;
	int arrHour;				 // Integers to hold hours and minutes for arrival and departur
	int arrMins;
	int deptHour;
	int deptMins;
	int hrsBetween;				// Integers to hold hours and minutes od travel from adjacent station
	int minsBetween;
};

struct layovers {				// Struct to hold layover times between stations

	int hrs;
	int mins;
	int setHrs;
	int setMins;

};

struct times {					// Struct to hold times recorded in Dijkstra's

	int hrs;
	int mins;

};

class GRAPH{

public:

	GRAPH();
	void setEdge(int, int, string, string);
	void setStation(int, string);
	void findNameByNumber(int);
	void findNumberByName(char arr[]);
	void printAll();
	void display();
	void select();
	void initialize(char arr[]);
	void printStation(int);
	void nonstop(int, int);
	void serviceAvailable(int,int);
	void shortestTime(int,int);
	void shortestTimeLayover(int,int);
	void shortestTimeLayoverPlusArrivalTime(int,int,string);

private:

	int nodeCount;									// Keeps track of nodes									
	node matrix[200][200];							// matrix of nodes
	layovers layover[200];								// keep track of layover times between stations
	string names [200];								// array of strings to hold station names
	int dijkstraVisited[200];
	int pathCapture[200];
	times timesRecorded[200];
	queue<int>myDijQueue;
	stack<int>myPathDst;
	stack<int>myPathSrc;
	int arriveAtStationHrs;
	int arriveAtStationMins;
	
	void a_to_b_hrs(int,int,int,int);
	void a_to_b_mins(int,int,int,int);
	void addHrs (int,int,int);
	void addMins(int,int,int);
	void dijkstra_helper(int,int);
	void countNodes();
	void timesRecordedSetUp();
	void pathCaptureSetUp();
	void dijkstraVisitedSetUp();
	bool haveAllBeenVisited();
	int findUnvisited();
	void print_dijkstra(int);
	int findMin(int,int,int);
	void printService(int,int);
	bool printShortestPath(int,int);
	void layoverSetUp();
	void dijkstra_layover_helper (int start, int finish, int arriveCheck);
	int layoverHrs (int,int);
	int layoverMins (int,int,int*n);
	void addLayHrs(int,int,int,int);
	void addLayMins(int,int,int,int);
	bool printShortestPathLayover(int,int);
	void itinerary(int, int);	
};

#endif