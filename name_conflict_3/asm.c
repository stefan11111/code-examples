/* -ffixed-rax -ffixed-rbx -ffixed-rdi -ffixed-rsi -ffixed-rdx */

#define SYS_write 1
#define SYS_exit 60

register long rax asm("rax");
register long rdi asm("rdi");
register long rsi asm("rsi");
register long rdx asm("rdx");

void _start()
{
  static const char str[] = "Hello, world!\n";

  rax = SYS_write;
  rdi = 1;
  rsi = (long)str;
  rdx = sizeof(str);
  asm("syscall\n\t");

  rax = SYS_exit;
  rdi = 0;
  asm("syscall\n\t");
}
