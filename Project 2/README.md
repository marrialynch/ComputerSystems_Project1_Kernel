# Projuect2
### Group 3: Michael Lembck, Dongze Li, Meilin Liu
-----
## Project Description  
In this group project, we solved a shared stair problem in a department store imitating multi-thread conditions using semaphores. Special attention was paid to increase program efficiency and deadlocks.

The question we were asked to solve was:

>A department store consists of two floors that are connected by narrow stairs that cannot fit more than one person on any given stair. The stairs can become deadlocked if both a first-floor customer is trying to go up to the second floor, while a second-floor customer is trying to go down to the first floor. Customers in such situation refuse to backup leading to deadlock. 
The store manager is hiring you to design a C program using POSIX synchronization, that 
>1.	prevents deadlock
>2.	prevents starvation (the situation in which first-floor customers prevent second-floor customers from using the stairs, or vice versa)
>3.	and allows more than one customer to use the stairs in the same direction in an “efficient” way that you determine.
>
>•	Represent first-floor customers and second-floor customers as separate threads. Once a customer is on the stairs, the associated thread will sleep for a random period of time, representing the time to go over all the 13 stairs between the two floors.
>•	Design your program so that you can create several threads representing first-floor customers and second-floor customers, and test it using different possible scenarios.
>•	Find the Turnaroud time and Response time for each customer


We had a basic version with 1 mutex, and an improved version with pthread_condition, and tried to implement a buffered queue to batch process customers (threads), but all couldn't achieve efficency and avoiding deadlock at the same time. The final version of our project used the starter code given by the professor and it finally worked.

## Implementation 
----
The main idea was to use 3 semaphores, 1 to control the stair and global counters related to the stair, and 2 direction semaphores: 1 to control the line waiting for upward direction, and 1 for the downward direction. 

Since there are 13 steps in the staircase, and in order to increase the efficiency of using stairs, we allow one direction to go in batch with size 10, and 6 people could be on the stairs at the same time. Of course, theses numbers could be changed as they were not hard-coded.

### - main()
Since threads here are customers waiting to use the stairs, we generated *tNum* threads and called either method *a()* (go up) or *b()* (go downn) depending on the random direction assigned to each thread to immitate the real situation where customers conme to the stairs randomly. 

### - a() and b()
The 2 methods are doing exactly same thing, but in opposite direction. Here we explain *b()* in detail. 

First, we wait for the stair mutex and check if people on the stairs reach max, we constanlty release mutex to let customer updat
e global counter xedCounter. Also, if the xing direction is the same as the current thread, we need to waken the other direction to unblock the update of global counters. This step is to solve the issue that stairs are not locked, but direction semephores block the customer from entering.

When a customer acquires the lock, it checks if the direction is either the same from the previous xing /xed customer, and the batch size and max number of people on the stairs are not maxed out. If so, the customer could go, update xing counter, and release the stair lock. At this point, the customer is on the stairs and others in the same direction could go.

But, if the previous conditions are not satisfied, this customer needs to wait. It updates directional wait counter and queue for directional semaphore. Once it's its turn to go that direction, it updates corresponding counter, and release directional lock and stair lock sequentially.

Then, it waits for stair lock again to update crossed counter. Now, the customer finishes crossing the stairs. The next step is to decide who should go next after this customer.

If there are people waiting in the same direction, the stairs could allow more people, and batch size is not reached, and there are no people waiting in the other direction, then we could awaken the next customer in the same direction.

When do we awaken the other direction? Only when we cannot go in the same direction. That means either batch size reaches max in the same direction, or nobody in the same direction is waiting. Of course, it only makes sense to allow other direction to go through if there are peple waiting there.

The other situation is that there are no people waiting or crossing, then we reset everything.

Now that the customer has crossed the stair, updated related counter, and awaken the right direction, its job is done.

### - semWait()
To queue in the semaphore's buffer, decrease the counter in the buffer, and check if there are enough space for new thread to enqueue.


### - semSignal()
To awaken a thread in semaphore's buffer, increase the available space counter, and check if space number is valid.

## Test

## Free of deadlock and starvation

## Turnaround Time and Response Time

Suppose the crossing time for 1 customer from A to B or B to A is always 1 second. 

When we permit at most 6 customers on the stairs and at most 10 customers of the same direction on a batch, 

 >The average Turnaroud time for 20 customers is 2.251068 seconds 

 >The average Response time for 20 customers is 1.250492 seconds 

 >The average Turnaroud time for 200 customers is 14.829678 seconds 

 >The average Response time for 200 customers is 13.829169 seconds 

 >The average Turnaroud time for 2000 customers is 143.287271 seconds 

 >The average Response time for 2000 customers is 142.286729 seconds 


It's not “efficient” when we have 2000 customers, we can adjust the value of `maxStairs` and `maxBatch`.

When we permit at most 10 customers on the stairs and at most 100 customers of the same direction on a batch, 

 >The average Turnaroud time for 2000 customers is 91.460500 seconds 

 >The average Response time for 2000 customers is 90.459962 seconds 


## How to compile, run and test the code

You can compile and run the code by running `make` in the terminal. There will be two output files `output1.txt` and `output2.txt` generated in the folder after the program ends.

To test the code, you can change the value of the follwing variables defined at the start of the `stairs.c` file:

-`tNum`: the maximum number of customers/threads in the system to test
-`maxStairs`: how many customers can be on the stairs at the same time
-`maxBatch`: how many customers of the same direction can be on a batch


## Contributions
---

After a brainstorming meeting, each of us came up with our own solution. Then we met and worked on the starter code together and went to TA Ying Tuo for suggestions. Dongze then compelted the 1st draft of the final version. Meilin and Michael debugged and tested the code. The whole team came up with the final working version together.

As for this markdown file, the work distribution is as the following:

>1. Your group number, and the name of the students in your group -**Meilin**
>2. A short description of the project. -**Meilin**
>3. Explain how you implemented the project:
>- List the functions you wrote, and the purpose of each function - **Meilin**
>- Explain how you tested your project and list the test cases you used -**Michael**
>- Explain how you are guaranteeing that your code is free of deadlock and starvation. - **Michael**
>- Find the average Turnaroud time and Response time of the examples you run, and explain using these performance measures how you adjusted your project to make your design “efficient”. - **Dongze**
>- Explain how we can compile, run and test your code. -**Dongze**
>- List the contributions of each student in your group -**Meilin**


