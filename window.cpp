#include "window.h"
#include <algorithm>
#include <QPainter>
#include <iostream>
#include <string>

Canvas::Canvas(QWidget* parent, int method, int n, int k, double a, double b)
    : QWidget(parent), n(n), a(a), b(b), function_number(k), current_graph(method - 1),
    active(std::vector<std::vector<bool>>(5, std::vector<bool>(5, false))),
    ctx1(interpolation_create(1, n, function_number, a, b)),
    ctx2(interpolation_create(2, n, function_number, a, b))
{

    functions[0] = &Canvas::function;
    functions[1] = &Canvas::method1;
    functions[2] = &Canvas::method2;
    functions[3] = &Canvas::error1;
    functions[4] = &Canvas::error2;

    colors[0] = QColor(0, 0, 120);;
    colors[1] = QColor(120, 0, 0);
    colors[2] = QColor(0, 100, 0);
    colors[3] = QColor(0, 0, 100);
    colors[4] = QColor(0, 100, 0);

    for (int i = 0; i < 3; ++i)
        active[i][0] = true;
    for (int i = 0; i < 4; i += 2)
        active[i][1] = true;
    active[1][2] = true;
    active[2][2] = true;
    active[3][3] = true;
    active[4][4] = true;

    draw_f();
    update();
}

void Canvas::new_interpolations(int n, int function_number, double a, double b)
{
    interpolation_ctx t = interpolation_create(1, n, function_number, a, b);
    interpolation_destroy(ctx1);
    ctx1 = t;

    t = interpolation_create(2, n, function_number, a, b);
    interpolation_destroy(ctx2);
    ctx2 = t;
}

void Canvas::draw_f()
{
    s = 0;
    new_interpolations(n, function_number, a, b);
    a1 = a;
    b1 = b;
    switch (function_number)
    {
    case 0:
        function_name = "f(x) = 1";
        break;
    case 1:
        function_name = "f(x) = x";
        break;
    case 2:
        function_name = "f(x) = x^2";
        break;
    case 3:
        function_name = "f(x) = x^3";
        break;
    case 4:
        function_name = "f(x) = x^4";
        break;
    case 5:
        function_name = "f(x) = e^x";
        break;
    case 6:
        function_name = "f(x) = 1 / (25x^2 + 1)";
        break;
    }
    error_drop = 0;
    last_inf = 0;
    last_sup = 0;
    update();
}

void Canvas::next_f()
{
    function_number = (function_number + 1) % 7;
    draw_f();
}

void Canvas::next_graph()
{
    current_graph = (current_graph + 1) % 5;
    last_inf = 0;
    last_sup = 0;
    update();
}

void Canvas::increase_points()
{
    n = n * 2;
    new_interpolations(n, function_number, a, b);
    last_inf = inf;
    last_sup = sup;
    update();
}

void Canvas::decrease_points()
{
    if (n < 10)
        return;
    n = n / 2;
    new_interpolations(n, function_number, a, b);
    last_inf = inf;
    last_sup = sup;
    update();
}

double Canvas::function(double x)
{
    return ctx1->f(function_number, x);
}

double Canvas::method1(double x)
{
    double res;
    res = interpolation_calculate(ctx1, x);
    if (fabs(res) < 1e-15)
    {
        res = 0;
    }
    return res;
}

double Canvas::method2(double x)
{
    double res;
    res = interpolation_calculate(ctx2, x);
    if (fabs(res) < 1e-15)
    {
        res = 0;
    }
    return res;
}

double Canvas::error1(double x)
{
    double res;
    res = ctx1->f(function_number, x) - interpolation_calculate(ctx1, x);
    if (fabs(res) < 1e-15)
    {
        res = 0;
    }
    return res;
}

double Canvas::error2(double x)
{
    double res;
    res = ctx2->f(function_number, x) - interpolation_calculate(ctx2, x);
    if (fabs(res) < 1e-15)
    {
        res = 0;
    }
    return res;
}

double Canvas::h()
{
    return (b1 - a1) / (double)width();
}

