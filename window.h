#include <cmath>
#include <QWidget>
#include "interpolation.h"

class Canvas : public QWidget
{
    Q_OBJECT
public:
    int n;
    double a, b;
    double a1;
    double b1;
    int function_number;
    int current_graph;
    std::vector<std::vector<bool>> active;
    interpolation_ctx ctx1, ctx2;
    const char* function_name;
    double sup, inf;
    int s;
    double last_sup, last_inf;
    double error_drop;

    double (Canvas::* functions[5])(double);
    QColor colors[5];

    double h();

    Canvas(QWidget* parent, int method, int n, int k, double a, double b);

    double function(double x);

    double method1(double x);

    double method2(double x);

    double error1(double x);

    double error2(double x);

    void paintEvent(QPaintEvent* event);

    ~Canvas();

public slots:
    void new_interpolations(int n, int function_number, double a, double b);

    void draw_f();

    void next_graph();

    void next_f();

    void decrease_scale();

    void increase_scale();

    void increase_points();

    void decrease_points();
};
