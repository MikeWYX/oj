#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef PI
#define PI 3.1415926
#endif

void fft(double complex *a, double complex *y, int n) {
    if (n == 1) {
        y[0] = a[0];
        return;
    }
    double complex w_n = cexp(2.0 * PI * I / (double)n);
    double complex w = 1;
    double complex *a0 = (double complex *)malloc((n / 2) * sizeof(double complex));
    double complex *a1 = (double complex *)malloc((n / 2) * sizeof(double complex));
    double complex *y0 = (double complex *)malloc((n / 2) * sizeof(double complex));
    double complex *y1 = (double complex *)malloc((n / 2) * sizeof(double complex));
    for (int i = 0; i < n / 2; i++) {
        a0[i] = a[2 * i];
        a1[i] = a[2 * i + 1];
    }
    fft(a0, y0, n / 2);
    fft(a1, y1, n / 2);
    for (int i = 0; i < n / 2; i++) {
        y[i] = y0[i] + w * y1[i];
        y[i + n / 2] = y0[i] - w * y1[i];
        w = w * w_n;
    }
    free(a0);
    free(a1);
    free(y0);
    free(y1);
}

void ifft(double complex *a, double complex *y, int n) {
    if (n == 1) {
        y[0] = a[0];
        return;
    }
    double complex w_n = cexp(-2.0 * PI * I / (double)n);
    double complex w = 1;
    double complex *a0 = (double complex *)malloc((n / 2) * sizeof(double complex));
    double complex *a1 = (double complex *)malloc((n / 2) * sizeof(double complex));
    double complex *y0 = (double complex *)malloc((n / 2) * sizeof(double complex));
    double complex *y1 = (double complex *)malloc((n / 2) * sizeof(double complex));
    for (int i = 0; i < n / 2; i++) {
        a0[i] = a[2 * i];
        a1[i] = a[2 * i + 1];
    }
    ifft(a0, y0, n / 2);
    ifft(a1, y1, n / 2);
    for (int i = 0; i < n / 2; i++) {
        y[i] = y0[i] + w * y1[i];
        y[i + n / 2] = y0[i] - w * y1[i];
        w = w * w_n;
    }
    free(a0);
    free(a1);
    free(y0);
    free(y1);
}

void multiply(int *f, int *g, int *h, int n) {
    int m = 1;
    while (m < 2 * n) {
        m *= 2;
    }
    double complex *a = (double complex *)malloc(m * sizeof(double complex));
    double complex *b = (double complex *)malloc(m * sizeof(double complex));
    double complex *c = (double complex *)malloc(m * sizeof(double complex));
    for (int i = 0; i < m; i++) {
        if (i < n) {
            a[i] = f[i];
            b[i] = g[i];
        } else {
            a[i] = 0;
            b[i] = 0;
        }
    }
    fft(a, c, m);
    fft(b, a, m);
    for (int i = 0; i < m; i++) {
        a[i] = c[i] * a[i];
    }
    ifft(a, c, m);
    for (int i = 0; i < 2 * n - 1; i++) {
        h[i] = (int)round(creal(c[i]) / (double)m);
    }
    free(a);
    free(b);
    free(c);
}

int main() {
    int i, n;
    scanf("%d", &n);
    int *f = (int *)malloc((n + 1) * sizeof(int));
    int *g = (int *)malloc((n + 1) * sizeof(int));
    for (int i = 0; i <= n; i++) {
        scanf("%d", &f[i]);
    }
    for (i = 0; i <= n; i++) {
        scanf("%d", &g[i]);
    }
    int *h = (int *)malloc((2 * n + 1) * sizeof(int));
    multiply(f, g, h, n + 1);
    for (int i = 0; i <= 2 * n; i++) {
        printf("%d\n", h[i]);
    }
    free(f);
    free(g);
    free(h);

    fclose(stdin);
    fclose(stdout);
    return 0;
}
