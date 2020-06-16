# LINUX programozása Házi feladat

Készítette
- Révész Levente

# Feladatkiírás
A szoftvervilágban dolgozó, tanuló társaimhoz hasonlóan gyakran használom éjszakába nyúlóan a számítógépem. Az egyre terjedő sötét témák, valamint a ma már minden asztali operációs rendszeren alapfunkciónak számító éjszakai mód megkönnyítik ezt a tevékenységet. Azonban van még egy fontos paraméter, melynek állítása nem automatikus: a képernyő fényereje. A laptopok képernyőjének háttérvilágítása szoftveresen állítható, az éjszakai mód azonban ezt nem használja ki egyik általam ismert rendszeren sem.

A tervezett program egy háttérben futó daemon, ami a beépített éjszakai mód működéséhez hasonlóan automatikusan állítja a monitor fényerejét. A szürkület és a pirkadat idejét figyelembe véve éjszakára visszaveszi a fényerőt, nappal pedig felemeli azt.

Az alábbi funkciók megvalósítása a cél:
+ napkelte és napnyugta idejének automatikus kiszámítása megadott lokáció alapján
+ daemonként, háttérben futás
+ beállítások beolvasása config fájlból
+ systemd integráció
+ syslog használat

# Megvalósított program
Az összes tervezett funkciót sikerült megvalósítani. A program az `/etc/backlightd/conf` fájlból olvassa be a beállításokat. A lokációt, az éjszakai és nappali fényerőt, valamint a backlight interfészt lehet beállítani. A programot superuserként kell futtatni, mert csak így tudja módosítani a fényerőt.

Konfigurációs fájl: `/etc/backlightd/conf`
```
latitude=46.16161       # positive for N, negative for S
longitude=18.35337      # positive for E, negative for W
brightness_min=10       # 0-100
brightness_max=100      # 0-100
interface=/sys/class/backlight/intel_backlight
```
### Működés

Az indítás után három dolog történhet:
- Ha még nem kelt fel a nap, várakozni kezd a napkeltére.
- Ha nappal van, várakozni kezd a napnyugtára.
- Ha már éjszaka van, várakozik a következő napig.

Az indítás után mindig a következő napkelte, napnyugta eseményre vár, és amikor az bekövetkezett, beállítja a megfelelő fényerőt. A napkelte és napnyugta dátumát az adott földrajzi pozícióra és az aktuális napra határozza meg. Az időpontokat az **api.sunrise-sunset.org** weboldalról olvassa be, egy REST API-n keresztül. A JSON parsing műveletet a `jsmn.h` library végzi (Copyright (c) 2010 Serge Zaitsev). A HTTP kommunikáció megvalósításához a `libcurl` könyvtárat használtam.

A fényerőállítás fokozatosan történik. Az átmenet kezdetén érzékelt fényerőről az éjszakai/nappali fényerőre áll át. Ezt 5 lépésben teszi. Az átmenet időtartamát a `backlightd.h` header fájlban lehet finomhangolni.

### Használat
1. Manuális:
   - Indítás: `sudo ./backlightd`
   - Monitorozás: `journalctl -f -t backlightd`
   - Leállítás: `sudo pkill backlightd`

2. systemd:
    - Indítás: `sudo systemctl start backlightd`
    - Monitorozás: `systemctl status backlightd` vagy `journalctl -f backlightd`
    - Leállítás: `sudo systemctl stop backlightd`

Kiegészítésként a biztonsággal lehetne még foglalkozni, superuser módban futtatott programról lévén szó, ne hagyjunk benne túl nagy biztonsági réseket.

**Bemutató videó:** [[Megtekintés]](https://bmeedu-my.sharepoint.com/:v:/g/personal/levente_revesz_edu_bme_hu/EfOmo2lXFa5Mttp4BDckbDMBIjnmhIgkclp5TRBI4HybCA?e=SOXlw6)

# Fordítás és telepítés

A program függőségei:

- libcurl

Fordítás:

```
cd src
make
```

Telepítés
```
sudo make install
```

Eltávolítás
```
sudo make remove
```

# Tapasztalatok

Az időkezelő függvények a C-ben több odafigyelést és munkát igényelnek, mint az általam eddig használt Python nyelven. Az időhöz kapcsolódó kódrészek megírásával sok időt töltöttem. 

A konfigurációs fájl beolvasása is nehezebbnek bizonyult a tervezettnél. Ellenben működőképes közbeiktatott szóközökkel, kommentekkel. Ennek a résznek több tervezéssel állnék neki, ha újra kellene kezdenem. Valószínűleg egy INI parse könyvtár használata a célszerű.

A daemon készítés viszont egészen egyszerű. A systemd integrációnak is könnyedén elkészíthető.
