OSI lab2
===
```
running JOS: (0.9s)
  Physical page allocator: OK
  Page management: OK
  Kernel page directory: OK
  Page management 2: OK
Score: 70/70
```

Exercise 1
===
In the file kern/pmap.c, you must implement code for the following functions (probably in the order given).

[boot_alloc()] (https://github.com/KACAH/lu_osi/blob/d5cd4c396761b9acc8e176994ef49b47102b227f/kern/pmap.c#L101)  
[mem_init()] (https://github.com/KACAH/lu_osi/blob/d5cd4c396761b9acc8e176994ef49b47102b227f/kern/pmap.c#L148)  
[page_init()] (https://github.com/KACAH/lu_osi/blob/d5cd4c396761b9acc8e176994ef49b47102b227f/kern/pmap.c#L256)  
[page_alloc()] (https://github.com/KACAH/lu_osi/blob/d5cd4c396761b9acc8e176994ef49b47102b227f/kern/pmap.c#L282)  
[page_free()] (https://github.com/KACAH/lu_osi/blob/d5cd4c396761b9acc8e176994ef49b47102b227f/kern/pmap.c#L302)  

Exercise 4
===
In the file kern/pmap.c, you must implement code for the following functions.

[pgdir_walk()] (https://github.com/KACAH/lu_osi/blob/d5cd4c396761b9acc8e176994ef49b47102b227f/kern/pmap.c#L344)  
[boot_map_region()] (https://github.com/KACAH/lu_osi/blob/d5cd4c396761b9acc8e176994ef49b47102b227f/kern/pmap.c#L375)  
[page_lookup()] (https://github.com/KACAH/lu_osi/blob/d5cd4c396761b9acc8e176994ef49b47102b227f/kern/pmap.c#L437)  
[page_remove()] (https://github.com/KACAH/lu_osi/blob/d5cd4c396761b9acc8e176994ef49b47102b227f/kern/pmap.c#L469)  
[page_insert()] (https://github.com/KACAH/lu_osi/blob/d5cd4c396761b9acc8e176994ef49b47102b227f/kern/pmap.c#L410)

Exercise 5
===
Fill in the missing code in [mem_init()] (https://github.com/KACAH/lu_osi/blob/d5cd4c396761b9acc8e176994ef49b47102b227f/kern/pmap.c#L172) after the call to check_page().
