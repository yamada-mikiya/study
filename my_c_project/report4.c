#include <stdio.h>
#include <time.h>

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

int main() {
    int repeat = 100;  // 繰り返し実行で平均化（大きいと遅くなるので注意）
    int start_n = 50;
    int end_n = 1050;
    int step = 10;

    for (int n = start_n; n <= end_n; n += step) {
        clock_t start, end;
        double total_duration = 0;
        int result;

        start = clock();
        for (int j = 0; j < repeat; j++) {
            result = pyth(n);
        }
        end = clock();

        total_duration = (double)(end - start) / CLOCKS_PER_SEC;
        double avg_duration = total_duration / repeat;

        printf("%d, %.6f\n",n, avg_duration);
    }
    return 0;
}
