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
The main idea was to use 3 semaphores, 1 to control the stair and global counters related to the stair, 1 to control the line waiting for upward direction, and 1 for the downward direction. 

Since there are 13 steps in the staircase, and in order to increase the efficiency of using stairs, we allow one direction to go in batch with size 10, and 6 people could be on the stairs at the same time. Of course, theses numbers could be changed as they were not hard-coded.

### - main()
Since threads here are customers waiting to use the stairs, we generated *tNum* threads and called either method *a()* (go up) or *b()* (go downn) depending on the random direction assigned to each thread to immitate the real situation where customers conme to the stairs randomly. 

### - a() and b()
### - semWait()
### - semSignal()

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


