HEADERS       = glwidget.h \
  shader.h \
  shaders/adsshader.h \
  shaders/basicshader.h \
                window.h \
                mainwindow.h \
    camera.h \
    renderobject.h \
    objects/cube.h \
    objects/floor.h \
    objects/lightsource.h \
    objects/sphere.h \
    shape.h \
    renderinfo.h
SOURCES       = glwidget.cpp \
                main.cpp \
  shader.cpp \
  shaders/adsshader.cpp \
  shaders/basicshader.cpp \
                window.cpp \
                mainwindow.cpp \
    camera.cpp \
    objects/cube.cpp \
    objects/floor.cpp \
    objects/lightsource.cpp \
    objects/sphere.cpp \
    shape.cpp \
    renderobject.cpp

QT           += widgets

# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/qgltemplate
INSTALLS += target

RESOURCES += \
    shaders.qrc \
    textures.qrc
