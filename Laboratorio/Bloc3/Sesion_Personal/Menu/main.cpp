#include <QApplication>
#include "MainWindow.h"

int main (int argc, char **argv)
{
  QApplication a( argc, argv);
  QSurfaceFormat f;
  f.setVersion(3,3);
  f.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(f);

  MainWindow mw;
  mw.show();

  return a.exec ();
}
