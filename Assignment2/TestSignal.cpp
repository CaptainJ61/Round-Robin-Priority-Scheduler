/* test signal function  */

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

using namespace std;

void wake_up(int s);
int intr = 0;

int main()
{
	cout << "Signal function test:\n";
	int pid = getpid();
	cout << "Process " << pid << " will go to sleep and wait for the ^C signal to wake up\n";
	signal(SIGINT, wake_up);
	while (!intr); //wait for the ^C to wake up
	cout << "Done signal function testing.\n";
	return 1;
}

void wake_up(int s)
{
	cout << "\nI will wake up now.\n";
	intr = 1;
}