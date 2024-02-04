// Initialisiere die serielle Kommunikation im setup()-Teil des Programms.
void setup() {
  // Starte die serielle Kommunikation mit einer Baudrate von 9600.
  Serial.begin(9600);
}

// Die loop()-Funktion wird kontinuierlich ausgeführt, solange das Arduino läuft.
void loop() {
  // Überprüfe, ob Daten auf der seriellen Schnittstelle verfügbar sind.
  if (Serial.available() > 0) {
    // Lese die eingehende Nachricht bis zum Zeilenende ('\n').
    String message = Serial.readStringUntil('\n');

    // Überprüfe, ob die Nachricht ein Testbefehl ist.
    if (message == "Test") {
      // Ignoriere den Testbefehl und kehre sofort zur loop()-Funktion zurück.
      return;
    }

    // Verarbeite die Nachricht, wenn sie nicht der Testbefehl ist.
    processMessage(message);
  }
}

// Die processMessage()-Funktion verarbeitet die empfangene Nachricht.
void processMessage(String message) {
  // Suche die Position des ersten und letzten Leerzeichens in der Nachricht.
  int firstSpaceIndex = message.indexOf(' ');
  int lastSpaceIndex = message.lastIndexOf(' ');
  
  // Überprüfe, ob das Format der Nachricht gültig ist.
  if (firstSpaceIndex == -1 || lastSpaceIndex == -1 || firstSpaceIndex == lastSpaceIndex) {
    // Sende eine Fehlermeldung zurück, wenn das Format ungültig ist.
    Serial.println("Error: Ungültiges Format");
    return;
  }

  // Extrahiere die Zahlen und den Operator aus der Nachricht.
  String num1Str = message.substring(0, firstSpaceIndex);
  String operatorStr = message.substring(firstSpaceIndex + 1, lastSpaceIndex);
  String num2Str = message.substring(lastSpaceIndex + 1);
  
  // Konvertiere die Zahlenstrings in Fließkommazahlen.
  float num1 = num1Str.toFloat();
  float num2 = num2Str.toFloat();
  float result;

  // Führe die entsprechende mathematische Operation aus.
  if (operatorStr == "+") {
    result = num1 + num2;
  } else if (operatorStr == "-") {
    result = num1 - num2;
  } else if (operatorStr == "*") {
    result = num1 * num2;
  } else if (operatorStr == "/") {
    // Überprüfe, ob eine Division durch Null versucht wird.
    if (num2 == 0) {
      Serial.println("Error: Division durch Null");
      return;
    }
    result = num1 / num2;
  } else {
    // Sende eine Fehlermeldung zurück, wenn der Operator unbekannt ist.
    Serial.println("Error: Unbekannter Operator");
    return;
  }

  // Sende das Ergebnis der Operation zurück über die serielle Schnittstelle.
  Serial.println(result);
}

