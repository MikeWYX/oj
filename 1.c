#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct information
{
    int num;
    char name[10];
    int grade1;
    int grade2;
} information;

void numsort(information *interviewee, int l, int r){
    int i = l, j = r;
    information temp, pivot;
    if(l >= r){
        return;
    }
    pivot = interviewee[(rand() % (r - l + 1)) + l];
    while(i <= j){
        while(interviewee[i].num - pivot.num < 0){
            i++;
        }
        while(interviewee[j].num - pivot.num > 0){
            j--;
        }
        if(i <= j){
            temp = interviewee[i];
            interviewee[i] = interviewee[j];
            interviewee[j] = temp;
            i++;
            j--;
        }
    }
    numsort(interviewee, l, j);
    numsort(interviewee, i, r);
}

void namesort(information *interviewee, int l, int r){
    int i = l, j = r;
    information temp, pivot;
    if(l >= r){
        return;
    }
    pivot = interviewee[(rand() % (r - l + 1)) + l];
    while(i <= j){
        while(strcmp(interviewee[i].name, pivot.name) < 0){
            i++;
        }
        while(strcmp(interviewee[j].name, pivot.name) > 0){
            j--;
        }
        if(i <= j){
            temp = interviewee[i];
            interviewee[i] = interviewee[j];
            interviewee[j] = temp;
            i++;
            j--;
        }
    }
    namesort(interviewee, l, j);
    namesort(interviewee, i, r);
}

void gradesort(information *interviewee, int l, int r){
    int i = l, j = r;
    information temp, pivot;
    if(l >= r){
        return;
    }
    pivot = interviewee[(rand() % (r - l + 1)) + l];
    while(i <= j){
        while(interviewee[i].grade1 - pivot.grade1 < 0){
            i++;
        }
        while(interviewee[j].grade1 - pivot.grade1 > 0){
            j--;
        }
        if(i <= j){
            temp = interviewee[i];
            interviewee[i] = interviewee[j];
            interviewee[j] = temp;
            i++;
            j--;
        }
    }
    gradesort(interviewee, l, j);
    gradesort(interviewee, i, r);
}

void sumsort(information *interviewee, int l, int r){
    int i = l, j = r;
    information temp, pivot;
    if(l >= r){
        return;
    }
    pivot = interviewee[(rand() % (r - l + 1)) + l];
    while(i <= j){
        while((interviewee[i].grade1 + interviewee[i].grade2 - pivot.grade1 - pivot.grade2) < 0){
            i++;
        }
        while((interviewee[j].grade1 + interviewee[j].grade2 - pivot.grade1 - pivot.grade2) > 0){
            j--;
        }
        if(i <= j){
            temp = interviewee[i];
            interviewee[i] = interviewee[j];
            interviewee[j] = temp;
            i++;
            j--;
        }
    }
    sumsort(interviewee, l, j);
    sumsort(interviewee, i, r);
}



int main(){
    int i, n;
    scanf("%d", &n);
    srand((unsigned)time(NULL));
    information *interviewee = (information *)malloc(n * sizeof(information));
    if (interviewee == NULL) {
        return 1;
    }

    for(i = 0;  i < n;  i++){
        scanf("%d %s %d %d", &interviewee[i].num, interviewee[i].name, &interviewee[i].grade1, &interviewee[i].grade2);
    }
    numsort(interviewee, 0, n-1);
    namesort(interviewee, 0, n-1);
    gradesort(interviewee, 0, n-1);
    sumsort(interviewee, 0, n-1);
    for(i = 0;  i < n;  i++){
        printf("%06d\n",interviewee[i].num);
    }
    free(interviewee);
    return 0;
}
