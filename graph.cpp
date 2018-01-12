#include "graph.h"

GRAPH::GRAPH() {         												// Constructor that takes no parameter 

	for(int i = 0; i < 200; i++){										// Initialize matrix of nodes to the following

		for(int j = 0; j < 200; j++){

			matrix[i][j].edge = false;									// Set edge to false, because no edge has yet been set
			matrix[i][j].arr = " ";										// Set strings for arrival and departure times as empty															
			matrix[i][j].dept = " ";
		}
	}

	for (int i = 0; i < 200; i++){										// Set station names to empty strings

		names[i] = " ";
	}

	nodeCount = 0;                                     					// Initialize nodeCount to 0
	arriveAtStationHrs = 0;
	arriveAtStationMins = 0;

} // end constructor

void GRAPH::setEdge(int source, int destination, string a, string d){	// Set weighted edge between source and destination	

	if(matrix[source][destination].edge == false ){						// If an edge does not already exist

		matrix[source][destination].dept = a;							// Set the arrival and departure times as strings a & d	
		matrix[source][destination].arr = d;
		matrix[source][destination].edge = true;						// An edge has now been set, so set edge to true
	}

	string aH = matrix[source][destination].arr.substr(0,2);			// Filtering substrings to split arrival hours and minutes
	string aM = matrix[source][destination].arr.substr(2,3);
	string dH = matrix[source][destination].dept.substr(0,2);			// Filtering substrings to split departure hours and minutes
	string dM = matrix[source][destination].dept.substr(2,3);

	stringstream aHrs (aH);												// Creating stringstream objects to convert string vals to ints
	stringstream aMin (aM);
	stringstream dHrs (dH);
	stringstream dMin (dM);
								
	aHrs >> matrix[source][destination].arrHour;						// Sending int values to arrival and departure hours and minutes
	aMin >> matrix[source][destination].arrMins;
	dHrs >> matrix[source][destination].deptHour;
	dMin >> matrix[source][destination].deptMins;

	int arrHour = matrix[source][destination].arrHour;					// Making copies of integer vals for hrs and mins
	int arrMins = matrix[source][destination].arrMins;
	int deptHour = matrix[source][destination].deptHour;
	int deptMins = matrix[source][destination].deptMins;

	a_to_b_hrs(source,destination,deptHour,arrHour);					// Calling function to calculate time to get from one node to next
	a_to_b_mins(source,destination,deptMins,arrMins);

	countNodes();														// Keep count of how many nodes are actually in the matrix

} // end function setEdge

// Purpose of the below is to keep track of nodes visited when using Dijkstra's, because we need to know number of elements used vs elements available
void GRAPH::countNodes(){												// Keeps track of actul used nodes in matrix
																		// Just in case stations added, but no trains pass 
	int counter = 0;
	int check[200]={0};													// Create array to check off marked nodes

	for(int i = 1; i<200;i++){											// Loop through matrix

		for(int j = 1; j<200;j++){

			if(names[i] != " "){										// Only check indices related to stations
																		// If edge exists and j unchecked
				if( (matrix[i][j].edge == true || matrix[j][i].edge == true) && check[j]!= 1){
					
					if(check[i] == 0){									// If i hasn't already been checked								
					
						check[i] = 1;									// Check i
					}

					check[j] = 1;										// Check j
				}			
			}
		}
	}

	for(int i = 0; i<200;i++){											// Loop through checked array

		if(check[i]==1){												// If index has been checked, increment counter
			counter++;
		}
	}

	nodeCount = counter;	
												// Set nodeCount to counter
} // end function countNodes

void GRAPH::serviceAvailable(int src, int dst){										// Public check if service available function

	if( (src <= 199 && dst <=199)  && (names[src]!= " " && names[dst]!= " ")   ){	// If valid source and destination entered
		
		dijkstra_helper(src, dst);													// Call helper function to check for path
		printService(src,dst);														// Print if service is available
	}
	else{

		cout <<endl;																// If invalid stations entered, alert user
		cout <<"Invalid stations entered"<< endl;
		cout <<endl;
	}

} // end function serviceAvailable

