#include <QComboBox>

class MyComboBox: public QComboBox
{
    Q_OBJECT

  public:

    MyComboBox(QWidget *parent);


  public slots:

    void setName(QString name);

};
