// This code belongs to Saksham Mittal
#include <bits/stdc++.h>

using namespace std;

class process {
	public:
		int m_id;		// Id of the process
		int m_period;	// Time period of arrival of processes
		int m_bursts;	// Processing time of process
		// deadline of the processes is assumed to be same as m_period
};

queue <process> readyQueue;

bool priority(const process &x, const process &y) {
	return (x.m_period < y.m_period);
}

void sortQueue() {
	vector<process> v;
	while(!readyQueue.empty()) {   
		v.push_back(*readyQueue.top());
	    readyQueue.dequeue();
	}

	sort(v.begin(), v.end(), priority);

	for(int i=0; i<v.size(); i++) {   
		readyQueue.enqueue(v[i]);
	}

}

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
		P[i].m_id = i + 1;
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
	// sort the ready queue
	sortQueue();

	process current = readyQueue.pop();
	cout << "1 ";
	while(time <= t) {
		time++;
		// check if a new process has to be added in the ready queue
		for(int i=0; i<noOfProcesses; i++) {
			if(time % P[i].m_period == 0) {
				readyQueue.push(P[i]);
			}
		}
		// sort the ready queue
		sortQueue();

		// check if burst time is over, else continue excecuting
		if(time == current.m_bursts) {
			// Its processing time is over
			cout << "P" << current.m_id << " " << time << endl;
		} else {
			// means current is excecuting
			current.m_bursts--;
		}

		if(current.m_bursts == 0 && readyQueue.size() != 0) {
			// If the current process has finished excecuting
			// and ready queue is not empty
			current = readyQueue.pop();
			cout << time << " ";
		} else if(readyQueue.front().m_period < current.m_period) {
			// Preempt the process if another higher priority process is present in the ready queue
			readyQueue.push(current);
			// sort the ready queue
			sortQueue();
			current = readyQueue.pop();
			cout << time << " ";
		}

	}
	return 0;
}