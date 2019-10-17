HEADERS       = glwidget.h \
                window.h \
                mainwindow.h \
    camera.h \
    renderobject.h \
    objects/cube.h \
    objects/floor.h
SOURCES       = glwidget.cpp \
                main.cpp \
                window.cpp \
                mainwindow.cpp \
    camera.cpp \
    objects/cube.cpp \
    objects/floor.cpp

QT           += widgets

# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/qgltemplate
INSTALLS += target

RESOURCES += \
    shaders.qrc \
    textures.qrc