void GRAPH::a_to_b_hrs(int src, int dst, int a, int b){					// Calculates total hrs between adjacent stations
	
    matrix[src][dst].hrsBetween = b - a;								
   
} // end function a_to_b_hrs

void GRAPH::addLayHrs(int a,int b, int c, int index){					// Adds hours for shortest path including layover

	timesRecorded[index].hrs = a + b + c;

} // end function addLayHrs


void GRAPH::addHrs(int src, int dst, int index){						// Adds hours for standard shortest path

	timesRecorded[index].hrs = src + dst;

} // end function addHrs

int GRAPH::layoverHrs(int dept, int arr){								// Calculates wait between two stations
		
	int hrs = arr - dept;

	if(arr < dept){

		hrs = (24 - dept) + arr;
	}

	return hrs;
} // end function layoverHrs

void GRAPH::addLayMins(int a, int b, int c, int index){					// Adds all minutes for shortes path incluing layover
	
	int min = a + b + c;

	if(min % 60 == 0){													// If mins added are divisible by 60

		timesRecorded[index].mins = 0;									// Set mins to 0
		timesRecorded[index].hrs += min / 60;							// Set hrs to hrs + integer only result of division
	}

	if(min > 60){														// If mins greater than 60

		timesRecorded[index].hrs += min / 60;							// Set hrs to hrs + integer only result of division
		timesRecorded[index].mins = min - ((min/60)*60);				// Set mins to remaining mins after hour(s) have been subtracted
	}

	if(min < 60){														// If mins are less than 60, just assign min

		timesRecorded[index].mins = min;		
	}
} // end function addLayMins

void GRAPH::addMins(int src, int dst, int index){						// Adds minutes
	
	int min = src + dst;												

	if(min % 60 == 0){													// If mins added are divisible by 60											

		timesRecorded[index].mins = 0;									// Set mins to 0							
		timesRecorded[index].hrs += min / 60;							// Set hrs to hrs + integer only result of division
		
	}

	if(min > 60){														// If mins greater than 60					

		timesRecorded[index].hrs += min / 60;							// Set hrs to hrs + integer only result of division
		timesRecorded[index].mins = min - ((min/60)*60);				// Set mins to remaining mins after hour(s) have been subtracted		
	}	

	if(min < 60){														// If mins are less than 60, just assign min

		timesRecorded[index].mins = min;		
	}
}

int GRAPH::layoverMins(int dep,int arr,int*hrs){						// Calculates total layover minutes

	int hourcheck = 60;	
	int mins;															// Variable representing mins in an hour and mins to be returned

	if(dep == arr){														// If mins are the same

		mins = 0;														// Set mins to 0
	}	

	if(dep == 0 && arr!= 0){											// If departing mins are 00 and arrival minutes are not
		
		mins = arr;														// Set mins to arrival mins
	}

	if(dep != 0 && arr ==0){											// if departure mins are not 0 and arrival mins are
		
		mins = hourcheck - dep;											// Set mins to 60 - departure mins
		*hrs -=  1;														// Decrease total layover hrs by 1
	}

	if(dep != 0 && arr!= 0 && arr > dep){								// If neither departure or arrival mins are 0 and arrival > departure 
		
		mins = arr-dep;													// Set mins to arrival mins - departure mins
	}

	if(dep != 0 && arr!= 0 && arr < dep){								// If neither departure or arrival mins are 0 and arrival < departure
		
		mins = (hourcheck - dep) + arr;									// Set mins to arrival mins plus the difference between 60 and departure
		*hrs -= 1;		
	}

	return mins;														// Return mins
}
// end function layoverMins
void GRAPH::a_to_b_mins(int src,int dst, int dep, int arr){				// Calculates total mins between adjacent stations (adjust hrs accordingly)
	
	int hourcheck = 60;													// Variable representing mins in an hour

	if(dep == arr){														// If departure mins equal arrival mins

		matrix[src][dst].minsBetween = 0;								// Set mins between stations to 0
	}	

	if(dep == 0 && arr!= 0){											// If departure is 0 and arrival isn't
		
		matrix[src][dst].minsBetween = arr;								// Set mins between stations to arrival mins
	}

	if(dep != 0 && arr ==0){											// If departure mins not 0 and arrival is
		
		matrix[src][dst].minsBetween = hourcheck - dep;					// Set mins between as 60 - departure
		matrix[src][dst].hrsBetween -= 1;								// Decrease hrs between stations by 1
	}

	if(dep != 0 && arr!= 0 && arr > dep){								// If depart and arrival mins not 0 and arrival > departure mins
		
		matrix[src][dst].minsBetween = arr-dep;							// Set mins between to arrival - departure mins
	}

	if(dep != 0 && arr!= 0 && arr < dep){								// If neither is 0 and arrival < departure
		
		matrix[src][dst].minsBetween = (hourcheck - dep) + arr;			// Set mins between to arrival plus difference between 60 and departure mins
		matrix[src][dst].hrsBetween -= 1;								// Decrease hrs between by 1
	}

} // end function a_to_b_mins

