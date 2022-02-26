/*
 * file:        part-3.c
 * description: part 3, CS5600 Project 1, Spring 2021
 */

/* NO OTHER INCLUDE FILES */
#include "elf64.h"
#include "sysdefs.h"
#define BUFFER_SIZE 201

extern void *vector[];
extern void switch_to(void **location_for_old_sp, void *new_value);
extern void *setup_stack0(void *_stack, void *func);
char stack1[4096];
char stack2[4096];
void *location_sp1[1];
void *location_sp2[1];
void *location_main[1];

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
int read(int fd, void *ptr, int len)
{
	if (len < 1)
	{
		return -1;
	}

	syscall(__NR_read, fd, ptr, len);
	return 1;
}

int write(int fd, void *ptr, int len)
{
	if (len < 1)
	{
		return -1;
	}
	syscall(__NR_write, fd, ptr, len);
	return 1;
}

void exit(int err)
{
	syscall(__NR_exit, err);
}

int open(char *path, int flags)
{
	return syscall(__NR_open, path, flags);
}

int close(int fd)
{
	return syscall(__NR_close, fd);
}

int lseek(int fd, int offset, int flag)
{
	return syscall(__NR_lseek, fd, offset, flag);
}

void *mmap(void *addr, int len, int prot, int flags, int fd, int offset)
{
	return (void *)syscall(__NR_mmap, addr, len, prot, flags, fd, offset);
}
int munmap(void *addr, int len)
{
	return syscall(__NR_munmap, addr, len);
}

/* copy from Part 2 */
void do_print(char *buf);

/* ---------- */
void do_print(char *buf)
{
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		write(1, &buf[i], 1);
		if (buf[i] == '\0')
		{
			break;
		}
	}
}

/* write these new functions */
void do_yield12(void);
void do_yield21(void);
void do_uexit(void);

/* ---------- */
void do_yield12(void)
{
	switch_to(location_sp1, location_sp2[0]);
}

void do_yield21(void)
{
	switch_to(location_sp2, location_sp1[0]);
}

void do_uexit(void)
{
	switch_to(NULL, location_main[0]);
}

// part2
// void* load(){}
void *load_prog(char *filename, int m)
{
	int fd;
	/* YOUR CODE HERE */
	if ((fd = open(filename, O_RDONLY)) < 0)
		exit(1); /* failure code */
	// fd now points to where the executable file is

	/* read the main header (offset 0) */
	struct elf64_ehdr hdr;
	read(fd, &hdr, sizeof(hdr));
	// store the executable's info in hdr -> elf header
	// do_print("entry point address: " + str(hdr.e_entry));

	/* read program headers (offset 'hdr.e_phoff') */
	int i, n = hdr.e_phnum;
	struct elf64_phdr phdrs[n];
	lseek(fd, hdr.e_phoff, SEEK_SET);
	// there are a lof of info in ELF hearder, we need to locate
	// program header table. Lseek is to move the fd to program header table in ELF header
	read(fd, phdrs, sizeof(phdrs));
	// read the program header table from the executable and stores it
	// in memory at address phdrs (an array)

	// ----------------------determine the value of M_offset------------------------------
	// multiple of 4096 = 64 * 4096
	int M_offset = m * 4096;
	char *addr_buf[n]; // array of pointer to store mmap address
	/* look at each section in program headers */
	for (i = 0; i < hdr.e_phnum; i++)
	{
		if (phdrs[i].p_type == PT_LOAD)
		{
			// not all segments should be loaded in memory, only those with p_type PT_LOAD
			int len = ROUND_UP(phdrs[i].p_memsz, 4096);
			long addrp = ROUND_DOWN((long)phdrs[i].p_vaddr, 4096); // starting address of the page
			addr_buf[i] = (char *)mmap((long *)(addrp + M_offset), len, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
			if (addr_buf == MAP_FAILED)
			{
				do_print("mmap failed\n");
				exit(0);
			}
			lseek(fd, (int)phdrs[i].p_offset, SEEK_SET); // on disk, fd pointer to disk
			read(fd, phdrs[i].p_vaddr + M_offset, (int)phdrs[i].p_filesz);
		}
	}
	void (*f)();
	f = hdr.e_entry + M_offset;
	close(fd);
	return f;
}

void main(void)
{
	vector[1] = do_print;

	vector[3] = do_yield12;
	vector[4] = do_yield21;
	vector[5] = do_uexit;
	location_main[0] = NULL;

	/* your code here */
	//	1. get the entry point of 2 processes
	//		part2 load and return entry point for process1 and process2
	location_sp1[0] = load_prog("process1", 32);
	location_sp2[0] = load_prog("process2", 256);

	//  2. create 2 stack frames for 2 processes

	//  3. setup_stack0() for process1 and 2
	location_sp1[0] = setup_stack0(stack1 + 4096, location_sp1[0]);
	location_sp2[0] = setup_stack0(stack2 + 4096, location_sp2[0]);

	//	4. switch to process 1 from OS stack
	switch_to(location_main, location_sp1[0]);

	//	5. yield12 and yield21
	//		swich_to(location_sp1[0],location_sp2[0])
	//		swich_to(location_sp2[0],location_sp1[0])

	//	6. uexit
	//		swich_to(NULL, location_main[0]);

	do_print("done\n");
	exit(0);
}
