#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char buf[1];
  int pfds[2], cfds[2];
  pipe(pfds);
  pipe(cfds);
  int pid = fork();
  if(pid > 0){
    //parent
    write(pfds[1], buf, 1);
    read(cfds[0], buf, 1);
    printf("%d: received pong\n", getpid());
  } else {
    read(pfds[0], buf, 1);
    printf("%d: received ping\n", getpid());
    write(cfds[1], buf, 1);
  }
  exit(0);
}
