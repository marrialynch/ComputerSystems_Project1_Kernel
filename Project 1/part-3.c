/*
 * file:        part-3.c
 * description: part 3, CS5600 Project 1, Spring 2021
 */

/* NO OTHER INCLUDE FILES */
#include "elf64.h"
#include "sysdefs.h"

extern void *vector[];
extern void switch_to(void **location_for_old_sp, void *new_value);
extern void *setup_stack0(void *_stack, void *func);
void* location_sp1[1];
void* location_sp2[1];
void* location_main[1];

/* ---------- */

/* write these 
*/
int read(int fd, void *ptr, int len);
int write(int fd, void *ptr, int len);
void exit(int err);
int open(char *path, int flags);
int close(int fd);
int lseek(int fd, int offset, int flag);
void *mmap(void *addr, int len, int prot, int flags, int fd, int offset);
int munmap(void *addr, int len);

/* ---------- */

/* copy from Part 2 */
void do_print(char *buf);

/* ---------- */

/* write these new functions */
void do_yield12(void);
void do_yield21(void);
void do_uexit(void);

/* ---------- */
// part2 
// void* load(){}

void main(void)
{
	vector[1] = do_print;

	vector[3] = do_yield12;
	vector[4] = do_yield21;
	vector[5] = do_uexit;
	void* location_main[0] = NULL;
	
	/* your code here */
//	1. get the entry point of 2 processes
//		part2 load and return entry point for process1 and process2

//  2. create 2 stack frames for 2 processes

//  3. setup_stack0() for process1 and 2
//		location_sp1[0] = setup_stack0(stack1+4096, location_sp1[0])
//		location_sp2[0] = setup_stack0(stack2+4096, location_sp2[0])

//	4. switch to process 1 from OS stack
//		switch_to(location_main[0],location_sp1[0])

//	5. yield12 and yield21
//		swich_to(location_sp1[0],location_sp2[0])
//		swich_to(location_sp2[0],location_sp1[0])

//	6. uexit
//		swich_to(NULL, location_main[0]);


	


	do_print("done\n");
	exit(0);
}