void GRAPH::shortestTime(int src, int dst){												// Calculates shortest train travel time to a destination
	
	bool pathExists = false;

	if( (src <= 199 && dst <=199)  && (names[src]!= " " && names[dst]!= " ")   ){		// Checks that source and destination stations are valid
		
		dijkstra_helper(src,dst);														// If so, call helper functions to check for path
		pathExists = printShortestPath(src, dst);
		
		if(pathExists == true){															// If path exists, print itinerary

			itinerary(src,dst);
		}
	}
	else{																				// Else if invalid stations entered, alert user

		cout<< endl;
		cout<<"Invalid stations entered"<< endl;
		cout<< endl;
	}

} // end public shortestTime

void GRAPH::shortestTimeLayoverPlusArrivalTime(int src, int dst, string time){			// Calculates shortest travel time with layovers from arrival

	bool pathExists = false;															

	if( (src <= 199 && dst <=199)  && (names[src]!= " " && names[dst]!= " ")   ){		// If valid stations entered

		string aH = time.substr(0,2);													// Filtering substrings to split arrival hours and minutes
		string aM = time.substr(3,2);
		stringstream aHrs (aH);															// Creating stringstream objects to convert string vals to ints
		stringstream aMin (aM);
		aHrs >> arriveAtStationHrs;														// Sending int values to arrival and departure hours and minutes
		aMin >> arriveAtStationMins;
		
		dijkstra_layover_helper(src, dst, 1);											// Calling relevant functions to check for path
		pathExists = printShortestPathLayover(src, dst);
		
		if(pathExists == true){															// If path exists, print itinerary
		
			itinerary(src,dst);
		}
	}
	else{																				// Else if invalid stations provided, alert user

		cout<< endl;
		cout<<"Invalid stations entered"<< endl;
		cout<< endl;
	}

} // end public shortestTimeLayoverPlusArrivalTime function

void GRAPH::shortestTimeLayover(int src, int dst){										// Calculates shortest travel time including layovers to a destination
	
	bool pathExists = false;

	if( (src <= 199 && dst <=199)  && (names[src]!= " " && names[dst]!= " ")   ){		// If stations entered valid, check for a path
		
		dijkstra_layover_helper(src,dst,0);
		pathExists = printShortestPathLayover(src,dst);
	
		if(pathExists == true){															// If path exists, print itinerary

			itinerary(src,dst);
		}
	}
	else{																				// Else if invalid stations entered, alert user

		cout<< endl;
		cout<<"Invalid stations entered"<< endl;
		cout<< endl;
	}

} // end shortestTimeLayover public function

