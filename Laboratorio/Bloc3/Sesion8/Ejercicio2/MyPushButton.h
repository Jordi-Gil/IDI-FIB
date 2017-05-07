#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>

class MyPushButton: public QPushButton
{
    Q_OBJECT

  public:

    MyPushButton(QWidget *parent);


  public slots:

    void myClicked();

  signals:
    void setFile(QString str);

  private:

    QString filename;
};
