OSI lab4
===
```
internal FS tests: OK (7.6s)
  fs i/o: OK
  check_super: OK
spawn via spawnhello: OK (7.3s)
PTE_SHARE [testpteshare]: OK (7.5s)
PTE_SHARE [testfdsharing]: OK (6.4s)
start the shell [icode]: OK (6.7s)
testshell: OK (8.7s)
primespipe: OK (14.3s)

Score: 75/75
```

Exercise 1
===
i386_init identifies the file system environment by passing the type ENV_TYPE_FS to your environment creation function, env_create. Modify [env_create] (https://github.com/KACAH/lu_osi/blob/9455d47397e8f9bda8a614a6ba374804ee80bc84/kern/env.c#L407) in env.c, so that it gives the file system environment I/O privilege, but never gives that privilege to any other environment.

Exercise 2
===
Implement the [bc_pgfault] (https://github.com/KACAH/lu_osi/blob/9455d47397e8f9bda8a614a6ba374804ee80bc84/fs/bc.c#L36) functions in fs/bc.c. bc_pgfault is a page fault handler, just like the one your wrote in the previous lab for copy-on-write fork, except that its job is to load pages in from the disk in response to a page fault. When writing this, keep in mind that (1) addr may not be aligned to a block boundary and (2) ide_read operates in sectors, not blocks.

Exercise 3
===
spawn relies on the new syscall sys_env_set_trapframe to initialize the state of the newly created environment. Implement [sys_env_set_trapframe] (https://github.com/KACAH/lu_osi/blob/9455d47397e8f9bda8a614a6ba374804ee80bc84/kern/syscall.c#L140). Test your code by running the user/spawnhello program from kern/init.c, which will attempt to spawn /hello from the file system.

Exercise 4
===
Change [duppage] (https://github.com/KACAH/lu_osi/blob/9455d47397e8f9bda8a614a6ba374804ee80bc84/lib/fork.c#L80) in lib/fork.c to follow the new convention. If the page table entry has the PTE_SHARE bit set, just copy the mapping directly. (You should use PTE_SYSCALL, not 0xfff, to mask out the relevant bits from the page table entry. 0xfff picks up the accessed and dirty bits as well.)

Likewise, implement [copy_shared_pages] (https://github.com/KACAH/lu_osi/blob/9455d47397e8f9bda8a614a6ba374804ee80bc84/lib/spawn.c#L301) in lib/spawn.c. It should loop through all page table entries in the current process (just like fork did), copying any page mappings that have the PTE_SHARE bit set into the child process.

Exercise 5
===
In your kern/trap.c, call [kbd_intr] (https://github.com/KACAH/lu_osi/blob/9455d47397e8f9bda8a614a6ba374804ee80bc84/kern/trap.c#L291) to handle trap IRQ_OFFSET+IRQ_KBD and [serial_intr] (https://github.com/KACAH/lu_osi/blob/9455d47397e8f9bda8a614a6ba374804ee80bc84/kern/trap.c#L296) to handle trap IRQ_OFFSET+IRQ_SERIAL.