void Canvas::paintEvent(QPaintEvent*)
{
    double y = 0;
    char s1[10];
    snprintf(s1, sizeof s1, "%d", s);
    inf = 0;
    sup = 0;
    for (int i = 0; i < 5; i++)
    {
        if (!active[current_graph][i])
            continue;
        for (double x = a1; x <= b1 + h(); x += h())
        {
            y = (this->*functions[i])(x);
            if (y >= sup)
            {
                sup = y;
            }
            if (y <= inf)
            {
                inf = y;
            }
        }
    }
    if ((sup < 1e-15) && (fabs(inf) < 1e-15))
    {
        sup = 10;
        inf = -10;
    }
    QPainter painter(this);
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(width() / (b1 - a1), -height() / (sup - inf) * 0.9);
    painter.translate(-(a1 + b1) / 2, -(inf + sup) / 2);


    QPen pen;
    pen.setWidth(0);
    pen.setColor("black");
    painter.setPen(pen);
    painter.drawLine(a1 - 1, 0.0, b1 + 1, 0.0);


    for (int j = 0; j < 5; ++j)
    {
        if (!active[current_graph][j])
            continue;

        pen.setColor(colors[j]);
        painter.setPen(pen);

        double x1 = a1;
        double y1 = (this->*functions[j])(x1);
        double x2, y2;

        while (x1 < b1 - h())
        {
            x2 = x1 + h();
            y2 = (this->*functions[j])(x2);
            painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
            x1 = x2;
            y1 = y2;
        }
        x2 = x1 + h();
        y2 = (this->*functions[j])(x2);
        painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
    }
    if (fmax(fabs(last_inf), fabs(last_sup)) > 1e-15)
    {
        error_drop = fmax(fabs(last_inf), fabs(last_sup)) / fmax(fabs(inf), fabs(sup));
        if ((error_drop < 1) && (error_drop > 1e-15))
        {
            error_drop = 1 / error_drop;
        }
    }
    else
    {
        error_drop = 0;
    }


    painter.restore();

    for (int i = 0; i < 5; i++)
    {
        if (!active[current_graph][i])
            continue;
        painter.setPen(colors[i]);
        painter.drawEllipse(200, 10 + i * 20, 10, 10);

    }



    painter.setPen(colors[0]);
    painter.drawText(20.0, 20.0, function_name);
    painter.setPen(colors[1]);
    painter.drawText(20.0, 40.0, "Hermite polynomials");
    painter.setPen(colors[2]);
    painter.drawText(20.0, 60.0, "Cubic splines");
    painter.setPen(colors[3]);
    painter.drawText(20.0, 80.0, "Error of hermite polynomials");
    painter.setPen(colors[4]);
    painter.drawText(20.0, 100.0, "Error of cubic splines");
    painter.setPen("black");
    painter.drawText(220.0, 80.0, "Scale:");
    painter.drawText(270.0, 80.0, s1);
    snprintf(s1, sizeof s1, "%lf", inf);
    painter.drawText(250.0, 20.0, s1);
    painter.drawText(220.0, 20.0, "inf:");
    snprintf(s1, sizeof s1, "%lf", sup);
    painter.drawText(360.0, 20.0, s1);
    painter.drawText(330.0, 20.0, "sup:");
    snprintf(s1, sizeof s1, "%d", n);
    painter.drawText(250.0, 40.0, s1);
    painter.drawText(220.0, 40.0, "n:");
    snprintf(s1, sizeof s1, "%lf", a1);
    painter.drawText(260.0, 60.0, s1);
    snprintf(s1, sizeof s1, "%lf", b1);
    painter.drawText(330.0, 60.0, s1);
    painter.drawText(220.0, 60.0, "[a;b]:");
    if ((current_graph == 3) || (current_graph == 4))
    {
        snprintf(s1, sizeof s1, "%lf", error_drop);
        painter.drawText(290.0, 100.0, s1);
        painter.drawText(220.0, 100.0, "Error drop");
    }
}

void Canvas::increase_scale()
{
    s = s + 1;
    double x, y;
    x = (a1 + b1) / 2 - (b1 - a1) / 4;
    y = (a1 + b1) / 2 + (b1 - a1) / 4;
    a1 = x;
    b1 = y;
    update();
}

void Canvas::decrease_scale()
{
    double x, y;
    s = s - 1;
    x = (a1 + b1) / 2 - (b1 - a1);
    y = (a1 + b1) / 2 + (b1 - a1);
    a1 = x;
    b1 = y;
    update();
}

Canvas::~Canvas()
{
    interpolation_destroy(ctx1);
    interpolation_destroy(ctx2);
}
