#include <stdio.h>
#include <time.h>
#include <math.h> // sqrt関数を使用するために追加

// O(n^2)に改善されたpyth関数
int pyth(int n_param) { // 引数名を n_param としましたが、mainに合わせて n でも構いません
    int count = 0;
    // a を 1 から n_param までループ (O(n)回)
    for (int a = 1; a <= n_param; ++a) {
        // b をループ
        // 条件 b <= c および a^2 = b^2 + 2c^2 より、3b^2 <= a^2 が導かれるため、
        // b の上限は a / sqrt(3) となる。
        // このループは最大で O(n/sqrt(3)) => O(n) 回実行される。
        for (int b = 1; (long long)b * b * 3 <= (long long)a * a; ++b) {
            // (long long)へのキャストは、b*b*3 や a*a がintの最大値を超えるのを防ぐため。
            // n_param が2100程度なら a*a は 4.41x10^6 でint範囲内だが、安全のため。

            long long a_squared = (long long)a * a;
            long long b_squared = (long long)b * b;

            // 2*c^2 = a^2 - b^2 を計算
            long long term_2c_squared = a_squared - b_squared;

            // a^2 - b^2 が正であり、かつ偶数である必要がある
            if (term_2c_squared > 0 && term_2c_squared % 2 == 0) {
                long long c_squared = term_2c_squared / 2;
                
                // c = sqrt(c_squared) が整数であるかを確認
                // sqrt関数はdouble型を返すため、結果を整数にキャストし、
                // 再度2乗して元の c_squared と一致するかで整数平方根かを確認する。
                int c_val = (int)sqrt((double)c_squared);
                if ((long long)c_val * c_val == c_squared) {
                    // 条件 b <= c を確認
                    if (c_val >= b) {
                        count++;
                    }
                }
            }
        }
    }
    return count;
}

int main() {
    int repeat = 100;   // 繰り返し実行で平均化（大きいと遅くなるので注意）
    int start_n = 10;
    int end_n = 100;
    int step = 10;

    for (int n = start_n; n <= end_n; n += step) {
        clock_t start, end; // Windows環境等で予約語と衝突する場合があるため、start_time, end_time などが推奨されることもあります
        double total_duration = 0;
        int result; // pyth関数の結果を格納（実際には最後の実行結果のみが残る）

        start = clock();
        for (int j = 0; j < repeat; j++) {
            result = pyth(n); // 改善されたO(N^2)のpyth関数を呼び出し
        }
        end = clock();

        total_duration = (double)(end - start) / CLOCKS_PER_SEC;
        double avg_duration = total_duration / repeat;

        // 元の出力形式に合わせて、平均実行時間のみを出力
        printf("%.6f\n", avg_duration); 
        // 必要であれば、Nやカウント結果も表示できます:
        printf("N = %d, Count = %d, Avg Duration = %.6f sec\n", n, result, avg_duration);
    }
    return 0;
}