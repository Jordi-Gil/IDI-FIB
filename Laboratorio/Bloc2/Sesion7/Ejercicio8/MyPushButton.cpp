#include "MyPushButton.h"

MyPushButton::MyPushButton(QWidget *parent = 0):QPushButton(parent)
{

}

void MyPushButton::myClicked()
{
    filename = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", "Music Files (*.mp3)");

    QMessageBox::information(this, tr("File Name"), filename);

    emit setFile(filename);
}
