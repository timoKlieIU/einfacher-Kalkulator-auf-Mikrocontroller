#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void performCalculation();
    void readSerialData();
    void onSaveLogClicked();
    void checkConnection();

private:
    Ui::MainWindow *ui;
    QSerialPort serialPort;
    QTimer *timer;
    QStringList messageLog;
    void setupSerialPort();
};

#endif
