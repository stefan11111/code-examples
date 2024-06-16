struct foo {
    char c;
    union
    {
        void *voidp;
        int fd;
    } data;
};

struct bar {
    char c;
    void* data;
};

int main()
{
    extern int printf();
    printf("%d", sizeof(struct foo) == sizeof(struct bar));
}

