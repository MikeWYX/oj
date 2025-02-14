#include <stdio.h>
#include <string.h>

#define MAXN 100000
#define MAXK 100
#define MOD 1000

int n, k, a[MAXN], dp[MAXK], bit[MAXK][MAXN], rk[MAXN], tot;

int lowbit(int x) {
    return x & -x;
}

void update(int i, int j, int x) {
    for (; j <= tot; j += lowbit(j))
        bit[i][j] = (bit[i][j] + x) % MOD;
}

int query(int i, int j) {
    int sum = 0;
    for (; j; j -= lowbit(j))
        sum = (sum + bit[i][j]) % MOD;
    return sum;
}

int lower_bound(int *arr, int size, int val) {
    int l = 0, r = size;
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (arr[mid] < val) {
            l = mid + 1;
        } else {
            r = mid;
        }
    }
    return l + 1;
}

int cmp(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

void discrete() {
    memcpy(rk, a, sizeof(int) * (n+1));
    qsort(rk + 1, n, sizeof(int), cmp);

    tot = 1;
    for (int i = 2; i <= n; ++i) {
        if (rk[i] != rk[tot]) {
            rk[++tot] = rk[i];
        }
    }

    for (int i = 1; i <= n; ++i) {
        a[i] = (int)(lower_bound(rk + 1, rk + tot + 1, a[i]) - rk);
    }
}


int main() {
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &a[i]);
    }
    
    discrete();

    for (int i = 1; i <= n; ++i) {
        for (int j = k; j; --j) {
            int val = j == 1 ? 1 : query(j - 1, a[i] - 1);
            if (val) {
                update(j, a[i], val);
                dp[j] = (dp[j] + val) % MOD;
            }
        }
    }

    printf("%d\n", dp[k]);
    return 0;
}
