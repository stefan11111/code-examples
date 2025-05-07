register long rflags asm("rflags");

int main()
{
    int _rflags = rflags;
    printf("%d\n", _rflags);
}
