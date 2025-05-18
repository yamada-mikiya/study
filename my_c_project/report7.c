#include <stdio.h>
#include <math.h> // sqrt関数を使用するために必要
#include <time.h>

// a^2 = b^2 + 2c^2 を満たす (a, b, c) の組の数を数える関数
// 計算量は O(n^2) を目指す
int pyth(int n) {
    int count = 0;
    // b と c のループで O(n^2) になる
    for (int b = 1; b <= n; b++) { // b は n 以下
        for (int c = b; c <= n; c++) { // c は b 以上 n 以下 (問題文の b <= c と a <= n から推測)
                                      // a^2 = b^2 + 2c^2 より a = sqrt(b^2 + 2c^2)
                                      // a <= n でなければならない
            double a_squared = (double)b * b + 2.0 * c * c;
            double a_val = sqrt(a_squared);

            // a が整数であり、かつ a <= n を満たすか確認
            if (a_val == floor(a_val) && a_val <= n) {
                // (a,b,c) = (6,2,4) の場合、 6*6 = 36, 2*2 + 2*4*4 = 4 + 32 = 36
                // この条件だと a が整数であることの確認のみ
                // 問題文の指示は「正の整数 a, b, c」なので、a > 0 も暗黙の条件
                // b, c はループの開始値から正であることが保証されている
                // a_val が正であることは、b, c が正であれば自明
                count++;
            }
        }
    }
    return count;
}

int main() {
    int repeat = 100;  // 繰り返し実行で平均化（大きいと遅くなるので注意）
    int start_n = 100;
    int end_n = 10000;
    int step = 100;

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