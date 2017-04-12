#include <QLabel>

class MyLabel: public QLabel
{
    Q_OBJECT

  public:

    MyLabel(QWidget *parent);


  public slots:

    void updatered(int r);
    void updategreen(int g);
    void updateblue(int b);
    void send();

  signals:
    void setred(int r);
    void setgreen(int g);
    void setblue(int b);

  private:

    int rd, gn, bl;
};
