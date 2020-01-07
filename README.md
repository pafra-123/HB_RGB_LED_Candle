[![Creative Commons Lizenzvertrag](https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png)](http://creativecommons.org/licenses/by-nc-sa/4.0/)

Dieses Werk ist lizenziert unter einer [Creative Commons Namensnennung - Nicht-kommerziell - Weitergabe unter gleichen Bedingungen 4.0 International Lizenz](http://creativecommons.org/licenses/by-nc-sa/4.0/).

# RGB-LED-Kerze mit Homematic-Anbindung

<img src="images/RGB-LED-Candle.jpg" width="350">

## Beschreibung:
Diese Projekt beschreibt den Aufbau eines RGB-LED Teelichts / einer RGB-LED Kerze mit Homematic-Anbindung.<br>
<br>
**Folgende Varianten können aufgebaut werden:**<br>
<br>
:arrow_right: LED-Teelicht mit bedrahteter 3mm Flacker-LED<br>
:arrow_right: RGB-LED-Teelicht mit WS2812 LED und Spannungsversorung über Micro-USB<br>
:arrow_right: RGB-Kerze mit WS2812 LED und Spannungsversorung wahlweise über Batterie ***oder*** Micro-USB
<br>
<br>
Die Spannungsversorung erfolgt bei der Teelicht-Variante über Micro-USB.<br>
Eine Batterieversion ist aktuell in Evaluierung.<br>
Da die CR2477 Batterie sich allerdings als ungeeignet erwiesen hat, wird diese eine größere Bauform haben und kein Teelicht mehr sein.<br>
Zusätzlich besteht auch noch die Möglichkeit die Platinen als "normales" LED-Teelicht mit bedrahteter Flacker-LED zu bestücken und zu betreiben.<br>
<br>

## Funktionen:
#### Hardware:
Micro-USB-Anschluss zur Spannungsversorgung.<br>
3pol Schiebeschalter auf der Batterieplatine: Ein- und Ausschalter (trennt die Batteriespannung)<br>
SW1 auf der Batterieplatine: Config-Taster.<br>
SW1 auf der Hautplatine: Reset-Taster (optional)<br>
<br>
#### Software:
Die Leuchtfarbe ist über die WebUI einstellbar.<br>
Zusätzlich gibt es ein paar auswählbare Automatikprogramme (Farbdurchlauf / verschiedene Flacker-Programme)<br>
<br>

## Platinen:
#### Hautplatine:
<img src="images/RGB-LED-Candle_Main-PCB.jpg" width="400">
<br>

#### Batterieplatine Teelicht-Version:
<img src="images/RGB-LED-Candle_USB-PCB.jpg" width="400">
<br>
Auf dieser Platine wäre ein Step-Up für den Betrieb mit einer CR2477 Batterie vorgesehen.<br>
Leider hat sich bei den Tests ergeben, dass die Batterie nur wenige Stunden hält. Aus diesem Grund wurde diese Lösung verworfen.<br>
Deshalb wird die Bestückung von diesem Teil der Platine auch nicht näher beschrieben.<br>
<br>
