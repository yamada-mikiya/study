#include <stdio.h>

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

// main関数
int main(void) {
    int n;
    printf("nを入力してください: ");
    scanf("%d", &n);

    int result = L_mult_R(n);
    printf("条件を満たす数の個数: %d\n", result);

    return 0;
}
