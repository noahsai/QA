#ifndef TEST_H
#define TEST_H

#include <QWidget>
#include<QTime>
#include<QTimer>
#include<QDebug>
#include<QMessageBox>
#include<QCloseEvent>
 #include <QStringList>

namespace Ui {
class test;
}

class test : public QWidget
{
    Q_OBJECT

public:
    explicit test( QStringList* dan ,  int dannum ,  QStringList* duo , int duonum , QStringList* pan , int pannum , int min, QWidget *parent = 0);
    explicit test( QWidget *parent = 0);
    ~test();

    void setcuoti(QList<int>* dan,QList<int>* duo ,QList<int>* pan);
    void start();


private slots:
    void on_xuanti_editingFinished();

    void on_pre_clicked();

    void on_next_clicked();

    void on_submit_clicked();

    void timeout();
    void saveanswer();
    void saveanswer2();


signals:
    void finish(int cuo , int all ,QList<int> dan, QList<int> duo ,QList<int> pan);

private:
    void closeEvent(QCloseEvent *event);

    Ui::test *ui;
    void rand();
    void refresh(int tinum , bool first = true);
    void settype1(  int t ,QString a, QString b , QString c , QString d , QString choose );
    void settype2(  int t ,QString a, QString b , QString c , QString d , QString choose );
    void initanswer();
    int now , all ,did;
    QStringList answer , youranswer;//记得清空

    QList<int> danrand;
    QList<int> duorand;
    QList<int> panrand;//记得清空

     QStringList* dan ;
     QStringList* pan ;
     QStringList* duo;
    int dannum, duonum,pannum;
    QList<int> dancuo , duocuo , pancuo ;
    QList<int> *dancuoall , *duocuoall , *pancuoall ;

    QTimer *timer;
    int deadline ;
};

#endif // TEST_H