void GRAPH::dijkstra_helper (int start, int finish){								// Function that find standard shortest path
	
	int unknownTime;																// Current vertex looking at
	int next;																		// Next vertex to be added to the queue
	int lastTimeAdded;																// Last vertex to which time added
	timesRecordedSetUp();															// Call function to set up times, visited and path capture arrays
	dijkstraVisitedSetUp();
	pathCaptureSetUp();																
	timesRecorded[start].hrs = 0;													// Set the time to reach the start vertex itself to 0:00
	timesRecorded[start].mins = 0;
	int tmpH1, tmpH2, tmpM1, tmpM2; 												// Variable to temporarily hold hrs and minutes

	myDijQueue.push(start);															// Push starting vertex onto the queue

	while(!myDijQueue.empty()){														// While the queue is not empty
		
		unknownTime = myDijQueue.front();											// Set unknown time to the front of the queue
		myDijQueue.pop();															// Pop the front of the queue
		dijkstraVisited[unknownTime] = 1;											// Set visited array for unknown distance to 1
		
		for (int i = 1; i < 200; i++){ 												// Search through all adjacent vertices
		
			if (matrix[unknownTime][i].edge == true && dijkstraVisited[i] == 0){ 	// If i is adjacent and i has not been visited				
																		  
				if(((timesRecorded[unknownTime].hrs + matrix[unknownTime][i].hrsBetween) < timesRecorded[i].hrs) ||
					((timesRecorded[unknownTime].hrs + matrix[unknownTime][i].hrsBetween) == timesRecorded[i].hrs) &&
					((timesRecorded[unknownTime].mins + matrix[unknownTime][i].minsBetween) < timesRecorded[i].mins) ||
					 (timesRecorded[i].hrs == -1 && timesRecorded[i].mins ==-1)){ 
					
					tmpH1 = timesRecorded[unknownTime].hrs;							// Set hrs and mins to temporary variables
					tmpH2 = matrix[unknownTime][i].hrsBetween;
					tmpM1 = timesRecorded[unknownTime].mins;
					tmpM2 = matrix[unknownTime][i].minsBetween;

					addHrs(tmpH1, tmpH2, i); 										// Call function to add hrs to the times recorded
					addMins(tmpM1,tmpM2, i);										// Call function to add mins to the times recorded

					pathCapture[i] = unknownTime;									// Capture the path

					lastTimeAdded = i;											  	// Update lastDistAdded to i
				}
			}
		}

		next = findMin(lastTimeAdded, timesRecorded[lastTimeAdded].hrs, timesRecorded[lastTimeAdded].mins);  // Find the next smallest time
		
		if (next != 0){															  // If value of next isn't 0
			
			myDijQueue.push(next); 												  // Push next onto the queue	
		}

		if(next == 0){															  // If the value of next is 0, we have visited every node, so break
			
			break;
		}
	}
														  
} // end dijkstra_helper function

void GRAPH::pathCaptureSetUp(){														// Initialize pathCapture arrau

	for(int i = 0; i < 200; i++){

		pathCapture[i] = -1;
	}

} // end function pathCaptureSetUp

void GRAPH::printService(int start, int finish){									// Print whether service is available between 2 stations

	if(pathCapture[finish] != -1){																		// If available, print

		cout << endl;
		cout <<"Service is available from " << names[start] << " to " << names[finish]<< endl;
		cout << endl;
		
	}	
	else{																								// If not, alert user

		cout << endl;	
		cout <<"Service is unavailable from " << names[start] << " to " << names[finish]<< endl;
		cout << endl;			
	}
	 
} // end function printPath

bool GRAPH:: printShortestPathLayover(int src,int dst){													// Prints shortest path with layovers

	bool pathExists = false;

	if(pathCapture[dst] != -1){																			// If a path exists, print confirmation
		
		cout << endl;
		cout <<"Time on train to go from " << names[src] << " to " << names[dst] << " including layovers is ";
		cout << timesRecorded[dst].hrs << " hours and ";

		if(timesRecorded[dst].mins != 0){

			cout << timesRecorded[dst].mins << " minutes";
			cout << endl;
		}

		if(timesRecorded[dst].mins == 0){

			cout << "00 minutes"<< endl;
		}

		cout << endl;

		pathExists = true;
	}
	else{																								// Else alert user of no path

		cout << endl;
		cout <<"Service unavailable from "<< names[src] << " to " << names[dst] << endl;
		cout << endl;
				
		pathExists = false;	
	}
	
	return pathExists;

} // end function printShortestPathLayover

