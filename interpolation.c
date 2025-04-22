#include "interpolation.h"
#include "method_1.h"
#include "method_2.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double f(int k, double x)
{
    double res = 0;
    if (k == 0)
    {
        res = 1;
    }
    if (k == 1)
    {
        res = x;
    }
    if (k == 2)
    {
        res = x * x;
    }
    if (k == 3)
    {
        res = x * x * x;
    }
    if (k == 4)
    {
        res = x * x * x * x;;
    }
    if (k == 5)
    {
        res = exp(x);
    }
    if (k == 6)
    {
        res = 1 / (25 * x * x + 1);
    }
    return res;
}

double der_f(int k, double x)
{
    double res = 0;
    if (k == 0)
    {
        res = 0;
    }
    if (k == 1)
    {
        res = 1;
    }
    if (k == 2)
    {
        res = 2 * x;
    }
    if (k == 3)
    {
        res = 3 * x * x;
    }
    if (k == 4)
    {
        res = 4 * x * x * x;;
    }
    if (k == 5)
    {
        res = exp(x);
    }
    if (k == 6)
    {
        res = -(50 * x) / pow(25 * x * x + 1, 2);
    }
    return res;
}

double interpolation_calculate(interpolation_ctx ctx, double x)
{
    double d = 0;
    int i = 0;
    d = (ctx->b - ctx->a) / (ctx->n - 1);

    i = (int)((x - ctx->a) / d);
    if (i > ctx->n - 2)
    {
        i = ctx->n - 2;
    }
    if (i < 0)
    {
        i = 0;
    }
    x = x - ctx->a - d * i;
    return ctx->P[i * 4] + ctx->P[i * 4 + 1] * x + ctx->P[i * 4 + 2] * x * x + ctx->P[i * 4 + 3] * x * x * x;
}

void interpolation_destroy(interpolation_ctx ctx)
{
    free(ctx->P);
    free(ctx);
}

interpolation_ctx interpolation_create(int method, int n, int k, double a, double b)
{
    interpolation_ctx res_ptr;

    if ((method > 2) || (method < 1))
    {
        return NULL;
    }
    if (n <= 4)
    {
        return NULL;
    }
    if (b <= a)
    {
        return NULL;
    }
    if ((k < 0) || (k > 6))
    {
        return NULL;
    }
    res_ptr = (interpolation_ctx)malloc(sizeof(*res_ptr));

    res_ptr->method = method;
    res_ptr->n = n;
    res_ptr->a = a;
    res_ptr->b = b;
    res_ptr->k = k;
    res_ptr->f = f;
    res_ptr->der_f = der_f;
    res_ptr->P = (double*)malloc(4 * n * sizeof(double));

    if (method == 1)
    {
        interpolation_method_1(res_ptr);
    }
    if (method == 2)
    {
        interpolation_method_2(res_ptr);
    }

    return res_ptr;
}
