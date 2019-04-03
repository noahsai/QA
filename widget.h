#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QFile>
#include<QDebug>
#include<QDataStream>
#include<QStandardPaths>
#include<QSettings>
#include<QDir>
#include<test.h>
#include<tiku.h>
#include<QSettings>
#include<QFileDialog>
#include<QProcess>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_test_clicked();


    void tested(int cuo , int all ,QList<int> dan, QList<int> duo ,QList<int> pan);

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void tikuclose(QObject*);

    void on_xuantiku_activated(int index);

private:
    Ui::Widget *ui;
    test *t;
    bool readtiku(QString&);
    void reset();
    void savetiku();
    void savecuoti();
    void savecfg();
    void readcfg();
    void inittiku();

    QString path ,nowtiku;
    QStringList danlist,duolist,panlist;
    QList<int>  dancuoti,duocuoti,pancuoti;
    int now;
    bool test_or_cuoti;//考试为真，做错题为假
    tiku *ku,*cuotiji;
};

#endif // WIDGET_H
