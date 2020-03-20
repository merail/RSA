#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    srand(time(NULL));

    ui->keyLengthComboBox->addItem("128");
    ui->keyLengthComboBox->addItem("256");
    ui->keyLengthComboBox->addItem("512");
    ui->keyLengthComboBox->addItem("1024");

    scene = new QGraphicsScene(ui->graphicsView);
    pen.setColor(Qt::black);
    scene->addLine(11,290,380,290,pen);
    scene->addLine(365,300,380,290,pen);
    scene->addLine(365,280,380,290,pen);
    scene->addLine(11,0,21,15,pen);
    scene->addLine(11,0,1,15,pen);
    scene->addLine(11,0,11,290,pen);
    ui->graphicsView->setScene(scene);

    rArray=new double[6];
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generatePrime()
{
    if(L>=128)
        L=ui->keyLengthComboBox->currentText().toInt();
    primeLength=L*r;
    primeArray=new int[primeLength];
    prime=1;
    primeArray[primeLength-1]=1;
    primeArray[0]=1;
    for(int i=1;i<primeLength;i++)
        prime=prime*2;
    prime=prime+1;
    for(int i=1;i<primeLength-1;i++)
    {
        primeArray[i]=rand()%2;
            //qDebug()<<primeArray[i];
        if(primeArray[i]==1)
        {
            degree=1;
            for(int j=0;j<i;j++)
                degree=degree*2;
            prime=prime + degree;
        }
    }
        //qDebug()<<primeArray[L/2-1];
}

BigInteger MainWindow::pow(BigInteger x, BigInteger d, BigInteger n)
{
    BigInteger y=1;
    while(d>0)
    {
        if(d%2!=0)
            y=(y*x)%n;
        d=d/2;
        x=(x*x)%n;
    }
    return y;
}

void MainWindow::on_genKeysPushButton_clicked()
{
    k=0;
    while(k<10)
    {
        k=0;
        generatePrime();
        for(int i=0;i<10;i++)
        {
            BigInteger x=rand()%100;
            mod=pow(x,prime-1,prime);
            if(mod==1)
                k++;
        }
    }
    primeP=prime;
    k=0;
    r=1-r;
    while(k<10)
    {
        k=0;
        generatePrime();
        for(int i=0;i<10;i++)
        {
            BigInteger x=rand()%100;
            mod=pow(x,prime-1,prime);
            if(mod==1)
                k++;
        }
    }
    primeQ=prime;

    //msgBox.setText("Простые числа успешно сгенерированы");
    //msgBox.exec();
}

BigInteger MainWindow::calcNODByEucl(BigInteger a, BigInteger b)
{
    BigInteger t;
    while (b != 0)
    {
        t = a % b;
        a = b;
        b = t;
    }
    return a;
}

BigInteger MainWindow::calcNODByExtEucl(BigInteger m, BigInteger n)
{
    BigInteger a = m, b = n;
    BigInteger u1 = 1, v1 = 0;
    BigInteger u2 = 0, v2 = 1;
    BigInteger q, r;
    while(b != 0)
    {
        q = a / b;
        r = a % b;
        a = b; b = r;
        r = u2;
        u2 = u1 - q*u2;
        u1 = r;
        r = v2;
        v2 = v1 - q*v2;
        v1 = r;
    }
    return u1;
}

void MainWindow::on_createKeysPushButton_clicked()
{
    encModule=primeP*primeQ;
    qDebug()<<"encModule:"<<QString::fromStdString(bigIntegerToString(encModule));
    eulerFunction=(primeP-1)*(primeQ-1);

    exponenta=ui->exponSpinBox->value();
    modByEucl=calcNODByEucl(eulerFunction,exponenta);

    d=calcNODByExtEucl(exponenta, eulerFunction);
    if(d<0)
        d=d+eulerFunction;
    qDebug()<<"d:"<<QString::fromStdString(bigIntegerToString(d));
    QFile file1("/home/rail/public.txt");
    if(file1.open(QIODevice::WriteOnly))
       {
           QTextStream stream(&file1);
           stream<<QString::fromStdString(bigIntegerToString(encModule));
           stream<<"\r";
           stream<<QString::fromStdString(bigIntegerToString(exponenta));
           file1.close();
       }
    QFile file2("/home/rail/private.txt");
    if(file2.open(QIODevice::WriteOnly))
       {
           QTextStream stream(&file2);
           stream<<QString::fromStdString(bigIntegerToString(encModule));
           stream<<"\r";
           stream<<QString::fromStdString(bigIntegerToString(d));
           file2.close();
       }
    if(modByEucl==1)
    {
        //msgBox.setText("Ключи успешно созданы и сохранены в файлы");
        //msgBox.exec();
    }
    else
    {
        msgBox.setText("Открытая экспонента не удовлетворяет условиям!");
        msgBox.exec();
    }
}

