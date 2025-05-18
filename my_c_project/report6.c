#include <stdio.h>
#include <stdlib.h> // For malloc, free, exit, EXIT_FAILURE
#include <math.h>   // For sqrt, fabs, log
#include <stdbool.h> // For bool, true, false (C99 onwards)
#include <string.h>  // For memset or explicit loop for calloc behavior
#include <time.h>    // For clock

// 動的配列ヘルパー (mobius_sieve_c で使用)
typedef struct {
    int* data;
    int size;
    int capacity;
} DynamicIntArray;

void initDynamicArray(DynamicIntArray* arr, int initial_capacity) {
    if (initial_capacity <= 0) initial_capacity = 1; // 正の容量を保証
    arr->data = (int*)malloc(initial_capacity * sizeof(int));
    if (arr->data == NULL) {
        perror("primes 配列のメモリ確保に失敗しました");
        exit(EXIT_FAILURE);
    }
    arr->size = 0;
    arr->capacity = initial_capacity;
}

void pushBackDynamicArray(DynamicIntArray* arr, int value) {
    if (arr->size >= arr->capacity) {
        arr->capacity = arr->capacity * 2;
        int* newData = (int*)realloc(arr->data, arr->capacity * sizeof(int));
        if (newData == NULL) {
            perror("primes 配列のメモリ再確保に失敗しました");
            free(arr->data); 
            exit(EXIT_FAILURE);
        }
        arr->data = newData;
    }
    arr->data[arr->size++] = value;
}

void freeDynamicArray(DynamicIntArray* arr) {
    if (arr->data != NULL) {
        free(arr->data);
        arr->data = NULL;
    }
    arr->size = 0;
    arr->capacity = 0;
}

// メビウス関数 μ を limit まで計算する篩 (C言語版)
int* mobius_sieve_c(int limit) {
    if (limit < 1) { 
        int* mu_empty = (int*)malloc(sizeof(int)); 
        if(mu_empty == NULL) {
            perror("mu_empty のメモリ確保に失敗しました");
            exit(EXIT_FAILURE);
        }
        // limit < 1 の場合、mu[0] や mu[1] はこの後のロジックでは通常アクセスされない想定
        // しかし、安全のため mu[1] を使うケースも考慮し、少なくとも mu[0] までアクセス可能にしておく
        // または、呼び出し側で limit_d が0にならないようにする。
        // ここでは limit=0 の場合に mu[0] のみを確保し、mu[1] を使う場合は呼び出し側で limit >= 1 を保証する。
        // pyth_O_N_c で limit_d が0の場合の処理を追加したので、ここは limit < 1 で最小確保。
        return mu_empty; 
    }

    int* mu = (int*)malloc((limit + 1) * sizeof(int));
    int* lp = (int*)calloc(limit + 1, sizeof(int)); // calloc で 0 初期化
    DynamicIntArray primes_list;

    if (mu == NULL || lp == NULL) {
        perror("mu または lp 配列のメモリ確保に失敗しました");
        if (mu) free(mu);
        if (lp) free(lp);
        exit(EXIT_FAILURE);
    }
    
    int prime_capacity_estimate = 10; 
    if (limit > 1) {
        prime_capacity_estimate = (int)(1.2 * limit / (log((double)limit > 1.0 ? (double)limit : 2.0))) + 100;
         if (prime_capacity_estimate <=0 ) prime_capacity_estimate = limit / 2 + 10; 
    }
    initDynamicArray(&primes_list, prime_capacity_estimate);

    mu[1] = 1;
    for (int i = 2; i <= limit; ++i) {
        if (lp[i] == 0) {
            lp[i] = i;
            pushBackDynamicArray(&primes_list, i);
            mu[i] = -1; 
        }
        for (int j = 0; j < primes_list.size; ++j) {
            int p = primes_list.data[j];
            if (p > lp[i] || (long long)i * p > limit) {
                break;
            }
            lp[i * p] = p;
            if (p == lp[i]) { 
                mu[i * p] = 0;
                break; 
            } else {
                mu[i * p] = -mu[i];
            }
        }
    }
    free(lp);
    freeDynamicArray(&primes_list);
    return mu;
}

