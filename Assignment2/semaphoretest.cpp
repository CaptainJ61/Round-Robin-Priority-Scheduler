/* create a semaphore using file create operation */
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

void down(int *semid, char* semname);
void up(int semid, char* semname);

int main()
{
	int semid;
	char semname[10];

	int pid;

	strcpy(semname, "mutex");
	pid = fork();
	if (pid == 0) {
		cout << "This is child ->\n";
		down(&semid, semname);
		cout << "Child: I will hold the critical region for 5 seconds.\n";
		sleep(5);
		cout << "Child: I am wake up now to release the critical region.\n";
		up(semid, semname);
		exit(0);
	}
	else if (pid > 0) {
		cout << "This is parent ->\n";
		down(&semid, semname);
		cout << "Parent: I will hold the critical region for 5 seconds.\n";
		sleep(5);
		cout << "Parent: I am wake up now to release the critical region.\n";
		up(semid, semname);
	}
	return 1;
}

void down(int *semid, char* semname)
{
	while (*semid = creat(semname, 0) == -1) /* && error == EACCES)*/
	{
		cout << "down " << semname << ": I am blocked.\n";
		sleep(1);
	}
}

void up(int semid, char* semname)
{
	close(semid);
	unlink(semname);
	cout << "up " << semname << ": I am waked up.\n";
}