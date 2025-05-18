#include <stdio.h>
#include <time.h>

// 最初の桁を取り出す関数
int saisho_no_keta(int num) {
    while (num >= 10) {
        num /= 10;
    }
    return num;
}

// 最後の桁を取り出す関数
int saigo_no_keta(int num) {
    return num % 10;
}

// 条件を満たす個数を数える関数
int L_mult_R(int n) {
    int count = 0;
    for (int i = 1; i <= n; i++) {
        int saisho = saisho_no_keta(i);
        int saigo = saigo_no_keta(i);

        if (saigo == 0) continue;

        if (saisho % saigo == 0) {
            count++;
        }
    }
    return count;
}

int main() {
    int n_values[] = {100, 1000, 5000, 10000};
    int num_cases = sizeof(n_values) / sizeof(n_values[0]);
    int repeat = 100000; // 繰り返し回数

    for (int i = 0; i < num_cases; i++) {
        int n = n_values[i];
        clock_t start, end;
        double total_duration;

        start = clock(); // 時間計測開始
        int result = 0;
        for (int j = 0; j < repeat; j++) {
            result = L_mult_R(n);
        }
        end = clock();   // 時間計測終了

        total_duration = (double)(end - start) / CLOCKS_PER_SEC;
        double avg_duration = total_duration / repeat;

        printf("n = %d のとき、一回あたりの実行時間 = %f 秒\n", n, avg_duration);
    }

    return 0;
}