// O(N) の pyth 関数
int pyth_O_N_c(int n_param) {
    if (n_param <= 0) {
        return 0;
    }

    long long count = 0;
    int limit_d = (int)(sqrt((double)n_param));
    
    if (limit_d == 0 && n_param >= 1) { // n_param が 1, 2, 3 の場合 limit_d は 1 になるべき
        limit_d = 1;
    }
    if (n_param == 0) limit_d = 0; // n_param=0 なら limit_d=0

    // limit_d が 0 の場合、mobius_sieve_c は1要素(mu[0]など)だけ確保するかもしれない。
    // mu[1] を使うので、limit_d は最低でも1が必要。
    // ただし、d=1 から limit_d までのループなので、limit_d=0ならループは実行されない。
    // mobius_sieve_c(0) が安全に動作するように修正、または limit_d=0 のケースをここで処理。
    int* mu;
    if (limit_d < 1) { // limit_d が 0 (n_param も 0) の場合
        mu = mobius_sieve_c(0); // 安全なダミーを返すように sieve を修正
                               // あるいは、ここで単に return 0;
    } else {
        mu = mobius_sieve_c(limit_d);
    }


    const double sqrt2_val = sqrt(2.0);
    const double sqrt3_val = sqrt(3.0);
    const double r1_low = sqrt3_val - 1.0; 
    const double r1_high = sqrt3_val + 1.0;

    const double r2_low = (sqrt3_val - 1.0) / 2.0;
    const double r2_high = (sqrt3_val + 1.0) / 2.0;
    const double epsilon = 1e-9; // 浮動小数点比較用の微小値

    // タイプ1: a0 = x0^2 + 2*y0^2 (原始形式部分、d^2 でスケール)
    // 有効な x_eff = d*x0, y_eff = d*y0。x_eff が奇数 => d と x0 が奇数。
    for (int d = 1; d <= limit_d; ++d) {
        if (limit_d < 1 || mu[d] == 0 || (d % 2 == 0)) { // d は奇数である必要あり
            continue;
        }
        long long term_sum_d = 0;
        long long d_squared = (long long)d * d;
        // d_squared が n_param を超える場合、n_param_div_d_sq は 0 になる。
        if (d_squared == 0 || d_squared > n_param ) continue; 
        long long n_param_div_d_sq = n_param / d_squared; 
        if (n_param_div_d_sq == 0) continue; // ループが実行されないのでスキップ
        
        int limit_x0 = (int)(sqrt((double)n_param_div_d_sq));

        for (int x0 = 1; x0 <= limit_x0; x0 += 2) { // x0 は奇数
            long long x0_squared = (long long)x0 * x0;
            long long val_for_2y0sq = n_param_div_d_sq - x0_squared;
            
            if (val_for_2y0sq <= 0) continue; 
            
            int limit_y0 = (int)(sqrt(val_for_2y0sq / 2.0));
            
            for (int y0 = 1; y0 <= limit_y0; ++y0) {
                double ratio_x0y0 = (double)x0 / y0;
                bool b_le_c_cond = false;
                // x0/y0 != sqrt(2) のチェック (b0 != 0 を保証)
                if (fabs(ratio_x0y0 - sqrt2_val) > epsilon) {
                     // b <= c 条件のチェック (浮動小数点境界に注意)
                     if (ratio_x0y0 >= r1_low - epsilon && ratio_x0y0 <= r1_high + epsilon) {
                         b_le_c_cond = true;
                     }
                }

                if (b_le_c_cond) {
                    long long a0_val_primitive_form = x0_squared + 2LL * y0 * y0;
                    if (a0_val_primitive_form == 0) continue; 
                    // ループ上限により、a0_val_primitive_form > n_param_div_d_sq は基本的に発生しないはず
                    // if (a0_val_primitive_form > n_param_div_d_sq) continue; 

                    term_sum_d += n_param / ( d_squared * a0_val_primitive_form );
                }
            }
        }
        count += (long long)mu[d] * term_sum_d;
    }
    
    // タイプ2: a0 = 2*x0^2 + y0^2
    // 有効な y_eff = d*y0, x_eff = d*x0。y_eff が奇数 => d と y0 が奇数。
    for (int d = 1; d <= limit_d; ++d) {
        if (limit_d < 1 || mu[d] == 0 || (d % 2 == 0)) { // d は奇数
            continue;
        }
        long long term_sum_d = 0;
        long long d_squared = (long long)d * d;
        if (d_squared == 0 || d_squared > n_param) continue;
        long long n_param_div_d_sq = n_param / d_squared;
        if (n_param_div_d_sq == 0) continue;
        
        int limit_y0 = (int)(sqrt((double)n_param_div_d_sq));

        for (int y0 = 1; y0 <= limit_y0; y0 += 2) { // y0 は奇数
            long long y0_squared = (long long)y0*y0;
            long long val_for_2x0sq = n_param_div_d_sq - y0_squared;

            if (val_for_2x0sq <= 0) continue;

            int limit_x0 = (int)(sqrt(val_for_2x0sq / 2.0));

            for (int x0 = 1; x0 <= limit_x0; ++x0) {
                double ratio_x0y0 = (double)x0 / y0;
                bool b_le_c_cond = false;
                // x0/y0 != 1/sqrt(2) のチェック (b0 != 0 を保証)
                if (fabs(ratio_x0y0 - (1.0/sqrt2_val)) > epsilon) {
                     // b <= c 条件のチェック
                     if (ratio_x0y0 >= r2_low - epsilon && ratio_x0y0 <= r2_high + epsilon) {
                         b_le_c_cond = true;
                     }
                }

                if (b_le_c_cond) {
                    long long a0_val_primitive_form = 2LL * x0 * x0 + y0_squared;
                    if (a0_val_primitive_form == 0) continue;
                    // if (a0_val_primitive_form > n_param_div_d_sq) continue;

                    term_sum_d += n_param / ( d_squared * a0_val_primitive_form );
                }
            }
        }
        count += (long long)mu[d] * term_sum_d;
    }

    if (limit_d >=0) { // mobius_sieve_c(0) は mu[0] のみを確保したかもしれないので、limit_d < 1 でも解放は試みる
      free(mu);
    }
    return (int)(count);
}

