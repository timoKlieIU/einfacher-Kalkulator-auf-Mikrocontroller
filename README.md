# einfacher-Kalkulator-auf-Mikrocontroller

Diese Anwendung ermöglicht es dem Benutzer, Daten über einen seriellen Port zu senden und zu empfangen, sowie mathematische Berechnungen durchzuführen und das Ergebnis über serielle Kommunikation zu übertragen. Sie ist ideal für die Interaktion mit seriellen Geräten oder für Lehrzwecke in der seriellen Datenkommunikation.

## Beginnen

Diese Anleitung führt Sie durch den Prozess der Installation, Konfiguration und Nutzung der Anwendung auf Ihrem System.

### Voraussetzungen

Bevor Sie die Anwendung installieren und ausführen können, stellen Sie sicher, dass Sie folgendes installiert haben:

- Qt SDK (Stellen Sie sicher, dass das Modul `Qt Serial Port` eingeschlossen ist)
- Ein kompatibler Compiler (z.B. GCC für Linux, MSVC für Windows, Clang für macOS)
- Git

### Installation

1. **Klonen Sie das Repository**:

    Öffnen Sie ein Terminal oder eine Eingabeaufforderung und führen Sie den folgenden Befehl aus:

    ```bash
    git clone <REPOSITORY-URL>
    cd <REPOSITORY-NAME>
    ```

    Ersetzen Sie `<REPOSITORY-URL>` mit der URL Ihres GitHub-Repositories und `<REPOSITORY-NAME>` mit dem Namen Ihres Repositories.

2. **Öffnen Sie das Projekt in Qt Creator**:

    - Starten Sie Qt Creator.
    - Wählen Sie `Datei` > `Öffnen Datei oder Projekt...`.
    - Navigieren Sie zum geklonten Projektverzeichnis und wählen Sie die `.pro`-Datei.
    - Klicken Sie auf `Öffnen`.

3. **Konfigurieren Sie das Projekt**:

    Stellen Sie sicher, dass Sie die richtige Qt-Version und den Compiler ausgewählt haben. Passen Sie die Einstellungen wie den seriellen Portnamen (`COM9`) im Code an Ihre Gerätekonfiguration an.

### Bauen und Ausführen

- Klicken Sie auf das Hammer-Symbol, um das Projekt zu kompilieren.
- Klicken Sie auf den grünen Pfeil, um die Anwendung auszuführen.

### Nutzung

- **Serielle Kommunikation**: Die Anwendung versucht, eine Verbindung über den seriellen Port `COM9` mit einer Baudrate von 9600 herzustellen. Stellen Sie sicher, dass Ihr Gerät entsprechend konfiguriert ist.
- **Mathematische Berechnungen**: Wählen Sie die Zahlen und die Operation (+, -, *, /) über die GUI und klicken Sie auf den Berechnungs-Button, um die Anfrage über den seriellen Port zu senden.
- **Log-Speicherung**: Klicken Sie auf den Button, um den Kommunikationsverlauf in eine Datei zu speichern und diese anschließend zu öffnen.

## Beiträge

Beiträge sind herzlich willkommen! Für größere Änderungen öffnen Sie bitte zuerst ein Issue, um zu diskutieren, was Sie ändern möchten.

## Lizenz

Dieses Projekt ist unter der MIT Lizenz lizenziert - siehe die LICENSE Datei für Details.
