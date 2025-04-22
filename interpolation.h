#ifndef __INTERPOLATION_H
#define __INTERPOLATION_H

struct interpolation_ctx_inner;
typedef struct interpolation_ctx_inner* interpolation_ctx;

struct interpolation_ctx_inner {
	int	method;
	int	n;
	double	a, b;
	int k;
	double* P;
	double	(*der_f)(int, double);
	double	(*f)(int, double);
};


#ifdef __cplusplus
extern "C" {
#endif

	void interpolation_destroy(interpolation_ctx ctx);
	interpolation_ctx interpolation_create(int method, int n, int k, double a, double b);
	double interpolation_calculate(interpolation_ctx ctx, double x);
	double der_f(int k, double x);
	double f(int k, double x);

#ifdef __cplusplus
}
#endif

#endif

