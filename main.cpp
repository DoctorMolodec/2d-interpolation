#include <iostream>
#include <QMainWindow>
#include <QApplication>
#include <QMenuBar>
#include "window.h"

int main(int argc, char* argv[])
{
    double a, b;
    int method, n, k;

    if (argc < 6)
    {
        std::cout << "Not enough arguments" << std::endl;
        return -1;
    }
    else
        if (argc > 6)
        {
            std::cout << "Åxceeding the number of arguments" << std::endl;
            return -1;
        }

    if (sscanf(argv[1], "%d", &method) != 1)
    {
        std::cout << "Error in the data type in the method " << std::endl;
        return -1;
    }
    else
        if (method != 1 && method != 2)
        {
            std::cout << "Unknown method" << std::endl;
            return -1;
        }

    if (sscanf(argv[2], "%d", &n) != 1)
    {
        std::cout << "Error in the data type in the amount of points" << std::endl;
        return -1;
    }
    else
        if (n < 5)
        {
            std::cout << "Not enough amount of points" << std::endl;
            return -1;
        }

    if (sscanf(argv[3], "%d", &k) != 1)
    {
        std::cout << "Error in the data type in the function" << std::endl;
        return -1;
    }
    else
        if (k < 0 || k > 6)
        {
            std::cout << "Invalid function" << std::endl;
            return -1;
        }

    if (sscanf(argv[4], "%lf", &a) != 1 || sscanf(argv[5], "%lf", &b) != 1)
    {
        std::cout << "Error in the data type in the segment" << std::endl;
        return -1;
    }
    else
        if (a > b || b - a > 1e9 || b - a < 1e-5)
        {
            std::cout << "Incorrect segment" << std::endl;
            return -1;
        }

    QApplication app(argc, argv);
    QMainWindow* window = new QMainWindow;
    window->resize(1100, 800);
    QMenuBar* bar = new QMenuBar(window);
    Canvas* canvas;

    canvas = new Canvas(window, method, n, k, a, b);

    QAction* action = bar->addAction("Next function", canvas, SLOT(next_f()));
    action->setShortcut(QString("0"));

    action = bar->addAction("Next graphics", canvas, SLOT(next_graph()));
    action->setShortcut(QString("1"));

    action = bar->addAction("Increase scale", canvas, SLOT(increase_scale()));
    action->setShortcut(QString("2"));

    action = bar->addAction("Decrease scale", canvas, SLOT(decrease_scale()));
    action->setShortcut(QString("3"));

    action = bar->addAction("Increase amount of points", canvas, SLOT(increase_points()));
    action->setShortcut(QString("4"));

    action = bar->addAction("Decrease amount of points", canvas, SLOT(decrease_points()));
    action->setShortcut(QString("5"));

    window->setMenuBar(bar);
    window->setCentralWidget(canvas);
    window->show();
    app.exec();

    delete canvas;
    delete bar;
    delete window;

    return 0;
}
