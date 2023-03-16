The code was written in C++ and compiled using a UNIX/LINUX VM avalible at Bradley called Putty.
jobGenerator() generates a collection of "jobs" numbered from 1 - 100. These jobs are then separated into 3 separate priority queues. The queues are then put through the jobScheduler() which then assigns a worker process to each job.

Below is the list of the problems with the code given to me by my professor after submission:
  
  "1. mutex semaphore should put inside the while loop in jobGenerator() just before and after each file io, not outside the whole loop to finish all the job generations. 2. mutex semaphore should put inside the selectJob() just before and after each file io. 2. The run results didn't show the multiple processes switching between jobGenerator() and jobScheduler()."
