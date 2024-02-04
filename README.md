# Einfacher Kalkulator auf Mikrocontroller

Diese Anwendung ermöglicht es dem Benutzer, Daten über einen seriellen Port zu senden und zu empfangen, sowie mathematische Berechnungen durchzuführen und das Ergebnis über serielle Kommunikation zu übertragen. Sie ist ideal für die Interaktion mit seriellen Geräten oder für Lehrzwecke in der seriellen Datenkommunikation.

## Beginnen

Diese Anleitung führt Sie durch den Prozess der Installation, Konfiguration und Nutzung der Anwendung auf Ihrem System. Das Projekt besteht aus zwei Hauptteilen: einer Arduino-Anwendung und einer Qt-basierten Desktop-Anwendung.

### Voraussetzungen

Bevor Sie die Anwendung installieren und ausführen können, stellen Sie sicher, dass Sie folgendes installiert haben:

- Qt SDK (Stellen Sie sicher, dass das Modul `Qt Serial Port` eingeschlossen ist)
- Arduino IDE für die Arduino-Anwendung
- Ein kompatibler Compiler für die Qt-Anwendung (z.B. GCC für Linux, MSVC für Windows, Clang für macOS)
- Git

## Installation

### Arduino-Anwendung

1. **Öffnen Sie die Arduino IDE** und navigieren Sie zum Verzeichnis `Arduino` innerhalb des geklonten Repositories.
2. **Öffnen Sie die Arduino-Sketch-Datei** (`.ino`) in der Arduino IDE.
3. **Wählen Sie den richtigen Port und Board-Typ** aus, der Ihrem Arduino-Board entspricht.
4. **Hochladen** Sie den Sketch auf Ihr Arduino-Board.

### Qt-Anwendung

1. **Klonen Sie das Repository**:

    ```bash
    git clone https://github.com/timoKlieIU/einfacher-Kalkulator-auf-Mikrocontroller.git
    cd einfacher-Kalkulator-auf-Mikrocontroller/QtAnwendung
    ```

2. **Öffnen Sie das Projekt in Qt Creator**:

    - Starten Sie Qt Creator.
    - Wählen Sie `Datei` > `Öffnen Datei oder Projekt...`.
    - Navigieren Sie zum geklonten Projektverzeichnis und wählen Sie die `.pro`-Datei.
    - Klicken Sie auf `Öffnen`.

3. **Konfigurieren Sie das Projekt**:

    Stellen Sie sicher, dass Sie die richtige Qt-Version und den Compiler ausgewählt haben. Passen Sie die Einstellungen wie den seriellen Portnamen (`COM9`) im Code an Ihre Gerätekonfiguration an.

### Bauen und Ausführen

#### Arduino-Anwendung

- Verwenden Sie die Arduino IDE, um den Code auf Ihren Arduino hochzuladen.

#### Qt-Anwendung

- Klicken Sie auf das Hammer-Symbol, um das Projekt zu kompilieren.
- Klicken Sie auf den grünen Pfeil, um die Anwendung auszuführen.

## Nutzung

### Serielle Kommunikation

Die Qt-Anwendung versucht, eine Verbindung über den seriellen Port (standardmäßig `COM9`, muss eventuell angepasst werden!!! ) mit einer Baudrate von 9600 herzustellen. Stellen Sie sicher, dass Ihr Gerät entsprechend konfiguriert ist.

### Mathematische Berechnungen

Wählen Sie die Zahlen und die Operation (+, -, *, /) über die GUI und klicken Sie auf den Berechnungs-Button, um die Anfrage über den seriellen Port zu senden.

### Log-Speicherung

Klicken Sie auf den Button, um den Kommunikationsverlauf in eine Datei zu speichern und diese anschließend zu öffnen.

