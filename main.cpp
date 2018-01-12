#include <fstream>
#include <iostream>
#include "graph.h"
using namespace std;

int main()
{
	GRAPH* g = NULL;										// Creating graph ptr

	ifstream file;											// Setting up IO variable
	int source, destination;								// Holds source and destination read
	string arr, dest, name;									// Holds times and names read
	int stationNum;
	char stationName[25] ={0};
	int station;
	int src,dst;
	string time;
	

	g = new GRAPH();

	file.open("stations.dat");								// Reading station names and numbers into names array
	if (!file.is_open())
	{
		cout << "Error: Failed to open input file\n";
		return 0;
	}
	
	while (file >> source)
	{                
		file >> name;
		g->setStation(source, name);
	}
	file.close();
	
	file.open("trains.dat");								
	if (!file.is_open())
	{
		cout << "Error: Failed to open input file\n";
		return 0;
	}
											// Setting new graph object to ptr

	while (file >> source)									// Reading arrival and departure times between relevant stations into matrix
	{                
		file >> destination;
		file >> arr;
		file >> dest;
		g->setEdge(source, destination, arr, dest);
	}
	file.close();

	g->display();												// Display program intro

    int choice = 1;												// Initializing choice to 1 to ensure it has a non 0 value  

	 while (choice != 0){
 
    	g->select();											// Display user options
    	cin >> choice;
    	
    	switch(choice){

    		case 0:
    			cout << endl;
    			cout <<"Goodbye!" << endl;
    			break;

    		case 1:												// Prints entire train schedule for all trains
        		 g->printAll();
        		break;

    		case 2:												// Prints entire train schedule for a specific train
       			cout << "Enter station id: ";
       			cin >> station;
       			g->printStation(station);
        		break;

    		case 3:												// Finds name of station by station number
    			cout << "Enter station id: ";
    			cin >> stationNum;
        		g->findNameByNumber(stationNum);
        		break;

        	case 4:												// Finds number of station by name      		
        		g->initialize(stationName);						// Reinitializes stationName for new name to be entered
	       		cout << "Enter station name: ";
        		cin >> stationName;
        		g->findNumberByName(stationName);
        		break;

        		
        	case 5:												// Determines if service available between stations
        		cout << "Enter departure station id: ";
        		cin >> src;
        		cout << "Enter destination station id: ";
        		cin >> dst;
        		g->serviceAvailable(src,dst);
        		break;

        	case 6:												// Find out if non stop service is available
        		cout <<"Enter departure station id: ";
        		cin >> src;
        		cout <<"Enter destination station id: ";
        		cin >> dst;
        		g->nonstop(src,dst);
        		break;

        	case 7:												// Find out shortest path (no layovers)
	        	cout << "Enter departure station id: ";
	        	cin >> src;
	        	cout <<"Enter destination station id: ";
	        	cin >> dst;
	        	g->shortestTime(src,dst);
	        	break;

	        case 8:												// Find shortest path (with layovers)
	        	cout <<"Enter departure station id: ";
	        	cin >> src;
	        	cout <<"Enter destination station id: ";
	        	cin >> dst;
	        	g->shortestTimeLayover(src,dst);
	        	break;

	        case 9:												// Find shortest path (with layovers including arrival time)
	        	cout << "Enter departure station id: ";
	        	cin >> src;
	        	cout << "Enter destination station id: ";
	        	cin >> dst;
	        	cout << "Enter arrival time (Format: 00:00): ";
	        	cin >> time;
	        	g->shortestTimeLayoverPlusArrivalTime(src,dst,time);
	        	break;

    		default:
        		cout <<"The number you have entered is invalid." << endl;
        		cout <<"Please try again."<< endl << endl;

    	}
 	}

	return 0;
}

