#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUFSZ 1024

int
main(int argc, char *argv[])
{
  int pid;
  int i, j, k;
  int n;
  char buf[BUFSZ];
  for(i = 1; i < argc; i++) {
    argv[i-1] = argv[i];
  }
  i = j = 0;
  while(1) {
    n = read(0, buf+i, BUFSZ);
    //printf("read(i:%d j:%d n:%d): %s\n", i, j, n,  buf);
    if(n==0) {
      break;
    }
    k = i;
    i = 0;
    while(j<k+n) {
      if(buf[j] != '\n') {
        j++;
        continue;
      }
      if(j-i>0) {
        pid = fork();
        if(pid == 0) {
          buf[j] = '\0';
          argv[argc-1] = buf+i;
          exec(argv[0], argv);
        } else {
          wait(0);
        }
      }
      j++;
      i = j;
    }
    if(i != 0 && buf[n-1] != '\n') {
      memcpy(buf, buf+i, j-i);
      i = j-i;
      j = i;
    } else {
      i = j;
    }
  }
  if(j-i > 0) {
    pid = fork();
    if(pid == 0) {
      buf[j] = '\0';
      argv[argc-1] = buf+i;
      exec(argv[0], argv);
    } else {
      wait(0);
    }
  }
  exit(0);
}
