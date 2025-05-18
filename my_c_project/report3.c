#include <stdio.h>

int pyth(int n) {
    int count = 0;

    for (int a = 1; a <= n; a++) {
        for (int b = 1; b * b < a * a; b++) {
            for (int c = b; b * b + 2 * c * c <= a * a; c++) {
                if (a * a == b * b + 2 * c * c) {
                    count++;
                }
            }
        }
    }

    return count;
}

// テスト用main関数（必要に応じて）
int main() {
    int n = 100;
    printf("pyth(%d) = %d\n", n, pyth(n));
    return 0;
}
