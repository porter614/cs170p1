Victor Porter
Nader Khalil

The lottery scheduler is not functional by any means, but the idea/concept of lottery scheduling is implemented/present. We found it very difficult and cumbersome to test our code. The Minix System seems very fragile and prone to breaking. This aside, we were able to create two new queues for user processes. We have a function do_lottery in schedule.c that selects a random number and give the winning process access to the higher priority queue. We found a way to pass nice values from the process manager utility.c into the scheduler. Once we decide if a given process is a system process vs. a user process, we can convert the nice value passed to it to a priority, or an amount of tickets. 