void GRAPH::itinerary(int start, int finish){															// Prints itinerary from source to destination

	int dst,src;

	if(pathCapture[finish] != -1){																		// If path exists
		
		for(int i = finish; i > 1;){																	// Cycle backwards from destination to source

			if(pathCapture[i] != -1){																	// Capture the sources and destinations
				
				myPathDst.push(i);																		// Push them onto relevant stacks
				myPathSrc.push(pathCapture[i]);
				i = pathCapture[i];																		// Set i to source
			}
		}

		cout <<"Itinerary"<< endl;
		cout <<"---------"<< endl;
		
		while(!myPathSrc.empty()){																		// While stack is not empty

			src = myPathSrc.top();																		// Get info from stacks and then pop both
			dst = myPathDst.top();
			myPathDst.pop();
			myPathSrc.pop();
																										// Print the path
			cout <<"Leave from "<< names[src] << " at " << matrix[src][dst].dept << ", arrive at " << names[dst] << " at " << matrix[src][dst].arr << endl;			
		}			
	}
	else{																								// Else if no path, inform user

		cout << endl;	
		cout <<"Service is unavailable from " << names[start] << " to " << names[finish]<< endl;				
	}

	cout << endl;

} // end function itinerary

bool GRAPH::printShortestPath(int src, int dst){														// Print the shortest path
	
	bool pathExists = false;

	if(pathCapture[dst] != -1){																			// If path exists
		
		cout << endl;
		cout <<"Time on train to go from " << names[src] << " to " << names[dst] << " is ";				// Print path details
		cout << timesRecorded[dst].hrs << " hours and ";

		if(timesRecorded[dst].mins != 0){

			cout<< timesRecorded[dst].mins << " minutes"<< endl;
		}

		if(timesRecorded[dst].mins == 0){

			cout <<"00 minutes"<< endl;
		}
		
		cout << endl;

		pathExists = true;
	}
	else{																								// Else if no path, inform user
			
		cout <<"Service unavailable from "<< names[src] << " to " << names[dst] << endl;
		
		pathExists = false;	
	}	
	
	return pathExists;

} // end printShortestPath

void GRAPH:: timesRecordedSetUp (){												 						// Initializes timesRecorded array to -1's
		
	for (int i = 0; i < 200 ; i++){    								  	
		
		timesRecorded[i].hrs = -1;
		timesRecorded[i].mins = -1;		
	}

} // end distSetUp function

void GRAPH:: dijkstraVisitedSetUp(){											 						// Initializes visited array to 0's
	
	for (int i = 0; i < 200 ; i++){    								  
		
		dijkstraVisited[i] = 0;		
	}

} // end dijkstraVisitedSetUp function

int GRAPH:: findUnvisited(){                									 	 					// Finds an unvisited vertex

	int unvisited;																

	for (int i = 1; i < 200; i ++){									  									// Loop through visited array

		if(dijkstraVisited[i] == 0){											  						// If find an unvisited index

			unvisited = i;														  						// Set unvisited to i
			return unvisited;													  						// Return index to caller
		}
	}

} // end findUnvisited function

bool GRAPH::haveAllBeenVisited(){											      						// Checks have all vertices been visited

	int count = 0;																  

	for (int i = 1; i < 200; i++){									  									// Loop through visited

		if(dijkstraVisited[i] != 0){											  						// If vertex has been visited
			
			count++;						                                      						// Increase count by 1
		}
	}

	if (count == nodeCount){													  // If count is the same as the number of nodes

		return true;															  // All nodes have been visited, so return true
	}

	if(count != nodeCount){														  // If count is not same as number of nodes

		return false;															  // All nodes have not been visited, so return false
	}

} // end haveAllBeenV

