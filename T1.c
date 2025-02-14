#include <stdio.h>
#include <stdlib.h>

int main(){
    int n, k, i, j;
    int temp_L = 0, min = -1;
    scanf("%d%d", &n, &k);
    int *a = (int *)malloc(n * sizeof(int));
    for(int i = 0; i < n; i++){
        scanf("%d", &a[i]);
    }
    for(i = 1; i < n; i++){
        a[i] += a[i-1];
    }
    for(i = 0; i < n; i++){
        if(a[i] < k)
            continue;
        while (temp_L < i && a[i] - a[temp_L] >= k) {
            temp_L++;
        }
        if (min == -1 || i - temp_L + 1 < min)
            min = i - temp_L + 1;
    }
    if(min == -1){
        min = 0;
    }
    printf("%d", min);
}
