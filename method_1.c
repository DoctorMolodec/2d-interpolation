#include "method_1.h"

void interpolation_method_1(interpolation_ctx ctx)
{
    double d = 0, f_x1_x2, d_i, d_i1;
    d = (ctx->b - ctx->a) / (ctx->n - 1);

    for (int i = 0; i < ctx->n; i++)
    {
        ctx->P[i * 4 + 0] = ctx->f(ctx->k, ctx->a + d * i);

        if (i == ctx->n - 1 || i == 0)
        {
            if (i == ctx->n - 1)
            {
                ctx->P[4 * i + 1] = (ctx->f(ctx->k, ctx->b + d) - ctx->P[4 * (i - 1)]) / (2 * d);
            }
            else
            {
                ctx->P[1] = (ctx->f(ctx->k, ctx->a + d * (i + 1)) - ctx->f(ctx->k, ctx->a - d)) / (2 * d);
            }
        }
        else
        {
            ctx->P[4 * i + 1] = ctx->der_f(ctx->k, ctx->a + d * i);
        }
    }


    for (int i = 0; i < ctx->n - 1; ++i)
    {
        f_x1_x2 = (ctx->P[(i + 1) * 4 + 0] - ctx->P[i * 4 + 0]) / d;
        d_i = ctx->P[i * 4 + 1];
        d_i1 = ctx->P[(i + 1) * 4 + 1];
        ctx->P[i * 4 + 2] = (3 * f_x1_x2 - 2 * d_i - d_i1) / d;
        ctx->P[i * 4 + 3] = (d_i + d_i1 - 2 * f_x1_x2) / d / d;
    }

}