int GRAPH::findMin (int index, int hrs, int mins){						 		 // Finds smallest distance and return that index

	int min, minH, minM;													     // Variables to keep track of minimum hrs, mins and index they are at
	
	if(dijkstraVisited[index] == 0){											  // If index passed has not been visited, use it
		
		minH = hrs;
		minM = mins;														  
		min = index;
	}

	if(dijkstraVisited[index] != 0){											  // If index passed has been visited
		
		min = findUnvisited();													  // Call findUnvisited to find unvisited
		minH = timesRecorded[min].hrs;
		minM = timesRecorded[min].mins;											  // Set minimum times accordingly
	}

	for (int i = 1; i < 200; i++){									  				// Cycle through visited and times arrays
		
		if((dijkstraVisited[i] == 0) && (minH > timesRecorded[i].hrs || (minH == timesRecorded[i].hrs && minM < timesRecorded[i].mins)) && 
		  (timesRecorded[i].hrs != -1 && timesRecorded[i].mins != -1)){  
			
				minH = timesRecorded[i].hrs;
				minM = timesRecorded[i].mins;									  // Switch hours and minutes if smaller found
				min = i;														  // Keep track of the vertex at i
		} 
	}
	
	if(!haveAllBeenVisited()){													  // If all the vertices have not been visited
		
		return min;																  // Return the index
	}

	if(haveAllBeenVisited()){													  // If all the vertices have been visited

		return 0;																  // Return zero, to end the calling function
	}

} // end findMin function

void GRAPH::layoverSetUp (){												  	  // Sets up layover array
	
	for (int i = 0; i < 200 ; i++){    								  
		
		layover[i].hrs = -1;
		layover[i].mins = -1;	
		layover[i].setHrs = -1;
		layover[i].setMins = -1;	
	}

} // end distSetUp function

void GRAPH::dijkstra_layover_helper (int start, int finish, int arriveCheck){	// Finds shortest path, including layovers
	
	int unknownTime;															// Current vertex looking at
	int next;																	// Next vertex to be added to the queue
	int lastTimeAdded;															// Last vertex to which time added
	timesRecordedSetUp();														// Call function to set up time,visited, path and layover arrays
	dijkstraVisitedSetUp();
	pathCaptureSetUp();
	layoverSetUp();
	
	timesRecorded[start].hrs = 0;												// Set the time to reach the start vertex itself to 0:00
	timesRecorded[start].mins = 0;
		
	int tmpH1, tmpH2, tmpM1, tmpM2; 											// Variables to temporarily hold hrs and mins
	int layHrs, layMins, tmpLH1, tmpLH2, tmpLM1, tmpLM2;

	myDijQueue.push(start);														// Push starting vertex onto the queue

	while(!myDijQueue.empty()){													// While the queue is not empty

		unknownTime = myDijQueue.front();										// Set unknown time to the front of the queue
		myDijQueue.pop();														// Pop the front of the queue
		dijkstraVisited[unknownTime] = 1;										// Set visited array for unknown time to 1		

		for (int i = 1; i < 200; i++){ 											// Search through all adjacent vertices
		
			if (matrix[unknownTime][i].edge == true && dijkstraVisited[i] == 0){ // If i is adjacent and i has not been visited
				
				if(layover[unknownTime].setHrs == -1 && layover[unknownTime].mins == -1){ 	// If looking at starting node, don't add time

					if(arriveCheck == 0){													// If 0 was sent in, we are not including arrival time

						layHrs = 0;
						layMins = 0;
					}

					if(arriveCheck == 1){													// If 1 sent in, we are including arrival time

						layHrs = layoverHrs (arriveAtStationHrs, matrix[unknownTime][i].deptHour);				// Find hrs between arrival and dept
						layMins = layoverMins (arriveAtStationMins, matrix[unknownTime][i].deptMins,&layHrs);	// Find mins between arrival and dept

					}
					
					layover[i].hrs = matrix[unknownTime][i].arrHour;											// Record arrival at destination hrs
					layover[i].mins = matrix[unknownTime][i].arrMins;											// Record arriva at destination mins
				}
				
				if(layover[unknownTime].setHrs != -1 && layover[unknownTime].mins != -1){						// If not looking at start node

					tmpLH1 = layover[unknownTime].setHrs;														
					tmpLH2 = matrix[unknownTime][i].deptHour;
					tmpLM1 = layover[unknownTime].setMins;
					tmpLM2 = matrix[unknownTime][i].deptMins;
						
					layHrs = layoverHrs(tmpLH1,tmpLH2);															// Find times between arrival and depart													
					layMins = layoverMins(tmpLM1,tmpLM2,&layHrs);
				
					layover[i].hrs = matrix[unknownTime][i].arrHour;											// Record arrival at destination hrs and mins
					layover[i].mins = matrix[unknownTime][i].arrMins;					
				}				
																			  
				if(((layHrs + timesRecorded[unknownTime].hrs + matrix[unknownTime][i].hrsBetween) < timesRecorded[i].hrs) ||
					((layHrs + timesRecorded[unknownTime].hrs + matrix[unknownTime][i].hrsBetween) == timesRecorded[i].hrs) &&
					((layMins + timesRecorded[unknownTime].mins + matrix[unknownTime][i].minsBetween) < timesRecorded[i].mins) ||
					 (timesRecorded[i].hrs == -1 && timesRecorded[i].mins ==-1)){ 

					tmpH1 = timesRecorded[unknownTime].hrs;
					tmpH2 = matrix[unknownTime][i].hrsBetween;
					tmpM1 = timesRecorded[unknownTime].mins;
					tmpM2 = matrix[unknownTime][i].minsBetween;

					addLayHrs(tmpH1, tmpH2, layHrs, i); 													// Add total hrs and mins travel to times recorded
					addLayMins(tmpM1,tmpM2, layMins, i);
					
					layover[i].setHrs = matrix[unknownTime][i].arrHour;										// Record hrs & mins arrived for later
					layover[i].setMins = matrix[unknownTime][i].arrMins;
					
					pathCapture[i] = unknownTime;															// Capture the path

					lastTimeAdded = i;											 						    // Update lastTimeAdded to i
				}
			}
		}

		next = findMin(lastTimeAdded, timesRecorded[lastTimeAdded].hrs, timesRecorded[lastTimeAdded].mins); // Find next smallest time
		
		if (next != 0){															  							// If value of next isn't 0
			
			myDijQueue.push(next); 												 							 // Push next onto the queue	
		}

		if(next == 0){															  // If the value of next is 0, we have visited every node, so break
			
			break;
		}
	}
														 
} // end dijkstra_layover_helper function

