## Student Name: Kate Nguyen

# Assignment 4: xv6 (10 points)

## Preparation:

- **Download** the xv6 repository from https://github.com/mit-pdos/xv6-public, and copy it 
into your assignment repository. The remaining steps of this assignment will be performed within this 
directory.
- You will need to install `qemu` (container `sudo` password is `goldenrams`):

```
$ sudo apt-get update
$ sudo apt-get install qemu
```

## Tasks:

Perform the following steps to add to xv6 a custom system call that displays the year West Chester University was founded.   

- Step 1: Edit `syscall.h` to include a line that describes the additional system call:

```
#define SYS_wcupa 22
```

- Step 2: Add a pointer to the system call in the `syscall.c` file:

```
[SYS_wcupa] sys_wcupa,
```

- Step 3: A function prototype also needs to be added to the `syscall.c` file:

```
extern int sys_wcupa(void);
```

- Step 4: Implement the system call function inside `sysproc.c` file:

```
int
sys_wcupa(void)
{
  return 1871;
}
```

- Step 5: Provide an interface for the system call in the `usys.S` file:

```
SYSCALL(wcupa)
```

- Step 6: Provide a declaration for the system call in the `user.h` file:

```
int wcupa(void);
```

- Step 7: Write a user program called `test_wcupa.c` to test the system call:

```
#include "types.h"
#include "stat.h"n
#include "user.h"
 
int main(void) {
  printf(1, "West Chester University was founded in %d\n", wcupa());
  exit();
}
```

- Step 8: Modify the Makefile file to include `test_wcupa.c` in the make process

```
UPROGS=\
        _cat\
        _echo\
        _forktest\
        _grep\
        _init\
        _kill\
        _ln\
        _ls\
        _mkdir\
        _rm\
        _sh\
        _stressfs\
        _usertests\
        _wc\
        _zombie\
        _test_wcupa\

EXTRA=\
        mkfs.c ulib.c user.h cat.c echo.c forktest.c grep.c kill.c\
        ln.c ls.c mkdir.c rm.c stressfs.c usertests.c wc.c zombie.c\
        printf.c umalloc.c test_wcupa.c\
        README dot-bochsrc *.pl toc.* runoff runoff1 runoff.list\
        .gdbinit.tmpl gdbutil\
```

- Step 9: Inside the xv6 directory, run:

```
$ make clean
$ make qemu-nox
```

- Step 10: Demonstrate the new application/system call inside xv6. To quit out of xv6, use `Ctr-A` then `X`. 


## Submission:

- Edit the README.md to add your name to the top of the file (1 point)
- The repository should have the **xv6** directory that contains all the modifications from the steps. 
- Perform all modifications described in the above steps and take screenshots at each step. 
- Upload all your screenshots to the assignment repository inside a folder called **screenshots**. 
- Submit the URL of your assignment git repository, as provided through GitHub Classroom to D2L. (1 point)

