// This code belongs to Saksham Mittal
#include <bits/stdc++.h>

using namespace std;

class process {
	public:
		int m_id;				// Id of the process
		int m_period;			// Time period of arrival of processes
		int m_burstLeft;		// Burst time left
		int m_processing;		// Processing time of process
		int m_arrivalTime;		// Arrival time of process
		int m_waitingTime;		// The waiting time it spends in ready queue
		int m_noTimes;			// Number of times a process has to execute
		bool m_isFinished;		// Whether the process is finished or not
		bool m_isResume;		// Whether the process is resuming or not
		// deadline of the processes is assumed to be same as m_period
};

queue <process> readyQueue, dummyQueue;

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
	int noOfProcesses, id, totalNoOfTimes = 0, time = 0, noDeadlinesMissed = 0, idleTime = 0, noSuccessful = 0, totalAvgTime = 0;
	float totalTurnaroundTime = 0;

	ifstream infile("inp-params.txt");

	infile >> noOfProcesses;

	// declaring an array of class process
	process P[noOfProcesses];
	
	for(int i=0; i<noOfProcesses; i++) {
		infile >> id;
		P[i].m_id = id;
		infile >> P[i].m_processing;
		infile >> P[i].m_period;
		P[i].m_burstLeft = P[i].m_processing;
		infile >> P[i].m_noTimes;
		P[i].m_isFinished = false;
		P[i].m_isResume = false;
		P[i].m_arrivalTime = 0;
		P[i].m_waitingTime = 0;
	}

	for(int i=0; i<noOfProcesses; i++) {
		totalNoOfTimes += P[i].m_noTimes;
	}

	// We are assuming that all processes arrive at t = 0
	// and hence are added to the ready queue at the beggining itself
	for(int i=0; i<noOfProcesses; i++) {
		readyQueue.push(P[i]);
	}
	// sort the ready queue
	sortQueue();

	process current = readyQueue.front();
	dummyQueue = readyQueue;
	readyQueue.pop();

	ofstream EDFLog, EDFStat, AvgTimes;
	EDFLog.open("EDF-Log.txt");		// opening the file in the append mode
	EDFStat.open("EDF-Stats.txt");	// opening the file in the append mode

	while(!dummyQueue.empty()) {
		EDFLog << "Process P" << dummyQueue.front().m_id << ": processing time=" << dummyQueue.front().m_processing << "; deadline:"<< dummyQueue.front().m_period << "; period:" << dummyQueue.front().m_period << " joined the system at time 0." << endl;
		dummyQueue.pop();
	}

	EDFLog << "Process P" << current.m_id << " starts execution at time 0." << endl; 
	while(totalNoOfTimes) {
		time++;
		bool flag = false;

		// check if a new process has to be added in the ready queue
		for(int i=0; i<noOfProcesses; i++) {
			if(time % P[i].m_period == 0) {
				P[i].m_arrivalTime = time;
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
			EDFLog << "Process P" << current.m_id << " finishes execution at time " << time << "." << endl;
			current.m_burstLeft--;
			current.m_isFinished = true;
			noSuccessful++;
			totalNoOfTimes--;
		} else {
			// means current is excecuting if it is not finished
			if(!current.m_isFinished) {
				current.m_burstLeft--;
			} else {
				idleTime++;
			}
		}

		if(!readyQueue.empty()) {
			if(current.m_burstLeft == 0) {
				// If the current process has finished excecuting
				// and ready queue is not empty
				current = readyQueue.front();
				readyQueue.pop();

				P[current.m_id - 1].m_waitingTime +=  (time - current.m_arrivalTime);

				if(idleTime > 0) {
					EDFLog << "CPU is idle till time " << time - 1 << "." << endl;
					idleTime = 0;
					if(current.m_isResume) {
						EDFLog << "Process P" << current.m_id << " resumes execution at time " << time << "." << endl; 
					} else {
						EDFLog << "Process P" << current.m_id << " starts execution at time " << time << "." << endl; 
					}
				} else {
					if(current.m_isResume) {
						EDFLog << "Process P" << current.m_id << " resumes execution at time " << time + 1 << "." << endl; 
					} else {
						EDFLog << "Process P" << current.m_id << " starts execution at time " << time + 1 << "." << endl; 
					}
				}
			} else if(time + readyQueue.front().m_period < current.m_arrivalTime + current.m_period) {
				// Preempt the process if deadline of current process is later than deadline of arriving process
				// If while preempting the new process has same id, then deadline missed
				if(readyQueue.front().m_id != current.m_id) {
					EDFLog << "Process P" << current.m_id << " is preempted by Process P" << readyQueue.front().m_id << " at time " << time << ". Remaining processing time:" << current.m_burstLeft << "." << endl;
					EDFLog << "Process P" << readyQueue.front().m_id << " starts execution at time " << time + 1 << "." << endl; 
					current.m_arrivalTime = time;
					current.m_isResume = true;
					// if id is different, then only we push the current process
					readyQueue.push(current);
					// sort the ready queue
					sortQueue();
				} else {
					noDeadlinesMissed++;
					EDFLog << "Process P" << current.m_id << " misses its deadline at time " << time << "." << endl;
					EDFLog << "Process P" << readyQueue.front().m_id << " starts execution at time " << time << "." << endl; 
				}
				current = readyQueue.front();
				readyQueue.pop();
				P[current.m_id - 1].m_waitingTime +=  (time - current.m_arrivalTime);
			}
		}

	}
	EDFLog.close();

	EDFStat << "Number of processes that missed their deadlines: " << noDeadlinesMissed << endl;
	EDFStat << "Number of processes that successfully complete: " << noSuccessful << endl << endl;
	for(int i=0; i<noOfProcesses; i++) {
		EDFStat << "Average waiting time for Process P" << i + 1 << ": " << P[i].m_waitingTime/P[i].m_noTimes << endl;
	}
	EDFStat.close();

	AvgTimes.open("Average_times.txt", ios::app);
	AvgTimes << endl << "EDF algorithm(Input according to 'inp-params.txt'): " << endl;
	for(int i=0; i<noOfProcesses; i++) {
		totalAvgTime += P[i].m_waitingTime/P[i].m_noTimes;
		totalTurnaroundTime += (P[i].m_waitingTime/P[i].m_noTimes + P[i].m_processing);
	}
	totalAvgTime /= noOfProcesses;
	totalTurnaroundTime /= noOfProcesses;
	AvgTimes << "Average Time: " << totalAvgTime << endl;
	AvgTimes << "Average Turnaround Time: " << totalTurnaroundTime << endl;
	AvgTimes.close();

	return 0;
}