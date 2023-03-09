/*
 * contains the implementation of all syscalls.
 */

#include <errno.h>
#include <stdint.h>

#include "pmm.h"
#include "process.h"
#include "spike_interface/spike_utils.h"
#include "string.h"
#include "syscall.h"
#include "util/functions.h"
#include "util/types.h"
#include "vmm.h"

//
// implement the SYS_user_print syscall
//
ssize_t sys_user_print(const char *buf, size_t n) {
  // buf is now an address in user space of the given app's user stack,
  // so we have to transfer it into phisical address (kernel is running in direct mapping).
  assert(current);
  char *pa = (char *) user_va_to_pa((pagetable_t) (current->pagetable), (void *) buf);
  sprint(pa);
  return 0;
}

//
// implement the SYS_user_exit syscall
//
ssize_t sys_user_exit(uint64 code) {
  sprint("User exit with code:%d.\n", code);
  // in lab1, PKE considers only one app (one process).
  // therefore, shutdown the system when the app calls exit()
  shutdown(code);
}

// # modified @ lab2_challenge2
//
// maybe, the simplest implementation of malloc in the world ... added @lab2_2
//
uint64 sys_user_allocate_page(uint64 n) {
  // void *pa = alloc_page();
  // uint64 va = g_ufree_page;
  // g_ufree_page += PGSIZE;
  // user_vm_map((pagetable_t) current->pagetable, va, PGSIZE, (uint64) pa,
  //             prot_to_type(PROT_WRITE | PROT_READ, 1));
  // return va;
  return malloc((int) n);
}

// # modified @ lab2_challenge2
//
// reclaim a page, indicated by "va". added @lab2_2
//
uint64 sys_user_free_page(uint64 va) {
  // user_vm_unmap((pagetable_t) current->pagetable, va, PGSIZE, 1);
  free((void *) va);
  return 0;
}

// # added @ lab2_challenge2
//
// allocate n bytes.
//
uint64 sys_user_sbrk(uint64 n) {
  uint64 addr = current->heap_sz;
  growprocess(n);
  return addr;
}

//
// [a0]: the syscall number; [a1] ... [a7]: arguments to the syscalls.
// returns the code of success, (e.g., 0 means success, fail for otherwise)
//
long do_syscall(long a0, long a1, long a2, long a3, long a4, long a5, long a6, long a7) {
  switch (a0) {
    case SYS_user_print:
      return sys_user_print((const char *) a1, a2);
    case SYS_user_exit:
      return sys_user_exit(a1);
    // # modified @ lab2_challenge2
    // added @lab2_2
    case SYS_user_allocate_page:
      return sys_user_allocate_page(a1);
    case SYS_user_free_page:
      return sys_user_free_page(a1);
    // # added @ lab2_challenge2
    case SYS_user_sbrk:
      return sys_user_sbrk(a1);
    default:
      panic("Unknown syscall %ld \n", a0);
  }
}
