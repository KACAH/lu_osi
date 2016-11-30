OSI lab4
===
```
dumbfork: OK (2.4s)
Part A score: 5/5

faultread: OK (1.2s)
faultwrite: OK (1.5s)
faultdie: OK (1.1s)
faultregs: OK (1.3s)
faultalloc: OK (1.7s)
faultallocbad: OK (1.1s)
faultnostack: OK (1.8s)
faultbadhandler: OK (2.2s)
faultevilhandler: OK (1.7s)
forktree: OK (1.8s)
Part B score: 50/50

spin: OK (2.4s)
stresssched: OK (3.1s)
pingpong: OK (2.3s)
primes: OK (8.2s)
Part C score: 20/20

Score: 75/75
```

Part A: Multiprocessor Support and Cooperative Multitasking
==

Exercise 1
===
Implement [mmio_map_region] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/kern/pmap.c#L552) in kern/pmap.c. To see how this is used, look at the beginning of lapic_init in kern/lapic.c. You'll have to do the next exercise, too, before the tests for mmio_map_region will run.

Exercise 2
===
Read boot_aps() and mp_main() in kern/init.c, and the assembly code in kern/mpentry.S. Make sure you understand the control flow transfer during the bootstrap of APs. Then modify your implementation of [page_init()] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/kern/pmap.c#L313) in kern/pmap.c to avoid adding the page at MPENTRY_PADDR to the free list, so that we can safely copy and run AP bootstrap code at that physical address. Your code should pass the updated check_page_free_list() test (but might fail the updated check_kern_pgdir() test, which we will fix soon).

Exercise 3
===
Modify [mem_init_mp()] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/kern/pmap.c#L248) (in kern/pmap.c) to map per-CPU stacks starting at KSTACKTOP, as shown in inc/memlayout.h. The size of each stack is KSTKSIZE bytes plus KSTKGAP bytes of unmapped guard pages. Your code should pass the new check in check_kern_pgdir().

Exercise 4
===
The code in [trap_init_percpu()] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/kern/trap.c#L160) (kern/trap.c) initializes the TSS and TSS descriptor for the BSP. It worked in Lab 3, but is incorrect when running on other CPUs. Change the code so that it can work on all CPUs. (Note: your new code should not use the global ts variable any more.)

Exercise 5
===
Apply the big kernel lock as described above, by calling lock_kernel() and unlock_kernel() at the proper locations.

In [i386_init()] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/kern/init.c#L53), acquire the lock before the BSP wakes up the other CPUs. 

In [mp_main()] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/kern/init.c#L123), acquire the lock after initializing the AP, and then call sched_yield() to start running environments on this AP. 

In [trap()] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/kern/trap.c#L325), acquire the lock when trapped from user mode. To determine whether a trap happened in user mode or in kernel mode, check the low bits of the tf_cs. 

In [env_run()] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/kern/env.c#L543), release the lock right before switching to user mode. Do not do that too early or too late, otherwise you will experience races or deadlocks. 

Exercise 6
===
Implement round-robin scheduling in [sched_yield()] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/kern/sched.c#L12) as described above. Don't forget to modify [syscall()] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/kern/syscall.c#L476) to dispatch sys_yield().

Modify [kern/init.c] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/kern/init.c#L64) to create three (or more!) environments that all run the program user/yield.c.

Exercise 7
===
Implement the system calls described above in kern/syscall.c.

[sys_exofork] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/kern/syscall.c#L84): This system call creates a new environment with an almost blank slate: nothing is mapped in the user portion of its address space, and it is not runnable. The new environment will have the same register state as the parent environment at the time of the sys_exofork call. In the parent, sys_exofork will return the envid_t of the newly created environment (or a negative error code if the environment allocation failed). In the child, however, it will return 0. (Since the child starts out marked as not runnable, sys_exofork will not actually return in the child until the parent has explicitly allowed this by marking the child runnable using....) 

[sys_env_set_status] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/kern/syscall.c#L113): Sets the status of a specified environment to ENV_RUNNABLE or ENV_NOT_RUNNABLE. This system call is typically used to mark a new environment ready to run, once its address space and register state has been fully initialized. 

[sys_page_alloc] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/kern/syscall.c#L207): Allocates a page of physical memory and maps it at a given virtual address in a given environment's address space. 

[sys_page_map] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/kern/syscall.c#L265): Copy a page mapping (not the contents of a page!) from one environment's address space to another, leaving a memory sharing arrangement in place so that the new and the old mappings both refer to the same page of physical memory. 

[sys_page_unmap] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/kern/syscall.c#L322): Unmap a page mapped at a given virtual address in a given environment. 

Part B: Copy-on-Write Fork
==

Exercise 8
===
Implement the [sys_env_set_pgfault_upcall] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/kern/syscall.c#L145) system call. Be sure to enable permission checking when looking up the environment ID of the target environment, since this is a "dangerous" system call.

Exercise 9
===
Implement the code in [page_fault_handler] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/kern/trap.c#L407) in kern/trap.c required to dispatch page faults to the user-mode handler. Be sure to take appropriate precautions when writing into the exception stack. (What happens if the user environment runs out of space on the exception stack?)

Exercise 10
===
Implement the _pgfault_upcall routine in [lib/pfentry.S] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/lib/pfentry.S#L68). The interesting part is returning to the original point in the user code that caused the page fault. You'll return directly there, without going back through the kernel. The hard part is simultaneously switching stacks and re-loading the EIP.

Exercise 11
===
Finish [set_pgfault_handler()] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/lib/pgfault.c#L25) in lib/pgfault.c.

Exercise 12
===
Implement [fork] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/lib/fork.c#L116), [duppage] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/lib/fork.c#L74) and [pgfault] (https://github.com/KACAH/lu_osi/blob/e59a479744b27d57c37afc189a1befc0707160d3/lib/fork.c#L18) in lib/fork.c.
