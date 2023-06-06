#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64 sys_sysinfo(void) //info syscall definition
{
  int param;
  argint(0, &param);
  return print_info(param);
}

uint64 
sys_procinfo(void)
{
  //struct proc *p = myproc();
  uint64 in_addr;

  argaddr(0,&in_addr);
  /*
  if (copyout(p->pagetable, pinfo_addr, (char *)&in, sizeof(in)) < 0)
  {
    return -1;
  }
*/
  return get_procinfo(in_addr);
}

uint64 sys_sched_statistics(void)
{
  sched_statistics();
  return 0;
}

uint64 sys_sched_tickets(void)
{
  int num_tickets;
  argint(0, &num_tickets);

  if(num_tickets <= 0) { 
    return -1;
  }
  
	sched_tickets(num_tickets);
	return 0;
}

uint64 sys_clone(void)
{
  void* stack;
  argaddr(0,(uint64*)&stack);

  if (stack == 0)
    return -1;
  clone(stack);
  return 0;
}
