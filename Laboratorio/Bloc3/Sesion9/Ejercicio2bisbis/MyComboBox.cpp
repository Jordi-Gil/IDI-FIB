#include "MyComboBox.h"

MyComboBox::MyComboBox(QWidget *parent = 0):QComboBox(parent)
{

}


void MyComboBox::setName(QString name)
{
  this->addItem(name);
}
