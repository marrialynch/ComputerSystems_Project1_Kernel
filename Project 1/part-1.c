/*
 * file:        part-1.c
 * description: Part 1, CS5600 Project 1, Spring 2022
 */

/* THE ONLY INCLUDE FILE */
#include "sysdefs.h"
#define BUFFER_SIZE 201

/* write these functions */

int read(int fd, void *ptr, int len);
int write(int fd, void *ptr, int len);
void exit(int err);

/* ---------- */

/* Factor, factor! Don't put all your code in main()!
   Write new functions using the functions you just wrote
*/


// len: number of bytes to read
// return: 1 for success
int read(int fd, void *ptr, int len)
{
   if (len < 1)
   {
      return -1;
   }

   syscall(__NR_read, fd, ptr, len);
   return 1;
}

int strcmp(const char* str1, const char* str2) {
   while(*str1 && (*str1 == *str2)) {
      str1++;
      str2++;
   }
   return *(const unsigned char*) str1 - *(const unsigned char*) str2;
}

/* read one line from stdin (file descriptor 0) into a buffer: */
void readline(char* buffer)
{
   
   for (int i = 0; i < BUFFER_SIZE; i++)
   {
      read(0, &buffer[i], 1);
      if (buffer[i] == '\n')
      {
         buffer[++i] = '\0';
         break;
      }
   }
}

int write(int fd, void *ptr, int len) {
      if (len < 1)
   {
      return -1;
   }
   syscall(__NR_write, fd, ptr, len);
   return 1;
}

/* print a string to stdout (file descriptor 1) */
void print(char* buffer) {
   for (int i = 0; i < BUFFER_SIZE; i++)
   {
      write(1, &buffer[i], 1);
      if (buffer[i] == '\0')
      {
         break;
      }
   }
}

void exit(int err){
   syscall(__NR_exit, err);
}

/* ---------- */
void main (){
   /* your code here */
   char buffer[BUFFER_SIZE];

   char* welcome_prompt = "hello, type lines of input, or 'quit'\n";
   char* print_prompt = "you typed: ";
   char* exit_str = "quit";

   print(welcome_prompt);
   while(1) {
      readline(buffer);
      if (strcmp(buffer, exit_str) == 0){
         exit(1);
      }
      print(print_prompt);
      print(buffer);
   }
}
