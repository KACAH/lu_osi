// implement fork from user space

#include <inc/string.h>
#include <inc/lib.h>

// PTE_COW marks copy-on-write page table entries.
// It is one of the bits explicitly allocated to user processes (PTE_AVAIL).
#define PTE_COW		0x800

extern void _pgfault_upcall();


//
// Custom page fault handler - if faulting page is copy-on-write,
// map in our own private writable copy.
//
static void
pgfault(struct UTrapframe *utf)
{
	void *addr = (void *) utf->utf_fault_va;
	uint32_t err = utf->utf_err;
	int r;

	// Check that the faulting access was (1) a write, and (2) to a
	// copy-on-write page.  If not, panic.
	// Hint:
	//   Use the read-only page table mappings at uvpt
	//   (see <inc/memlayout.h>).

	// LAB 4: Your code here.
    if ((err & FEC_WR) == 0) {
        panic("Page fault reason is not 'write'");
    }
    if ((uvpt[PGNUM(addr)] & PTE_COW) == 0) {
        panic("Not copy-on-write page fault 'write'");
    }

	// Allocate a new page, map it at a temporary location (PFTEMP),
	// copy the data from the old page to the new page, then move the new
	// page to the old page's address.
	// Hint:
	//   You should make three system calls.
	//   No need to explicitly delete the old page's mapping.

	// LAB 4: Your code here.
    addr = ROUNDDOWN(addr, PGSIZE);
    r = sys_page_alloc(0, PFTEMP, PTE_W|PTE_U|PTE_P);
    if (r < 0) {
        panic("Failed to allocate new page for copy-on-write");
    }
    memcpy(PFTEMP, addr, PGSIZE);
    r = sys_page_map(0, PFTEMP, 0, addr, PTE_W|PTE_U|PTE_P);
    if (r < 0) {
       panic("Failed to map new page for copy-on-write");
    }
    r = sys_page_unmap(0, PFTEMP);
    if (r < 0) {
        panic("Failed to unmap temporary page for copy-on-write");
    }
}

//
// Map our virtual page pn (address pn*PGSIZE) into the target envid
// at the same virtual address.  If the page is writable or copy-on-write,
// the new mapping must be created copy-on-write, and then our mapping must be
// marked copy-on-write as well.  (Exercise: Why do we need to mark ours
// copy-on-write again if it was already copy-on-write at the beginning of
// this function?)
//
// Returns: 0 on success, < 0 on error.
// It is also OK to panic on error.
//
static int
duppage(envid_t envid, unsigned pn)
{
	int r;

	// LAB 4: Your code here.
    void *va = (void*) (pn * PGSIZE);
    if ((uvpt[pn] & (PTE_W | PTE_COW)) > 0) {
        r = sys_page_map(0, va, envid, va, PTE_COW|PTE_U|PTE_P);
        if (r < 0) {
            return r;
        }
        r = sys_page_map(0, va, 0, va, PTE_COW|PTE_U|PTE_P);
        if (r < 0) {
            return r;
        }
    } else {
        r = sys_page_map(0, va, envid, va, PTE_U|PTE_P);
        if (r < 0) {
            return r;
        }
    }

    return 0;
}

//
// User-level fork with copy-on-write.
// Set up our page fault handler appropriately.
// Create a child.
// Copy our address space and page fault handler setup to the child.
// Then mark the child as runnable and return.
//
// Returns: child's envid to the parent, 0 to the child, < 0 on error.
// It is also OK to panic on error.
//
// Hint:
//   Use uvpd, uvpt, and duppage.
//   Remember to fix "thisenv" in the child process.
//   Neither user exception stack should ever be marked copy-on-write,
//   so you must allocate a new page for the child's user exception stack.
//
envid_t
fork(void)
{
	// LAB 4: Your code here.
    int res;
    set_pgfault_handler(pgfault);

    envid_t envid = sys_exofork();
    if (envid < 0) {
        return envid;
    }
    if (envid == 0) {
        // We're the child.
        // The copied value of the global variable 'thisenv'
        // is no longer valid (it refers to the parent!).
        // Fix it and return 0.
        thisenv = &envs[ENVX(sys_getenvid())];
        return 0;
	}

    for (uint32_t va = 0; va < USTACKTOP; va += PGSIZE) {
        uint32_t pgnum = PGNUM(va);
        int flags = PTE_P|PTE_U;
        if ((uvpd[PDX(va)] & PTE_P) > 0 && (uvpt[pgnum] & flags) == flags) {
            res = duppage(envid, pgnum);
            if (res < 0) {
                return res;
            }
        }
    }

    res = sys_page_alloc(envid, (void *) (UXSTACKTOP - PGSIZE), PTE_U|PTE_W|PTE_P);
    if (res < 0) {
        return res;
    }

    sys_env_set_pgfault_upcall(envid, _pgfault_upcall);
    res = sys_env_set_status(envid, ENV_RUNNABLE);
    if (res < 0) {
        return res;
    }
    return envid;
}

// Challenge!
int
sfork(void)
{
	panic("sfork not implemented");
	return -E_INVAL;
}
