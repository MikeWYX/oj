#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

typedef struct {
    double x, y;
} Point;

int compare_x(const void *a, const void *b) {
    Point *p1 = (Point *)a;
    Point *p2 = (Point *)b;
    if (p1->x < p2->x) return -1;
    if (p1->x > p2->x) return 1;
    return 0;
}

int compare_y(const void *a, const void *b) {
    Point *p1 = (Point *)a;
    Point *p2 = (Point *)b;
    if (p1->y < p2->y) return -1;
    if (p1->y > p2->y) return 1;
    return 0;
}

double distance(Point p1, Point p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

double closest_pair_recursive(Point points_x[], Point points_y[], int n) {
    if (n <= 5) {
        double min_distance = DBL_MAX;
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                double dist = distance(points_x[i], points_x[j]);
                if (dist < min_distance) {
                    min_distance = dist;
                }
            }
        }
        return min_distance;
    }

    int mid = n / 2;
    Point mid_point = points_x[mid];

    Point points_y_left[mid];
    Point points_y_right[n - mid];
    int left_index = 0, right_index = 0;
    for (int i = 0; i < n; i++) {
        if (points_y[i].x < mid_point.x) {
            points_y_left[left_index++] = points_y[i];
        } else {
            points_y_right[right_index++] = points_y[i];
        }
    }

    double left_distance = closest_pair_recursive(points_x, points_y_left, mid);
    double right_distance = closest_pair_recursive(points_x + mid, points_y_right, n - mid);
    double min_distance = left_distance < right_distance ? left_distance : right_distance;

    Point strip[n];
    int strip_size = 0;
    for (int i = 0; i < n; ++i) {
        if (fabs(points_y[i].x - mid_point.x) < min_distance) {
            strip[strip_size++] = points_y[i];
        }
    }

    for (int i = 0; i < strip_size; ++i) {
        for (int j = 1; j <= 5; j++) {
            if(i + j < strip_size){
                double dist = distance(strip[i], strip[i+j]);
                if (dist < min_distance) {
                    min_distance = dist;
                }
            }
        }
    }

    return min_distance;
}

double closest_pair(Point points[], int n) {
    Point points_y[n];
    for (int i = 0; i < n; ++i) {
        points_y[i] = points[i];
    }
    quick_sort(points, 0, n - 1, compare_x);
    quick_sort(points_y, 0, n - 1, compare_y);
    return closest_pair_recursive(points, points_y, n);
}

double read_double() {
    double result = 0.0;
    double fraction = 1.0;
    char ch = getchar();
    while (ch < '0' || ch > '9') {
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        result = result * 10 + (ch - '0');
        ch = getchar();
    }
    if (ch == '.') {
        ch = getchar();
        while (ch >= '0' && ch <= '9') {
            fraction /= 10;
            result += (ch - '0') * fraction;
            ch = getchar();
        }
    }
    return result;
}

void swap(Point *a, Point *b) {
    Point temp = *a;
    *a = *b;
    *b = temp;
}

int partition(Point arr[], int low, int high, int (*compare)(const void *, const void *)) {
    Point pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (compare(&arr[j], &pivot) < 0) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quick_sort(Point arr[], int low, int high, int (*compare)(const void *, const void *)) {
    if (low < high) {
        int pi = partition(arr, low, high, compare);

        quick_sort(arr, low, pi - 1, compare);
        quick_sort(arr, pi + 1, high, compare);
    }
}

int main() {
    int n = 0;
    char ch = getchar();
    while (ch < '0' || ch > '9') {
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        n = n * 10 + ch - '0';
        ch = getchar();
    }
    Point points[n];
    for (int i = 0; i < n; ++i) {
        points[i].x = read_double();
        points[i].y = read_double();
    }
    double min_distance = closest_pair(points, n);
    printf("%.6lf\n", min_distance);
    return 0;
}