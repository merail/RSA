#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<BigIntegerLibrary.hh>
#include <QDebug>
#include <ctime>
#include <random>
#include <cstdlib>
#include <QMessageBox>
#include <QTime>
#include <QBitArray>
#include <QGraphicsScene>
#include <QFile>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void generatePrime();

    BigInteger pow(BigInteger x, BigInteger d, BigInteger n);

    void on_genKeysPushButton_clicked();

    BigInteger calcNODByEucl(BigInteger a, BigInteger b);

    BigInteger calcNODByExtEucl(BigInteger m, BigInteger n);

    void on_createKeysPushButton_clicked();

    void on_codePushButton_clicked();

    void on_decodePushButton_clicked();


    void on_atackPushButton_clicked();

    void on_testPushButton_clicked();

    void attack();

private:
    Ui::MainWindow *ui;

    int L=128;
    BigInteger prime;
    BigInteger degree;
    int * primeArray;
    BigInteger mod=0;
    int k=0;
    BigInteger primeP, primeQ;
    BigInteger encModule;
    BigInteger eulerFunction;
    BigInteger modByEucl;
    BigInteger exponenta;

    QMessageBox msgBox;

    BigInteger d;

    QByteArray textByteArray;
    QBitArray textBitArray;
    int textLength;
    BigInteger M, code;

    int *x;

    BigInteger modForRo;

    BigInteger modForM, divForM;

    QGraphicsScene *scene;
    QPen pen;

    int *lengthes;
    int *times;
    int lengthForAttack;

    double r=0.5;

    QTime start;

    int primeLength;
    double *rArray;
};

#endif // MAINWINDOW_H