void GRAPH::setStation(int station, string name){    				   							 // Setting station name

	names[station] = name;

} // end function setStation

void GRAPH::findNameByNumber(int number){														// Finds the name of station by number

	if(number >= 1 && number <= 199 && names[number] != " "){									// If valid number received

		cout << endl;
		cout << "The name of station id " << number << " is " << names[number] << endl;	// Print the name of the station
		cout << endl;
	}
	else{

		cout << endl;																 			// Else state that invalid number was entered
		cout <<"Invalid station number entered." << endl;
		cout << "Number must be between 1 - 99." << endl << endl;
	}

} // end function findNameByNumber

void GRAPH::findNumberByName(char station[]){											// Finds station number by station name

	int count = 0;																		// Counter variable to hold characters to be checked
		
	for(int i = 0; i<25; i++){															// Loop through array ignoring null, '-' or '_'

		if(station[i] != '\0' && station[i]!= '_'){										// If element holds character, increase count

			count++;
		}		
	}
		
	int loop = count + 1;																// Set loop to go count + 1 times								
	int match = 0;																		// Counter to hold matches found
	char upper;																			// Holds uppercase version of a character
	locale loc;																			// Required to use function toupper
		
	for (int i = 1; i < 200; i++){														// Search through station names array
		
		for(int j =0, k = 0; j < loop; j++, k++){										// Loop for as many characters as needed
						
			if((station[j] =='_')){														// If input in array sent is '_',skip to next element
				
				j++;
			}

			if(names[i][k] == '_'){														// If element in station name holds '_', skip to next element
				
				k++;
			}
																						
			if(station[j] == names[i][k] || station[j] == (upper = toupper(names[i][k], loc))){		// If a match is found, increase match
				
				match++;			
	
				if(match == count){															// If complete word match, break loop
					
					break;
				}
			}
		}

		if(match == count){																	// If complete word match, print station number

			cout <<endl;
			cout << names[i] <<"'s station id is " << i << "" << endl;
			cout << endl;
			break;
		}
		
		match = 0;																			// Reset match counter for next iteration
	}

	if(match < count){
		cout << endl;
		cout <<"A station of that name has not been found"<< endl;	
		cout << endl;																		// If match < count, state no match found
	}
		
} // end function findNumberByName

