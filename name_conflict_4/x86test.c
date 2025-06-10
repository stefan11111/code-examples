int main(){
#if 2 * sizeof(int) == sizeof(void*)
printf("yes\n"):
#else
printf("no\n");
#endif
}
