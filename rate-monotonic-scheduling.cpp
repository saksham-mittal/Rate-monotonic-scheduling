// This code belongs to Saksham Mittal
#include <bits/stdc++.h>

using namespace std;

class process {
	public:
		int m_id;				// Id of the process
		int m_period;			// Time period of arrival of processes
		int m_burstLeft;		// Burst time left	
		int m_processing;		// Processing time of process
		int m_startTime;		// Start time of the process
		bool m_isFinished;		// Whether the process is finished or not
		// deadline of the processes is assumed to be same as m_period
};

queue <process> readyQueue;

bool priority(const process &x, const process &y) {
	return (x.m_period < y.m_period);
}

void sortQueue() {
	vector<process> v;
	while(!readyQueue.empty()) {   
		v.push_back(readyQueue.front());
	    readyQueue.pop();
	}

	sort(v.begin(), v.end(), priority);

	for(int i=0; i<v.size(); i++) {   
		readyQueue.push(v[i]);
	}
}

void printQueue() {
	queue <process> dummy;
	dummy = readyQueue;
	while(!dummy.empty()) {
		cout << "P" << dummy.front().m_id << endl;
		dummy.pop();
	}
}

int main(int argc, char const *argv[]) {
	// Your code here
	int noOfProcesses, t, time = 0, noDeadlinesMissed = 0;

	cout << "-------------------------------------\n      Rate Monotonic Scheduling      \n-------------------------------------\n" << endl;

	cout << "Enter the number of processes you want to schedule: ";
	cin >> noOfProcesses;

	cout << "Enter the time till you want to scheduling to happen: ";
	cin >> t;
	cout << endl;

	// declaring an array of class process
	process P[noOfProcesses];
	
	cout << "Enter the details of the processes: " << endl;
	for(int i=0; i<noOfProcesses; i++) {
		cout << "Process " << i + 1 << ": " << endl;
		P[i].m_id = i + 1;
		cout << "Period: ";
		cin >> P[i].m_period;
		cout << "Processing time: ";
		cin >> P[i].m_processing;
		P[i].m_burstLeft = P[i].m_processing;
		P[i].m_isFinished = false;

	}

	// We are assuming that all processes arrive at t = 0
	// and hence are added to the ready queue at the beggining itself
	for(int i=0; i<noOfProcesses; i++) {
		readyQueue.push(P[i]);
	}
	// sort the ready queue
	sortQueue();

	// printQueue();

	process current = readyQueue.front();
	readyQueue.pop();
	current.m_startTime = 1;

	// printQueue();

	cout << endl;
	while(time <= t) {
		time++;
		bool flag = false;

		// check if a new process has to be added in the ready queue
		for(int i=0; i<noOfProcesses; i++) {
			if(time % P[i].m_period == 0) {
				readyQueue.push(P[i]);
				flag = true;
			}
		}
		// sort the ready queue
		if(!readyQueue.empty() && flag) {
			sortQueue();
		}

		// check if burst time is over, else continue excecuting
		if(current.m_burstLeft == 1) {
			cout << current.m_startTime << " P" << current.m_id << " " << time << endl;
			current.m_burstLeft--;
			current.m_isFinished = true;
		} else {
			// means current is excecuting if it is not finished
			if(!current.m_isFinished) {
				current.m_burstLeft--;
			}
		}

		if(!readyQueue.empty()) {
			if(current.m_burstLeft == 0) {
				// If the current process has finished excecuting
				// and ready queue is not empty
				current = readyQueue.front();
				readyQueue.pop();
				current.m_startTime = time;
			} else if(readyQueue.front().m_period <= current.m_period) {
				// Preempt the process if another higher priority process is present in the ready queue
				// If while preempting the new process has same id, then deadline missed
				if(readyQueue.front().m_id != current.m_id) {
					// if id is different, then only we push the current process
					readyQueue.push(current);
					// sort the ready queue
					sortQueue();
				} else {
					noDeadlinesMissed++;
				}
				cout << current.m_startTime << " P" << current.m_id << " " << time << endl;
				current = readyQueue.front();
				readyQueue.pop();
				current.m_startTime = time;
			}
		}

	}
	cout << endl;
	cout << "Number of deadlines missed: " << noDeadlinesMissed << endl;
	return 0;
}