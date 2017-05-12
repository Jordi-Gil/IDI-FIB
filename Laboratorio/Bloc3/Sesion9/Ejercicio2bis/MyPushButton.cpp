#include "MyPushButton.h"

MyPushButton::MyPushButton(QWidget *parent = 0):QPushButton(parent)
{

}

void MyPushButton::myClicked()
{
    filename = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", "Music Files (*.mp3)");

    QMessageBox::information(this, tr("File Name"), filename);

    if(!objfile.isEmpty()) emit setFile(filename);
}

void MyPushButton::myClicked2()
{
  objfile = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", "OBJ Files (*.obj)");

  QMessageBox::information(this, tr("File Name"), objfile);

  if(!objfile.isEmpty()){

    bool ok;
    QString name = QInputDialog::getText(this, tr("Afegir nom"), tr("Model name:"), QLineEdit::Normal, QDir::home().dirName(), &ok);

    if(ok){
      if(!name.isEmpty()){
        emit setOBJ(objfile);
        emit sendName(name);
      }
      else{
        QMessageBox::information(this, tr("ERROR"), "Modelo no añadido, inserte un nombre");
      }
    }
  }
}
