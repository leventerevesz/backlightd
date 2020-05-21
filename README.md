# Házi feladat

Név/Nevek NEPTUN kóddal:
- Révész Levente (IRW6T2)

# Feladatkiírás
A szoftvervilágban dolgozó, tanuló társaimhoz hasonlóan gyakran használom éjszakába nyúlóan a számítógépem. Az egyre terjedő sötét témák, valamint a ma már minden asztali operációs rendszeren alapfunkciónak számító éjszakai mód megkönnyítik ezt a tevékenységet. Azonban van még egy fontos paraméter, melynek állítása nem automatikus: a képernyő fényereje. A laptopok képernyőjének háttérvilágítása szoftveresen állítható, az éjszakai mód azonban ezt nem használja ki egyik általam ismert rendszeren sem.

A tervezett program egy háttérben futó daemon, ami a beépített éjszakai mód működéséhez hasonlóan automatikusan állítja a monitor fényerejét. A szürkület és a pirkadat idejét figyelembe véve éjszakára visszaveszi a fényerőt, nappal pedig felemeli azt.

Az alábbi funkciók megvalósítása a cél:
+ napkelte és napnyugta idejének automatikus kiszámítása megadott lokáció alapján
+ daemonként, háttérben futás
+ beállítások beolvasása config fájlból

# Megvalósított program
Az összes tervezett funkciót sikerült megvalósítani. A program az `/etc/backlightd/conf` fájlból olvassa be a beállításokat. A lokációt, az éjszakai és nappali fényerőt, valamint a backlight interfészt lehet beállítani. A programot superuserként kell futtatni, mert csak így tudja módosítani a fényerőt.

Az indítás után három dolog történhet:
- Ha még nem kelt fel a nap, várakozni kezd a napkeltére.
- Ha nappal van, várakozni kezd a napnyugtára.
- Ha már éjszaka van, várakozik a következő napig.

Az indítás után mindig a következő napkelte, napnyugta eseményre vár, és amikor az bekövetkezett, beállítja a megfelelő fényerőt. A napkelte és napnyugta dátumát az adott földrajzi pozícióra és az aktuális napra határozza meg a `sunrise.c` segítségével.

Kiegészítésként még jó ötlet lenne egy lockfile, hogy egyszerre csak egy példány fusson. Systemd integráció az automatikus indításhoz. A fényerőállítás történhetne több lépésben. Valamint a biztonsággal is lehetne még foglalkozni, superuser módban futtatott programról lévén szó, ne hagyjunk benne túl nagy biztonsági réseket.

Konfigurációs fájl: `/etc/backlightd/conf`
```
latitude=46.16161       # positive for N, negative for S
longitude=18.35337      # positive for E, negative for W
brightness_min=10       # 0-100
brightness_max=100      # 0-100
automatic=true
interface=/sys/class/backlight/intel_backlight
```

A programhoz felhasználtam _Mark S. Teel_ `sunrise.c` kódját.

**Bemutató videó:** [[Megtekintés]](https://bmeedu-my.sharepoint.com/:v:/g/personal/levente_revesz_edu_bme_hu/EfOmo2lXFa5Mttp4BDckbDMBIjnmhIgkclp5TRBI4HybCA?e=SOXlw6)

# Tapasztalatok

Az időkezelő függvények a C-ben több odafigyelést és munkát igényelnek, mint az általam eddig használt Python nyelven. Az időhöz kapcsolódó kódrészek megírásával sok időt töltöttem. 

A konfigurációs fájl beolvasása is nehezebbnek bizonyult a tervezettnél, sajnos nem is sikerült túl szépre az implementáció. Ellenben működőképes közbeiktatott szóközökkel, kommentekkel. Ennek a résznek több tervezéssel állnék neki, ha újra kellene kezdenem.

A daemon készítés viszont egészen egyszerű. A systemd integrációnak is utánanéztem. Sajnos idő hiányában ehhez a kódhoz nem tudtam elkészíteni, de a jövőben biztosan bátrabban állok neki a daemon készítésnek.