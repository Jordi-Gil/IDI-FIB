#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>

class MyPushButton: public QPushButton
{
    Q_OBJECT

  public:

    MyPushButton(QWidget *parent);


  public slots:

    void myClicked();
    void myClicked2();

  signals:
    void setFile(QString str);
    void setOBJ(QString str);
    void sendName(QString str);

  private:

    QString filename;
    QString objfile;
};
