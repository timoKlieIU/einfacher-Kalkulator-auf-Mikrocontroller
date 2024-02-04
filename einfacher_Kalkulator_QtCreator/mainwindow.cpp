// Inkludiert die notwendigen Header-Dateien für die GUI und serielle Kommunikation
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "QSerialPort"
#include "QSerialPortInfo"
#include "QTimer"
#include "QFile"
#include "QTextStream"
#include "QDesktopServices"
#include "QUrl"
#include "QDir"

// Konstruktor der MainWindow-Klasse, initialisiert die UI und konfiguriert die serielle Verbindung
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this); // Initialisiert die in Qt Designer erstellte UI

    setupSerialPort(); // Konfiguriert die serielle Portverbindung

    // Verbindet das Signal für verfügbare Daten am seriellen Port mit dem entsprechenden Slot
    connect(&serialPort, &QSerialPort::readyRead, this, &MainWindow::readSerialData);

    // Erstellt und konfiguriert einen QTimer für periodische Aufgaben
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::checkConnection);
    timer->start(1000); // Startet den Timer mit einem Intervall von 1000ms (1 Sekunde)

    // Verbindet den Klick-Event des Log-Speicher-Buttons mit dem entsprechenden Slot
    connect(ui->saveLogButton, &QPushButton::clicked, this, &MainWindow::onSaveLogClicked);

    // Fügt die mathematischen Operationen zur Auswahl in die ComboBox ein
    ui->comboBox->addItem("+");
    ui->comboBox->addItem("-");
    ui->comboBox->addItem("*");
    ui->comboBox->addItem("/");

    // Verbindet den Klick-Event des Berechnungs-Buttons mit dem entsprechenden Slot
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::performCalculation);
}

// Destruktor der MainWindow-Klasse, schließt den seriellen Port und bereinigt die UI
MainWindow::~MainWindow()
{
    if (serialPort.isOpen()) {
        serialPort.close(); // Schließt den seriellen Port, falls geöffnet
    }
    delete ui; // Gibt den Speicher der UI-Komponenten frei
}

// Konfiguriert die serielle Portverbindung
void MainWindow::setupSerialPort() {
    serialPort.setPortName("COM9"); // Legt den Portnamen fest
    serialPort.setBaudRate(QSerialPort::Baud9600); // Setzt die Baudrate
    serialPort.setDataBits(QSerialPort::Data8); // Setzt die Datenbits
    serialPort.setParity(QSerialPort::NoParity); // Setzt die Parität
    serialPort.setStopBits(QSerialPort::OneStop); // Setzt die Stopbits
    serialPort.setFlowControl(QSerialPort::NoFlowControl); // Deaktiviert die Flusskontrolle

    // Versucht, den Port zu öffnen und aktualisiert das Statuslabel entsprechend
    if (!serialPort.open(QIODevice::ReadWrite)) {
        ui->statuslabel->setText("Kein Gerät angeschlossen oder Port nicht verfügbar.");
    } else {
        ui->statuslabel->setText("Port ist geöffnet.");
    }
}

// Überprüft periodisch die Verbindung über den seriellen Port
void MainWindow::checkConnection() {
    if (serialPort.isOpen()) {
        if (serialPort.write("Test\n") == -1) {
            ui->statuslabel->setText("Verbindung unterbrochen.");
            serialPort.close();
            setupSerialPort(); // Versucht, die Verbindung neu zu konfigurieren
        }
    }
}

// Sendet eine Berechnungsanfrage über den seriellen Port
void MainWindow::performCalculation() {
    QString num1 = ui->lineEdit->text(); // Liest den ersten Wert
    QString num2 = ui->lineEdit_2->text(); // Liest den zweiten Wert
    QString operation = ui->comboBox->currentText(); // Liest die ausgewählte Operation

    bool ok1, ok2;
    double val1 = num1.toDouble(&ok1); // Konvertiert den ersten Wert in einen Double
    double val2 = num2.toDouble(&ok2); // Konvertiert den zweiten Wert in einen Double

    // Überprüft die Gültigkeit der Eingaben
    if (!ok1 || !ok2) {
        ui->inputlabel->setText("Error: Ungültige Eingabe");
        return;
    }

    // Erstellt den zu sendenden String und sendet ihn, falls möglich
    QString toSend = num1 + " " + operation + " " + num2 + "\n";
    if (serialPort.isOpen() && serialPort.isWritable()) {
        serialPort.write(toSend.toUtf8());
        ui->statuslabel->setText("gesendet.");
        messageLog.append("Gesendet: " + toSend);
    } else {
        ui->statuslabel->setText("Kann nicht senden: keine Verbindung.");
    }
}

// Liest Daten vom seriellen Port
void MainWindow::readSerialData() {
    QByteArray data = serialPort.readAll(); // Liest alle verfügbaren Daten
    QString receivedData = QString::fromUtf8(data); // Konvertiert die Daten in einen String
    ui->inputlabel->setText(receivedData); // Zeigt die empfangenen Daten an
    messageLog.append("Empfangen: " + receivedData); // Fügt die Daten zum Log hininzu
}

// Speichert den Nachrichtenverlauf in eine Datei und öffnet diese
void MainWindow::onSaveLogClicked() {
    QString filePath = QDir::currentPath() + "/nachrichtenaustausch_log.txt"; // Bestimmt den Dateipfad
    QFile file(filePath); // Erstellt ein QFile-Objekt
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file); // Erstellt einen QTextStream für das Schreiben
        for (const QString &message : messageLog) {
            out << message << "\n"; // Schreibt jede Nachricht in die Datei
        }
        file.close(); // Schließt die Datei
        ui->statuslabel->setText("Nachrichtenaustausch gespeichert und wird geöffnet.");
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath)); // Öffnet die Datei
    } else {
        ui->statuslabel->setText("Fehler beim Speichern der Datei.");
    }
}
