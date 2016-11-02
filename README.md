OSI lab3
===
```
divzero: OK (1.2s)
softint: OK (1.0s)
badsegment: OK (0.9s)
Part A score: 30/30

faultread: OK (1.1s)
faultreadkernel: OK (1.1s)
faultwrite: OK (0.9s)
faultwritekernel: OK (1.0s)
breakpoint: OK (1.0s)
testbss: OK (1.0s)
hello: OK (1.8s)
buggyhello: OK (1.2s)
buggyhello2: OK (1.6s)
evilhello: OK (1.4s)
Part B score: 50/50

Score: 80/80
```

Part A: User Environments and Exception Handling
==

Exercise 1
===
Modify [mem_init()] (https://github.com/KACAH/lu_osi/blob/96eb71a999501354449c92e7116ba9a554995d93/kern/pmap.c#L154) in kern/pmap.c to allocate and map the envs array. This array consists of exactly NENV instances of the Env structure allocated much like how you allocated the pages array. Also like the pages array, the memory backing envs should also be mapped user read-only at UENVS (defined in inc/memlayout.h) so user processes can read from this array.

Exercise 3
===
In the file env.c, finish coding the following functions:

[env_init()] (https://github.com/KACAH/lu_osi/blob/96eb71a999501354449c92e7116ba9a554995d93/kern/env.c#L115)  
Initialize all of the Env structures in the envs array and add them to the env_free_list. Also calls env_init_percpu, which configures the segmentation hardware with separate segments for privilege level 0 (kernel) and privilege level 3 (user).  

[env_setup_vm()] (https://github.com/KACAH/lu_osi/blob/96eb71a999501354449c92e7116ba9a554995d93/kern/env.c#L162)  
Allocate a page directory for a new environment and initialize the kernel portion of the new environment's address space.  

[region_alloc()] (https://github.com/KACAH/lu_osi/blob/96eb71a999501354449c92e7116ba9a554995d93/kern/env.c#L270)  
Allocates and maps physical memory for an environment  

[load_icode()] (https://github.com/KACAH/lu_osi/blob/96eb71a999501354449c92e7116ba9a554995d93/kern/env.c#L315)  
You will need to parse an ELF binary image, much like the boot loader already does, and load its contents into the user address space of a new environment.  

[env_create()] (https://github.com/KACAH/lu_osi/blob/96eb71a999501354449c92e7116ba9a554995d93/kern/env.c#L385)  
Allocate an environment with env_alloc and call load_icode load an ELF binary into it.  

[env_run()] (https://github.com/KACAH/lu_osi/blob/96eb71a999501354449c92e7116ba9a554995d93/kern/env.c#L487)  
Start a given environment running in user mode.  

Exercise 4
===
Edit trapentry.S and trap.c and implement the features described above. The macros TRAPHANDLER and TRAPHANDLER_NOEC in trapentry.S should help you, as well as the T_* defines in inc/trap.h. You will need to add an entry point in [trapentry.S] (https://github.com/KACAH/lu_osi/blob/96eb71a999501354449c92e7116ba9a554995d93/kern/trapentry.S#L50) (using those macros) for each trap defined in inc/trap.h, and you'll have to provide [_alltraps] (https://github.com/KACAH/lu_osi/blob/96eb71a999501354449c92e7116ba9a554995d93/kern/trapentry.S#L75) which the TRAPHANDLER macros refer to. You will also need to modify [trap_init()] (https://github.com/KACAH/lu_osi/blob/96eb71a999501354449c92e7116ba9a554995d93/kern/trap.c#L63) to initialize the idt to point to each of these entry points defined in trapentry.S; the SETGATE macro will be helpful here.

Part B: Page Faults, Breakpoints Exceptions, and System Calls
==

Exercise 5
===
Modify [trap_dispatch()] (https://github.com/KACAH/lu_osi/blob/96eb71a999501354449c92e7116ba9a554995d93/kern/trap.c#L189) to dispatch page fault exceptions to page_fault_handler(). You should now be able to get make grade to succeed on the faultread, faultreadkernel, faultwrite, and faultwritekernel tests. If any of them don't work, figure out why and fix them. Remember that you can boot JOS into a particular user program using make run-x or make run-x-nox.

Exercise 6
===
Modify [trap_dispatch()] (https://github.com/KACAH/lu_osi/blob/96eb71a999501354449c92e7116ba9a554995d93/kern/trap.c#L193) to make breakpoint exceptions invoke the kernel monitor. You should now be able to get make grade to succeed on the breakpoint test.

Exercise 7
===
Add a handler in the kernel for interrupt vector T_SYSCALL. You will have to edit [kern/trapentry.S] (https://github.com/KACAH/lu_osi/blob/96eb71a999501354449c92e7116ba9a554995d93/kern/trapentry.S#L68) and [kern/trap.c's trap_init()] (https://github.com/KACAH/lu_osi/blob/96eb71a999501354449c92e7116ba9a554995d93/kern/trap.c#L90). You also need to change [trap_dispatch()] (https://github.com/KACAH/lu_osi/blob/96eb71a999501354449c92e7116ba9a554995d93/kern/trap.c#L197) to handle the system call interrupt by calling syscall() (defined in kern/syscall.c) with the appropriate arguments, and then arranging for the return value to be passed back to the user process in %eax. Finally, you need to implement [syscall()] (https://github.com/KACAH/lu_osi/blob/96eb71a999501354449c92e7116ba9a554995d93/kern/syscall.c#L73) in kern/syscall.c. Make sure syscall() returns -E_INVAL if the system call number is invalid. You should read and understand lib/syscall.c (especially the inline assembly routine) in order to confirm your understanding of the system call interface. You may also find it helpful to read inc/syscall.h.

Exercise 8
===
Add the [required code to the user library] (https://github.com/KACAH/lu_osi/blob/96eb71a999501354449c92e7116ba9a554995d93/lib/libmain.c#L16), then boot your kernel. You should see user/hello print "hello, world" and then print "i am environment 00001000". user/hello then attempts to "exit" by calling sys_env_destroy() (see lib/libmain.c and lib/exit.c). Since the kernel currently only supports one user environment, it should report that it has destroyed the only environment and then drop into the kernel monitor. You should be able to get make grade to succeed on the hello test.

Exercise 9
=== 
Change [kern/trap.c] (https://github.com/KACAH/lu_osi/blob/96eb71a999501354449c92e7116ba9a554995d93/kern/trap.c#L265) to panic if a page fault happens in kernel mode.

Read user_mem_assert in kern/pmap.c and implement [user_mem_check] (https://github.com/KACAH/lu_osi/blob/96eb71a999501354449c92e7116ba9a554995d93/kern/pmap.c#L527) in that same file.

Change [kern/syscall.c] (https://github.com/KACAH/lu_osi/blob/96eb71a999501354449c92e7116ba9a554995d93/kern/syscall.c#L27) to sanity check arguments to system calls.
	
Finally, change [debuginfo_eip in kern/kdebug.c] (https://github.com/KACAH/lu_osi/blob/96eb71a999501354449c92e7116ba9a554995d93/kern/kdebug.c#L145) to call user_mem_check on usd, stabs, and stabstr. If you now run user/breakpoint, you should be able to run backtrace from the kernel monitor and see the backtrace traverse into lib/libmain.c before the kernel panics with a page fault. What causes this page fault? You don't need to fix it, but you should understand why it happens.