void MainWindow::on_codePushButton_clicked()
{

    QFile file1("/home/rail/public.txt");
    QString buf;
    if(file1.open(QIODevice::ReadOnly))
       {
           QTextStream stream(&file1);
           buf=stream.readLine();
           QStringList lst=buf.split(QRegExp("\r"));
           encModule=stringToBigInteger(std::string(QString(lst[0]).toUtf8()));
           exponenta=stringToBigInteger(std::string(QString(lst[1]).toUtf8()));
           file1.close();
       }
    M=0;
    QString s="привет";
    qDebug()<<"message:"<<s;
    textByteArray=s.toUtf8();
    textLength=textByteArray.count()*8;
    textBitArray.resize(textLength);
    for(int i=0; i<textLength/8; ++i)
        for(int b=0; b<8; ++b)
            textBitArray.setBit(i*8+b, textByteArray.at(i)&(1<<b));
    for(int i=0;i<textLength;i++)
    {
        if(textBitArray.at(i)==true)
        {
            degree=1;
            for(int j=0;j<i;j++)
                degree=degree*2;
            M=M+degree;
        }
    }
    qDebug()<<"M:"<<QString::fromStdString(bigIntegerToString(M));
    code=pow(M,exponenta,encModule);
    qDebug()<<"code:"<<QString::fromStdString(bigIntegerToString(code));
    QFile file2("/home/rail/encrypted.txt");
    if(file2.open(QIODevice::WriteOnly))
       {
           QTextStream stream(&file2);
           stream<<QString::fromStdString((bigIntegerToString(code)));
           file2.close();
       }
}

void MainWindow::on_decodePushButton_clicked()
{
    QFile file1("/home/rail/private.txt");
    QString buf;
    if(file1.open(QIODevice::ReadOnly))
       {
           QTextStream stream(&file1);
           buf=stream.readLine();
           QStringList lst=buf.split(QRegExp("\r"));
                      //qDebug()<<lst[0];
           encModule=stringToBigInteger(std::string(QString(lst[0]).toUtf8()));
           d=stringToBigInteger(std::string(QString(lst[1]).toUtf8()));
           file1.close();
       }
    QFile file2("/home/rail/encrypted.txt");
    if(file2.open(QIODevice::ReadOnly))
       {
           QTextStream stream(&file2);
           QString buf;
           buf=stream.readLine();
           code=stringToBigInteger(std::string(buf.toUtf8()));
           file2.close();
       }
    M=pow(code,d,encModule);
    qDebug()<<"M after decode:"<<QString::fromStdString(bigIntegerToString(M));
    divForM=M;
    int i=0;
    while(divForM>=2)
    {
        modForM=divForM%2;
        divForM=divForM/2;
        textBitArray.resize(i+1);
        if(modForM==0)
            textBitArray.setBit(i, false);
        else
            textBitArray.setBit(i, true);
        //qDebug()<<QString::fromStdString(bigIntegerToString(divForM));
        i++;
    }
    textBitArray.resize(textBitArray.size()+1);
    if(divForM==1)
        textBitArray.setBit(i,true);
    else
        textBitArray.setBit(i,false);
    textByteArray.resize((textBitArray.size()+1)/8);
    for(int b=0; b<i; ++b)
            textByteArray[b/8] = (textByteArray.at(b/8) | ((textBitArray[b]?1:0)<<(b%8)));
    qDebug()<<"message after decode:"<<QString::fromUtf8(textByteArray);
}

void MainWindow::on_atackPushButton_clicked()
{
    L=30;
    lengthForAttack=15;
    lengthes=new int[lengthForAttack];
    times=new int[lengthForAttack];
    for(int j=0;j<lengthForAttack;j++)
    {
         attack();
         times[j]=start.elapsed();
         lengthes[j]=L;
         qDebug()<<"L:"<<lengthes[j];
         qDebug()<<"time:"<<times[j];
         if(start.elapsed()>10000)
             L=L+1;
         else
             L=L+2;
     }
    pen.setColor(Qt::red);
    double t=0;
    for(int i=0;i<lengthForAttack;i++)
        if(times[i]>t)
            t=times[i];
    t=t+500;
    for(int j=1;j<lengthForAttack;j++)
        scene->addLine(lengthes[j-1]*4,-times[j-1]*300/t+280,lengthes[j]*4,-times[j]*300/t+280,pen);
    ui->graphicsView->setScene(scene);
}

void MainWindow::on_testPushButton_clicked()
{
    L=80;
    double buf;
    r=0.25;
    buf=r;
    times=new int[6];
    for(int j=0;j<6;j++)
    {
        attack();
        times[j]=start.elapsed();
        rArray[j]=buf;
        //qDebug()<<"L:"<<lengthes[j];
        qDebug()<<"time:"<<times[j];
        qDebug()<<"r:"<<rArray[j];
        r=buf+0.05;
        buf=r;
    }
    double t=0;
    for(int i=0;i<6;i++)
        if(times[i]>t)
            t=times[i];
    t=t+t/10;
    for(int j=1;j<6;j++)
        scene->addLine(rArray[j-1]*100,-times[j-1]*300/t+280,rArray[j]*100,-times[j]*300/t+280,pen);
    ui->graphicsView->setScene(scene);
}

void MainWindow::attack()
{
    on_genKeysPushButton_clicked();
    on_createKeysPushButton_clicked();
    start = QTime::currentTime();
    BigInteger x = rand()%1000;
    BigInteger y = 1; BigInteger i = 0; BigInteger stage = 2;
    modForRo=1;
    while (modForRo == 1)
    {
       if(x>y)
            modForRo=calcNODByEucl(encModule, x - y);
       else
           modForRo=calcNODByEucl(encModule, y - x);
        if (i == stage){
            y = x;
            stage = stage*2;
     }
     x = (x*x - 1) % encModule;
     i = i + 1;
    }
}
