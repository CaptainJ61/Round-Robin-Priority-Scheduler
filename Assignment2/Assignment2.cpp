/* Simple process scheduler simulator program */
/* include c header files */
#include <stdlib.h>
#include <unistd.h> // for function fork()
#include <stdio.h>
#include <time.h> //for generate random seed
// include c++ header files
#include <string>
#include <iostream>
#include <queue>
#include <fstream>

#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <sstream>

#define N 10 //N is the number of the worker processes. You may increase N to 100 when your program runs correctly
#define M 10 //M is the number of jobs. You may increase M to 50 when your program runs correctly



using namespace std;

void setJobQueues();
void jobGenerator();
void jobScheduler();
int selectJob();
void executeJob(int n);

void testSignal();
void wake_up(int s);
int intr = 0;

void queueOrg();

queue<string> priority1;
queue<string> priority2;
queue<string> priority3;

//for the semaphore
void down(int *semid, char* semname);
void up(int semid, char* semname);

int main() {



	int pid = 0;
	setJobQueues(); /* Set up the priority job queues with chosen file and/or data structure */
	if (pid = fork() > 0) { /* jobGenerator process */

		jobGenerator(); /* generate random jobs and put them into the priority queues. The priority queues must be protected in a critical region */
		
	}
	else {/* job scheduler process */
		sleep(5);
		jobScheduler(); /* schedule and execute the jobs. */
		exit(0);
	}

	return (1);
}

void setJobQueues()
{
	cout << "Main: Set up the job priority queue: \n";
	
	/*THIS IS A WARNING. THESE COMMANDS WILL DELETE 
	ANY FILE THAT HAPPENS TO HAVE THE SAME NAME IN 
	THE CURRENT DIRECTORY. The purpose of this is to
	reset the files so they only contain the jobs 
	generated the last time this program was run.*/
	system("rm priority1.txt");
	system("rm priority2.txt");
	system("rm priority3.txt");
	system("touch priority1.txt");
	system("touch priority2.txt");
	system("touch priority3.txt");
}

void jobGenerator()
{
	//for semaphores
	int semid;
	char semname[10];
	strcpy(semname, "mutex");
	down(&semid, semname);

	cout << "jobGenerator: Entering critical region\n";

	ofstream priorityA;
	ofstream priorityB;
	ofstream priorityC;

	priorityA.open("priority1.txt");
	priorityB.open("priority2.txt");
	priorityC.open("priority3.txt");

	int i = 0, n = 0;
	cout << "jobGenerator: Use a loop to generate M jobs in the priority queue: \n";
	// initialize random seed
	srand(time(0));



	while (i < M) {
		// generate a random number between 1-100
		n = rand() % 100 + 1;
		cout << "jobGenerator: Job number is : " << n << endl;
		// Put the job n into the priority queue
		/* ... */
		if (n >= 1 && n <= 30)
		{

			priorityA << n << endl;
		}
		else if (n >= 31 && n <= 60) 
		{

			priorityB << n << endl;
		}
		else if (n >= 61 && n <= 100) 
		{

			priorityC << n << endl;
		}

		i++;
		usleep(100);
	}

	priorityA.close();
	priorityB.close();
	priorityC.close();

	up(semid, semname);

	cout << "jobGenerator: Leaving critical region\n";
}

void jobScheduler()
{

	queueOrg();
	int i = 0, n = 0, pid = 0;
	while (i < N) { /* schedule and run maximum N jobs */
		n = selectJob(); /* pick a job from the job priority queues */
		if (n > 0) { /* valid job id */
			if (pid = fork() == 0) { /* child worker process */
				//cout << "did it get here?";
				executeJob(n); /* execute the job */
				exit(0);
			}
		}
		i++;
		sleep(1);
	}
}

void queueOrg() {

	//for semaphores
	int semid;
	char semname[10];
	strcpy(semname, "mutex");
	down(&semid, semname);

	cout << "\nqueueOrg: Entering critical region";

	string n;

	ifstream priorityA;
	ifstream priorityB;
	ifstream priorityC;

	priorityA.open("priority1.txt");
	priorityB.open("priority2.txt");
	priorityC.open("priority3.txt");
	sleep(1);
	cout << "\nqueueOrg: storing priority 1 jobs into queue\n";
	while (getline(priorityA, n))
	{
		priority1.push(n);
	}
	sleep(1);
	cout << "queueOrg: storing priority 2 jobs into queue\n";
	while (getline(priorityB, n))
	{
		priority2.push(n);
	}
	sleep(1);
	cout << "queueOrg: storing priority 3 jobs into queue\n";
	while (getline(priorityC, n))
	{
		priority3.push(n);
	}

	priorityA.close();
	priorityB.close();
	priorityC.close();

	up(semid, semname);

	cout << "queueOrg: Leaving critical region\n";
	sleep(1);
}

int selectJob()
{
	string n = "";
	int x = 0;
	/* ... */
	if (!priority1.empty())
	{
		n = priority1.front();
		priority1.pop();
	} else if (!priority2.empty())
	{
		n = priority2.front();
		priority2.pop();
	} else if (!priority3.empty())
	{
		n = priority3.front();
		priority3.pop();
	}

	cout << "selectJob: Select a highest priority job from the priority queue:" << n << endl;
	stringstream func(n);
	func >> x;
	return x;
}

void executeJob(int n)
{
	if (n >= 1 && n <= 30) {
		cout << "executeJob: execute server job " << n << endl;
		/* ... */
		cout << "Process id: " << getpid() << endl;
	}
	else if (n >= 31 && n <= 60) {
		cout << "executeJob: execute power user job " << n << endl;
		/* 
		system("g++ TestSignal.cpp");
		system("./a.out"); 
		*/
		testSignal();
		
	}
	else if (n >= 61 && n <= 100) {
		cout << "executeJob: execute user job " << n << endl;
		/* ... */
		cout << "Going to sleep..." << endl;
		sleep(2);
		cout << "Woke up" << endl;

	}

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//for test signal job

void testSignal() {
	cout << "Signal function test:\n";
	int pid = getpid();
	cout << "Process " << pid << " will go to sleep and wait for the ^C signal to wake up\n";
	signal(SIGINT, wake_up);
	while (!intr); //wait for the ^C to wake up
	cout << "Done signal function testing.\n";
}

void wake_up(int s)
{
	cout << "\nI will wake up now.\n";
	intr = 1;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//semaphores for critical regions

void down(int *semid, char* semname)
{
	while (*semid = creat(semname, 0) == -1) /* && error == EACCES)*/
	{
		sleep(1);
	}
}

void up(int semid, char* semname)
{
	close(semid);
	unlink(semname);
}