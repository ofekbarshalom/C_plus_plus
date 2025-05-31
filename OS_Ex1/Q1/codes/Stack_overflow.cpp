//Stack overflow by infinite recursion

void crash() {
    crash();
}

int main() {
    crash();
    return 0;
}
