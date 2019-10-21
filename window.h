#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class QSlider;
class QPushButton;
class GLWidget;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent);

public slots:
    void onSliderChange();

private:
    QSlider *createSlider();

    GLWidget *glWidget;
    QSlider *xSlider;
    QSlider *ySlider;
    QSlider *zSlider;
};

#endif
