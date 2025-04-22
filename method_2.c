#include "method_2.h"

double der2_f(int k, double x);

void interpolation_method_2(interpolation_ctx ctx)
{
    double d = 0, f_x1_x2, d_i, d_i1;
    d = (ctx->b - ctx->a) / (ctx->n - 1);




    ctx->P[4 * 0 + 0] = 0;
    ctx->P[4 * 0 + 1] = 2;
    ctx->P[4 * 0 + 3] = 1;
    ctx->P[4 * 0 + 2] = 3 * (ctx->f(ctx->k, ctx->a + d) - ctx->f(ctx->k, ctx->a)) / d - d * der2_f(ctx->k, ctx->a) / 2;
    for (int i = 1; i < ctx->n - 1; i++)
    {
        ctx->P[4 * i + 0] = d;
        ctx->P[4 * i + 1] = 4 * d;
        ctx->P[4 * i + 3] = d;
        ctx->P[4 * i + 2] = 3 * (ctx->f(ctx->k, ctx->a + d * (i + 1)) - ctx->f(ctx->k, ctx->a + d * (i - 1)));
    }
    ctx->P[4 * (ctx->n - 1) + 0] = 1;
    ctx->P[4 * (ctx->n - 1) + 1] = 2;
    ctx->P[4 * (ctx->n - 1) + 3] = 0;
    ctx->P[4 * (ctx->n - 1) + 2] = 3 * (ctx->f(ctx->k, ctx->b) - ctx->f(ctx->k, ctx->b - d)) / d + d * der2_f(ctx->k, ctx->b) / 2;



    for (int i = 0; i < ctx->n - 1; i++)
    {
        ctx->P[4 * (i + 1) + 1] = ctx->P[4 * (i + 1) + 1] - ctx->P[4 * i + 3] * ctx->P[4 * (i + 1) + 0] / ctx->P[4 * i + 1];
        ctx->P[4 * (i + 1) + 2] = ctx->P[4 * (i + 1) + 2] - ctx->P[4 * (i + 1) + 0] / ctx->P[4 * i + 1] * ctx->P[4 * i + 2];
        ctx->P[4 * (i + 1)] = 0;
    }

    ctx->P[4 * (ctx->n - 1) + 2] = ctx->P[4 * (ctx->n - 1) + 2] / ctx->P[4 * (ctx->n - 1) + 1];
    ctx->P[4 * (ctx->n - 1) + 1] = 1;
    for (int i = ctx->n - 1; i > 0; i--)
    {
        ctx->P[4 * (i - 1) + 2] = ctx->P[4 * (i - 1) + 2] - ctx->P[4 * (i - 1) + 3] * ctx->P[4 * i + 2];
        ctx->P[4 * (i - 1) + 3] = 0;
        ctx->P[4 * (i - 1) + 2] = ctx->P[4 * (i - 1) + 2] / ctx->P[4 * (i - 1) + 1];
        ctx->P[4 * (i - 1) + 1] = 1;
    }
    ctx->P[4 * 0 + 2] = ctx->P[4 * 0 + 2] / ctx->P[4 * 0 + 1];


    for (int i = 0; i < ctx->n - 1; ++i)
    {
        f_x1_x2 = (ctx->f(ctx->k, ctx->a + d * (i + 1)) - ctx->f(ctx->k, ctx->a + d * i)) / d;
        d_i = ctx->P[i * 4 + 2];
        d_i1 = ctx->P[(i + 1) * 4 + 2];

        ctx->P[i * 4 + 0] = ctx->f(ctx->k, ctx->a + d * i);
        ctx->P[i * 4 + 1] = ctx->P[i * 4 + 2];

        ctx->P[i * 4 + 2] = (3 * f_x1_x2 - 2 * d_i - d_i1) / d;
        ctx->P[i * 4 + 3] = (d_i + d_i1 - 2 * f_x1_x2) / d / d;
    }



}

double der2_f(int k, double x)
{
    double res = 0;
    if (k == 0)
    {
        res = 0;
    }
    if (k == 1)
    {
        res = 0;
    }
    if (k == 2)
    {
        res = 2;
    }
    if (k == 3)
    {
        res = 6 * x;
    }
    if (k == 4)
    {
        res = 12 * x * x;;
    }
    if (k == 5)
    {
        res = exp(x);
    }
    if (k == 6)
    {
        res = -(50 * (1 - 75 * x * x)) / pow(25 * x * x + 1, 3);
    }
    return res;
}
