# Newsbox
**05.03.2024** Stromlaufplan komplett überarbeitet </p>
**29.02.2024** Stromlaufplan Anpassung Anschluss Piezosummer </p>
**25.02.2024** Stromlaufplan hinzugefügt</p>
**21.02.2024** Gehäuse Prototyp für 3D Druck: https://www.thingiverse.com/thing:72719</p>
**11.02.2024** die info.txt ist jetzt unter: https://newsbox.r04.afu.de abgelegt</p>
**01.02.2024** Der Programmcode ist jetzt in der main.cpp</p>

## JSon-Format (aktuelle Version)

```
{
 "ID": 1111111,  //long int
 "Datum": "30.03.2024",
 "Message": {
    "Topic": "Topic", //9 Zeichen
    "Zeile1": "line1", //20 Zeichen
    "Zeile2": "line2", //20 Zeichen
    "Zeile3": "line3" //20 Zeichen
             }
}
```       

## JSon-Forma (geplante Version)
```
{
 "ID": 1111111,  //long int
 "date": "30.03.2024",
 "topic": "Topic", //9 Zeichen
 "line1": "line1", //20 Zeichen
 "line2": "line2", //20 Zeichen
 "line3": "line3" //20 Zeichen
}
```
