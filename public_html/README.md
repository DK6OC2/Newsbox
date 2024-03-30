# The Newsbox

This is a hamradio project for amateur radio hobby enthusiasts.  

# Thanks to Arno Richter

A lot of the following code has been taken from https://github.com/oelna/microblog (MIT) Copyright 2023 Arno Richter
Arno already had the approach for his microblog application that I was looking for and consider to be very user-friendly. So I orientated myself on his code and adopted some parts of it. Thank you Arno for your very good work. It has helped and inspired me. 

# Datei Beschreibung

Im folgenden werden Dateien und Verzeichnisse beschrieben.

## /css

Speicherplatz für die projektspeziefische css Dateien. Grundsätzlich verwendet das Projekt das Bootstrap Framework in der Version 5.

## /js

Speicherplatz für JavaScript

## /inc Folder for includes

### database.php

Mit dieser Datei wird initial die SQLite3 Datenbank aufgebaut. Sie bietet eine einfache Versionierung, so kann später ein Update von Version x auf Version y gemacht werden.
Die Datenbank selbst beinhaltet die Tabellen:
| Name | Function |
| ---- | -------- |
| topics | Die Namen der News Kategorien |
| messages | Die Nachrichten |
| map_messages_topics | Verknüpfung von Nachricht zu Kategorie |
| clients | die Adresse der Boxen und deren Inhaber |
| map_clients_topics | Welche Box möchte welche Kategorie empfangen |
| admins | Liste der Admin, die Nachrichten erstellen dürfen |
| settings | Einstellungen zum Newsbox System |

### functions.php

Ist der zentrale Ort für alle globalen Funktionen. Wie zum Beispiel das 

## /templates

Hier befinden sich alle HTML webseiten.

## .htaccess

Mit hilfer dieser Datei werden einfach zu lesende URLs definiert.

## newsbox.db

Diese Datei ist die Datenbank. Sie enthält alle wichten und indiviuellen Daten. Das sind unteranderem die Nachrichten die anzeigt werden. 
Da die Nachrichten in der Datenbank gespeichert werden, ist es leicht möglich, dass später durch Plugins zum Beispiel Nachrichten aus anderen Quellen in die Datenbank geschrieben werden können.

## README.md

;-) ohne diese Datei könntest du diese Dokumentation nicht lesen.

## robots.txt

Diese Datei wird von Suchmaschinen ausgelesen. In unserem Projekt soll verhindert werden, dass eine Suchmaschine unsere URLs in deren Verzeichnis aufnimmt. 

# Description of definitions

## NOW 
Is the current date and time in ISO8601 format


# Description of variables

## $characters
Hier werden die Zeichen hinterlegt, die für den PassKey genutzt werden. 
Der PassKey wird dem Client initial angezeigt, damit er seine Box konfigurieren kann.

## $uri

## $path

## $clientStatus

## array $config

## array $settings
