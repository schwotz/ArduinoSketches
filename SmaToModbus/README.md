# SMA to MODBUS

Diese Programm arbeitet als Daten-Konverter im Zusammenspiel mit einem SMA-Energy-Meter.

Ein SMA-Energy-Meter verteilt die Zählerstände als Datagram per Multicast im Teilnetz.

Das Programm nimmt diese Daten entgegen und stellt sie per MODBUS zur Verfügung. 
In der vorliegenden Version werden nur die Werte 1.7.0 (aktueller Verbrauch) und 2.7.0 (aktuelle Einspeisung) verarbeitet.

Ziel ist es Geräten Zugriff auf die Zäherstände zu geben, die keine Datagramme verarbeiten können aber per MODBUS kommunizieren (z.B. Siemens LOGO!).

### Das Programm basiert auf
- UDPSendReceiveString (Michael Margolis)
- MsgModubus_test_Slave: (Marco Gerritse)

### Die Dateien
- **SmaToModbus.ino** - der igentlicher Programmcode

### Dateien, die zusätzlich im Programmverzeichnis seim müssen
- **MgsModbus.h** - Änderung von zwei Variablen (Code von Marco Gerritse)
- **MgsModbus.cpp** - unveränderte Datei (Code vermutlich von Marco Gerritse)

### getestete Geräte
Es läuft auf "Arduino Ethernet". 

Zum Übertragen des Sketches benötigt man ein "Arduino USB2SERIAL".
