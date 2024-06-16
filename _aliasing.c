struct foo {
    union
    {
        void *voidp;
        char fd[8];
    } data;
    int i;
};

struct bar {
    char fd[8];
    int i;
};

int main()
{
    extern int printf(const char *, ...);
    union u {
        void *voidp;
        char fd[8];
    };
    printf("%zd %zd %zd %d", sizeof(struct foo), sizeof (struct bar), sizeof (union u), sizeof(struct foo) == sizeof(struct bar));
}
