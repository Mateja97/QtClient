#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  readPort();
  _socket = new QTcpSocket(this);
  connectToHost();
  connect(_socket, &QIODevice::readyRead, this, &MainWindow::readData); //receiving data
  in.setDevice(_socket);
  in.setVersion(QDataStream::Qt_4_0);
  writeData("freq\n");
  writeData("100\n");


  this->x = {};
  this->y = {};
  MainWindow::makePlot();
}

MainWindow::~MainWindow()
{
  delete ui;
}
void MainWindow::readPort(){

  QFile file("port.txt");
  if(!file.exists()){
          qDebug() << "Port file doesnt exist!";
      }
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
         QTextStream stream(&file);
         _PORT = stream.readAll().toUShort();
     }else{
      _PORT = 3000;
    }
     file.close();
}

bool MainWindow::writeData(QString data)
{
    if(_socket->state() == QAbstractSocket::ConnectedState)
    {
        _socket->write(data.toLocal8Bit().data());
        return _socket->waitForBytesWritten();
    }
    else
        return false;
}

void MainWindow::readData()
{
   double freq = ui->spinBox->value();
   char buffer[4096];
   int ret;
   while((ret = _socket->read(buffer,sizeof (buffer)-1)) > 0){

       double value = QString(buffer).toDouble();
       double linearX;
       double y;
     
       linearX = qRound(value*100)/100.0;
       int d = 10*freq;
       int d1= linearX*freq;
       y = d1%d;

       this->x.push_back(linearX);
       this->y.push_back(y);
       ui->customPlot->graph(0)->setData(this->x, this->y);
       ui->customPlot->replot();
       n++;
     }
}
   
bool MainWindow::connectToHost()
{
    _socket->connectToHost(QHostAddress::LocalHost,_PORT);
    return _socket->waitForConnected();
}

void MainWindow::on_spinBox_valueChanged(const QString &arg1)
{
    writeData("freq\n");
    writeData(arg1+"\n");
}


void MainWindow::on_startButton_clicked()
{

    writeData("start\n");

}

void MainWindow::on_stopButton_clicked()
{
    writeData("stop\n");

}

void MainWindow::makePlot(){

  ui->customPlot->addGraph();
  ui->customPlot->graph(0)->setData(this->x, this->y);
  ui->customPlot->setInteraction(QCP::iRangeZoom,true);
  ui->customPlot->setInteraction(QCP::iRangeDrag,true);
  ui->customPlot->xAxis->setLabel("x");
  ui->customPlot->yAxis->setLabel("y");
  ui->customPlot->xAxis->setRange(0, 1);
  ui->customPlot->yAxis->setRange(0, 100);
  ui->customPlot->replot();
}

void MainWindow::on_clearButton_clicked()
{
    writeData("clear\n");
    ui->customPlot->graph(0)->data().clear();
    ui->customPlot->replot();
    this->x = {};
    this->y = {};
}
