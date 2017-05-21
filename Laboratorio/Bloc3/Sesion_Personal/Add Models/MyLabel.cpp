#include "MyLabel.h"

MyLabel::MyLabel(QWidget *parent = 0):QLabel(parent)
{
  this->setAutoFillBackground(true);
  rd = gn = bl = 0;
  QString aux = QString("rgb(%1,%2,%3)").arg(rd).arg(gn).arg(bl);
  this->setStyleSheet("QLabel { background-color : "+aux+" }");
}

void MyLabel::updatered(int r)
{
  rd = r;
  this->setAutoFillBackground(true);
  QString aux = QString("rgb(%1,%2,%3)").arg(rd).arg(gn).arg(bl);
  this->setStyleSheet("QLabel { background-color : "+aux+" }");
}

void MyLabel::updategreen(int g)
{
  gn = g;
  this->setAutoFillBackground(true);
  QString aux = QString("rgb(%1,%2,%3)").arg(rd).arg(gn).arg(bl);
  this->setStyleSheet("QLabel { background-color : "+aux+" }");
}

void MyLabel::updateblue(int b)
{
  bl = b;
  this->setAutoFillBackground(true);
  QString aux = QString("rgb(%1,%2,%3)").arg(rd).arg(gn).arg(bl);
  this->setStyleSheet("QLabel { background-color : "+aux+" }");

}

void MyLabel::send()
{
  emit setred(rd);
  emit setgreen(gn);
  emit setblue(bl);
}