void GRAPH::initialize(char arr[]){															// initializes a char array to "_"'s

	for (int i = 0; i < 25;i++){
        	arr[i] = '_';
    }

} // end function initialize 

void GRAPH::printAll(){																		// Prints schedule for all trains

	int count = 0;																			// Counter to check if station has any arrivals/departures

	cout << endl;
	cout <<"SCHEDULE FOR ALL TRAINS" << endl;
	cout <<"=======================" << endl;

	for(int i = 1; i < 200; i++){															// Loop through all trains

		if(names[i] != " "){																// If the index has a name, print it
			cout << endl;
			cout <<"Schedule for " << names[i] << endl;
		}

		for(int j = 1; j < 200; j++){														// Check all adjacent stations

			if( matrix[i][j].edge != false){												// If edge found, print depart/arrive times

				cout <<"Departure to " << names[j] << " at ";			
				cout << matrix[i][j].dept << ", arriving at " << matrix[i][j].arr << endl;
				count++;
			}

			if( matrix[j][i].edge != false){												// If another station has current as en edge

				cout << "Arrival from " << names[j] << " at " << matrix[j][i].arr << endl;	// Print what time train arrives at current station
				count++;
			}
		}

		if(count == 0 && names[i] != " "){													// If no arrivals or departures are available, print this
			cout <<"No departures or arrivals available"<< endl;
		}

		count = 0;																			// Reset count for next iteration
	}

	cout << endl;

} // end function printAll

void GRAPH::printStation(int station){														// Prints schedule for a specific station

	if( station <= 199  && names[station]!= " ") {
		
		cout << endl;
		cout<< "Schedule for " << names[station] << endl;

			for(int i = 1; i < 200; i++){														// Search adjacent stations and print depart/arrive times

				if( matrix[station][i].edge != false){

					cout <<"Departure to " << names[i] << " at ";			
					cout << matrix[station][i].dept << ", arriving at " << matrix[station][i].arr << endl;
				}

				if( matrix[i][station].edge != false){											// Check if current is adjacent to another station
																								// If so, print arrival to current station
					cout << "Arrival from " << names[i] << " at " << matrix[i][station].arr << endl;
				}
			}

		cout << endl;
	}
	else{																							// Else state invalid station entered

		cout << endl;
		cout <<"Invalid id entered"<< endl;
		cout << endl;
	}

} // end function printStation

void GRAPH::nonstop(int src, int dst){																// Function to check if a service is available

	if( (src <= 199 && dst <=199)  && (names[src]!= " " && names[dst]!= " ")   ){					// If valid stations entered
		
		cout << endl;

		if(matrix[src][dst].edge == true){															// If path exists, print confirmation

			cout<<"Nonstop service available between "<< names[src] << " and "<< names[dst]<< endl;
			cout << endl;
		}

		if(matrix[src][dst].edge == false && names[dst] != " "){									// If not, print confirmation
						
			cout<<"Nonstop service is not available between " << names[src]<< " and " << names[dst]<< endl;
			cout << endl;
		}
		
	}
	else{																							// Else if invalid stations entered, alert user

		cout<< endl;
		cout<<"Invalid stations entered"<< endl;
		cout<< endl;
	}

} // end function serviceAvailable

void GRAPH::display(){																		// Starting display for program

    cout << "=========================================================\n";
    cout << "                READING RAILWAYS SCHEDULER               \n";
    cout << "=========================================================\n";
    cout << endl;

} // end function display

void GRAPH::select(){
	cout <<"Options - (Enter the number of your selected option)" << endl;					// User's options display
    cout <<"(1) - Print full schedule" << endl;
    cout <<"(2) - Print station schedule"<< endl;
    cout <<"(3) - Look up station name" << endl;
    cout <<"(4) - Look up station id" << endl;
    cout <<"(5) - Service available" << endl;
    cout <<"(6) - Nonstop service available" << endl;
    cout <<"(7) - Find route (Shortest riding time)" << endl;
    cout <<"(8) - Find route (Shortest overall travel time)" << endl;
    cout <<"(9) - Find route from given time (Shortest overall travel time)"<< endl;
    cout <<"(0) - Exit" << endl;
    cout << endl;
    cout <<"Enter option: ";

} // end function select