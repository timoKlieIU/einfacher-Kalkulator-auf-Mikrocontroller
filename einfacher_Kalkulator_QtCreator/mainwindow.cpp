// Inkludiere die notwendigen Header-Dateien für die GUI und die serielle Kommunikation
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

// Konstruktor der MainWindow-Klasse
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this); // Initialisiere die Benutzeroberfläche

    // Setze das Hintergrundbild des Hauptfensters
    QPixmap background(":/new/prefix1/Backround.png");
    this->setFixedSize(background.size()); // Fixiere die Größe des Fensters auf die Größe des Hintergrundbildes
    this->setObjectName("mainWindow"); // Setze den Objektnamen des Hauptfensters
    // Setze den Stil des Hauptfensters und der PushButtons
    this->setStyleSheet("#mainWindow { "
                        "background-image: url(:/new/prefix1/Backround.png); "
                        "background-repeat: no-repeat; "
                        "background-position: center; "
                        "}"
                        "QPushButton { "
                        "background-color: white; "
                        "}");

    listAvailablePorts(); // Liste die verfügbaren seriellen Ports auf
    setupSerialPort(); // Konfiguriere den seriellen Port

    // Erstelle und konfiguriere einen Timer zur Überprüfung der Verbindung
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::checkConnection);
    timer->start(1000); // Timer wird jede Sekunde ausgelöst

    // Erstelle und konfiguriere einen Timer zur Aktualisierung der Liste der verfügbaren Ports
    QTimer *portsUpdateTimer = new QTimer(this);
    connect(portsUpdateTimer, &QTimer::timeout, this, &MainWindow::listAvailablePorts);
    portsUpdateTimer->start(1000); // Aktualisiere die Liste jede Sekunde

    // Verbinde die Signale von UI-Elementen mit den entsprechenden Slots
    connect(ui->saveLogButton, &QPushButton::clicked, this, &MainWindow::onSaveLogClicked);
    ui->comboBox->addItem("+"); // Füge mathematische Operatoren zur ComboBox hinzu
    ui->comboBox->addItem("-");
    ui->comboBox->addItem("*");
    ui->comboBox->addItem("/");

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::performCalculation);
    connect(ui->portsComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(on_portsComboBox_currentIndexChanged(const QString &)));
}

// Destruktor der MainWindow-Klasse
MainWindow::~MainWindow()
{
    if (serialPort.isOpen()) {
        serialPort.close(); // Schließe den seriellen Port, falls geöffnet
    }
    delete ui; // Lösche die Benutzeroberfläche
}

// Konfiguriere den seriellen Port
void MainWindow::setupSerialPort() {
    serialPort.setBaudRate(QSerialPort::Baud9600); // Setze die Baudrate
    serialPort.setDataBits(QSerialPort::Data8); // Setze die Datenbits
    serialPort.setParity(QSerialPort::NoParity); // Setze die Parität
    serialPort.setStopBits(QSerialPort::OneStop); // Setze die Stopbits
    serialPort.setFlowControl(QSerialPort::NoFlowControl); // Setze die Flusskontrolle

    // Versuche, den seriellen Port zu öffnen und zeige eine Fehlermeldung an, falls es nicht klappt
    if (!serialPort.open(QIODevice::ReadWrite)) {
        QString errorString = "Fehler: Port kann nicht geöffnet werden. \n ";
        switch (serialPort.error()) {
        case QSerialPort::PermissionError:
            errorString += "Keine Berechtigung.";
            break;
        case QSerialPort::DeviceNotFoundError:
            errorString += "Gerät nicht gefunden.";
            break;
        case QSerialPort::OpenError:
            errorString += "Kann nicht geöffnet werden.";
            break;
        case QSerialPort::WriteError:
            errorString += "Schreibfehler.";
            break;

        default:
            errorString += "Unbekannter Fehler.";
        }
        ui->statuslabel->setText(errorString); // Zeige die Fehlermeldung an
    } else {
        ui->statuslabel->setText("Port ist geöffnet."); // Bestätigung, dass der Port geöffnet ist
    }
}

