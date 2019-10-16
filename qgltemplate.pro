HEADERS       = glwidget.h \
                window.h \
                mainwindow.h
SOURCES       = glwidget.cpp \
                main.cpp \
                window.cpp \
                mainwindow.cpp

QT           += widgets

# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/qgltemplate
INSTALLS += target

RESOURCES += \
    shaders.qrc