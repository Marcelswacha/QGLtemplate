#include "glwidget.h"
#include "window.h"

#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>

Window::Window(QWidget* parent)
    : QWidget(parent)
{
    glWidget = new GLWidget(this);

    xSlider = createSlider();
    ySlider = createSlider();
    zSlider = createSlider();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *container = new QHBoxLayout;
    container->addWidget(glWidget);
    container->addWidget(xSlider);
    container->addWidget(ySlider);
    container->addWidget(zSlider);

    connect(xSlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderChange()));
    connect(ySlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderChange()));
    connect(zSlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderChange()));

    QWidget *w = new QWidget;
    w->setLayout(container);
    mainLayout->addWidget(w);

    setLayout(mainLayout);

    setWindowTitle(tr("Balls"));
}

void Window::onSliderChange()
{
    float r = xSlider->value() / 255.0;
    float g = ySlider->value() / 255.0;
    float b = zSlider->value() / 255.0;

    glWidget->setLightColor(r, g, b);
}

QSlider *Window::createSlider()
{
    QSlider *slider = new QSlider(Qt::Vertical, this);
    slider->setRange(0, 255);
    slider->setSingleStep(1);
    slider->setPageStep(32);
    slider->setValue(255);

    return slider;
}
