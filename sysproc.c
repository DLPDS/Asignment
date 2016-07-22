#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "guestSysCalls.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_getyear(void)
{
  return 1975;
}

int
sys_getkernelendaddr(void){
return KERNBASE+PHYSTOP;
}

int
sys_getkernelvariaddr(void){
  int x=2;
  int *ad=&x;
  return (int)ad;
}

int sys_getsystemcalladdr(void){
  return (int)sys_fork;
}

int sys_getkernelstartaddr(void){
  return KERNBASE;
}

int sys_setpriority(void){
  int n;
  if(argint(0, &n) < 0)
    return 0;
  proc->priority=n;
  return 1;
}

int
sys_mycall(void)
{
  int size;
  char *buf;


  if (argint(0, &size) <0){
    return -1;
  }

  if (argptr(1, &buf,size) <0){
    return -1;
  }

  walkprocesstable(buf,size);


  return 3;
}

int
sys_vmtrap(void){
  int para;
  if (argint(0, &para) <0){
    return -1;
  }
  if(proc->name[0]=='g' && proc->name[1]=='u' && proc->name[0]=='e' && proc->name[0]=='s'){
    return gtrap(para);
  }
  else{
    kill(proc->pid);
    return 0;
  }
}