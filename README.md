[![Creative Commons Lizenzvertrag](https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png)](http://creativecommons.org/licenses/by-nc-sa/4.0/)

Dieses Werk ist lizenziert unter einer [Creative Commons Namensnennung - Nicht-kommerziell - Weitergabe unter gleichen Bedingungen 4.0 International Lizenz](http://creativecommons.org/licenses/by-nc-sa/4.0/).

# RGB-LED-Kerze mit Homematic-Anbindung

<img src="images/RGB-LED-Candle.jpg" width="350">

## Beschreibung:
Diese Projekt beschreibt den Aufbau eines RGB-LED Teelichts / einer RGB-LED Kerze mit Homematic-Anbindung.<br>
Die Leuchtfarbe ist über die WebUI einstellbar.<br>
Zusätzlich gibt es ein paar auswählbare Automatikprogramme (Farbdurchlauf / verschiedene Flacker-Programme).<br>
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
- Micro-USB-Anschluss zur Spannungsversorgung (je nach Variante).<br>
- 3pol Schiebeschalter auf der Batterieplatine: Ein- und Ausschalter (trennt die Batterie- bzw. USB-Spannung)<br>
- Config-Taster (Power-PCB).<br>
- Reset-Taster (Hauptplatine - optional)<br>
<br>

#### Software:
Der Sketch und das Addon für die Homematic wurde von Jerome entwickelt.<br>
Die Leuchtfarbe ist über die WebUI einstellbar.<br>
Zusätzlich gibt es ein paar auswählbare Automatikprogramme (Farbdurchlauf / verschiedene Flacker-Programme)<br>
<br>
<img src="images/WebUI_LED-Candle.png" width="900">
<br>
<br>

## Platinen:
#### Hauptplatine:
<img src="images/RGB-LED-Candle_Main-PCB.jpg" width="400">
<br>

#### Power-PCB Teelicht-Version:
<img src="images/RGB-LED-Candle_USB-Power-PCB.jpg" width="400">
<br>
Spannungsversorung wahlweise über Batterie (Version mit 3 mm Flacker-LED) ***oder*** Micro-USB (Version mit WS2812 LED).<br>
<br>

_Auf dieser Platine wäre ein Step-Up für den Betrieb mit einer CR2477 Batterie vorgesehen._
<br>
_Leider hat sich bei den Tests ergeben, dass die Batterie nur wenige Stunden hält. Aus diesem Grund wurde diese Lösung verworfen.<br>
Deshalb wird die Bestückung von diesem Teil der Platine auch nicht näher beschrieben._
<br>
<br>

#### Power-PCB Kerzen-Version:
<img src="images/RGB-LED-Candle_Batterie-PCB.jpg" width="400">
<br>
Spannungsversorung wahlweise über Batterie ***oder*** Micro-USB<br>
<br>
