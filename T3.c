#include <stdio.h>
#include <stdlib.h>

#define MOD 1000

int n, k, *a, **dp;

int main() {
    scanf("%d %d", &n, &k);
    a = (int *)malloc((n+1) * sizeof(int));
    dp = (int **)malloc((n+1) * sizeof(int *));
    for (int i = 0; i <= n; i++)
        dp[i] = (int *)malloc((k+1) * sizeof(int));
    for(int i = 0; i <= n; i++){
        for(int j = 0; j <= k; j++){
            dp[i][j] = 0;
        }
    }
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        dp[i][1] = 1;
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 2; j <= k; j++) {
            for (int k = 1; k < i; k++) {
                if (a[k] < a[i]) {
                    dp[i][j] = (dp[i][j] + dp[k][j - 1]) % MOD;
                }
            }
        }
    }
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        ans = (ans + dp[i][k]) % MOD;
    }
    printf("%d\n", ans);
    for (int i = 0; i <= n; i++)
        free(dp[i]);
    free(dp);
    free(a);
    return 0;
}
