# SMA to MODBUS

Diese Programm arbeitet als Daten-Konverter im Zusammenspiel mit einem SMA-Energy-Meter.

Das SMA-Energy-Meter verteilt die Zählerstände als Datagram per Multicast im Teilnetz.

Das Programm nimmt diese Daten entgegen und stellt sie per MODBUS zur Verfügung.

Ziel ist es auch den Geräten Zugriff auf die Zäherstände zu geben, die vorwiegend per MODBUS kommunizieren (z.B. Siemens LOGO!).

Das Programm basiert auf
- UDPSendReceiveString (Michael Margolis)
- MsgModubus_test_Slave: (Marco Gerritse)

Es wurde geteste auf "Arduino Ethernet". Zum Übertragen des Sketches benötigt man noch ein "Arduino USB2SERIAL".
