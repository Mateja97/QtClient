#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QFile>
#include <QHostAddress>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void readPort();
  bool writeData(QString data);
  void makePlot();
  bool connectToHost();

private slots:
  void on_spinBox_valueChanged(const QString &arg1);
  void readData();
  void on_startButton_clicked();

  void on_stopButton_clicked();

  void on_clearButton_clicked();

private:
  Ui::MainWindow *ui;
  QTcpSocket*  _socket;
  QDataStream in;
  QString currentData;
  quint16 _PORT;
  QVector<double> x, y;
  quint16 n = 1;
};
#endif // MAINWINDOW_H
