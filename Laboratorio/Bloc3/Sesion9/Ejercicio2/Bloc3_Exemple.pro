TEMPLATE    = app
QT         += opengl multimedia multimediawidgets

INCLUDEPATH +=  /usr/include/glm
INCLUDEPATH += ../../Model

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h ../../Model/model.h MyLabel.h MyPushButton.h

SOURCES += main.cpp MyForm.cpp \
        MyGLWidget.cpp ../../Model/model.cpp MyLabel.cpp MyPushButton.cpp
