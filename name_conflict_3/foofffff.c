#define SYS_write 1
#define SYS_exit 60

void _start()
{
  static const char str[] = "Hello, world!\n";

  volatile register long rax asm("rax");
  volatile register long rdi asm("rdi");
  volatile register long rsi asm("rsi");
  volatile register long rdx asm("rdx");

  rax = SYS_write;
  rdi = 1;
  rsi = (long)str;
  rdx = sizeof(str);
  /* rax = SYS_write; */
  asm("syscall\n\t");

  rax = SYS_exit;
  rdi = 0;
  asm("syscall\n\t");
}
