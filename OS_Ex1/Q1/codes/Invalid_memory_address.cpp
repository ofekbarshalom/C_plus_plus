// Invalid memory address

int main() {
    int* p = (int*)0xdeadbeef;
    *p = 7;

    return 0;
}