// ユーザー提供の元の O(N^2) 関数 (C言語互換と仮定)
int pyth_original(int n_param) {
    int count = 0;
    for (int a = 1; a <= n_param; ++a) {
        for (int b = 1; (long long)b * b * 3 <= (long long)a * a; ++b) {
            long long a_squared = (long long)a * a;
            long long b_squared = (long long)b * b;
            long long term_2c_squared = a_squared - b_squared;

            if (term_2c_squared > 0 && term_2c_squared % 2 == 0) {
                long long c_squared = term_2c_squared / 2;
                int c_val = (int)sqrt((double)c_squared); 
                if ((long long)c_val * c_val == c_squared) {
                    // 元のコードではこの if (c_val >= b) があった。
                    // b のループ条件 (b*b*3 <= a*a) により、この条件は常に真となるため冗長。
                    // しかし、元のコードの動作に合わせるなら残す。
                    if (c_val >= b) { 
                        count++;
                    }
                }
            }
        }
    }
    return count;
}

// テスト用 main 関数
int main() {
    int repeat = 100; 
    int start_n = 10; 
    int end_n = 100; 
    int step = 10;

    printf("O(N) バージョン (pyth_O_N_c) のテスト:\n");
    printf("N      | カウント| 平均実行時間 (sec)\n");
    printf("-------|--------|-------------------\n");
    for (int n = start_n; n <= end_n; n += step) {
        clock_t start_time, end_time;
        double total_duration = 0;
        int result = 0;

        start_time = clock();
        for (int j = 0; j < repeat; j++) {
            result = pyth_O_N_c(n); 
        }
        end_time = clock();

        total_duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        double avg_duration = total_duration / repeat;
        printf("%-7d| %-7d| %.10f\n", n, result, avg_duration);
    }

    printf("\n元の O(N^2) バージョン (pyth_original) のテスト (比較用):\n");
    printf("N      | カウント| 平均実行時間 (sec)\n");
    printf("-------|--------|-------------------\n");
    int end_n_orig = 500; // O(N^2) は遅いので、Nの上限を小さくする
    int repeat_orig = 5;
     for (int n = start_n; n <= end_n_orig; n += step) {
        clock_t start_time, end_time;
        double total_duration = 0;
        int result_orig = 0;

        start_time = clock();
        for (int j = 0; j < repeat_orig; j++) {
            result_orig = pyth_original(n);
        }
        end_time = clock();

        total_duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        double avg_duration = total_duration / repeat_orig;
        printf("%-7d| %-7d| %.10f\n", n, result_orig, avg_duration);
    }
    
    int n_test_large = 2000;
    printf("\nO(N) バージョン pyth_O_N_c(%d) の単一テスト:\n", n_test_large);
    clock_t s_t_large = clock();
    int res_test_large = pyth_O_N_c(n_test_large);
    clock_t e_t_large = clock();
    printf("結果: %d, 時間: %.6f s\n", res_test_large, (double)(e_t_large-s_t_large)/CLOCKS_PER_SEC);
    
    // 参考: 元のO(N^2)で N=2000 を実行すると非常に時間がかかる
    // int n_test_orig_large = 2000;
    // printf("\n元のO(N^2) pyth_original(%d) の単一テスト (時間がかかります):\n", n_test_orig_large);
    // clock_t s_t_orig_large = clock();
    // int res_test_orig_large = pyth_original(n_test_orig_large);
    // clock_t e_t_orig_large = clock();
    // printf("結果: %d, 時間: %.6f s\n", res_test_orig_large, (double)(e_t_orig_large-s_t_orig_large)/CLOCKS_PER_SEC);

    return 0;
}