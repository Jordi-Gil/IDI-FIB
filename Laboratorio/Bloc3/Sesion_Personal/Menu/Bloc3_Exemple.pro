TEMPLATE    = app
QT         += opengl multimedia multimediawidgets

INCLUDEPATH +=  /usr/include/glm
INCLUDEPATH += ../../Model

FORMS += MainWindow.ui

HEADERS += MainWindow.h MyGLWidget.h ../../Model/model.h MyLabel.h MyPushButton.h MyComboBox.h

SOURCES += main.cpp MainWindow.cpp MyGLWidget.cpp ../../Model/model.cpp MyLabel.cpp MyPushButton.cpp MyComboBox.cpp