// Liste die verfügbaren seriellen Ports auf
void MainWindow::listAvailablePorts() {
    ui->portsComboBox->clear(); // Lösche die aktuelle Liste
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        ui->portsComboBox->addItem(info.portName(), info.portName()); // Füge jeden verfügbaren Port zur ComboBox hinzu
    }
}

// Behandele die Auswahl eines Ports
void MainWindow::on_portsComboBox_currentIndexChanged(const QString &portName) {
    if (serialPort.isOpen()) {
        serialPort.close(); // Schließe den aktuellen Port
    }
    serialPort.setPortName(portName); // Setze den neuen Portnamen
    setupSerialPort(); // Konfiguriere den seriellen Port neu
}

// Überprüfe die Verbindung zum seriellen Port
void MainWindow::checkConnection() {
    if (serialPort.isOpen()) {
        if (serialPort.write("Test\n") == -1) {
            ui->statuslabel->setText("Verbindung unterbrochen. Versuche neu zu verbinden...");
            serialPort.close(); // Schließe den Port bei Verbindungsfehlern
            setupSerialPort(); // Versuche, den Port neu zu öffnen
        }
    }
}

// Führe eine Berechnung durch und sende die Daten über den seriellen Port
void MainWindow::performCalculation() {
    QString num1 = ui->lineEdit->text(); // Lese den ersten Wert
    QString num2 = ui->lineEdit_2->text(); // Lese den zweiten Wert
    QString operation = ui->comboBox->currentText(); // Lese den ausgewählten Operator

    bool ok1, ok2;
    double val1 = num1.toDouble(&ok1); // Konvertiere den ersten Wert in eine Zahl
    double val2 = num2.toDouble(&ok2); // Konvertiere den zweiten Wert in eine Zahl

    // Überprüfe die Gültigkeit der Eingaben
    if (!ok1 || !ok2) {
        QString errorMessage = "Error: Ungültige Eingabe bei ";
        if (!ok1) errorMessage += "erstem Wert";
        if (!ok1 && !ok2) errorMessage += " und \n";
        if (!ok2) errorMessage += "zweitem Wert";
        ui->inputlabel->setText(errorMessage); // Zeige eine Fehlermeldung an
        return;
    }

    // Erstelle den zu sendenden String
    QString toSend = num1 + " " + operation + " " + num2 + "\n";
    // Versuche, die Daten zu senden
    if (serialPort.isOpen() && serialPort.isWritable()) {
        if (serialPort.write(toSend.toUtf8()) == -1) {
            ui->statuslabel->setText("Fehler beim Senden: Schreibfehler.");
        } else {
            ui->statuslabel->setText("Daten erfolgreich gesendet.");
            messageLog.append("Gesendet: " + toSend); // Füge die gesendete Nachricht zum Log hinzu
        }
    } else {
        ui->statuslabel->setText("Kann nicht senden: keine Verbindung."); // Fehlermeldung, wenn keine Verbindung besteht
    }
}

// Lies Daten vom seriellen Port
void MainWindow::readSerialData() {
    QByteArray data = serialPort.readAll(); // Lese alle verfügbaren Daten
    QString receivedData = QString::fromUtf8(data); // Konvertiere die Daten in einen String
    ui->inputlabel->setText(receivedData); // Zeige die empfangenen Daten an
    messageLog.append("Empfangen: " + receivedData); // Füge die empfangenen Daten zum Log hinzu
}

// Speichere den Nachrichtenverlauf in einer Datei
void MainWindow::onSaveLogClicked() {
    QString filePath = QDir::currentPath() + "/nachrichtenaustausch_log.txt"; // Bestimme den Pfad der Log-Datei
    QFile file(filePath); // Erstelle ein QFile-Objekt für die Log-Datei
    // Versuche, die Datei zu öffnen
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file); // Erstelle einen QTextStream zum Schreiben in die Datei
        for (const QString &message : messageLog) {
            out << message << "\n"; // Schreibe jede Nachricht in die Datei
        }
        file.close(); // Schließe die Datei
        ui->statuslabel->setText("Nachrichtenaustausch gespeichert und wird geöffnet.");
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath)); // Öffne die gespeicherte Log-Datei
    } else {
        ui->statuslabel->setText("Fehler beim Öffnen der Datei zum Speichern.\n Bitte Berechtigungen überprüfen oder Speicherort ändern.");
    }
}
