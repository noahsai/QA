#ifndef TIKU_H
#define TIKU_H

#include <QWidget>
#include<QStandardPaths>
#include<QDebug>
namespace Ui {
class tiku;
}

class tiku : public QWidget
{
    Q_OBJECT

public:
    explicit tiku(QWidget *parent = 0);
    ~tiku();

    void settiku(QStringList dan, QStringList duo , QStringList pan);

private slots:
    void on_fontsize_valueChanged(int arg1);

    void on_black_clicked(bool checked);

private:
    Ui::tiku *ui;
};

#endif // TIKU_H
