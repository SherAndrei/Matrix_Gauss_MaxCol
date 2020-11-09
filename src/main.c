/*
    Метод Гаусса с выбором максимального элемента по столбцу
    решения системы линейных уравнений
*/
#define _GNU_SOURCE
#include <sys/sysinfo.h>
#include <sched.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "matrix.h"
#include "error.h"
#include "print.h"
#include "fill.h"
#include "solve.h"

int main(int argc, const char* argv[]) {
    int errcode = 0;
    int n, m, r, s;
    // Solving AX = B
    // V1, V2, V3 вспомогательные матрицы
    double *A  = NULL, *B  = NULL, *X  = NULL,
           *V1 = NULL, *V2 = NULL, *V3 = NULL;
    double res;
    time_t start, end, start_solving, end_solving;
    // Чтобы зафиксировать выделение памяти только на одном процессоре
    cpu_set_t cpu;
    int nprocs = 0;
    nprocs = get_nprocs();
    CPU_ZERO(&cpu);
    CPU_SET(nprocs - 1, &cpu);
    sched_setaffinity(getpid(), sizeof(cpu), &cpu);

    printf(" Usage: [");
    for (int i = 1; i < argc - 1; i++)
        printf("%s, ", argv[i]);
    printf("%s]\n", argv[argc - 1]);

    if (!((argc == 5 || argc == 6) &&
        (sscanf(argv[1], "%d", &n) == 1) &&
        (sscanf(argv[2], "%d", &m) == 1) &&
        (sscanf(argv[3], "%d", &r) == 1) &&
        (sscanf(argv[4], "%d", &s) == 1) &&
        (n >= 0)           &&
        (m >  0 && m <= n) &&
        (r >= 0 && r <= n)))
            return error(1);

    // выделяем память под матрицу и под правый вектор
    A  = alloc_matrix(n * n), B  = alloc_matrix(n * 1), X  = alloc_matrix(n * 1);
    V1 = alloc_matrix(m * m), V2 = alloc_matrix(m * m), V3 = alloc_matrix(m * m);
    if (A  == NULL || B  == NULL || X  == NULL ||
        V1 == NULL || V2 == NULL || V3 == NULL) {
        free_matrix(A),  free_matrix(B),  free_matrix(X);
        free_matrix(V1), free_matrix(V2), free_matrix(V3);
        return error(2);
    }

    if (s == 0 && argc == 6)
        fill(A, n, m, 0, argv[5], &errcode);
    else if ((s > 0 && s < 5) && argc == 5)
        fill(A, n, m, s, NULL, NULL);
    else
        errcode = 1;

    if (errcode > 0) {
        free_matrix(A),  free_matrix(B),  free_matrix(X);
        free_matrix(V1), free_matrix(V2), free_matrix(V3);
        return error(errcode);
    }

    fill_right_part(A, B, n, m);

    print_matrix(A, n, n, m, r);
    print_matrix(B, n, 1, m, r);

    start_solving = clock();
    errcode = solve(n, m, A, B, X, V1, V2, V3);
    end_solving = clock();
    if (errcode < 0) {
        free_matrix(A),  free_matrix(B),  free_matrix(X);
        free_matrix(V1), free_matrix(V2), free_matrix(V3);
        return error(5);
    }
    printf(" Result:\n");
    print_matrix(X, n, 1, m, r);
    if (s == 0)
        fill(A, n, m, 0, argv[5], &errcode);
    else
        fill(A, n, m, s, NULL, NULL);
    if (errcode > 0) {
        free_matrix(A),  free_matrix(B),  free_matrix(X);
        free_matrix(V1), free_matrix(V2), free_matrix(V3);
        return error(errcode);
    }

    fill_right_part(A, B, n, m);

    start = clock();
    res = residual(A, B, X, n, m);
    end = clock();
    printf(" Difference: %10.3e\n", difference(X, n));
    printf(" Time computing residual: %6.3f sec\n\n", ((float)(end - start))/ CLOCKS_PER_SEC);
    printf("%s : residual = %e elapsed = %.2f for s = %d n = %d m = %d\n",
            argv[0], res, ((float)(end_solving - start_solving))/ CLOCKS_PER_SEC,
            s, n, m);

    free_matrix(A),  free_matrix(B),  free_matrix(X);
    free_matrix(V1), free_matrix(V2), free_matrix(V3);
    return 0;
}

#undef LOG
