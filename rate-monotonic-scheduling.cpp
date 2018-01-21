// This code belongs to Saksham Mittal
#include <bits/stdc++.h>

using namespace std;

class process {
	public:
		int m_period;	// Time period of arrival of processes
		int m_bursts;	// Processing time of process
		// deadline of the processes is assumed to be same as m_period
};

queue <process> readyQueue;

int main(int argc, char const *argv[]) {
	// Your code here
	int noOfProcesses, t, time = 0;

	cout << "-------------------------------------\n      Rate Monotonic Scheduling      \n-------------------------------------\n" << endl;

	cout << "Enter the number of processes you want to schedule: ";
	cin >> noOfProcesses;

	cout << "Enter the time till you want to scheduling to happen: ";
	cin >> t;

	// declaring an array of class process
	process P[noOfProcesses];
	
	cout << "Enter the details of the processes: " << endl;
	for(int i=0; i<noOfProcesses; i++) {
		cout << "Process " << i + 1 << ": " << endl;
		cout << "Period: ";
		cin >> P[i].m_period;
		cout << "Processing time: ";
		cin >> P[i].m_bursts;
	}

	// We are assuming that all processes arrive at t = 0
	// and hence are added to the ready queue at the beggining itself
	for(int i=0; i<noOfProcesses; i++) {
		readyQueue.push(P[i]);
	}

	while(time != t) {
		time++;
		// check if a new process has to be added in the ready queue
		for(int i=0; i<noOfProcesses; i++) {
			if(time % P[i].m_period == 0) {
				readyQueue.push(P[i]);
			}
		}
		// currently the readyQueue.pop() process will excecute

	}
	return 0;
}