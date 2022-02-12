/*
 * file:        part-1.c
 * description: Part 1, CS5600 Project 1, Spring 2022
 */

/* THE ONLY INCLUDE FILE */
#include "sysdefs.h"
#include <stdio.h>
#include <stdlib.h>

/* write these functions */

int read(int fd, void *ptr, int len);
int write(int fd, void *ptr, int len);
void exit(int err);

/* ---------- */

/* Factor, factor! Don't put all your code in main()!
   Write new functions using the functions you just wrote
*/

/* read one line from stdin (file descriptor 0) into a buffer: */
// len: number of bytes to read
// return: 0 for success
int read(int fd, void *ptr, int len)
{
   if (len < 1)
   {
      return -1;
   }

   syscall(__NR_read, fd, ptr, len);
   // printf("%s \n", (char *)ptr);
   return 1;
}

char *readline()
{
   char *buffer = (char *)malloc(sizeof(char) * 201);
   for (int i = 0; i < 201; i++)
   {
      read(0, &buffer[i], 1);
      if (buffer[i] == '\n')
      {
         buffer[++i] = '\0';
         break;
      }
   }
   return buffer;
}
/* print a string to stdout (file descriptor 1) */

/* ---------- */

int main(void)
{
   /* your code here */
   char *res = (char *)malloc(sizeof(char) * 3);
   printf("hello, type: \n");
   read(0, res, 1);
   return 1;
}
