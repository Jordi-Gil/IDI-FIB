#include "MyForm.h"

MyForm::MyForm (QWidget* parent) : QWidget(parent)
{
  this->setStyleSheet("background-color: #D1DBDD;");
  ui.setupUi(this);
}
