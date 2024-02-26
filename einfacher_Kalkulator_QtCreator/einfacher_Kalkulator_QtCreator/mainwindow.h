#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo> // Hinzugefügt für die Abfrage verfügbarer Ports
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Bestehende Slots
    void readSerialData();
    void checkConnection();
    void performCalculation();
    void onSaveLogClicked();

    // Neuer Slot für die Auswahländerung der PortsComboBox
    void on_portsComboBox_currentIndexChanged(const QString &portName);

private:
    Ui::MainWindow *ui;
    QSerialPort serialPort;
    QTimer *timer;
    QStringList messageLog; // Zum Speichern von Nachrichtenlogs

    // Hilfsfunktionen
    void setupSerialPort();
    void listAvailablePorts(); // Neue Funktion, um verfügbare Ports aufzulisten
};

#endif // MAINWINDOW_H
