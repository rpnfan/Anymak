// Copyright (c) 2009-2021, Andreas Wettstein
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     - Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     - Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

constexpr char opt_version[] = "1.271";

//--------------- src/konstanten.hh ---------------
#ifndef BELEGUNGSOPTIMIERER_KONSTANTEN_H
#define BELEGUNGSOPTIMIERER_KONSTANTEN_H

#include <string>

// Umlaute für die Verwendung in der Ausgabe; diese kann entweder in 8-Bit
// Codierung (Latin-1) oder UTF-8-Codierung erfolgen.

#ifdef AUSGABE_8BIT
#define strAe "\xE4"
#define strAE "\xC4"
#define strOe "\xF6"
#define strUe "\xFC"
#define strNBS "\xA0"
#else
#define strAe "\u00e4"
#define strAE "\u00c4"
#define strOe "\u00f6"
#define strUe "\u00fc"
#define strNBS "\u00A0"
#endif // !AUSGABE_8BIT

// Ein primitives Schema, um beim Compiliern deutsche oder englische Ausgaben
// zu wählen.

#ifdef ENGLISH
#define SPRACHE(x,y) y
#else
#define SPRACHE(x,y) x
#endif

#ifndef TASTENZAHL
#define TASTENZAHL 35
#endif // !TASTENZAHL

constexpr int nshift = 2;                 // Anzahl Shifttasten
constexpr int ntaste = TASTENZAHL-nshift; // Anzahl Symboltasten
constexpr int nebene = 2;                 // Anzahl Ebenen
constexpr int nmaxkorpus = 10;

#ifdef OHNE2SHIFT
constexpr int nebene2 = 1;
#else
constexpr int nebene2 = nebene;
#endif

// Konstanten, um Zeilennummern zu bezeichnen.
struct zeilen_t {
   enum {
      Zahlenreihe, Obere_Zeile, Mittelzeile, Untere_Zeile, Leerzeichenzeile,
      nzeile
   };
};

// nzeile ist die Höhe und nspalte ist die Breite eines vollen Tastenfelds.
constexpr int nzeile = zeilen_t::nzeile;
constexpr int nspalte = 16;

// Anzahl der Finger (einschliesslich der Daumen)
constexpr int nfinger = 10;

class finger_t {
public:
   // EinerDerDaumen ist ein Daumen, aber es ist nicht festgelegt, welcher.
   enum {
      KleinfingerLinks = -5,  RingfingerLinks = -4,
      MittelfingerLinks = -3, ZeigefingerLinks = -2,
      DaumenLinks = -1,
      EinerDerDaumen = 0,
      DaumenRechts = 1,
      ZeigefingerRechts = 2,  MittelfingerRechts = 3,
      RingfingerRechts = 4,   KleinfingerRechts = 5
   } _;

   operator int() const { return _; }
   explicit finger_t(int w) : _(static_cast<decltype(_)>(w)){}
   finger_t(decltype(_) w) : _(w){}
   finger_t() : _(EinerDerDaumen){}
};

// Konstanten, um Bigrammkategorien zu bezeichnen.
struct kategorie_t {
   enum {
      Handwechsel, Kollision, Doppeltanschlag,
      Auswaerts, Einwaerts, MitUndefDaumen, nKategorie
   } _;

   operator int() const { return _; }
   explicit kategorie_t(int w) : _(static_cast<decltype(_)>(w)){}
   kategorie_t(decltype(_) w) : _(w){}
   kategorie_t() : _(nKategorie){}
};

#endif // !BELEGUNGSOPTIMIERER_KONSTANTEN_H

//--------------- src/utfhilfe.hh ---------------
#ifndef BELEGUNGSOPTIMIERER_UTFHILFE_H
#define BELEGUNGSOPTIMIERER_UTFHILFE_H

#include <cstdint>
#include <string>

std::string utf32_in_utf8(char32_t c);
std::string utf32_in_utf8(const std::u32string& s);
std::string utf32_in_utf8(uint64_t u);

char32_t utf8_in_utf32(const char*& s, bool& fehler);

std::u32string zahl_in_utf32(int z);

std::string utf32_in_ausgabe(char32_t);
std::string utf32_in_ausgabe(const std::u32string&);

bool ist_ziffer(char32_t c);
bool ist_zwischenraum(char32_t c);

constexpr uint64_t utf_maske = 0x1fffff;

#endif //!BELEGUNGSOPTIMIERER_UTFHILFE_H

//--------------- src/Unicode.hh ---------------
#ifndef BELEGUNGSOPTIMIERER_UNICODE_H
#define BELEGUNGSOPTIMIERER_UNICODE_H

#include <unordered_map>
#include <unordered_set>

class Unicode final {
   std::unordered_set<char32_t> buchstaben;
   std::unordered_map<char32_t, char32_t> gross_in_klein;
   Unicode(const Unicode& ) = delete;
   Unicode();
public:
   static const Unicode& get(){ static const Unicode x; return x; }

   char32_t kleinbuchstabe(char32_t c) const;
   bool ist_buchstabe(char32_t c) const;
};

#endif // !BELEGUNGSOPTIMIERER_UNICODE_H

//--------------- src/Eingabestream.hh ---------------
#ifndef BELEGUNGSOPTIMIERER_EINGABESTREAM_H
#define BELEGUNGSOPTIMIERER_EINGABESTREAM_H

#include <fstream>
#include <string>

class Eingabestream final {
   std::string zeile, name;
   std::u32string buffer;
   std::ifstream f;
   size_t l, p, maxl, izeile;
   int errpos;
   bool utf8ein, geaendert;

   bool fuellen();

   // Übergehen alle Leerzeichen und Tabulatoren, beginnend von der aktuellen
   // Position, höchstens bis zum Ende der Zeile.
   void zwischenraum_uebergehen();
public:
   // f: Geöffneter Stream; utf8: true wenn der Stream UTF-8 enthalten sollte,
   // false, wenn er CP-1252 enthalten sollte.
   Eingabestream(const std::string& nam, bool utf8,
                 bool muss_existieren = true);

   ~Eingabestream();

   // Lies nächstes Zeichen (einschliesslich Zeilenenden '\n'); falls Eingabe
   // erschöpft ist gibt Null zurück.
   char32_t lieszeichen();

   // Falls aus der aktuellen Zeile schon Zeichen gelesen wurden, fange eine
   // neue an.  Gib false zurück, wenn die Eingabe erschöpft ist.
   bool neuezeile();

   // Wie neuezeile, übergeht aber Kommentar- und Leerzeilen.
   bool echte_neuezeile();

   // Anzahl der Zeichen, die in der aktuellen Zeile noch verbleiben.
   size_t restzeichen() const;

   // Ist aktuelles Zeichen in aktueller Zeile ein Leerzeichen oder Tabulator?
   bool ist_zwischenraum() const;

   // Ist aktuelles Zeichen in aktueller Zeile eine Ziffer
   bool ist_ziffer() const;

   // Ist aktuelles Zeichen in aktueller Zeile gleich dem übergebenen?
   bool ist(char32_t c) const;

   // übergeht das aktuelle Zeichen, fängt aber nie eine neue Zeile an.
   void uebergehen();

   // Liest nächstes Zeichen in aktueller Zeile; gibt 0 am Zeilenende;
   char32_t lies_in_zeile();

   // Prüft, ob in Rest der Zeile höchstens noch Zwischenraum vorkommt und
   // übergeht diesen.
   bool zeilenende();

   // Übergeht führenden Zwischenraum und erwartet dahinter eine Zahl, die bis
   // zum Zeilenende oder einem Zwischenraum reicht.  Falls das nicht so ist,
   // wird false zurückgegeben. Im Erfolgsfall wird true zurückgegeben, der
   // gelese Wert steht in wert.
   bool hole_zahl(double& wert);

   // Übergeht führenden Zwischenraum und gibt das dahinter stehende Wort
   // zurück, das durch Zwischenraum oder das Zeilenende begrenzt ist.  Falls
   // die Zeile kein Wort mehr enthält, gib false zurück.  Falls man auf ein
   // Kommentarzeichen # stösst gilt die Zeile als ohne beendet, kein Wort wird
   // gefunden.
   bool hole_wort(std::u32string& wert);

   // Übergeht führenden Zwischenraum und gibt das dahinter stehende Flag (+
   // für true, - für false) in wert zurück, das durch Zwischenraum oder das
   // Zeilenende begrenzt ist.  Falls kein Flag gefunden wird, gib false
   // als Returnwert.
   bool hole_flag(bool& wert);

   // übergeht Zwischenraum, und erwartet dann noch mindestens zwei Zeichen in
   // der Zeile.  Das erste davon ist der Stringbegrenzer; die folgenden
   // Zeichen bis zum nächsten Stringbegrenzer gehören zum String.  Kommt kein
   // weiterer Stringbegrenzer ist das ein Fehler.
   bool hole_string(std::u32string& wert);

   bool encoding_geaendert() const;

   [[noreturn]] void fehler(size_t off = 0) const;

   size_t aktuelle_zeile() const;
   const std::string& aktuelles_file() const;
};

double hole_zahl(Eingabestream& f, double rmin, double rmax);

void pruefe_leer_dann_N(Eingabestream& f, size_t N);

#endif // !BELEGUNGSOPTIMIERER_EINGABESTREAM_H

//--------------- src/typen.hh ---------------
#ifndef BELEGUNGSOPTIMIERER_TYPEN_H
#define BELEGUNGSOPTIMIERER_TYPEN_H

//#include "konstanten.hh"
#include <memory>
#include <string>

// Koordinate
struct koordinate_t { double x, y; };

// Beschreibung einer einzelnen Taste.
struct taste_t final {
   std::u32string name;
   double x, y, aufwand;
   int spalte, zeile;
   finger_t finger;
   bool istGrundposition, seite;
};

// Beschreibung eines n-Gramms.
struct n_gramm_t final {
   n_gramm_t(int T1, int T2, int T3, const std::u32string& N)
      : t1(T1), t2(T2), t3(T3), name(N){}
   int t1, t2, t3;
   std::u32string name;
};


// zaehl_t ist der Datentyp, der zum Zählen von n-Grammen im Korpus verwendet
// wird.  Double vermeidet Überläufe bei der Berechnung von Korrelationen und
// ist daher der sinnvollste Typ.
using zaehl_t = double;
using zaehl_ta = std::unique_ptr<zaehl_t[]>;

// akkumuations_t wird zur Berechnung der Aufwände verwendet.
using akkumuations_t = float;

// aufwand_t wird zur Tabellierung der Aufwände verwendet.  Um die Tabellen
// klein zu halten, verwende ich hier nur float; ist allemal genau genug.
using aufwand_t = float;

// haeufigkeit_t wird zur Tabellierung der Häufigkeiten verwendet, die in der
// Optimierung verwendet werden.  Auch hier ein float, um die Tabellen klein zu
// halten; die sieben Dezimale sollten mehr als ausreichend genau sein.
using haeufigkeit_t = float;

// belegung_t ist eine Zeichenpermutation und repräsentiert Belegungen.
using belegung_t = unsigned char[ntaste];

// Fingerbelastung durch die verschiedenen Korpora.
using fingerbelastung_t = akkumuations_t[nmaxkorpus][nfinger+1];

#endif // !BELEGUNGSOPTIMIERER_TYPEN_H

//--------------- src/Naechstes_zeichen.hh ---------------
#ifndef BELEGUNGSOPTIMIERER_NAECHSTES_ZEICHEN_H
#define BELEGUNGSOPTIMIERER_NAECHSTES_ZEICHEN_H

//#include "Eingabestream.hh"
#include <string>

class Naechstes_zeichen {
   Eingabestream text;
public:
   Naechstes_zeichen(const std::string& name, bool utf8) : text(name, utf8){}
   virtual ~Naechstes_zeichen(){}
   virtual char32_t get(){ return text.lieszeichen(); }
   bool encoding_geaendert() const { return text.encoding_geaendert(); }
};
#endif // !BELEGUNGSOPTIMIERER_NAECHSTES_ZEICHEN_H

//--------------- src/Haeufigkeit.hh ---------------
#ifndef BELEGUNGSOPTIMIERER_HAEUFIGKEIT_H
#define BELEGUNGSOPTIMIERER_HAEUFIGKEIT_H

//#include "typen.hh"
#include <cassert>
#include <unordered_map>
#include <vector>

class Tastatur;
class Kodierung;

class Haeufigkeit final {
private:
   haeufigkeit_t h3[ntaste][ntaste][ntaste][nebene];
   haeufigkeit_t h2[ntaste][ntaste][nebene][nebene2];
   haeufigkeit_t h1[ntaste][nebene], h1e[nmaxkorpus][ntaste][nebene];
   haeufigkeit_t gewichte[nmaxkorpus];
   haeufigkeit_t hunbekannt;
   const int nkorpus;
   bool trigramme;
   const Tastatur& tastatur;
   const Kodierung& kodierung;

   haeufigkeit_t *h11, *h21, *h22;
   haeufigkeit_t *h11e[nmaxkorpus], *h11ee[nmaxkorpus], *h21e[nmaxkorpus];

   void akkumuliere(int korpus, double gewicht,
                    const zaehl_t* uh1, const zaehl_t* uh2, const zaehl_t* uh3,
                    const zaehl_t* uh11, const zaehl_t* uh12,
                    const zaehl_t* uh22, const zaehl_t* uh1l,
                    const zaehl_t* uh1s, const zaehl_t* uh2l,
                    const zaehl_t* uh2s, zaehl_t ll, zaehl_t ls, zaehl_t ss,
                    const std::unordered_map<char32_t, zaehl_t>& unbekannt,
                    std::unordered_map<char32_t, zaehl_t>& summe_unbekannt);
   haeufigkeit_t& ein(int p, int e)
   { assert(p < ntaste && e < nebene); return h1[p][e]; }
   haeufigkeit_t& ein_k(int korpus, int p, int e) {
      assert(korpus < nkorpus && p < ntaste && e < nebene);
      return h1e[korpus][p][e];
   }
   haeufigkeit_t& bi(int p1, int e1, int p2, int e2){
      assert(p1 < ntaste && e1 < nebene && p2 < ntaste && e2 < nebene2);
      return h2[p1][p2][e1][e2];
   }
   haeufigkeit_t& tri(int p1, int p2, int p3, int e3){
      assert(p1 < ntaste && p2 < ntaste && p3 < ntaste && e3 < nebene);
      return h3[p1][p2][p3][e3];
   }

   haeufigkeit_t& ein_ein(int oi, int oj){
      assert(h11 && oi < groesse1 && oj < groesse1);
      return h11[sym_index(oi, oj)];
   }
   haeufigkeit_t& ein_k_ein_k(int korpus, int oi, int oj){
      assert(korpus<nkorpus && h11ee[korpus] && oi < groesse1 && oj < groesse1);
      return h11ee[korpus][sym_index(oi, oj)];
   }
   haeufigkeit_t& ein_ein_k(int korpus, int oi, int oj){
      assert(korpus<nkorpus && h11e[korpus] && oi < groesse1 && oj < groesse1);
      return h11e[korpus][index_bi_flach(oi, oj)];
   }
   haeufigkeit_t& ein_bi(int ok, int oij){
      assert(h21 && ok < groesse1 && oij < groesse2);
      return h21[index_tri21_flach(oij, ok)];
   }
   haeufigkeit_t& ein_k_bi(int korpus, int ok, int oij){
      assert(korpus<nkorpus && h21e[korpus] && ok < groesse1 && oij < groesse2);
      return h21e[korpus][index_tri21_flach(oij, ok)];
   }
   haeufigkeit_t& bi_bi(int oij, int okl){
      assert(h22 && oij < groesse2 && okl < groesse2);
      return h22[sym_index(oij, okl)];
   }

   void varianzen_anlegen();

public:
   Haeufigkeit(const Tastatur& tastatur, const Kodierung& kodierung,
               const std::vector<std::string>& basis,
               const std::vector<double>& gewicht,
               const std::vector<bool>& t,
               bool varianzen,
               bool berichte_unbekanne_zeichen);
   Haeufigkeit(const Haeufigkeit&) = delete;
   // das int-Argument dient lediglich der Unterscheidung vom Copy-Konstruktor
   Haeufigkeit(const Haeufigkeit&, int);

   void setze(const Haeufigkeit& h, const belegung_t p);
   void swap(int p1, int p2);

   ~Haeufigkeit();

   haeufigkeit_t operator()(int p, int e) const
   { assert(p < ntaste && e < nebene); return h1[p][e]; }
   haeufigkeit_t operator()(int p1, int e1, int p2, int e2) const {
      assert(p1 < ntaste && e1 < nebene && p2 < ntaste && e2 < nebene2);
      return h2[p1][p2][e1][e2];
   }
   haeufigkeit_t operator()(int korpus, int p, int e) const {
      assert(korpus < nkorpus && p < ntaste && e < nebene);
      return h1e[korpus][p][e];
   }
   haeufigkeit_t tri(int p1, int p2, int p3, int e3) const {
      assert(trigramme && p1 < ntaste && p2 < ntaste && p3 < ntaste &&
             e3 < nebene);
      return h3[p1][p2][p3][e3];
   }
   int num_korpus() const { return nkorpus; }
   haeufigkeit_t gewicht(int korpus) const
   { assert(korpus < nkorpus); return gewichte[korpus]; }
   bool mit_trigrammen() const { return trigramme; }
   bool mit_varianzen() const { return h11 != nullptr; }

   haeufigkeit_t ein_ein(int oi, int oj) const {
      assert(h11 && oi < groesse1 && oj < groesse1);
      return h11[sym_index(oi, oj)];
   }
   haeufigkeit_t ein_k_ein_k(int korpus, int oi, int oj) const {
      assert(korpus<nkorpus && h11ee[korpus] && oi < groesse1 && oj < groesse1);
      return h11ee[korpus][sym_index(oi, oj)];
   }
   haeufigkeit_t ein_ein_k(int korpus, int oi, int oj) const {
      assert(korpus<nkorpus && h11e[korpus] && oi < groesse1 && oj < groesse1);
      return h11e[korpus][index_bi_flach(oi, oj)];
   }
   haeufigkeit_t ein_bi(int ok, int oij) const {
      assert(h21 && ok < groesse1 && oij < groesse2);
      return h21[index_tri21_flach(oij, ok)];
   }
   haeufigkeit_t ein_k_bi(int korpus, int ok, int oij) const {
      assert(korpus<nkorpus && h21e[korpus] && ok < groesse1 && oij < groesse2);
      return h21e[korpus][index_tri21_flach(oij, ok)];
   }
   haeufigkeit_t bi_bi(int oi, int oj) const {
      assert(h22 && oi < groesse2 && oj < groesse2);
      return h22[sym_index(oi, oj)];
   }
   haeufigkeit_t unbekannt() const { return hunbekannt; }

   void statistik() const;

   // Ein paar Hilfsroutinen, um mehrdimensionale Felder flach umzunummerieren.
   static int sym_index(int i1, int i2){
      const int k = i1 < i2 ? i1 : i2;
      const int g = i1 < i2 ? i2 : i1;
      return (g*(g+1))/2+k;
   }
   static int index_ein_flach(int tastenindex, int ebenenindex)
   { return tastenindex*nebene+ebenenindex; }
   static int index_bi_flach(int idx_ein1, int idx_ein2)
   { return groesse1*idx_ein1+idx_ein2; }
   static int index_tri21_flach(int idx_bi, int idx_ein)
   { return groesse1*idx_bi+idx_ein; }
   static int index_tri_flach(int idx_ein1, int idx_ein2, int idx_ein3)
   { return index_tri21_flach(index_bi_flach(idx_ein1, idx_ein2), idx_ein3); }

   static constexpr int groesse1 = ntaste*nebene;
   static constexpr int groesse2 = groesse1*groesse1;
   static constexpr int groesse3 = groesse1*groesse2;
   static constexpr int groesse11 = (groesse1*(groesse1+1))/2;
   static constexpr int groesse21 = groesse1*groesse2;
   static constexpr int groesse22 = (groesse2*(groesse2+1))/2;
};

#endif // !BELEGUNGSOPTIMIERER_HAEUFIGKEIT_H

//--------------- src/Kodierung.hh ---------------
#ifndef BELEGUNGSOPTIMIERER_KODIERUNG_H
#define BELEGUNGSOPTIMIERER_KODIERUNG_H

//#include "konstanten.hh"
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class Kodierung final {
   std::unordered_map<char32_t, std::pair<int, int>> invers;
   std::unordered_map<char32_t, std::vector<std::pair<int, int>>> inversstr;
   std::string strings[ntaste][nebene], glyphname[ntaste], platzhalterstr;
   char32_t chars[ntaste][nebene];
   unsigned char psenc[ntaste];

public:
   Kodierung(const std::vector<std::u32string>& klartext,
             const std::vector<std::u32string>& ersatzstring,
             const std::vector<std::u32string>& glyph,
             char32_t platzhalter);
   bool ist_platzhalter(int i, int e) const;
   const std::string& txt(int i, int e) const;
   char32_t uchar(int i, int e) const;
   const std::string& bevorzugt(int i) const;
   const std::string& txt(int ie) const;
   std::pair<int, int> position(char32_t z) const;
   const std::vector<std::pair<int,int>>* ersatz(char32_t z) const;
   int psencoding(int i) const;
   std::string psencstr(int i) const;
   const std::string& psglyphname(int i) const;
};

#endif // !BELEGUNGSOPTIMIERER_KODIERUNG_H

//--------------- src/Tastatur.hh ---------------
#ifndef BELEGUNGSOPTIMIERER_TASTATUR_H
#define BELEGUNGSOPTIMIERER_TASTATUR_H

//#include "konstanten.hh"
//#include "typen.hh"
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Tastatur final {
public:
   // Tests, die feststellen, ob eine Fingernummer ein bestimmter Finger
   // (unabhängig von der Hand) ist.
   static bool istDaumen(finger_t i);
   static bool istKleinfinger(finger_t i);

   // Kurze Beschreibung der Kategorie.
   static const char* kategorie_str(int kategorie);

   // Längere Beschreibung des Bigramms aus Taste i und j
   std::string kategorie_lang(int i, int j) const;

   // Spaltennummer der Taste i.
   int spalte(int i) const;
   // Zeilennummer der Taste i.
   int zeile(int i) const;

   // Finger, der Taste i betätigt; der Rückgabewert ist eine der oben
   // definierten Konstanten in finger_t.
   finger_t finger(int i) const;

   // Fingerindex zu Taste i; das ist ein Wert zwischen 0 und nfinger
   // (einschliesslich).
   int finger_index(int i) const;

   // Nummer der Taste, die die Shifttaste ist, die zusammen mit der
   // Symboltaste i benutzt wird, das heisst, die Nummer der Shifttaste, die in
   // der anderen Tastaturhälfte wie Taste i liegt.
   int shifttaste(int i) const;

   // Fingerindex für den Finger, der die Shifttaste bedient, die zusammen mit
   // der Symboltaste i benutzt wird.
   int shift_finger_index(int i) const;

   // Taste in der der Finger, der Taste i bedient, seine Grundstellung hat.
   int grundposition(int i) const;

   // Die Nummer der Taste in Zeile z und Spalte s.  Falls an dieser Position
   // keine Taste liegt, wird -1 zurückgegeben.
   int taste(int z, int s) const;

   // Nummer der Taste, die als seq_taste-te in der Konfiguration aufgeführt
   // ist (Shifttasten nicht mitgezählt).
   int taste(int seq_taste) const;

   // Name der Taste i.
   const std::u32string& name(int i) const;

   // Tastenname -> Tastennummer, -1 falls es den Namen nicht gibt.
   int taste(const std::u32string& n) const;

   // Kategorie, zu der das Tastenbigramm aus Taste i gefolgt von Taste j
   // gehört; die Kategorie wird als eine der oben definierten Konstanten
   // zurückgegeben.
   kategorie_t kategorie(int i, int j) const;

   // Benutzerdefinierte Kategorien, zu dem das Bi- oder Trigramm gemäss der
   // Tasten in i gehört.
   const std::vector<int>& benutzerkategorie(const std::vector<int>& i) const;

   // Nummer eine Benutzerkategorie in Namen umwandeln.
   const std::string& benutzerkategorie_name(int i) const;

   // Testet, ob das Tastenbigramm aus Taste i gefolgt von Taste j eine
   // Handwiederholung ist (dafür gibt es keine eigene Kategorie).
   bool istHandwiederholung(int i, int j) const;

   // Prüft, ob das Tastentrigramm aus Tasten i, j und k ein Wippbewegung ist.
   bool istWippe(int i, int j, int k) const;

   // Koordinate der Taste i; die Längeneinheit des Koordinatensystems ist die
   // Breite einer normalen Taste.  Die Koordinate bezeichnet eine geometrische
   // Position.
   koordinate_t tastenkoord(int i) const;

   // Distanz der Tasten i und j.
   double distanz(int i, int j) const;

   // Teste, ob Finger mit Index i nur Tasten bedient, deren Belegung fest
   // vorgegeben ist.
   bool finger_fix(int i) const;

   // Anzahl der Tasten, die für Zeichen gedacht sind und deren Belegung nicht
   // fest ist.
   int nvariabel() const;

   // Lageaufwand der Taste i.
   double lageaufwand(int i) const;

   Tastatur(const std::vector<taste_t>& tasten,
            const std::unordered_set<std::u32string>& fixe_tasten);

   void neue_kategorien(const std::vector<n_gramm_t>& benutzerkategorie);

private:
   std::u32string namen[ntaste+nshift];
   int spalten[ntaste+nshift], zeilen[ntaste+nshift], nvar;
   finger_t fingertab[ntaste+nshift];
   int fingerind[ntaste+nshift];
   int shifttast[ntaste+nshift], sfingerind[ntaste+nshift], seq[ntaste+nshift];
   int grundpos[ntaste+nshift], tastennr[nzeile][nspalte];
   kategorie_t tastenkategorie[ntaste+nshift][ntaste+nshift];
   double x[ntaste+nshift], y[ntaste+nshift], aufwand[ntaste+nshift];
   bool fix[nfinger+1];
   std::unordered_map<std::u32string, int> namennr;
   std::vector<std::string> kategorie_liste;
   std::map<std::vector<int>, std::vector<int>> benutzerkat;
};

#endif // !BELEGUNGSOPTIMIERER_TASTATUR_H

//--------------- src/Grafik.hh ---------------
#ifndef BELEGUNGSOPTIMIERER_GRAFIK_H
#define BELEGUNGSOPTIMIERER_GRAFIK_H

//#include "typen.hh"
#include <fstream>
#include <string>

class Haeufigkeit;
class Kodierung;
class Konfiguration;
class Tastatur;

class Grafik final {
   std::ofstream grafik;
   int seite;
   bool ungerade;
   const Tastatur& tastatur;
   const Kodierung& kodierung;
public:
   Grafik(const std::string& name, const Tastatur& tastatur,
          const Kodierung& kodierung, const Haeufigkeit& h,
          const Konfiguration& konfiguration);
   ~Grafik();
   void ausgabe(const belegung_t b);
};

#endif // !BELEGUNGSOPTIMIERER_GRAFIK_H

//--------------- src/Statistik.hh ---------------
#ifndef BELEGUNGSOPTIMIERER_STATISTIK_H
#define BELEGUNGSOPTIMIERER_STATISTIK_H

//#include "konstanten.hh"
//#include "typen.hh"
#include <map>
#include <string>

class Aufwandstabelle;
class Haeufigkeit;
class Kodierung;
class Tastatur;

struct Statistik {
   akkumuations_t aeinzel = 0;
   haeufigkeit_t hpos[2][nzeile] = { { 0, 0, 0, 0, 0 },
                                               { 0, 0, 0, 0, 0 } };
   haeufigkeit_t hk[kategorie_t::nKategorie] = { 0 };
   haeufigkeit_t hs[kategorie_t::nKategorie] = { 0 };
   haeufigkeit_t hi0[kategorie_t::nKategorie] = { 0 };
   haeufigkeit_t hi2[kategorie_t::nKategorie] = { 0 };
   haeufigkeit_t hfinger[nfinger+1] = { 0 };
   haeufigkeit_t hkollision1[nfinger] = { 0 }, hkollision2[nfinger] = { 0 };
   haeufigkeit_t hskollision1[nshift] = { 0 }, hskollision2[nshift] = { 0 };
   haeufigkeit_t hnachbar1[nfinger] = { 0 }, hnachbar2[nfinger] = { 0 };
   haeufigkeit_t hsnachbar1[nshift] = { 0 }, hsnachbar2[nshift] = { 0 };
   haeufigkeit_t h1tot = 0, h2tot = 0, hs2tot = 0, h3tot = 0;
   haeufigkeit_t hlinks = 0,  hrechts = 0, hslinks = 0, hsrechts = 0;
   haeufigkeit_t hnachbar = 0, hsnachbar = 0;
   haeufigkeit_t hwippe = 0, hdoppelhw = 0, hkeinhw = 0;
   haeufigkeit_t hrel[3][2] = { {0,0}, {0,0}, {0,0} };
   bool mitfinger[nfinger+1] = { false };

   std::map<int, haeufigkeit_t> hs_benutzer, hk_benutzer, ht_benutzer;
   std::multimap<haeufigkeit_t, std::string> ngramm[3][2];

   Statistik(const belegung_t b, const Tastatur& tastatur,
             const Kodierung& kodierung, const Haeufigkeit& h,
             const Aufwandstabelle& a, const double ngrammakkumlimit[3]);
};

#endif // !BELEGUNGSOPTIMIERER_STATISTIK_H

//--------------- src/Konfiguration.hh ---------------
#ifndef BELEGUNGSOPTIMIERER_KONFIGURATION_H
#define BELEGUNGSOPTIMIERER_KONFIGURATION_H

//#include "konstanten.hh"
#include <memory>
#include <string>
#include <vector>

class Tastatur;
class Kodierung;

class Konfiguration final {
   // Unnormierte Zielhäufigkeit für Finger von links nach rechts
   double fh_unnorm[nfinger];
   // Die Häufigkeiten der Tastendrücke werden auf 1 normiert, positive
   // Abweichungen von den normierten Zielhäufigkeiten quadriert, mit folgenden
   // Gewichten multipliziert und zur Gesamtaufwand addiert:
   double mf_input[nfinger];
   // Multiplikator für Shift-Bigramme.  Mit diesem Faktor werden die normalen
   // Bigrammaufwände für die Bigramme aus erster und letzter Taste (Shift und
   // zweite Symboltaste) in diesen Tastentrigrammen multipliziert.  Erster
   // Wert für normale Bigramme mit positivem Gewicht, zweiter mit negativem
   // Gewicht.
   double mult_shiftindirekt[2];
   // Multiplikator Trigramme, die aufgrund von Bigrammen aus erster und
   // letzter Taste des Trigramms bewertet werden.  Erster Wert für normale
   // Bigramme mit positivem Gewicht, zweiter mit negativem Gewicht.
   double mult_indirekt[2];
   // Der Bequemlichkeit halber haben wir für verschiedene Bigrammkategorien
   // Vorfaktoren für den Aufwand eingeführt.
   double mult_handwiederholung, mult_auswaerts, mult_handwechsel;
   double mult_doppelkomp, mult_zeilenkomp[5];
   // Für Bigramme ohne Hand-, aber mit Zeilenwechsel
   double mult_schraegZS[2];
   double mult_schraegYX[2];
   double add_schraegDX[2];
   // Fixer Anteil des Aufwand für jede Kollision, Daumen...Kleinfinger
   double mult_kollision_konstant[5];
   // Variabler Anteil, wird mit Tastenabstand multipliziert,
   // Daumen...Kleinfinger
   double mult_kollision_distanz[5];
   // Nachbarstrafe Daumen/Zeigefinger, Zeige/Mittelfinger, Mittel/Ringfinger,
   // Ringfinger/Kleinfinger
   double nachbarstrafe[4];
   // Mit diesen Parametern kann für Trigramme mit zwei Handwechseln
   // bzw. solche ganz ohne Handwechsel einen zusätzlichen Aufwand anrechnen.
   double mult_doppelwechsel, mult_doppelwiederholung, mult_wippe;
   // Beliebige Bi- und Trigramme
   double bigramm_roh[ntaste+nshift][ntaste+nshift];
   double trigramm_roh[ntaste+nshift][ntaste+nshift][ntaste+nshift];
   // Für Verwechslungspotenzial/Ähnlichkeit
   double mult_kollision, mult_nachbar, mult_symmetrisch;
   double mult_symmetrisch_gleichzeile, mult_hand_verschieden;
   double verwechslungspotenzial_roh[ntaste+nshift][ntaste+nshift];
   double aehnlichkeit_roh[ntaste][ntaste];
   // Vorlieben
   double vorliebe_roh[ntaste][ntaste];
   double vorliebe_knick;
   // Aufwand aufgrund nicht unterstützter Zeichen.
   double aunbekannt;
   // Die Schriftarten in Postscriptgrafiken.
   std::string _zeichenfont, _beschreibungsfont;
public:
   Konfiguration(const std::vector<std::string>& namen,
                 std::unique_ptr<const Tastatur>& tastatur,
                 std::unique_ptr<const Kodierung>& kodierung);

   // Aufwand für Bigramm der Tasten i und j.
   double bigrammaufwand(int i, int j, const Tastatur& tastatur) const;

   // Aufwand für Trigramm der Tasten i, j und k.
   double trigrammaufwand(int i, int j, int k, const Tastatur& tastatur) const;

   // Das Risiko, Tasten i und j zu verwechseln, ausgedrückt als Aufwand.
   double verwechslungspotenzial(int i, int j, const Tastatur& tastatur) const;

   double vorliebe(int i, int j) const;
   double vorliebenknick() const;
   double aehnlichkeit(int i, int j) const;
   double shiftindirekt(double a) const;
   double indirekt(double a) const;
   double zielhaeufigkeit(int f) const;
   double multfinger(int f) const;
   double unbekannt() const;

   const std::string& zeichenfont() const ;
   const std::string& beschreibungsfont() const;
};

#endif // !BELEGUNGSOPTIMIERER_KONFIGURATION_H

//--------------- src/Aufwandstabelle.hh ---------------
#ifndef BELEGUNGSOPTIMIERER_AUFWANDSTABELLE_H
#define BELEGUNGSOPTIMIERER_AUFWANDSTABELLE_H

//#include "konstanten.hh"
//#include "typen.hh"
#include <cassert>

class Kodierung;
class Konfiguration;
class Tastatur;

class Aufwandstabelle final {
   aufwand_t a3[ntaste][ntaste][ntaste][nebene];
   aufwand_t va2[ntaste][ntaste], vl[ntaste][ntaste], vl_knick;
   aufwand_t ae[ntaste][ntaste], a2[ntaste][ntaste][nebene][nebene2];
   aufwand_t a1[ntaste][nebene], multfinger[nfinger], aunbekannt;
   haeufigkeit_t finger_zielhaeufigkeit[nfinger];
   const bool mit_trigrammen;
   bool mit_vorlieben, mit_aehnlichkeit;

   const Tastatur& tastatur;
public:
   Aufwandstabelle(bool mit_trigrammen, const Tastatur&,
                   const Konfiguration&);

   aufwand_t operator()(int p, int e) const
   { assert(p < ntaste && e < nebene); return a1[p][e]; }
   aufwand_t operator()(int p1, int e1, int p2, int e2) const {
      assert(p1 < ntaste && e1 < nebene && p2 < ntaste && e2 < nebene2);
      return a2[p1][p2][e1][e2];
   }
   aufwand_t tri(int p1, int p2, int p3, int e3) const {
      assert(p1 < ntaste && p2 < ntaste && p3 < ntaste && e3 < nebene);
      return a3[p1][p2][p3][e3];
   }
   akkumuations_t fingerabweichung(int i, akkumuations_t rel) const {
      assert(i < nfinger);  assert(rel <= 1);
      return rel-finger_zielhaeufigkeit[i];
   }
   aufwand_t mult_finger(int fi) const { return multfinger[fi]; }
   aufwand_t unbekannt() const { return aunbekannt; }

   aufwand_t verwechslungspotenzial(int i, int j) const
   { assert(i < ntaste && j < ntaste); return va2[i][j]; }
   bool hat_aehnlichkeit() const { return mit_aehnlichkeit; }
   aufwand_t aehnlichkeit(int i, int j) const
   { assert(i < ntaste && j < ntaste); return ae[i][j]; }
   bool hat_vorlieben() const { return mit_vorlieben; }
   aufwand_t vorliebe(int z, int p) const
   { assert(z < ntaste && p < ntaste); return vl[z][p]; }
   void anzeigen(const Kodierung&, const Konfiguration&) const;

   aufwand_t vorliebenknick() const { return vl_knick; }
   aufwand_t knick(aufwand_t v) const
   { return v < vl_knick ? vl_knick : v; }
};

#endif // !BELEGUNGSOPTIMIERER_AUFWANDSTABELLE_H

//--------------- src/string_in_belegung.hh ---------------
#ifndef BELEGUNGSOPTIMIERER_STRING_IN_BELEGUNG_H
#define BELEGUNGSOPTIMIERER_STRING_IN_BELEGUNG_H

//#include "typen.hh"
#include <string>

class Kodierung;
class Tastatur;

void string_in_belegung(const std::u32string& z, belegung_t b, bool* fest,
                        const Tastatur& tastatur, const Kodierung& kodierung);

#endif //!BELEGUNGSOPTIMIERER_STRING_IN_BELEGUNG_H

//--------------- src/html_markup.hh ---------------
#ifndef BELEGUNGSOPTIMIERER_HTML_MARKUP_H
#define BELEGUNGSOPTIMIERER_HTML_MARKUP_H

#include <string>

class Kodierung;
class Tastatur;

void html_markup(const std::string& textfile,
                 const Kodierung& kodierung,
                 const Tastatur& tastatur,
                 const std::string& referenztastatur);

#endif // !BELEGUNGSOPTIMIERER_HTML_MARKUP_H

//--------------- src/schreibe_belegung.hh ---------------
#ifndef BELEGUNGSOPTIMIERER_SCHREIBE_BELEGUNG_H
#define BELEGUNGSOPTIMIERER_SCHREIBE_BELEGUNG_H

//#include "typen.hh"
#include <string>
#include <unordered_map>

class Aufwandstabelle;
class Haeufigkeit;
class Kodierung;
class Tastatur;

void
schreibe_belegung(const belegung_t b, const Tastatur& tastatur,
                  const Kodierung& kodierung, const Haeufigkeit& h,
                  const Aufwandstabelle& a, const akkumuations_t A,
                  const std::u32string& name,
                  const double ngrammakkumlimit[3],
                  const std::unordered_map<std::string, haeufigkeit_t>&
                  wortliste,
                  const akkumuations_t handeinsatzlimit,
                  bool als_fixeszeichen);

void
schreibe_belegung(const belegung_t b, double A, const Tastatur& tastatur,
                  const Kodierung& kodierung,
                  const std::u32string* name);

void
schreibe_zyklen(const belegung_t b1, const belegung_t b2,
                const Kodierung& kodierung);

#endif // !BELEGUNGSOPTIMIERER_SCHREIBE_BELEGUNG_H

//--------------- src/trennen.hh ---------------
#ifndef BELEGUNGSOPTIMIERER_TRENNEN_H
#define BELEGUNGSOPTIMIERER_TRENNEN_H

//#include "Naechstes_zeichen.hh"
#include <string>
#include <unordered_map>
#include <vector>

class hole_mit_trennung final : public Naechstes_zeichen {
   std::unordered_map<std::u32string, std::vector<char>> trennmuster;
   std::u32string wort;
   std::vector<bool> trennstellen;
   std::unordered_map<std::u32string, std::vector<bool>> cache;
   size_t wort_i, maxlen;
   bool naechste_ist_trennung;
   char32_t rest;

   char32_t fuelle_buffer();
public:
   hole_mit_trennung(const std::string& name, const std::string& tmfile,
                     bool utf8);
   char32_t get() override;
};

void markiere_alle_trennstellen(const std::string& ein, const std::string& aus,
                                const std::string& trennmuster);

#endif // !BELEGUNGSOPTIMIERER_TRENNEN_H

//--------------- src/wortliste.hh ---------------
#ifndef BELEGUNGSOPTIMIERER_WORTLISTE_H
#define BELEGUNGSOPTIMIERER_WORTLISTE_H

//#include "typen.hh"
#include <string>
#include <unordered_map>

class Kodierung;

void lies_wortliste(const std::string& name,
                    std::unordered_map<std::u32string, zaehl_t>& wl,
                    bool muss_existieren);

void lies_wortliste(const std::string name,
                    std::unordered_map<std::string, haeufigkeit_t>& wortliste,
                    const Kodierung& kodierung);

#endif // !BELEGUNGSOPTIMIERER_WORTLISTE_H

//--------------- src/vollkorpus.hh ---------------
#ifndef BELEGUNGSOPTIMIERER_VOLLKORPUS_H
#define BELEGUNGSOPTIMIERER_VOLLKORPUS_H

//#include "typen.hh"
#include <cstdint>
#include <string>
#include <unordered_map>

void lies_vollkorpus(const std::string& name, const std::string* trennmuster,
                     std::unordered_map<std::u32string, zaehl_t>& wl,
                     std::unordered_map<uint64_t, zaehl_t> uh[3]);


#endif // !BELEGUNGSOPTIMIERER_VOLLKORPUS_H

//--------------- src/ngramme.hh ---------------
#ifndef BELEGUNGSOPTIMIERER_NGRAMME_H
#define BELEGUNGSOPTIMIERER_NGRAMME_H

#include <string>
#include <vector>

void erzeuge_ngrammtabellen(const std::vector<std::string>& namen);

#endif // !BELEGUNGSOPTIMIERER_NGRAMME_H

//--------------- src/berechnungen.hh ---------------
#ifndef BELEGUNGSOPTIMIERER_BERECHNUNGEN_H
#define BELEGUNGSOPTIMIERER_BERECHNUNGEN_H

//#include "typen.hh"
#include <unordered_map>

class Aufwandstabelle;
class Grafik;
class Haeufigkeit;
class Kodierung;
class Tastatur;

void
suche_optimum(const Tastatur& tastatur, const Kodierung& kodierung,
              const Haeufigkeit* korpus, const Aufwandstabelle& a,
              akkumuations_t minimum, bool schoene_tastatur, bool alle_guten,
              bool als_fixeszeichen, int saatwert, int iterationen,
              int lebenszeichen, const double ngrammakkumlimit[3],
              const std::unordered_map<std::string, haeufigkeit_t>& wortliste,
              akkumuations_t handeinsatzlimit, Grafik* grafik,
              akkumuations_t* globalesMinimum);

double anzahl_varianten(int N, int n);

void
erzeuge_variationen(const belegung_t ausgangsbelegung,
                    const Tastatur& tastatur, const Kodierung& kodierung,
                    Haeufigkeit& h, const Aufwandstabelle& a,
                    akkumuations_t A, int tiefe,
                    akkumuations_t limit, const bool* fest);

akkumuations_t
aufwandsvarianz(const belegung_t b1, const belegung_t* b2,
                const Tastatur& tastatur,
                const Haeufigkeit& h,
                const Aufwandstabelle& a);

// Konstanter (belegungsunabhängiger) Aufwand.
akkumuations_t
konstanter_aufwand(const Haeufigkeit& h, const Aufwandstabelle& a);

// Variabler (belegungsabhängiger) Aufwand.
akkumuations_t
variabler_aufwand(const belegung_t b, const Haeufigkeit& h,
                  const Tastatur& tastatur, const Aufwandstabelle& a);

#endif // !BELEGUNGSOPTIMIERER_BERECHNUNGEN_H

//--------------- src/Konfiguration.cc ---------------
//#include "Konfiguration.hh"

//#include "Eingabestream.hh"
//#include "Kodierung.hh"
//#include "Tastatur.hh"
//#include "typen.hh"
//#include "utfhilfe.hh"
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <unordered_map>
#include <utility>

namespace {

using herkunft_t = std::pair<size_t, std::string>;
using str_herkunft_t = std::unordered_map<std::u32string, herkunft_t>;
using char_herkunft_t = std::unordered_map<char32_t, herkunft_t>;
using zs_herkunft_t =
              std::map<std::pair<int, int>, std::pair<std::u32string, int>>;
using gpos_herkunft_t = std::unordered_map<int, std::u32string>;

std::u32string hole_string(Eingabestream& f, size_t lmin = 0,
                           size_t lmax = std::numeric_limits<size_t>::max()){
   std::u32string w;
   if(f.hole_string(w)){
      if(w.length() >= lmin && w.length() <= lmax) return w;
      std::cerr << SPRACHE("Der String '", "The string '")
                << utf32_in_ausgabe(w) << SPRACHE("' ist zu ", "' is too ")
                << (w.length() < lmin
                    ? SPRACHE("kurz", "short") : SPRACHE("lang", "long"))
                << (w.length() < lmin ? ". Minimal" : ". Maximal")
                << SPRACHE("e L" strAe "nge: ", " length: ")
                << (w.length() < lmin ? lmin : lmax) << std::endl;
   }else{
      std::cerr << SPRACHE("Ein String wurde erwartet, das abschliessende Anf"
                           strUe "hrungszeichen jedoch nicht gefunden.",
                           "A string has been expected, however, the closing "
                           "quotation mark has not been found.") << std::endl;
   }
   f.fehler();
}

std::u32string hole_wort(Eingabestream& f){
   std::u32string w;
   if(f.hole_wort(w)) return w;
   f.fehler();
}

void pruefe_tastenname(Eingabestream& f, const std::u32string& name,
                       const str_herkunft_t& tn_herkunft)
{
   if(tn_herkunft.find(name) == tn_herkunft.end()){
      std::cerr << SPRACHE("Die Taste '", "The key '") << utf32_in_ausgabe(name)
                << SPRACHE("' ist unbekannt.", "' is not known.") << std::endl;
      f.fehler();
   }
}

std::vector<std::u32string>
hole_tastenliste(Eingabestream& f, const str_herkunft_t& tn_herkunft){
   std::vector<std::u32string> namen;
   std::u32string name;
   while(f.hole_wort(name)){
      pruefe_tastenname(f, name, tn_herkunft);
      namen.push_back(name);
   }
   if(namen.size() >= 1) return namen;
   std::cerr << SPRACHE("Die Liste von Tasten darf nicht leer sein.",
                        "The list of keys must not be empty.") << std::endl;
   f.fehler();
}

int hole_int(Eingabestream& f, int rmin = -std::numeric_limits<int>::max(),
             int rmax = std::numeric_limits<int>::max()){
   const double w = hole_zahl(f, rmin, rmax);
   const int i = static_cast<int>(w);
   if(static_cast<double>(i) != w){
      std::cerr << SPRACHE("Ganze Zahl erwartet, ",
                           "Expected an integer, found ") << w
                << SPRACHE(" gefunden.", ".") << std::endl;
      f.fehler();
   }
   return i;
}

bool hole_flag(Eingabestream& f){
   bool w;
   if(f.hole_flag(w)) return w;
   std::cerr << SPRACHE(
      "Ein Flag ('-' oder '+') wurde erwartet, jedoch nicht gefunden.",
      "A flag ('-' or '+') has been expected, however, none was found.")
             << std::endl;
   f.fehler();
}

void das_wars(Eingabestream& f){
   if(!f.zeilenende()){
      if(f.ist(U'#')) f.uebergehen();
      else{
         std::cerr << SPRACHE(strUe "berf" strUe "ssiger Text am Zeilenende.",
                              "Excess input at end of the line.") << std::endl;
         f.fehler();
      }
   }
}

std::u32string hole_wort_oder_leer(Eingabestream& f){
   std::u32string w = U"";
   if(!f.zeilenende() && !f.ist(U'#')) f.hole_wort(w);
   return w;
}

std::u32string hole_string_oder_leer(Eingabestream& f){
   if(!f.zeilenende() && !f.ist(U'#')) return hole_string(f, 1);
   return U"";
}

int hole_flag_oder_leer(Eingabestream& f){
   if(!f.zeilenende() && !f.ist(U'#')) return hole_flag(f) ? 1 : -1;
   return 0;
}

void schonmal(Eingabestream& f, herkunft_t& h){
   std::cerr << SPRACHE("' wurde bereits in File ",
                        "' has already been introduced in file ")
             << h.second << SPRACHE(", Zeile ", ", line ")
             << h.first << SPRACHE(" eingef" strUe "hrt.", ".") << std::endl;
}

void tastenname_eindeutigkeit(Eingabestream& f, const std::u32string& name,
                              str_herkunft_t& tn_herkunft){
    const auto p = tn_herkunft.find(name);
    if(p != tn_herkunft.end()){
       std::cerr << SPRACHE("Der Tastenname '", "The key name '")
                 << utf32_in_ausgabe(name);
       schonmal(f, p->second);
       f.fehler();
    }
    tn_herkunft[name] = std::make_pair(f.aktuelle_zeile(), f.aktuelles_file());
}

void fixtasten_eindeutigkeit(Eingabestream& f, const std::u32string& name,
                             str_herkunft_t& herkunft){
   const auto p = herkunft.find(name);
   if(p != herkunft.end()){
      std::cerr << SPRACHE("Der Tastenname '", "The key name '")
                << utf32_in_ausgabe(name)
                << SPRACHE("' wurde in File ", "' has already been used for "
                           "'FixesZeichen' in file ")
                << p->second.second << SPRACHE(", Zeile ", ", line ")
                << p->second.first
                << SPRACHE(" bereits f" strUe "r 'FixesZeichen' verwendet.",".")
                << std::endl;
      f.fehler();
   }
   herkunft[name] = std::make_pair(f.aktuelle_zeile(), f.aktuelles_file());
}

void zs_eindeutigkeit(Eingabestream& f, int spalte, int zeile, int finger,
                      const std::u32string& name, zs_herkunft_t& zs_herkunft){
   const auto zs = std::make_pair(spalte, zeile);
   const auto p = zs_herkunft.find(zs);
   if(p != zs_herkunft.end() && p->second.second != finger){
      std::cerr << SPRACHE("Spalte ", "Column ") << spalte
                << SPRACHE(", Zeile ", " row ") << zeile
                << SPRACHE(" wurde bereits der Taste '",
                           " has already been assigned to the key '")
                << utf32_in_ausgabe(p->second.first)
                << SPRACHE("' zugewiesen, und diese ist Finger ",
                           "' which is associated to finger ")
                << p->second.second
                << SPRACHE(" statt Finger ", " instead of finger ")
                << finger
                << SPRACHE(" zugeordnet.", ".") << std::endl;
      f.fehler();
   }
   zs_herkunft[zs] = std::make_pair(name, finger);
}

void gpos_eindeutigkeit(Eingabestream& f, int finger,
                        const std::u32string& name,
                        gpos_herkunft_t& gpos_herkunft){
   const auto p = gpos_herkunft.find(finger);
   if(p != gpos_herkunft.end()){
      std::cerr << "Finger " << finger
                << SPRACHE(" wurde bereits die Taste '",
                           " has been assigned the key '")
                << utf32_in_ausgabe(p->second)
                << SPRACHE("' als Grundposition zugewiesen.",
                           "' as rest position already.") << std::endl;
      f.fehler();
   }
   gpos_herkunft[finger] = name;
}

taste_t hole_taste(Eingabestream& f, str_herkunft_t& tn_herkunft,
                   zs_herkunft_t& zs_herkunft,
                   gpos_herkunft_t& gpos_herkunft, int minf,int maxf)
{
   const std::u32string name = hole_wort(f);
   tastenname_eindeutigkeit(f, name, tn_herkunft);
   const int spalte = hole_int(f, 0,nspalte-1);
   const int zeile = hole_int(f, 0, nzeile-1);
   const double x = hole_zahl(f, -5., 20.), y = hole_zahl(f, -5., 10.);
   const finger_t finger(hole_int(f, minf, maxf));
   zs_eindeutigkeit(f, spalte, zeile, finger, name, zs_herkunft);
   const bool gpos = hole_flag(f);
   if(gpos) gpos_eindeutigkeit(f, finger, name, gpos_herkunft);
   const double aufwand = hole_zahl(f, 0, 1e15);
   const int optss = hole_flag_oder_leer(f);
   const bool seite = optss ? optss > 0 : finger > 0;
   taste_t t{name, x,y, aufwand, spalte, zeile, finger, gpos, seite};
   das_wars(f);
   return t;
}

std::u32string hole_tastennamen(Eingabestream& f,
                                const str_herkunft_t& tn_herkunft){
   const std::u32string name = hole_wort(f);
   pruefe_tastenname(f, name, tn_herkunft);
   return name;
}

void nur_bekannte_zeichen(Eingabestream& f, const std::u32string& s,
                          const char_herkunft_t& zeichen_herkunft){
   for(size_t i = 0; i < s.length(); ++i){
      if(zeichen_herkunft.find(s[i]) == zeichen_herkunft.end()){
         std::cerr << SPRACHE("Das Zeichen '", "The symbol '")
                   << utf32_in_ausgabe(s[i])
                   << SPRACHE("' ist unbekannt.", "' is unknown.") << std::endl;
         f.fehler(i+1);
      }
   }
}

void zeichen_eindeutigkeit(Eingabestream& f, const std::u32string& klartext,
                           char32_t platzhalter,
                           char_herkunft_t& zeichen_herkunft){
   for(size_t j = 0; j < klartext.length(); ++j){
      if(klartext[j] != platzhalter){
         const auto p = zeichen_herkunft.find(klartext[j]);
         if(p != zeichen_herkunft.end()){
            std::cerr << SPRACHE("Das Zeichen '", "The symbol '")
                      << utf32_in_ausgabe(klartext[j]);
            schonmal(f, p->second);
            f.fehler(j+1);
         }
      }else if(j || klartext.length() == 1u){
         std::cerr << SPRACHE("Das Platzhalter-Zeichen '",
                              "The placeholder symbol '")
                   << utf32_in_ausgabe(klartext[j])
                   << SPRACHE("' darf nur in Ebene 1 einer Taste mit "
                              "mehreren Ebenen erscheinen.",
                              "' must only appear on level 1 of a "
                              "key with multiple levels.") << std::endl;
         f.fehler();
      }
   }
   for(auto j : klartext)
      zeichen_herkunft[j] = std::make_pair(f.aktuelle_zeile(),
                                           f.aktuelles_file());
}

}



Konfiguration::
Konfiguration(const std::vector<std::string>& namen,
              std::unique_ptr<const Tastatur>& tastatur,
              std::unique_ptr<const Kodierung>& kodierung)
{
   struct stdaufwaende {
      double* tabelle;
      size_t n;
      double wmin, wmax;
   };
   std::unordered_map<std::u32string, stdaufwaende> aufwaende;
   aufwaende[U"Zielh\u00e4ufigkeit"]= { fh_unnorm, nfinger, 0., 1e15 };
   aufwaende[U"Fingerbelastung"]   = { mf_input, nfinger, 0., 1e15 };
   aufwaende[U"Shiftbigramm"]      = { mult_shiftindirekt, 2, 0., 1. };
   aufwaende[U"Indirekt"]          = { mult_indirekt, 2, 0., 1. };
   aufwaende[U"Handwiederholung"]  = { &mult_handwiederholung, 1, -1e15, 1e15 };
   aufwaende[U"Ausw\u00e4rts"]     = { &mult_auswaerts, 1, -1e15, 1e15 };
   aufwaende[U"Handwechsel"]       = { &mult_handwechsel, 1, -1e15, 1e15 };
   aufwaende[U"DoppeltRabatt"]     = { &mult_doppelkomp, 1, 0., 1. };
   aufwaende[U"Schr\u00e4gZS"]     = { mult_schraegZS, 2, -1e15, 1e15 };
   aufwaende[U"Schr\u00e4gYX"]     = { mult_schraegYX, 2, -1e15, 1e15 };
   aufwaende[U"Schr\u00e4gNenner0"]= { add_schraegDX, 2, 0, 1e15 };
   aufwaende[U"Doppelwechsel"]     = { &mult_doppelwechsel, 1, -1e15, 1e15 };
   aufwaende[U"Doppelwiederholung"]= { &mult_doppelwiederholung, 1, -1e15,1e15};
   aufwaende[U"Wippe"]             = { &mult_wippe, 1, -1e15, 1e15 };
   aufwaende[U"Fehlt"]             = { &aunbekannt, 1, 0, 1e15 };
   aufwaende[U"KollisionKonstant"] = { mult_kollision_konstant, 5, -1e15, 1e15};
   aufwaende[U"KollisionDistanz"]  = { mult_kollision_distanz, 5, -1e15, 1e15 };
   aufwaende[U"Nachbar"]           = { nachbarstrafe, 4, -1e15, 1e15 };
   aufwaende[U"VPKollision"]       = { &mult_kollision, 1, -1e15, 1e15 };
   aufwaende[U"VPNachbar"]         = { &mult_nachbar, 1, -1e15, 1e15 };
   aufwaende[U"VPHandwechsel"]     = { &mult_hand_verschieden, 1, -1e15, 1e15 };
   aufwaende[U"VPSymmetrisch"]     = { &mult_symmetrisch, 1, -1e15, 1e15 };
   aufwaende[U"VPSymmetrischGleicheZeile"]= { &mult_symmetrisch_gleichzeile,
                                              1, -1e15, 1e15 };
   aufwaende[U"ZeilenwiederholungRabatt"] = { mult_zeilenkomp, 5, 0., 1. };
#ifdef EXPERIMENTELL
   aufwaende[U"VorliebeKnick"] = { &vorliebe_knick, 1, -1e15, 1e15 };
#endif // !EXPERIMENTELL
   struct bigramm {
      bigramm(const std::u32string& T1, const std::u32string& T2,
              const std::u32string& N, double w)
         : t1(T1), t2(T2), name(N), a(w){}
      std::u32string t1, t2, name; double a;
   };
   struct verwechslungspotenzial {
      verwechslungspotenzial(const std::u32string& T1, const std::u32string& T2,
                             double w) : t1(T1), t2(T2), a(w){}
      std::u32string t1, t2; double a;
   };
   struct trigramm {
      trigramm(const std::u32string& T1, const std::u32string& T2,
               const std::u32string& T3, const std::u32string& N, double w)
         : t1(T1), t2(T2), t3(T3), name(N), a(w){}
      std::u32string t1, t2, t3, name; double a;
   };
   struct vorliebe {
      vorliebe(const std::u32string& Z, double w,
               const std::vector<std::u32string>& T) : z(Z), a(w), t(T){}
      std::u32string z; double a; std::vector<std::u32string> t;
   };
   std::vector<bigramm> bigramme;
   std::vector<trigramm> trigramme;
   std::vector<verwechslungspotenzial> verwechslungspot;
   std::vector<n_gramm_t> benutzerkategorien;
   std::vector<std::u32string> klartext, ersatzstring, glyphnamen;
   std::unordered_map<std::u32string, std::u32string> fixedtext;
   std::unordered_map<std::u32string, std::u32string> fixedglyphs;
   char_herkunft_t zeichen_herkunft;
   gpos_herkunft_t gpos_herkunft;
   zs_herkunft_t zs_herkunft;
   str_herkunft_t tn_herkunft, fixtasten;
   std::vector<taste_t> tasten;
   std::vector<std::pair<std::u32string, double>> aehnlichkeit;
   std::vector<vorliebe> vorlieben;
   std::vector<bool> utf8(namen.size(), true);
   taste_t ShiftL, ShiftR;
   herkunft_t platzhalter_herkunft;
   char32_t platzhalter;
   bool mitSL, mitSR, nochmal;

   do{
      for(auto& i : fh_unnorm) i = 1;
      for(auto& i : mult_shiftindirekt) i = 1;
      for(auto& i : mult_indirekt) i = 1;
      vorliebe_knick = 1e15;
      _zeichenfont = "Courier-Bold";
      _beschreibungsfont = "Courier-Bold";

      bigramme.clear(); trigramme.clear(); verwechslungspot.clear();
      klartext.clear(); glyphnamen.clear(); ersatzstring.clear();
      fixedtext.clear();  fixedglyphs.clear(); fixtasten.clear();
      tasten.clear();  benutzerkategorien.clear();
      zeichen_herkunft.clear();  tn_herkunft.clear();
      zs_herkunft.clear(); gpos_herkunft.clear();
      aehnlichkeit.clear();
      vorlieben.clear();
      mitSL = mitSR = nochmal = false;
      platzhalter = 0;
      for(size_t i = 0; !nochmal && i < namen.size(); ++i){
         Eingabestream f(namen[i], utf8[i]);
         while(f.echte_neuezeile()){
            std::u32string wort;
            if(!f.hole_wort(wort)) f.fehler();
            if(wort == U"Taste"){
               tasten.push_back(hole_taste(f, tn_herkunft, zs_herkunft,
                                           gpos_herkunft, -5, 5));
            }else if(wort == U"ShiftL"){
               ShiftL = hole_taste(f, tn_herkunft, zs_herkunft,
                                   gpos_herkunft, -5, -1);
               mitSL = true;
            }else if(wort == U"ShiftR"){
               ShiftR = hole_taste(f, tn_herkunft, zs_herkunft,
                                   gpos_herkunft, 1, 5);
               mitSR = true;
            }else if(wort == U"Platzhalter"){
               if(platzhalter){
                  std::cerr << "'Platzhalter";
                  schonmal(f, platzhalter_herkunft);
                  f.fehler();
               }
               if(klartext.size() || fixedtext.size()){
                  std::cerr << "'Platzhalter' "
                     SPRACHE("muss vor der ersten Zeichenfestlegung stehen.",
                             "must appear before the first symbol definition.")
                            << std::endl;
                  f.fehler();
               }
               platzhalter = hole_string(f, 1, 1)[0];
               platzhalter_herkunft =
                  std::make_pair(f.aktuelle_zeile(), f.aktuelles_file());
            }else if(wort == U"Zeichen"){
               const auto zeichen = hole_string(f, 1);
               zeichen_eindeutigkeit(f, zeichen, platzhalter, zeichen_herkunft);
               klartext.push_back(zeichen);
               glyphnamen.push_back(hole_wort_oder_leer(f));
               das_wars(f);
            }else if(wort == U"FixesZeichen"){
               const auto taste = hole_tastennamen(f, tn_herkunft);
               if((mitSL && taste == ShiftL.name) ||
                  (mitSR && taste == ShiftR.name)){
                  std::cerr << SPRACHE(
                     "Shifttasten d" strUe "rfen nicht in 'FixesZeichen' "
                     "verwendet werden.",
                     "Shift keys must not be used in 'FixesZeichen'.")
                            << std::endl;
                  f.fehler();
               }
               fixtasten_eindeutigkeit(f, taste, fixtasten);
               const auto zeichen = hole_string(f, 1);
               zeichen_eindeutigkeit(f, zeichen, platzhalter, zeichen_herkunft);
               const auto glyph = hole_wort_oder_leer(f);
               das_wars(f);
               fixedglyphs[taste] = glyph;
               fixedtext[taste] = zeichen;
            }else if(wort == U"Vorliebe"){
               const std::u32string s = hole_string(f, 1);
               nur_bekannte_zeichen(f, s, zeichen_herkunft);
               vorlieben.push_back(vorliebe{s, -hole_zahl(f, -1e15, 1e15),
                                            hole_tastenliste(f, tn_herkunft) });
            }else if(wort == U"Ersatz"){
               ersatzstring.push_back(hole_string(f, 2));
               das_wars(f);
            }else if(wort == U"\u00c4hnlich"){
               const std::u32string s = hole_string(f, 2);
               nur_bekannte_zeichen(f, s, zeichen_herkunft);
               aehnlichkeit
                  .push_back(std::make_pair(s, hole_zahl(f, 0, 1e15)));
               das_wars(f);
            }else if(wort == U"Bigramm"){
               const std::u32string t1 = hole_tastennamen(f, tn_herkunft);
               const std::u32string t2 = hole_tastennamen(f, tn_herkunft);
               const double aufwand = hole_zahl(f, -1e15, 1e15);
               const std::u32string name = hole_string_oder_leer(f);
               das_wars(f);
               bigramme.push_back(bigramm{t1, t2, name, aufwand});
            }else if(wort == U"Verwechslungspotenzial"){
               verwechslungspot.push_back
                  (verwechslungspotenzial{hole_tastennamen(f, tn_herkunft),
                                          hole_tastennamen(f,tn_herkunft),
                                          hole_zahl(f, -1e15, 1e15)});
               if(verwechslungspot.back().t1 == verwechslungspot.back().t2){
                  std::cerr << SPRACHE("Ein Verwechslungspotenzial gibt es nur "
                                             "zwischen verschiedenen Tasten.",
                                       "Confusability only exists between "
                                       "different keys.") << std::endl;
                  f.fehler();
               }
               das_wars(f);
            }else if(wort == U"Trigramm"){
               const std::u32string t1 = hole_tastennamen(f, tn_herkunft);
               const std::u32string t2 = hole_tastennamen(f, tn_herkunft);
               const std::u32string t3 = hole_tastennamen(f, tn_herkunft);
               const double aufwand = hole_zahl(f, -1e15, 1e15);
               const std::u32string name = hole_string_oder_leer(f);
               das_wars(f);
               trigramme.push_back(trigramm{t1, t2, t3, name, aufwand});
            }else if(wort == U"Zeichenfont"){
               _zeichenfont = utf32_in_ausgabe(hole_wort(f));
               das_wars(f);
            }else if(wort == U"Beschreibungsfont"){
               _beschreibungsfont = utf32_in_ausgabe(hole_wort(f));
               das_wars(f);
            }else{
               const auto k = aufwaende.find(wort);
               if(k == aufwaende.end()){
                  std::cerr << SPRACHE("Unbekanntes Schl" strUe "sselwort ",
                                       "Unknown keyword ")
                            << utf32_in_ausgabe(wort) << std::endl;
                  f.fehler();
               }else{
                  double* a = k->second.tabelle;
                  const size_t n = k->second.n;
                  const double wmin = k->second.wmin, wmax = k->second.wmax;
                  for(size_t j = 0; j < n; ++j) a[j] = hole_zahl(f, wmin, wmax);
                  das_wars(f);
               }
            }
         }
         if(f.encoding_geaendert()){
            nochmal = f.encoding_geaendert();
            utf8[i] = false;
         }
      }
   }while(nochmal);

   if(tasten.size() != ntaste){
      std::cerr << "'Taste' " SPRACHE("kommt ", "appears ")
                << tasten.size() << SPRACHE(" mal vor, erwartet ist ",
                                            " times, expected is ")
                << ntaste
                << SPRACHE(" mal.  Sie k" strOe "nnen entweder " strAe "ndern, "
                           "wie oft 'Taste' vorkommt oder die erwartete "
                           "Tastenzahl mit der Compileroption",
                           " times.  You can either fix the number of "
                           "occurrences of 'Taste', or adjust the number of "
                           "expected keys by using the compiler option")
                           " '-DTASTENZAHL=" << tasten.size()+nshift
                << SPRACHE("' einstellen.", "'.") << std::endl;
      exit(1);
   }

   // Die Zahl der Shifttasten festgelegen stimmen.
   if(nshift != (mitSL ? 1 : 0)+(mitSR ? 1 : 0)){
      if(!mitSL)
         std::cerr << SPRACHE("ShiftL wurde nicht festgelegt.",
                              "ShiftL has not been defined.") << std::endl;
      if(!mitSR)
         std::cerr << SPRACHE("ShiftR wurde nicht festgelegt.",
                              "ShiftR has not been defined.") << std::endl;
      if(mitSL && mitSR)
         std::cerr << SPRACHE("ShiftL und ShiftR wurden beide festgelegt.",
                              "ShiftL and ShiftR have both been defined.")
                   << std::endl;
      exit(1);
   }

   if(mitSL) tasten.push_back(ShiftL);
   if(mitSR) tasten.push_back(ShiftR);

   std::unordered_set<std::u32string> fixset;
   for(const auto& i : fixtasten) fixset.insert(i.first);
   Tastatur* neuetastatur = new Tastatur(tasten, fixset);
   tastatur = std::unique_ptr<const Tastatur>(neuetastatur);

   for(int i = tastatur->nvariabel(); i < ntaste; ++i){
      klartext.push_back(fixedtext[tastatur->name(i)]);
      glyphnamen.push_back(fixedglyphs[tastatur->name(i)]);
   }
   kodierung = std::unique_ptr<const Kodierung>
      (new Kodierung(klartext, ersatzstring, glyphnamen, platzhalter));

   for(const auto& i : bigramme){
      const int i1 = tastatur->taste(i.t1), i2 = tastatur->taste(i.t2);
      bigramm_roh[i1][i2] = i.a;
      if(i.name.length())
         benutzerkategorien.push_back(n_gramm_t{i1, i2, -1, i.name});
   }
   for(const auto& i : verwechslungspot){
      const int i1 = tastatur->taste(i.t1), i2 = tastatur->taste(i.t2);
      verwechslungspotenzial_roh[i1][i2] =
         verwechslungspotenzial_roh[i2][i1] = i.a;
   }
   for(const auto& i : trigramme){
      const int i1 = tastatur->taste(i.t1), i2 = tastatur->taste(i.t2);
      const int i3 = tastatur->taste(i.t3);
      trigramm_roh[i1][i2][i3] = i.a;
      if(i.name.length())
         benutzerkategorien.push_back(n_gramm_t{i1, i2, i3, i.name});
   }
   for(const auto& i : vorlieben){
      for(const auto& j : i.t){
         const int taste = tastatur->taste(j);
         const double aufwand = i.a;
         for(const auto& s : i.z){
            const auto z = kodierung->position(s);
            vorliebe_roh[z.first][taste] += aufwand;
         }
      }
   }

   neuetastatur->neue_kategorien(benutzerkategorien);

   for(const auto& i : aehnlichkeit){
      const std::u32string t = i.first;
      const double w = i.second;
      for(size_t j = 0; j+1 < t.length(); ++j){
         const auto p1 = kodierung->position(t[j]);
         for(size_t k = j+1; k < t.length(); ++k){
            const auto p2 = kodierung->position(t[k]);
            aehnlichkeit_roh[p2.first][p1.first] += w;
            aehnlichkeit_roh[p1.first][p2.first] += w;
         }
      }
   }
}

double
Konfiguration::
bigrammaufwand(int i, int j, const Tastatur& tastatur) const {
   const int zeile_i = tastatur.zeile(i);
   const int spalte_i = tastatur.spalte(i);
   const finger_t finger_i = tastatur.finger(i);

   const auto zeile_j = tastatur.zeile(j);
   const int spalte_j = tastatur.spalte(j);
   const finger_t finger_j = tastatur.finger(j);
   const int grundpos_j = tastatur.grundposition(j);

   if(tastatur.kategorie(i,j) == kategorie_t::Handwechsel){
      return mult_handwechsel+bigramm_roh[i][j];
   }else if(tastatur.kategorie(i,j) == kategorie_t::MitUndefDaumen){
      return bigramm_roh[i][j];
   }else if(tastatur.kategorie(i,j) == kategorie_t::Doppeltanschlag){
      // Wenn wir eine Taste abseits der Grundposition zweimal anschlagen,
      // müssen wir den Finger nur einmal dorthin bewegen, der Mehraufwand
      // gegenüber dem Anschlag auf der Grundposition entfällt somit beim
      // zweiten Anschlag.
      return mult_doppelkomp*
         (tastatur.lageaufwand(grundpos_j)-tastatur.lageaufwand(j))
         +bigramm_roh[i][j];
   }else if(tastatur.kategorie(i,j) == kategorie_t::Kollision){
      // Kollisionen sind umso schlimmer, je weiter der Finger springen muss.
      const int idx = std::abs(finger_i)-finger_t::DaumenRechts;
      return mult_handwiederholung+mult_kollision_konstant[idx]+
         mult_kollision_distanz[idx]*tastatur.distanz(i,j)+bigramm_roh[i][j];
   }else{
      // Verschiedene Finger auf derselben Hand.
      const bool auswaerts = tastatur.kategorie(i,j) == kategorie_t::Auswaerts;
      const double dspalte = std::abs(spalte_i-spalte_j);
      const double dx =
         std::abs(tastatur.tastenkoord(i).x-tastatur.tastenkoord(j).x);
      const double dy =
         std::abs(tastatur.tastenkoord(i).y-tastatur.tastenkoord(j).y);
      const double dfinger = std::abs(finger_i-finger_j);
      const double dzeile = std::abs(zeile_i-zeile_j);
      const int minfingerindex =
         std::min(std::abs(finger_i), std::abs(finger_j))
         -finger_t::DaumenRechts;
      const bool mitDaumen = (minfingerindex == 0);
      // Wenn wir eine Taste in einer Zeile abseits der Grundzeile anschlagen
      // bewegen wir die Hand dorthin, folgende Anschläge von anderen Tasten
      // in dieser Zeile wird leichter.
      const int dspaltem1 = std::abs(std::abs(spalte_i-spalte_j)-1);
      const double zkomp = mult_zeilenkomp[dspaltem1 < 5 ? dspaltem1 : 4];
      const double komp =
         (!mitDaumen && dzeile == 0 && zeile_i != zeilen_t::Mittelzeile &&
          (zeile_i != zeilen_t::Untere_Zeile ||
           !Tastatur::istKleinfinger(finger_i)))
         ? zkomp*(tastatur.lageaufwand(grundpos_j)-tastatur.lageaufwand(j))
         : 0;

      const double offset = add_schraegDX[finger_i > 0];
      const double nZS = dspalte+offset, nYX = dx+offset;
      const double zZS = mitDaumen
         ? 0. : mult_schraegZS[finger_i > 0]*dzeile;
      const double zYX = mitDaumen
         ? 0. : mult_schraegYX[finger_i > 0]*dy;
      const double zaehler = zZS*nYX+zYX*nZS, nenner = nZS*nYX;
      if((nZS == 0. && zZS != 0.) || (nYX == 0. && zYX != 0.)){
         std::cerr << SPRACHE(
            "Division durch Null f" strUe "r schr" strAe "ge Griffe.  "
            "Dieser Fehler kann auftreten, wenn verschiedene Finger "
            "Tasten in derselben Zeile bedienen.  'Schr" strAe
            "gNenner0' schafft Abhilfe.",
            "Division by zero for hand distorting digrams.  This "
            "error can occur if different fingers operate keys in "
            "the same column. 'Schr" strAe "gNenner0' can solve this.")
                   << std::endl;
         exit(1);
      }
      const double quotient = zaehler == 0. ? 0. : zaehler/nenner;

      return komp+
         // Aufwand für Handwiederholung.
         mult_handwiederholung
         // Zusatzaufwand für Auswärtsbewegung.
         +(auswaerts && !mitDaumen ? mult_auswaerts : 0)
         // Aufwand wenn die Tasten benachbart sind
         +(dfinger == 1 ? nachbarstrafe[minfingerindex] : 0)
         // Aufwand, wenn die Tasten in verschiedenen Zeilen liegen; je
         // schräger der Griff desto schlimmer.
         +quotient
         +bigramm_roh[i][j];
   }
}

double
Konfiguration::
trigrammaufwand(int i, int j, int k, const Tastatur& tastatur) const {
   // Sinnvoll sind Aufwände nur, wenn erste und letzte Taste auf derselben
   // Hand sind.
   if(!tastatur.istHandwiederholung(i,k)) return trigramm_roh[i][j][k];

   // Ansonsten bauen wir die Trigrammaufwände aus den Bigrammaufwänden.  bi
   // ist der Bigrammaufwand für die erste und dritte Taste:
   const double bi = bigrammaufwand(i, k, tastatur);
   const double ik = mult_indirekt[bi < 0]*bi;
   if(tastatur.istHandwiederholung(i,j)){
      // Drei Tasten mit derselben Hand.
      const double extra = trigramm_roh[i][j][k]+mult_doppelwiederholung+
         (tastatur.istWippe(i, j, k) ? mult_wippe : 0);
      if(ik <= 0) return extra;
      const double ij = bigrammaufwand(i, j, tastatur);
      const double jk = bigrammaufwand(j, k, tastatur);
      // Insgesamt sollte der Aufwand mindestens so hoch wie er gewesen
      // wäre, wenn die mittlere Taste auf der anderen Hand gelegen wäre.
      // Man könnte vielleicht in einigen Fällen mehr berechnen, aber sicher
      // nicht weniger.
      return ik > ij+jk ? extra+ik-ij-jk : extra;
   }else
      // Zwei Handwechsel.
      return ik+mult_doppelwechsel+trigramm_roh[i][j][k];
}

double
Konfiguration::
verwechslungspotenzial(int i, int j, const Tastatur& tastatur) const {
   const finger_t finger_i = tastatur.finger(i);
   const finger_t finger_j = tastatur.finger(j);
   if(!tastatur.istHandwiederholung(i,j)){
      if(!Tastatur::istDaumen(finger_i) &&
         std::abs(finger_i) == std::abs(finger_j)){
         const int zi = tastatur.zeile(i), zj = tastatur.zeile(j);
         return (zi == zj ? mult_symmetrisch_gleichzeile : mult_symmetrisch)
            +mult_hand_verschieden
            +verwechslungspotenzial_roh[i][j];
      }else return mult_hand_verschieden
               +verwechslungspotenzial_roh[i][j];
   }else if(tastatur.kategorie(i,j) == kategorie_t::Kollision){
      return mult_kollision+verwechslungspotenzial_roh[i][j];
   }else{
      return (std::abs(finger_i-finger_j) == 1 ? mult_nachbar : 0)
         +verwechslungspotenzial_roh[i][j];
   }
}

double
Konfiguration::vorliebe(int i, int j) const {
   assert(i < ntaste+nshift && j < ntaste+nshift);
   return vorliebe_roh[i][j];
}

double
Konfiguration::vorliebenknick() const
{ return vorliebe_knick; }

double
Konfiguration::aehnlichkeit(int i, int j) const {
   assert(i < ntaste+nshift && j < ntaste+nshift);
   return aehnlichkeit_roh[i][j];
}

double
Konfiguration::shiftindirekt(double a) const
{ return mult_shiftindirekt[a < 0]; }

double
Konfiguration::indirekt(double a) const
{ return mult_indirekt[a < 0]; }

double
Konfiguration::zielhaeufigkeit(int f) const {
   assert(f < nfinger);
   return fh_unnorm[f];
}

double
Konfiguration::multfinger(int f) const {
   assert(f < nfinger);
   return mf_input[f];
}

double
Konfiguration::unbekannt() const
{ return aunbekannt; }

const std::string&
Konfiguration::zeichenfont() const
{ return _zeichenfont; }

const std::string&
Konfiguration::beschreibungsfont() const
{ return _beschreibungsfont; }

//--------------- src/main.cc ---------------
//#include "Aufwandstabelle.hh"
//#include "Eingabestream.hh"
//#include "Grafik.hh"
//#include "Haeufigkeit.hh"
//#include "Kodierung.hh"
//#include "Konfiguration.hh"
//#include "Tastatur.hh"
//#include "berechnungen.hh"
//#include "copyright.hh"
//#include "html_markup.hh"
//#include "konstanten.hh"
//#include "ngramme.hh"
//#include "schreibe_belegung.hh"
//#include "string_in_belegung.hh"
//#include "trennen.hh"
//#include "typen.hh"
//#include "utfhilfe.hh"
//#include "wortliste.hh"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#ifdef MIT_THREADS
#include <mingw.thread.h>
#endif // MIT_THREADS
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <typename S, typename Z>
void einfuegen(std::unordered_map<S, Z>& summe,
               std::unordered_map<S, Z>& summand)
{
   if(summe.size())
      for(const auto& k : summand) summe[k.first] += k.second;
   else summe.swap(summand);
}

void hilfe()
{
#ifdef ENGLISH
   std::cout <<
"Keyboard layout optimiser version " << opt_version << ", compiled for "
                                     << ntaste+nshift << " keys"
#ifdef OHNE2SHIFT
"\n(using option -DOHNE2SHIFT).\n\n"
#else
".\n\n"
#endif
"opt [-2|-3 prefix][-A][-b upto][-g file][-G num][-H upto][-i maxiter]\n"
"    [-k][-K][-m max][-r file][-s seed][-t num][-T][-V num][-w file]\n\n"
"-2 prefix  Specifies prefix for UTF-8 encoded files with character frequencies,\n"
"           (suffix .1) and digram frequencies (suffix .2).\n"
"-3 prefix  As -2, additionally with trigram frequencies (suffix .3).\n"
"-A         Dump the efforts used.\n"
"-b upto    Create summary of digrams up to given cumulative frequency per hand.\n"
"-f         Displays layout as 'FixesZeichen'; no not use with option -k.\n"
"-g file    Create a PostScript file with a graphical evaluation of the layouts.\n"
"-G weight  Weight used for subsequent frequency files.\n"
"-H upto    Print one-handed sequences up to given cumulative frequency.\n"
"-i maxiter Number of local optima to compute.\n"
"-k         Output results in compact form (one line per layout).\n"
"-K file    Use the given configuration file.\n"
"-m max     If this option is present, all layouts with a total effort below the\n"
"           given threshold will be printed.\n"
"-M file    Displays how the file is entered, as HTML (must be used with -r).\n"
"-r file    Output layouts read from the file; do not perform an optimisation.\n"
"-s seed    Seed for random number generator, a positive number.\n"
#ifdef MIT_THREADS
"-t num     Use num threads (default 1).\n"
#endif // MIT_THREADS
"-T         Add soft hyphenation in corpus.\n"
"-V maxdiff Variation depth (must be used with -r).\n"
"-w file    Specifies a word list." << std::endl;
#else
   std::cout << "Belegungsoptimierer Version " << opt_version
             << ", " strUe "bersetzt f" strUe "r "
             << ntaste+nshift << " Tasten"
#ifdef OHNE2SHIFT
"\n(mit Option -DOHNE2SHIFT).\n\n"
#else
".\n\n"
#endif
"opt [-2|-3 pr" strAe "fix][-A][-b biszu][-g file][-G num][-H upto][-i maxiter]\n"
"    [-k][-K][-m max][-r file][-s Saat][-t num][-T][-V num][-w file]\n\n"
"-2 pr" strAe "fix  Spezifiziert Pr" strAe "fix f" strUe "r UTF-8-kodierte Files mit\n"
"           Zeichenh" strAe "ufigkeiten (Suffix .1) und Bigrammh" strAe "ufigkeiten (Suffix .2).\n"
"-3 pr" strAe "fix  Wie -2, zus" strAe "tzlich mit Trigrammh" strAe "ufigkeiten (Suffix .3).\n"
"-A         Gib verwendete Aufw" strAe "nde aus.\n"
"-b biszu   Gib Bigrammaufstellung bis zu angegebener summierter H" strAe "ufigkeit\n"
"           pro Hand aus.\n"
"-f         Gibt Belegung als 'FixesZeichen' aus; nicht mit Option -k verwenden.\n"
"-g file    Erzeuge ein PostScript-File mit grafischer Auswertung\n"
"           der Belegungen.\n"
"-G gewicht Gewicht f" strUe "r folgende H" strAe "ufigkeitsfiles.\n"
"-H biszu   Kumulierte H" strAe "ufigkeit, bis zu der Handeins" strAe "tze ausgegeben werden.\n"
"-i maxiter Anzahl der zu berechnenden lokalen Optimierungen.\n"
"-k         Gibt Resultate in einer kompakten Form (eine Zeile pro Belegung) aus.\n"
"-K file    Benutze die Konfiguration im angegeben File.\n"
"-m max     Wenn diese Option angegeben wird, werden Belegungen mit\n"
"           Gesamtaufwand unter der angegbenen Schranke angezeigt.\n"
"-M file    Zeigt als HTML, wie das File eingegeben wird (in Verbindung mit -r).\n"
"-r file    Gibt Referenzbelegungen im File aus; es wird keine Optimierung\n"
"           durchgef" strUe "hrt.\n"
"-s Saat    Saat f" strUe "r den Zufallsgenerator, eine positive, ganze Zahl.\n"
#ifdef MIT_THREADS
"-t num     Benutze num Threads (Voreinstellung 1).\n"
#endif // MIT_THREADS
"-T         Weiche Trennzeichen in Korpus einf" strUe "gen.\n"
"-V maxdiff Variantentiefe (in Verbindung mit -r).\n"
"-w file    Spezifiziert eine Wortliste." << std::endl;
#endif
   exit(0);
}

const char* argument(int& argc, char* const*& argv)
{
   if(argc < 2){
      std::cerr << SPRACHE("Die Option '", "Option '") << *argv
                << SPRACHE("' erwartet ein Argument.",
                           "' requires an argument.") << std::endl;
      exit(1);
   }
   argv++; argc--;
   return *argv;
}

double fargument(int& argc, char* const*& argv)
{
   const std::string opt = *argv;
   const char* c = argument(argc, argv);
   char* x;
   const double r = strtod(c, &x);
   if(*x){
      std::cerr << SPRACHE("Das Argument der Option '", "The argument of option '")
                  << opt << SPRACHE("' muss eine Zahl sein.", "' must be a number.")
                  << std::endl;
        exit(1);
    }
    return r;
}
int iargument(int& argc, char* const*& argv)
{
   const std::string opt = *argv;
   const char* c = argument(argc, argv);
   char* x;
   const long r = strtol(c, &x, 10);
   if(*x){
      std::cerr << SPRACHE("Das Argument der Option '",
                           "The argument of option '") << opt
                << SPRACHE("' muss ganzzahlig sein.", "' must be an integer.")
                << std::endl;
      exit(1);
   }
   if(std::abs(r) > std::numeric_limits<int>::max()){
      std::cerr << SPRACHE("Der Betrag des Arguments der Option '",
                           "The magnitude of the argument of option '") << opt
                << SPRACHE("' muss muss kleiner oder gleich ",
                           "' must be less than or equal to ")
                << std::numeric_limits<int>::max()
                << SPRACHE(" sein.", ".") << std::endl;
      exit(1);
   }
   return r;
}

int main(int argc, char* const argv[])
{
   std::random_device rd;

   bool alle_guten = false, trigramm = false, korpusstatistik = false;
   bool schoene_tastatur = true, zyklen = false, aufwandstabelle = false;
   bool irgendeine_option = false, trennen = false, als_fixeszeichen = false;
   std::string referenztastatur, grafikname;
   int iterationen = std::numeric_limits<int>::max(), num_variierte_tasten = 0;
   int saatwert = rd(), nthreads = 1, iakkumlimit = 0;
   akkumuations_t minimum = 1e38, handeinsatzlimit = 95;
   double ngrammakkumlimit[3] = { -1, -1, -1 }, gewicht = 1, gewichtssumme = 0;
   std::vector<std::string> korpusnamen, freie_argumente;
   std::vector<std::string> konfigurationsfiles, wortlisten, markup;
   std::vector<double> gewichte;
   std::vector<bool> trigramme;
   std::unordered_map<std::string, haeufigkeit_t> wortliste;

   const std::string progname = *argv++;  argc--;
   for(; argc; argv++, argc--){
      const std::string option = *argv;
      bool ist_option = true;

      if(option == "-2" || option == "-3"){
         if(korpusnamen.size() >= nmaxkorpus){
            std::cerr << SPRACHE("Man kann maximal ",
                                 "You can specify at most ") << nmaxkorpus
                      << SPRACHE(" verschiedene Korpora angeben.  Im "
                                 "Sourcecode 'nmaxkorpus' erh" strOe "hen.",
                                 " different corpora.  In the source code, "
                                 "increase 'nmaxkorpus'.")
                      << std::endl;
            exit(1);
         }
         korpusnamen.emplace_back(argument(argc, argv));
         gewichte.push_back(gewicht);
         gewichtssumme += gewicht;
         trigramme.push_back(option == "-3");
         trigramm |= trigramme.back();
      }else if(option == "-A"){
         aufwandstabelle = true;
      }else if(option == "-b"){
         if(iakkumlimit < 3){
            ngrammakkumlimit[iakkumlimit] = fargument(argc, argv)/100.;
            if(ngrammakkumlimit[iakkumlimit] < 0){
               std::cerr << SPRACHE(
                  "Das Angument von -b darf nicht negativ sein.",
                  "The argument of -b must not be negative.") << std::endl;
               exit(1);
            }
            ++iakkumlimit;
         }
      }else if(option == "-f"){
         als_fixeszeichen = true;
      }else if(option == "-g"){
         grafikname = argument(argc, argv);
      }else if(option == "-G"){
         gewicht = fargument(argc, argv);
         if(gewicht <= 0.){
            std::cerr << SPRACHE(
               "Gewichte m" strUe "ssen positive Zahlen sein.",
               "Weights must be positive numbers.") << std::endl;
            exit(1);
         }
      }else if(option == "-h"){
         hilfe();
      }else if(option == "-H"){
         handeinsatzlimit = fargument(argc, argv);
      }else if(option == "-i"){
         iterationen = iargument(argc, argv);
      }else if(option == "-k"){
         schoene_tastatur = false;
      }else if(option == "-K"){
         konfigurationsfiles.emplace_back(argument(argc, argv));
      }else if(option == "-m"){
         minimum = fargument(argc, argv)/100.;
         alle_guten = true;
      }else if(option == "-M"){
         markup.emplace_back(argument(argc, argv));
      }else if(option == "-r"){
         if(referenztastatur.size()){
            std::cerr << SPRACHE(
               "Es darf nur ein Belegungsfile angegeben werden.",
               "You can specify only one layout file.") << std::endl;
            exit(1);
         }
         referenztastatur = argument(argc, argv);
      }else if(option == "-s"){
         saatwert = (iargument(argc, argv));
      }else if(option == "-S"){
         korpusstatistik = true;
      }else if(option == "-t"){
         nthreads = (iargument(argc, argv));
         if(nthreads < 1){
            std::cerr << SPRACHE("Die Anzahl Threads muss mindestens 1 sein.",
                                 "The number of threads must be at least 1.")
                      << std::endl;
            exit(1);
         }
#ifndef MIT_THREADS
         std::cerr << SPRACHE(
            "Die Option -t ist nicht unterst" strUe "tzt, denn der Optimierer "
            "wurde ohne die Option -DMIT_THREADS " strUe "bersetzt.  ",
            "The option -t is not supported, as the optimiser was compiled "
            "without using option -DMIT_THREADS.") << std::endl;
#endif // !MIT_THREADS
      }else if(option == "-T"){
         trennen = true;
         ist_option = false;
      }else if(option == "-V"){
         num_variierte_tasten = iargument(argc, argv);
      }else if(option == "-w"){
         wortlisten.emplace_back(argument(argc, argv));
      }else if(option == "-Z"){
         zyklen = true;
      }else{
         ist_option = false;
         freie_argumente.push_back(option);
      }

      irgendeine_option |= ist_option;
   }

   if(freie_argumente.size() || trennen){
      if(irgendeine_option){
         std::cerr << SPRACHE("Bei Erstellen von H" strAe "ufigkeitstabellen d"
                              strUe "rfen keine Optionen angegeben werden.",
                              "When creating frequency files, you must not "
                              "specify any options.")
                   << std::endl;
         exit(1);
      }
      if(trennen){
         if(freie_argumente.size() != 3){
            std::cerr << SPRACHE(
               "Zum Trennen genau drei Argumente angeben: "
               "Trennmuster, Eingabefile, Ausgabefile",
               "To hyphenate, provide exactly three arguments: "
               "hyphenation patterns, input file, output file") << std::endl;
            exit(1);
         }
         markiere_alle_trennstellen(freie_argumente[1], freie_argumente[2],
                                    freie_argumente[0]);
         exit(0);
      }else{
         erzeuge_ngrammtabellen(freie_argumente);
         exit(0);
      }
   }

   if(!konfigurationsfiles.size())
      konfigurationsfiles.push_back("standard.cfg");

   std::unique_ptr<const Kodierung> kodierung;
   std::unique_ptr<const Tastatur> tastatur;
   std::unique_ptr<Konfiguration> konfiguration
      (new Konfiguration(konfigurationsfiles, tastatur, kodierung));
   std::unique_ptr<const Aufwandstabelle> a
      (new Aufwandstabelle(trigramm, *tastatur, *konfiguration));
   if(aufwandstabelle){
      a->anzeigen(*kodierung, *konfiguration);
      exit(0);
   }

   if(markup.size()){
      if(!referenztastatur.size()){
         std::cerr << SPRACHE(
            "Mit Option -M muss auch Option -r verwendet werden.",
            "When using option -M, you must use option -r as well.")
                   << std::endl;
         exit(1);
      }
      for(auto& i : markup)
         html_markup(i, *kodierung, *tastatur, referenztastatur);
      exit(0);
   }

   if(!korpusnamen.size()){
      std::cerr << SPRACHE("Quellen f" strUe "r Buchstaben- und Bi/Trigrammh"
                           strAe "ufigkeiten m" strUe "ssen angegeben werden.",
                           "Sources for character and di/trigram frequencies "
                           "must be specified.") << std::endl;
      exit(1);
   }

   for(auto& i : gewichte) i /= gewichtssumme;
   std::unique_ptr<const Haeufigkeit> korpus
      (new Haeufigkeit(*tastatur, *kodierung, korpusnamen, gewichte, trigramme,
                       referenztastatur.size()&&schoene_tastatur && !trigramm,
                       a->unbekannt() > 0));
   if(korpusstatistik) korpus->statistik();

   std::unique_ptr<Grafik> grafik
      (grafikname.size()
       ? new Grafik(grafikname, *tastatur, *kodierung, *korpus, *konfiguration)
       : nullptr);

   for(const auto& i : wortlisten){
      std::unordered_map<std::string, haeufigkeit_t> ht;
      lies_wortliste(i, ht, *kodierung);
      einfuegen(wortliste, ht);
   }

   const char* vgl[2] = { SPRACHE("Gegen erste: ",  " Compared to first: "),
                          SPRACHE("Gegen vorige: ", " Compared to previous: ")};

   const akkumuations_t K = konstanter_aufwand(*korpus, *a);
   minimum -= K;
   if(referenztastatur.size()){
      constexpr bool utf8 = true;
      belegung_t b2[2];  b2[0][0] = b2[1][0] = ntaste;
      Eingabestream liste(referenztastatur, utf8);
      while(liste.echte_neuezeile()){
         std::u32string bs, uname;
         if(!liste.hole_wort(bs) || !liste.hole_wort(uname)){
            std::cerr << SPRACHE("Fehlerhaft formatiertes Belegungsfile ",
                                 "Incorrectly formatted layout file ")
                      << referenztastatur << std::endl;
            liste.fehler();
         }

         if(bs.size()){
            belegung_t b;
            bool fest[ntaste];
            string_in_belegung(bs, b, fest, *tastatur, *kodierung);
            if(zyklen){
               for(int z = 0; z < 2; ++z){
                  if(b2[z][0] >= ntaste) continue;
                  int ndiff = 0;
                  for(int i = 0; i < tastatur->nvariabel(); ++i)
                     if(b[i] != b2[z][i]) ++ndiff;
                  std::cout << vgl[z] << ndiff
                            << SPRACHE(" Tasten umbelegt, Zyklen:",
                                       " keys reassigned, cycles:");
                  schreibe_zyklen(b2[z], b, *kodierung);
                  std::cout << "\n";
               }
               std::cout << "\n";
            }

            std::unique_ptr<Haeufigkeit> arbeitskorpus
               (new Haeufigkeit(*korpus, 0));
#ifndef PERMUTATIONEN_ALT
            arbeitskorpus->setze(*korpus, b);
#endif // !PERMUTATIONEN_ALT
            const akkumuations_t A =
               variabler_aufwand(b, *arbeitskorpus, *tastatur, *a);

            if(num_variierte_tasten > 1){
               int numfrei = 0;
               for(int i = 0; i < tastatur->nvariabel(); ++i)
                  if(!fest[i]) ++numfrei;
               std::cerr << numfrei << SPRACHE(" freie Tasten, ",
                                               " variable keys, ")
                         << std::setprecision(16)
                         << anzahl_varianten(numfrei, num_variierte_tasten)
                         << SPRACHE(" Varianten", " variants") << std::endl;

               if(A < minimum) schreibe_belegung(b, A+K, *tastatur,
                                                 *kodierung, &uname);

               erzeuge_variationen(b, *tastatur, *kodierung, *arbeitskorpus, *a,
                                   A, num_variierte_tasten, minimum, fest);
            }else{
               if(schoene_tastatur){
                  schreibe_belegung(b, *tastatur, *kodierung, *korpus, *a, A+K,
                                    uname, ngrammakkumlimit, wortliste,
                                    handeinsatzlimit, als_fixeszeichen);
                  if(korpus->mit_varianzen() && b2[0][0] < ntaste){
                     std::cout << SPRACHE(
                        "Standardabweichung der " "Aufwandsdifferenz zur "
                        "ersten Belegung: ",
                        "Standard deviation of the difference of efforts "
                        "compared to the first layout: ")
                               << std::setprecision(9)
                               << 100*std::sqrt(
                                  aufwandsvarianz(b, &b2[0], *tastatur,
                                                  *korpus, *a))
                               << "\n";
                  }
                  if(korpus->mit_varianzen()){
                     std::cout << SPRACHE(
                        "Standardabweichung des Gesamtaufwands: ",
                        "Standard deviation of the total effort: ")
                               << std::setprecision(9)
                               << 100*std::sqrt(
                                  aufwandsvarianz(b, nullptr, *tastatur,
                                                  *korpus, *a))
                               << "\n";
                  }
                  std::cout << std::endl;
               }else{
                  schreibe_belegung(b, A+K, *tastatur, *kodierung, &uname);
               }

               if(grafik) grafik->ausgabe(b);

            }

            if(b2[0][0] >= ntaste)
               for(int i = 0; i < ntaste; ++i) b2[0][i] = b[i];
            if(zyklen)
               for(int i = 0; i < ntaste; ++i) b2[1][i] = b[i];
         }
      }
   }else{
      const auto startzeit = std::chrono::high_resolution_clock::now();

      const int lebenszeichen =
         (trigramm ? 1000 : 10000)*(schoene_tastatur ? 1 : 100);

      std::seed_seq saatgen{saatwert};
      std::vector<int> saaten(nthreads);
      saatgen.generate(saaten.begin(), saaten.end());
      akkumuations_t globalesMinimum = minimum;
#ifdef MIT_THREADS
      std::vector<std::thread> threads;
      for(int i = 1; i < nthreads; ++i){
         threads.push_back(
            std::thread(suche_optimum, *tastatur, *kodierung, korpus.get(), *a,
                        minimum, schoene_tastatur, alle_guten, als_fixeszeichen,
                        saaten[i], iterationen, std::numeric_limits<int>::max(),
                        ngrammakkumlimit, wortliste, handeinsatzlimit,
                        grafik.get(), &globalesMinimum));
      }
#endif // MIT_THREADS
      suche_optimum(*tastatur, *kodierung, korpus.get(), *a, minimum,
                    schoene_tastatur, alle_guten, als_fixeszeichen, saaten[0],
                    iterationen, lebenszeichen, ngrammakkumlimit, wortliste,
                    handeinsatzlimit, grafik.get(), &globalesMinimum);
#ifdef MIT_THREADS
      for(auto& t : threads) t.join();
#endif // MIT_THREADS

      const auto endzeit = std::chrono::high_resolution_clock::now();
      const std::chrono::nanoseconds zeitdifferenz = endzeit-startzeit;
      std::cerr << SPRACHE("Laufzeit: ", "Run time: ")
                << std::fixed << std::setw(9)
                << std::setprecision(9) << 1e-9*zeitdifferenz.count()
                << SPRACHE(" Sekunden", " seconds") << std::endl;
   }

   return 0;
}

//--------------- src/berechnungen.cc ---------------
//#include "berechnungen.hh"

//#include "Aufwandstabelle.hh"
//#include "Grafik.hh"
//#include "Haeufigkeit.hh"
//#include "Kodierung.hh"
//#include "Tastatur.hh"
//#include "schreibe_belegung.hh"
//#include "utfhilfe.hh"
#include <cstring>
#include <iostream>
#ifdef MIT_THREADS
#include <mutex>
#endif // MIT_THREADS
#include <random>

namespace {

#ifdef PERMUTATIONEN_ALT
#define BELEGUNG(z, b, p) const int z = b[p]
#else
#define BELEGUNG(z, b, p) const int& z = p
#endif

// Der Zufallszahlengenerator kann Einfluss auf die Geschwindigkeit haben.
using rngengine_t = std::mt19937;
using uniform_t = std::uniform_int_distribution<char>;

#ifdef MIT_THREADS
std::mutex ausgabemutex;
#endif // MIT_THREADS

using fingerbelastung_t = akkumuations_t[nmaxkorpus][nfinger+1];

inline akkumuations_t
berechne_vorliebensumme(const belegung_t b, const Aufwandstabelle& a){
   if(!a.hat_vorlieben()) return 0;
   akkumuations_t vs = 0;
   for(int p = 0; p < ntaste; ++p) vs += a.vorliebe(b[p], p);
   return vs;
}

inline void
berechne_fingerbelastung(const belegung_t b,
                         const Haeufigkeit& h,
                         const Tastatur& tastatur,
                         fingerbelastung_t fh)
{
   for(int k = 0; k < h.num_korpus(); ++k){
      for(auto& i : fh[k]) i = 0;
      for(int p1 = 0; p1 < ntaste; ++p1){
         BELEGUNG(z1, b, p1);
         const int fi = tastatur.finger_index(p1);
         for(int e1 = 0; e1 < nebene; ++e1) fh[k][fi] += h(k,z1,e1);
         const int sf = tastatur.shift_finger_index(p1);
         fh[k][sf] += h(k,z1,1);
      }
   }
}

inline void zufallsbelegung(belegung_t b, int nv,
                            rngengine_t& rng, uniform_t& d){
   for(int j = 0; j < ntaste; ++j) b[j] = j;
   for(int j = nv-1; j ; --j){
      const int pos = d(rng, uniform_t::param_type(0, j));
      std::swap(b[j], b[pos]);
   }
}

// Berechne Aufwanderhöhung dadurch, dass man die Belegung der Plätze p1 und p2
// vertauscht.
akkumuations_t
diff_aufwand(int p1, int p2, const belegung_t b,
             const Tastatur& tastatur,
             const Haeufigkeit& h,
             const fingerbelastung_t fh_alt,
#ifdef EXPERIMENTELL
             const akkumuations_t vs_alt,
#endif // EXPERIMENTELL
             const Aufwandstabelle& a)
{
   assert(p1 < tastatur.nvariabel() && p2 < tastatur.nvariabel());
   BELEGUNG(z1, b, p1);  BELEGUNG(z2, b, p2);
   akkumuations_t summe = 0;

   for(int e1 = 0; e1 < nebene; ++e1){
      const akkumuations_t dh1 = (h(z2,e1)-h(z1,e1));
      const akkumuations_t da1 = a(p1,e1)-a(p2,e1);
      summe += da1*dh1;

      for(int e2 = 0; e2 < nebene2; ++e2){
         const akkumuations_t dh1 = h(z1,e1,z1,e2)-h(z1,e1,z2,e2);
         const akkumuations_t dh2 = h(z2,e1,z2,e2)-h(z2,e1,z1,e2);
         const akkumuations_t da1 = a(p1,e1,p1,e2)-a(p1,e1,p2,e2);
         const akkumuations_t da2 = a(p2,e1,p2,e2)-a(p2,e1,p1,e2);
         summe += (da1+da2)*(dh1+dh2);
      }
   }
   for(int p = 0; p < ntaste; ++p){
      BELEGUNG(z, b, p);
      for(int e1 = 0; e1 < nebene; ++e1){
         for(int e2 = 0; e2 < nebene2; ++e2){
            const akkumuations_t dh2_1 = h(z2,e1,z,e2)-h(z1,e1,z,e2);
            const akkumuations_t da2_1 = a(p1,e1,p,e2)-a(p2,e1,p,e2);
            const akkumuations_t dh2_2 = h(z,e1,z2,e2)-h(z,e1,z1,e2);
            const akkumuations_t da2_2 = a(p,e1,p1,e2)-a(p,e1,p2,e2);
            summe += da2_1*dh2_1+da2_2*dh2_2;
         }
      }
   }

   if(a.hat_vorlieben()){
      const int z1 = b[p1], z2 = b[p2];
      const akkumuations_t diff_vs =
         (a.vorliebe(z2,p1)+a.vorliebe(z1,p2))
         -(a.vorliebe(z1,p1)+a.vorliebe(z2,p2));
#ifdef EXPERIMENTELL
      const akkumuations_t vs_neu = vs_alt+diff_vs;
      summe += a.knick(vs_neu)-a.knick(vs_alt);
#else
      summe += diff_vs;
#endif // !EXPERIMENTELL
   }

   if(a.hat_aehnlichkeit()){
      const int z1v = b[p1], z2v = b[p2];
      for(int p = 0; p < ntaste; ++p){
         if(p == p1 || p == p2) continue;
         const int z = b[p];
         const akkumuations_t dh2_1 =
            a.aehnlichkeit(z2v,z)-a.aehnlichkeit(z1v,z);
         const akkumuations_t da2_1 =
            a.verwechslungspotenzial(p1,p)-a.verwechslungspotenzial(p2,p);
         summe += 2.*da2_1*dh2_1;
      }
   }

   if(h.mit_trigrammen()){
      for(int pi = 0; pi < ntaste; ++pi){
         if(pi == p1 || pi == p2) continue;
         BELEGUNG(zi, b, pi);
         for(int pj = 0; pj < ntaste; ++pj){
            if(pj == p1 || pj == p2) continue;
            BELEGUNG(zj, b, pj);
            for(int e = 0; e < nebene; ++e){
               const akkumuations_t dh3_1 = h.tri(zi,zj,z2,e)-h.tri(zi,zj,z1,e);
               const akkumuations_t da3_1 = a.tri(pi,pj,p1,e)-a.tri(pi,pj,p2,e);
               summe += da3_1*dh3_1;
               const akkumuations_t dh3_2 = h.tri(zi,z2,zj,e)-h.tri(zi,z1,zj,e);
               const akkumuations_t da3_2 = a.tri(pi,p1,pj,e)-a.tri(pi,p2,pj,e);
               summe += da3_2*dh3_2;
               const akkumuations_t dh3_3 = h.tri(z2,zi,zj,e)-h.tri(z1,zi,zj,e);
               const akkumuations_t da3_3 = a.tri(p1,pi,pj,e)-a.tri(p2,pi,pj,e);
               summe += da3_3*dh3_3;
            }
         }
      }

      for(int pi = 0; pi < ntaste; ++pi){
         if(pi == p1 || pi == p2) continue;
         BELEGUNG(zi, b, pi);
         for(int e = 0; e < nebene; ++e){
            const akkumuations_t dh3_1a = h.tri(zi,z2,z2,e)-h.tri(zi,z1,z1,e);
            const akkumuations_t da3_1a = a.tri(pi,p1,p1,e)-a.tri(pi,p2,p2,e);
            summe += da3_1a*dh3_1a;
            const akkumuations_t dh3_1b = h.tri(zi,z1,z2,e)-h.tri(zi,z2,z1,e);
            const akkumuations_t da3_1b = a.tri(pi,p2,p1,e)-a.tri(pi,p1,p2,e);
            summe += da3_1b*dh3_1b;
            const akkumuations_t dh3_2a = h.tri(z2,zi,z2,e)-h.tri(z1,zi,z1,e);
            const akkumuations_t da3_2a = a.tri(p1,pi,p1,e)-a.tri(p2,pi,p2,e);
            summe += da3_2a*dh3_2a;
            const akkumuations_t dh3_2b = h.tri(z1,zi,z2,e)-h.tri(z2,zi,z1,e);
            const akkumuations_t da3_2b = a.tri(p2,pi,p1,e)-a.tri(p1,pi,p2,e);
            summe += da3_2b*dh3_2b;
            const akkumuations_t dh3_3a = h.tri(z2,z2,zi,e)-h.tri(z1,z1,zi,e);
            const akkumuations_t da3_3a = a.tri(p1,p1,pi,e)-a.tri(p2,p2,pi,e);
            summe += da3_3a*dh3_3a;
            const akkumuations_t dh3_3b = h.tri(z1,z2,zi,e)-h.tri(z2,z1,zi,e);
            const akkumuations_t da3_3b = a.tri(p2,p1,pi,e)-a.tri(p1,p2,pi,e);
            summe += da3_3b*dh3_3b;
         }
      }
      for(int e = 0; e < nebene; ++e){
         const akkumuations_t dh3_a = h.tri(z2,z2,z2,e)-h.tri(z1,z1,z1,e);
         const akkumuations_t da3_a = a.tri(p1,p1,p1,e)-a.tri(p2,p2,p2,e);
         summe += da3_a*dh3_a;
         const akkumuations_t dh3_b = h.tri(z2,z2,z1,e)-h.tri(z1,z1,z2,e);
         const akkumuations_t da3_b = a.tri(p1,p1,p2,e)-a.tri(p2,p2,p1,e);
         summe += da3_b*dh3_b;
         const akkumuations_t dh3_c = h.tri(z2,z1,z2,e)-h.tri(z1,z2,z1,e);
         const akkumuations_t da3_c = a.tri(p1,p2,p1,e)-a.tri(p2,p1,p2,e);
         summe += da3_c*dh3_c;
         const akkumuations_t dh3_d = h.tri(z1,z2,z2,e)-h.tri(z2,z1,z1,e);
         const akkumuations_t da3_d = a.tri(p2,p1,p1,e)-a.tri(p1,p2,p2,e);
         summe += da3_d*dh3_d;
      }
   }

   const int f1 = tastatur.finger_index(p1), f2 = tastatur.finger_index(p2);
   if(f1 == f2) return summe;

   const int sf1 = tastatur.shift_finger_index(p1);
   const int sf2 = tastatur.shift_finger_index(p2);
   akkumuations_t fsgesamt = 0;
   for(int k = 0; k < h.num_korpus(); ++k){
      akkumuations_t fh_neu[nfinger+1];
      for(int i = 0; i < nfinger+1; ++i) fh_neu[i] = fh_alt[k][i];
      for(int e1 = 0; e1 < nebene; ++e1){
         const akkumuations_t dh = h(k,z1,e1)-h(k,z2,e1);
         fh_neu[f1] -= dh;
         fh_neu[f2] += dh;
      }
      if(sf1 != sf2){
         const akkumuations_t dh = h(k,z1,1)-h(k,z2,1);
         fh_neu[sf1] -= dh;
         fh_neu[sf2] += dh;
      }

      akkumuations_t fs = 0;
      for(int i = 0; i < nfinger; ++i){
         const akkumuations_t diff = (fh_neu[i]-fh_alt[k][i]);
         if(diff){
            const akkumuations_t d_neu = a.fingerabweichung(i, fh_neu[i]);
            const akkumuations_t d_alt = a.fingerabweichung(i, fh_alt[k][i]);
            if(d_neu > 0){
               if(d_alt > 0){
                  const akkumuations_t plus =
                     a.fingerabweichung(i, 0.5*(fh_neu[i]+fh_alt[k][i]));
                  fs += a.mult_finger(i)*plus*diff;
               }else fs += 0.5*a.mult_finger(i)*d_neu*d_neu;
            }else if(d_alt > 0)
               fs -= 0.5*a.mult_finger(i)*d_alt*d_alt;
         }
      }
      fsgesamt += h.gewicht(k)*fs;
   }

   return summe+2*fsgesamt;
}

// Berechne variablen Gesamtaufwand einer Belegung
akkumuations_t
variabler_aufwand(const belegung_t b, const Haeufigkeit& h,
        const fingerbelastung_t fh,
        const Aufwandstabelle& a)
{
   akkumuations_t summe = 0;

   for(int p1 = 0; p1 < ntaste; ++p1){
      BELEGUNG(z1, b, p1);
      for(int e1 = 0; e1 < nebene; ++e1){
         const akkumuations_t a1 = a(p1,e1), h1 = h(z1,e1);
         summe += a1*h1;
      }
   }

   for(int p1 = 0; p1 < ntaste; ++p1){
      BELEGUNG(z1, b, p1);
      for(int p2 = 0; p2 < ntaste; ++p2){
         BELEGUNG(z2, b, p2);
         for(int e1 = 0; e1 < nebene; ++e1){
            for(int e2 = 0; e2 < nebene2; ++e2){
               const akkumuations_t a2 = a(p1,e1,p2,e2), h2 = h(z1,e1,z2,e2);
               summe += a2*h2;
            }
         }
      }
   }

   if(a.hat_vorlieben()) summe += a.knick(berechne_vorliebensumme(b, a));

   if(a.hat_aehnlichkeit()){
      for(int p1 = 0; p1 < ntaste; ++p1){
         const int z1 = b[p1];
         for(int p2 = 0; p2 < p1; ++p2){
            const int z2 = b[p2];
            const akkumuations_t a2 = a.verwechslungspotenzial(p1,p2);
            const akkumuations_t h2 = a.aehnlichkeit(z1,z2);
            // Faktor 2, weil wir in der Doppelschleife Symmetrie ausnutzen.
            summe += 2.*a2*h2;
         }
      }
   }

   if(h.mit_trigrammen()){
      for(int p1 = 0; p1 < ntaste; ++p1){
         BELEGUNG(z1, b, p1);
         for(int p2 = 0; p2 < ntaste; ++p2){
            BELEGUNG(z2, b, p2);
            for(int p3 = 0; p3 < ntaste; ++p3){
               BELEGUNG(z3, b, p3);
               for(int e3 = 0; e3 < nebene; ++e3){
                  const akkumuations_t a3 = a.tri(p1,p2,p3,e3);
                  const akkumuations_t h3 = h.tri(z1,z2,z3,e3);
                  summe += a3*h3;
               }
            }
         }
      }
   }

   akkumuations_t fsgesamt = 0;
   for(int k = 0; k < h.num_korpus(); ++k){
      akkumuations_t fs = 0;
      for(int i = 0; i < nfinger; ++i){
         if(a.mult_finger(i) == 0) continue;
         const akkumuations_t diff = a.fingerabweichung(i, fh[k][i]);
         if(diff > 0) fs += a.mult_finger(i)*diff*diff;
      }
      fsgesamt += h.gewicht(k)*fs;
   }
   return summe+fsgesamt;
}

inline akkumuations_t
zufaelliger_abstieg(belegung_t b, const Tastatur& tastatur, Haeufigkeit& h,
                    const Aufwandstabelle& a, rngengine_t& rng, uniform_t& d)
{
   bool reduktion;
   belegung_t p;
   int probiert[ntaste][ntaste];
   int variante = 1;
   std::memset(probiert, 0, sizeof(probiert));
   fingerbelastung_t fh;
   berechne_fingerbelastung(b, h, tastatur, fh);
#ifndef NDEBUG
   akkumuations_t alt = variabler_aufwand(b, h, fh, a);
#endif // !NDEBUG
#ifdef EXPERIMENTELL
   akkumuations_t vs = berechne_vorliebensumme(b, a);
#endif // EXPERIMENTELL
   do{
      reduktion = false;
      zufallsbelegung(p, tastatur.nvariabel(), rng, d);
      for(int i = 0; i < tastatur.nvariabel()-1; ++i){
         const int pi = p[i];
         for(int j = i+1;  j < tastatur.nvariabel(); ++j){
            const int pj = p[j];
            if(probiert[pi][pj] == variante) continue;
            const akkumuations_t diff =
#ifdef EXPERIMENTELL
               diff_aufwand(pi, pj, b, tastatur, h, fh, vs, a);
#else
               diff_aufwand(pi, pj, b, tastatur, h, fh, a);
#endif // !EXPERIMENTELL
               if(diff < 0){
                  std::swap(b[pi], b[pj]);
#ifndef PERMUTATIONEN_ALT
                  h.swap(pi, pj);
#endif // !PERMUTATIONEN_ALT
#ifdef EXPERIMENTELL
                  vs = berechne_vorliebensumme(b, a);
#endif // EXPERIMENTELL
                  berechne_fingerbelastung(b, h, tastatur, fh);
                  reduktion = true;
                  ++variante;
#ifndef NDEBUG
                  const akkumuations_t neu = variabler_aufwand(b, h, fh, a);
                  if(std::abs((neu-alt)-diff) >= 1e-5*std::abs(neu+alt)){
                     std::cerr << SPRACHE("Relativer Fehler ","Relative error ")
                               << std::abs((neu-alt)-diff)/std::abs(neu+alt)
                               << std::endl;
                     exit(1);
                  }
                  alt = neu;
#endif // !NDEBUG
               }
               probiert[pi][pj] = probiert[pj][pi] = variante;
         }
      }
   }while(reduktion);

   return variabler_aufwand(b, h, fh, a);
}

struct zyklen_t {
   int pos[ntaste][ntaste];
   int idx[ntaste+1];
   int teilweise;
};




void variiere_tasten(belegung_t b, const Tastatur& tastatur,
                     const Kodierung& kodierung, Haeufigkeit& h,
                     const fingerbelastung_t& fh,
#ifdef EXPERIMENTELL
                     const akkumuations_t vs,
#endif // EXPERIMENTELL
                     const Aufwandstabelle& a, akkumuations_t A, int tiefe,
                     zyklen_t& zyklen, int ab, akkumuations_t limit,
                     const bool* fest)
{
   --tiefe;
   for(int p2 = ab; p2 < tastatur.nvariabel(); ++p2){
      if(fest[p2]) continue;
      int z = 0;
      for(; zyklen.idx[z]; ++z){
         const int zlaenge = zyklen.idx[z];
         if(zlaenge == 1) --zyklen.teilweise;
         zyklen.pos[z][zlaenge] = p2;
         if(tiefe) ++zyklen.idx[z];

         for(int j = 0; j < zlaenge; ++j){
            const int p1 = zyklen.pos[z][j];

            const akkumuations_t Aneu =
#ifdef EXPERIMENTELL
               A+diff_aufwand(p1, p2, b, tastatur, h, fh, vs, a);
#else
               A+diff_aufwand(p1, p2, b, tastatur, h, fh, a);
#endif // !EXPERIMENTELL

               std::swap(b[p1], b[p2]);

               if(zyklen.teilweise == 0 && Aneu < limit)
                  schreibe_belegung(b, Aneu+konstanter_aufwand(h, a),
                                    tastatur, kodierung, 0);

               if(tiefe && tiefe >= zyklen.teilweise &&
                  p2+1 < tastatur.nvariabel()){
#ifndef PERMUTATIONEN_ALT
                  h.swap(p1, p2);
#endif // !PERMUTATIONEN_ALT
                  fingerbelastung_t fh2;
                  berechne_fingerbelastung(b, h, tastatur, fh2);
#ifdef EXPERIMENTELL
                  const akkumuations_t vs2 = berechne_vorliebensumme(b, a);
                  variiere_tasten(b, tastatur, kodierung, h, fh2, vs2, a, Aneu,
                                  tiefe, zyklen, p2+1, limit, fest);
#else
                  variiere_tasten(b, tastatur, kodierung, h, fh2, a, Aneu,
                                  tiefe, zyklen, p2+1, limit, fest);
#endif // !EXPERIMENTELL

#ifndef PERMUTATIONEN_ALT
                  h.swap(p1, p2);
#endif // !PERMUTATIONEN_ALT
               }
               std::swap(b[p1], b[p2]);
         }
         zyklen.idx[z] = zlaenge;
         if(zlaenge == 1) ++zyklen.teilweise;
      }

      if(tiefe > zyklen.teilweise && p2+1 < tastatur.nvariabel()){
         zyklen.idx[z] = 1;
         zyklen.pos[z][0] = p2;
         ++zyklen.teilweise;
#ifdef EXPERIMENTELL
         variiere_tasten(b, tastatur, kodierung, h, fh, vs, a, A,
                         tiefe, zyklen, p2+1, limit, fest);
#else // !EXPERIMENTELL
         variiere_tasten(b, tastatur, kodierung, h, fh, a, A,
                         tiefe, zyklen, p2+1, limit, fest);
#endif // !EXPERIMENTELL
         zyklen.idx[z] = 0;
         --zyklen.teilweise;
      }
   }
}

} // Ende des namenlosen Namensraums.


void
suche_optimum(const Tastatur& tastatur, const Kodierung& kodierung,
              const Haeufigkeit* korpus, const Aufwandstabelle& a,
              akkumuations_t minimum, bool schoene_tastatur, bool alle_guten,
              bool als_fixeszeichen, int saatwert, int iterationen,
              int lebenszeichen, const double ngrammakkumlimit[3],
              const std::unordered_map<std::string, haeufigkeit_t>& wortliste,
              akkumuations_t handeinsatzlimit, Grafik* grafik,
              akkumuations_t* globalesMinimum)
{
   // Es ist nicht klar, dass diese beiden Kopien die Geschwindigkeit
   // verbessern.  Auf meiner Maschine tun sie es.
   std::unique_ptr<const Haeufigkeit> h(new Haeufigkeit(*korpus, 0));
   std::unique_ptr<const Aufwandstabelle> aufwand(new Aufwandstabelle(a));

   rngengine_t rng(saatwert);
   uniform_t uniform;
   std::unique_ptr<Haeufigkeit> arbeitskorpus(new Haeufigkeit(*h, 0));

   for(int j = 0; j++ < iterationen;){
      belegung_t b; zufallsbelegung(b, tastatur.nvariabel(), rng, uniform);

#ifndef PERMUTATIONEN_ALT
      arbeitskorpus->setze(*h, b);
#endif // !PERMUTATIONEN_ALT
      const akkumuations_t A = zufaelliger_abstieg(b, tastatur, *arbeitskorpus,
                                                   *aufwand, rng, uniform);
      if(A < minimum){
#ifdef MIT_THREADS
         std::lock_guard<std::mutex> sperre(ausgabemutex);
#endif // MIT_THREADS
         if(A < *globalesMinimum){
            if(!alle_guten) minimum = *globalesMinimum = A;
            const akkumuations_t K =
               konstanter_aufwand(*arbeitskorpus, *aufwand);
            if(schoene_tastatur)
               schreibe_belegung(b, tastatur, kodierung, *h, *aufwand, A+K,
                                 zahl_in_utf32(j), ngrammakkumlimit, wortliste,
                                 handeinsatzlimit, als_fixeszeichen);
            else
               schreibe_belegung(b, A+K, tastatur, kodierung, 0);

            if(grafik) grafik->ausgabe(b);
         }else if(!alle_guten) minimum = *globalesMinimum;
      }

      if(j % lebenszeichen == 0)
         std::cerr << SPRACHE("Thread 0: Lokale Optimierung ",
                              "Thread 0: Local optimisation ")<< j << std::endl;
   }
}

double anzahl_varianten(int N, int n)
{
   double C = N;
   double sf2 = 1, sf1 = 0;
   double summe = 1;
   for(int i = 2; i <= n; ++i){
      C = (C*(N-(i-1)))/i;
      const double sf = (i-1)*(sf1+sf2);
      sf2 = sf1; sf1 = sf;
      summe += C*sf;
   }
   return summe;
}

void
erzeuge_variationen(const belegung_t ausgangsbelegung,
                    const Tastatur& tastatur, const Kodierung& kodierung,
                    Haeufigkeit& h, const Aufwandstabelle& a,
                    akkumuations_t A, int tiefe,
                    akkumuations_t limit, const bool* fest)
{
   belegung_t b;
   for(int i = 0; i < ntaste; ++i) b[i] = ausgangsbelegung[i];
   fingerbelastung_t fh;
   berechne_fingerbelastung(b, h, tastatur, fh);
   zyklen_t zyklen;
   for(int i = 0; i < tastatur.nvariabel()+1; ++i) zyklen.idx[i] = 0;
   zyklen.teilweise = 0;
#ifdef EXPERIMENTELL
   const akkumuations_t vs = berechne_vorliebensumme(b, a);
   variiere_tasten(b, tastatur, kodierung, h, fh, vs, a, A,
                   tiefe, zyklen, 0, limit, fest);
#else
   variiere_tasten(b, tastatur, kodierung, h, fh, a, A,
                   tiefe, zyklen, 0, limit, fest);
#endif // !EXPERIMENTELL
}

akkumuations_t
aufwandsvarianz(const belegung_t b1, const belegung_t* b2,
                const Tastatur& tastatur,
                const Haeufigkeit& h,
                const Aufwandstabelle& a)
{
   assert(h.mit_varianzen());
   akkumuations_t summe = 0;
   // Umkehrung der Belegung(en)
   belegung_t ib1, ib2;
   for(int i = 0; i < ntaste; ++i) ib1[b1[i]] = ib2[b1[i]] = i;
   if(b2) for(int i = 0; i < ntaste; ++i) ib2[(*b2)[i]] = i;

   // Ableitung der (Differenz der) Aufwände für das Überschreiten der
   // Zielhäufigkeiten nach den Anschlagsshäufigkeiten, tabelliert nach Zeichen.
   akkumuations_t fhdiff[nmaxkorpus][ntaste][nebene];
   for(int k = 0; k < h.num_korpus(); ++k){
      akkumuations_t fh[2][nfinger+1];
      memset(fh, 0, sizeof(fh));
      for(int j = 0; j < (b2 ? 2 : 1); ++j){
         for(int z = 0; z < ntaste; ++z){
            const int p = j ? ib2[z] : ib1[z];
            const int fi = tastatur.finger_index(p);
            const int sf = tastatur.shift_finger_index(p);
            for(int e = 0; e < nebene; ++e) fh[j][fi] += h(k,z,e);
            fh[j][sf] += h(k,z,1);
         }
         for(int i = 0; i < nfinger; ++i){
            const akkumuations_t diff = a.fingerabweichung(i, fh[j][i]);
            fh[j][i] = diff > 0 ? 2.*h.gewicht(k)*a.mult_finger(i)*diff : 0;
         }
         fh[j][nfinger] = 0;
      }
      for(int z = 0; z < ntaste; ++z){
         const int p1 = ib1[z], p2 = ib2[z];
         const int f1 = tastatur.finger_index(p1);
         const int s1 = tastatur.shift_finger_index(p1);
         const int f2 = tastatur.finger_index(p2);
         const int s2 = tastatur.shift_finger_index(p2);
         for(int e = 0; e < nebene; ++e) fhdiff[k][z][e] = fh[0][f1]-fh[1][f2];
         fhdiff[k][z][1] += fh[0][s1]-fh[1][s2];
      }
   }

   akkumuations_t a1diff[Haeufigkeit::groesse1];
   for(int z1 = 0; z1 < ntaste; ++z1){
      for(int e = 0; e < nebene; ++e)
         a1diff[nebene*z1+e] = b2 ? a(ib1[z1],e)-a(ib2[z1],e) : a(ib1[z1],e);
   }

   for(int z1 = 0; z1 < ntaste; ++z1){
      const int p1_1 = ib1[z1], p1_2 = ib2[z1];
      for(int e1 = 0; e1 < nebene; ++e1){
         const int o1 = nebene*z1+e1;
         for(int z2 = 0; z2 < ntaste; ++z2){
            int p2_1 = ib1[z2], p2_2 = ib2[z2];
            for(int e2 = 0; e2 < nebene; ++e2){
               const int o2 = nebene*z2+e2;
               const haeufigkeit_t h11 = h.ein_ein(o1, o2);
               summe += a1diff[o1]*a1diff[o2]*h11;

               for(int k1 = 0; k1 < h.num_korpus(); ++k1){
                  const haeufigkeit_t h11k = h.ein_ein_k(k1, o1, o2);
                  summe += a1diff[o1]*fhdiff[k1][z2][e2]*h11k;
                  const haeufigkeit_t h11kk = h.ein_k_ein_k(k1, o1, o2);
                  summe += fhdiff[k1][z1][e1]*fhdiff[k1][z2][e2]*h11kk;
               }

               if(e2 >= nebene2) continue;

               const int o12 = Haeufigkeit::index_bi_flach(o1, o2);
               const akkumuations_t a12_1 =      a(p1_1, e1, p2_1, e2);
               const akkumuations_t a12_2 = b2 ? a(p1_2, e1, p2_2, e2) : 0;
               for(int z3 = 0; z3 < ntaste; ++z3){
                  int p3_1 = ib1[z3], p3_2 = ib2[z3];
                  for(int e3 = 0; e3 < nebene; ++e3){
                     const int o3 = nebene*z3+e3;
                     const haeufigkeit_t h21 = h.ein_bi(o3, o12);
                     summe += (a12_1-a12_2)*a1diff[o3]*h21;

                     for(int k2 = 0; k2 < h.num_korpus(); ++k2){
                        const haeufigkeit_t h21k = h.ein_k_bi(k2, o3, o12);
                        summe += (a12_1-a12_2)*fhdiff[k2][z3][e3]*h21k;
                     }

                     for(int z4 = 0; z4 < ntaste; ++z4){
                        int p4_1 = ib1[z4], p4_2 = ib2[z4];
                        for(int e4 = 0; e4 < nebene2; ++e4){
                           const int o4 = nebene*z4+e4;
                           const int o34 = Haeufigkeit::index_bi_flach(o3, o4);
                           const akkumuations_t a34_1 = a(p3_1, e3, p4_1, e4);
                           const akkumuations_t a34_2 =
                              b2 ? a(p3_2, e3, p4_2, e4 ) : 0;
                           const haeufigkeit_t h22 = h.bi_bi(o12, o34);
                           summe += (a12_1-a12_2)*(a34_1-a34_2)*h22;
                        }
                     }
                  }
               }
            }
         }
      }
   }

   return summe;
}

akkumuations_t
konstanter_aufwand(const Haeufigkeit& h, const Aufwandstabelle& a)
{ return a.unbekannt()*h.unbekannt(); }

akkumuations_t
variabler_aufwand(const belegung_t b, const Haeufigkeit& h,
                  const Tastatur& tastatur, const Aufwandstabelle& a)
{
   fingerbelastung_t fh;
   berechne_fingerbelastung(b, h, tastatur, fh);
   return variabler_aufwand(b, h, fh, a);
}

//--------------- src/Haeufigkeit.cc ---------------
//#include "Haeufigkeit.hh"

//#include "Kodierung.hh"
//#include "Naechstes_zeichen.hh"
//#include "Tastatur.hh"
//#include "utfhilfe.hh"
#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>

namespace {

bool lies_codierte_ngramme(const std::string& name, size_t N, size_t Nmax,
                           zaehl_t** uh,  const Kodierung& kodierung,
                           bool muss_existieren, bool& utf8ein,
                           std::unordered_map<char32_t, zaehl_t>* unbekannt)
{
   Eingabestream tabelle(name, utf8ein, muss_existieren);
   if(!tabelle.neuezeile()) return false;

   while(tabelle.echte_neuezeile()){
      const zaehl_t h = hole_zahl(tabelle, 0, 1e15);
      pruefe_leer_dann_N(tabelle, N);
      tabelle.uebergehen();

      size_t ende1 = 0, anfangN = 0;  bool ok = true;
      std::vector<std::pair<int, int>> alles;
      for(size_t i = 0; i < N; ++i){
         const char32_t z = tabelle.lies_in_zeile();
         const auto p = kodierung.position(z);
         anfangN = alles.size();
         if(p.first >= 0){
            alles.push_back(p);
         }else if(auto s = kodierung.ersatz(z)){
            for(const auto& j : *s) alles.push_back(j);
         }else{
            ok = false;
            if(unbekannt) (*unbekannt)[z] += h;
         }
         if(!i) ende1 = alles.size();
      }

      if(!ok) continue;
      for(size_t i = 0; i < ende1; ++i){
         int offset = 0;
         for(size_t j = 0; j < Nmax && i+j < alles.size(); ++j){
            const int p = alles[i+j].first, e = alles[i+j].second;
            offset = (offset*ntaste+p)*nebene+e;
            if(j+1 >= N && i+j >= anfangN) uh[j][offset] += h;
         }
      }
   }

   if(tabelle.encoding_geaendert()) utf8ein = false;
   return true;
}

int leerzeichenindex(const Kodierung& kodierung){
   const auto p = kodierung.position(U' ');
   if(p.first < 0){
      const auto ersatz = kodierung.ersatz(U' ');
      return ersatz ? (*ersatz)[0].first*nebene+(*ersatz)[0].second : -1;
   }else return p.first*nebene+p.second;
}

}

Haeufigkeit::Haeufigkeit(const Tastatur& tastatur, const Kodierung& kod,
                         const std::vector<std::string>& basis,
                         const std::vector<double>& gewicht,
                         const std::vector<bool>& tri,
                         bool varianzen, bool berichte_unbekanne_zeichen)
   : nkorpus(basis.size()), trigramme(false), tastatur(tastatur),
     kodierung(kod), h11(nullptr), h21(nullptr), h22(nullptr)
{
   memset(h3, 0, sizeof(h3));
   memset(h2, 0, sizeof(h2));
   memset(h1, 0, sizeof(h1));
   memset(h1e, 0, sizeof(h1e));
   memset(h11e, 0, sizeof(h11e));
   memset(h11ee, 0, sizeof(h11ee));
   memset(h21e, 0, sizeof(h21e));

  std::unordered_map<char32_t, zaehl_t> summe_unbekannt;

  for(size_t i = 0; i < basis.size(); ++i){
      std::unordered_map<char32_t, zaehl_t> unbekannt;
      zaehl_ta uh1(new zaehl_t[groesse1]), uh2(new zaehl_t[groesse2]);
      zaehl_ta uh3, uh11, uh21, uh22, uh1l, uh1s, uh2l, uh2s;
      const bool t = tri[i];
      if(t){
         trigramme = true;
         uh3 = zaehl_ta(new zaehl_t[groesse3]);
      }
      zaehl_t llvarianz = 0, lsvarianz = 0, ssvarianz = 0;

      zaehl_t* uh[3] = { uh1.get(), uh2.get(), uh3.get() };

      bool nochmal, utf8[3] = {true, true, true}, utf8a[3] = {true, true, true};
      do{
         unbekannt.clear();
         nochmal = false;
         memset(uh1.get(), 0, sizeof(zaehl_t)*groesse1);
         memset(uh2.get(), 0, sizeof(zaehl_t)*groesse2);
         if(uh3) memset(uh3.get(), 0, sizeof(zaehl_t)*groesse3);

         if(lies_codierte_ngramme(basis[i]+".1", 1, t ? 3 : 2, uh, kodierung,
                                  false, utf8[0], berichte_unbekanne_zeichen
                                  ? &unbekannt : nullptr)){
            lies_codierte_ngramme(basis[i]+".2", 2, t ? 3 : 2, uh, kodierung,
                                  true, utf8[1], nullptr);
            if(t) lies_codierte_ngramme(basis[i]+".3", 3, 3, uh, kodierung,
                                        true, utf8[2], nullptr);
            for(size_t j = 0; j < 3; ++j){
               nochmal |= (utf8a[j] != utf8[j]);
               utf8a[j] = utf8[j];
            }
         }else{
            if(varianzen && utf8[0]){// nur beim ersten Versuch anlegen/init.
               uh11 = zaehl_ta(new zaehl_t[groesse11]);
               uh21 = zaehl_ta(new zaehl_t[groesse21]);
               uh22 = zaehl_ta(new zaehl_t[groesse22]);
               uh1l = zaehl_ta(new zaehl_t[groesse1]);
               uh1s = zaehl_ta(new zaehl_t[groesse1]);
               uh2l = zaehl_ta(new zaehl_t[groesse2]);
               uh2s = zaehl_ta(new zaehl_t[groesse2]);
               memset(uh11.get(), 0, sizeof(zaehl_t)*groesse11);
               memset(uh21.get(), 0, sizeof(zaehl_t)*groesse21);
               memset(uh22.get(), 0, sizeof(zaehl_t)*groesse22);
               memset(uh1l.get(), 0, sizeof(zaehl_t)*groesse1);
               memset(uh1s.get(), 0, sizeof(zaehl_t)*groesse1);
               memset(uh2l.get(), 0, sizeof(zaehl_t)*groesse2);
               memset(uh2s.get(), 0, sizeof(zaehl_t)*groesse2);
            }

            std::unique_ptr<Naechstes_zeichen> nz
               (new Naechstes_zeichen(basis[i], utf8[0]));

            const int o_korrelationsende = leerzeichenindex(kodierung);
            std::u32string wort;
            zaehl_t nworte = 0;
            std::unordered_map<std::u32string, zaehl_t> worthaeufigkeit;

            // Zähle Wörter, sammle 1/2/3-Gramme
            int o1 = -1, o2 = -1;
            while(const char32_t z = nz->get()){
               const auto p = kodierung.position(z);
               std::vector<std::pair<int,int>> pos(1, p);
               if(p.first < 0){
                  const auto ersatz = kodierung.ersatz(z);
                  if(ersatz) pos = *ersatz;
               }

               for(const auto j : pos){
                  const int o3 = j.first < 0
                     ? -1 : index_ein_flach(j.first, j.second);
                  if(o3 >= 0){
                     wort.push_back(o3);
                     ++uh1[o3];
                     if(o2 >= 0){
                        ++uh2[index_bi_flach(o2, o3)];
                        if(t && o1 >= 0)
                           ++uh3[index_tri_flach(o1,o2,o3)];
                     }
                     o1 = o2; o2 = o3;
                  }else{
                     if(berichte_unbekanne_zeichen && z >= U' ') ++unbekannt[z];
                     o1 = o2 = -1;
                  }

                  if(o3 < 0 || o3 == o_korrelationsende){
                     if(varianzen && wort.size()){
                        ++worthaeufigkeit[wort];
                        ++nworte;
                     }
                     wort.clear();
                     if(o3 >= 0) wort.push_back(o3);
                  }
               }
            }
            if(varianzen && wort.size()){
               ++worthaeufigkeit[wort];
               ++nworte;
            }

            nochmal = nz->encoding_geaendert();

            if(nochmal){
               utf8[0] = false;
            }else if(varianzen){
               const double rez_nworte = 1./nworte;
               for(const auto& wort_h : worthaeufigkeit){
                  const std::u32string& wort = wort_h.first;
                  const size_t rohe_wortlaenge = wort.length();
                  double anschlaege = 0., wortlaenge = 0.;

                  // Zähle n-Gramme im Wort
                  int o2 = -1;
                  std::unordered_map<int, double> wh1, wh2;
                  for(size_t j = 0; j < rohe_wortlaenge; ++j){
                     const int o3 = wort[j];
                     // Jedes "o_korrelationsende" wurde in zwei Wörter
                     // aufgenommen, daher zählen wir sie hier nur halb.
                     const double plus = o3 != o_korrelationsende ? 1 : 0.5;
                     if(o3%nebene) anschlaege += plus;
                     wh1[o3] += plus;
                     anschlaege += plus;
                     wortlaenge += plus;

                     if(o2 >= 0) ++wh2[index_bi_flach(o2, o3)];
                     o2 = o3;
                  }

                  // Aus Wortzahl und n-Grammzahl pro Wort die
                  // n-Grammhäufigkeiten bestimmen, Varianzen (unter Annahme
                  // einer Binominalverteilung der Worte) berechnen.
                  const zaehl_t h = wort_h.second;
                  const double varianz = h*(1.-h*rez_nworte);
                  llvarianz += wortlaenge*wortlaenge*varianz;
                  lsvarianz += wortlaenge*anschlaege*varianz;
                  ssvarianz += anschlaege*anschlaege*varianz;
                  for(auto j = wh1.cbegin(); j != wh1.cend(); ++j){
                     const int oj = j->first;
                     const auto hj = j->second;
                     uh1l[oj] += varianz*hj*wortlaenge;
                     uh1s[oj] += varianz*hj*anschlaege;
                     for(auto k = j; k != wh1.cend(); ++k){
                        const int ok = k->first;
                        const auto hk = k->second;
                        uh11[sym_index(oj, ok)] += varianz*(hj*hk);
                     }
                     for(auto k = wh2.cbegin(); k != wh2.cend(); ++k){
                        const int ok = k->first;
                        const auto hk = k->second;
                        uh21[index_tri21_flach(ok, oj)] += varianz*(hj*hk);
                     }
                  }
                  for(auto j = wh2.cbegin(); j != wh2.cend(); ++j){
                     const int oj = j->first;
                     const auto hj = j->second;
                     uh2l[oj] += varianz*hj*wortlaenge;
                     uh2s[oj] += varianz*hj*anschlaege;
                     for(auto k = j; k != wh2.cend(); ++k){
                        const int ok = k->first;
                        const auto hk = k->second;
                        uh22[sym_index(ok, oj)] += varianz*(hj*hk);
                     }
                  }
               }
            }
         }
      }while(nochmal);
      akkumuliere(i, gewicht[i], uh1.get(), uh2.get(), uh3.get(),
                  uh11.get(), uh21.get(), uh22.get(),
                  uh1l.get(), uh1s.get(), uh2l.get(), uh2s.get(),
                  llvarianz, lsvarianz, ssvarianz,
                  unbekannt, summe_unbekannt);
      gewichte[i] = gewicht[i];
   }

   for(int i = 0; i < ntaste; ++i){
      for(int ei = 0; ei < nebene; ++ei){
         if(kodierung.ist_platzhalter(i, ei)) continue;
         const auto& txt = kodierung.txt(i, ei);
         if(h1[i][ei] == 0 && (ei == 0 || txt != kodierung.txt(i, 0)))
            std::cerr << SPRACHE("Zeichen '", "Symbol '") << txt
                      << SPRACHE("' tritt im Korpus nie auf.",
                                 "' does not appear in the corpus.")
                      << std::endl;
      }
   }

   hunbekannt = 0;
   zaehl_t hmax = 0; char32_t zmax = 0;
   for(const auto& zh : summe_unbekannt){
      hunbekannt += zh.second;
      if(zh.second > hmax){
         hmax = zh.second; zmax = zh.first;
      }
   }
   if(berichte_unbekanne_zeichen && hunbekannt > 0){
      std::cerr << summe_unbekannt.size()
                << SPRACHE(" verschiedene unbekannte Zeichen, relative "
                           "Gesamth" strAe "ufigkeit ",
                           " distinct unknown symbols, relative total "
                           "frequency ")
                << hunbekannt*100 << "%.  "
                << SPRACHE("Das wichtigste dieser Zeichen ist '",
                           "The most important of these symbols is '")
                << utf32_in_ausgabe(zmax) << "'.\n" << std::endl;
   }
}

// Kein copy-Konstruktor: Korrelationstabellen werden nicht kopiert.
Haeufigkeit::Haeufigkeit(const Haeufigkeit& h, int)
   : hunbekannt(h.hunbekannt), nkorpus(h.nkorpus),
     trigramme(h.trigramme), tastatur(h.tastatur), kodierung(h.kodierung),
     h11(nullptr) // nur für Destruktor
{
   for(int i = 0; i < nkorpus; ++i) gewichte[i] = h.gewichte[i];
   belegung_t b;
   for(int j = 0; j < ntaste; ++j) b[j] = j;
   setze(h, b);
}

Haeufigkeit::~Haeufigkeit(){
   if(!h11) return;
   delete []h11; delete []h21; delete []h22;
   for(int i = 0; i < nkorpus; ++i){
      delete []h11e[i]; delete []h11ee[i]; delete []h21e[i];
   }
}

void
Haeufigkeit::varianzen_anlegen(){
   if(mit_varianzen()) return;
   h11 = new haeufigkeit_t[groesse11];
   h21 = new haeufigkeit_t[groesse21];
   h22 = new haeufigkeit_t[groesse22];
   memset(h11, 0, sizeof(haeufigkeit_t)*groesse11);
   memset(h21, 0, sizeof(haeufigkeit_t)*groesse21);
   memset(h22, 0, sizeof(haeufigkeit_t)*groesse22);
   for(int i = 0; i < nkorpus; ++i){
      h11e[i]  = new haeufigkeit_t[groesse1*groesse1];
      memset(h11e[i],  0, sizeof(haeufigkeit_t)*groesse1*groesse1);
      h11ee[i] = new haeufigkeit_t[groesse11];
      memset(h11ee[i], 0, sizeof(haeufigkeit_t)*groesse11);
      h21e[i]  = new haeufigkeit_t[groesse21];
      memset(h21e[i],  0, sizeof(haeufigkeit_t)*groesse21);
   }
}

void
Haeufigkeit::setze(const Haeufigkeit& h, const belegung_t p)
{
   for(int p1 = 0; p1 < ntaste; ++p1){
      const int z1 = p[p1];
      for(int e1 = 0; e1 < nebene; ++e1)
         h1[p1][e1] = h.h1[z1][e1];
      for(int k = 0; k < nkorpus; ++k)
         for(int e1 = 0; e1 < nebene; ++e1)
            h1e[k][p1][e1] = h.h1e[k][z1][e1];
   }
   for(int p1 = 0; p1 < ntaste; ++p1){
      const int z1 = p[p1];
      for(int p2 = 0; p2 < ntaste; ++p2){
         const int z2 = p[p2];
         for(int e1 = 0; e1 < nebene; ++e1)
            for(int e2 = 0; e2 < nebene2; ++e2)
               h2[p1][p2][e1][e2] = h.h2[z1][z2][e1][e2];
      }
   }
   if(trigramme){
      for(int p1 = 0; p1 < ntaste; ++p1){
         const int z1 = p[p1];
         for(int p2 = 0; p2 < ntaste; ++p2){
            const int z2 = p[p2];
            for(int p3 = 0; p3 < ntaste; ++p3){
               const int z3 = p[p3];
               for(int e1 = 0; e1 < nebene; ++e1)
                  h3[p1][p2][p3][e1] = h.h3[z1][z2][z3][e1];
            }
         }
      }
   }
}

// Sollte mit C++11 funktionieren, Solaris Studio 12.4 packt es aber nicht:
// void Haeufigkeit::swap(int p1, int p2){
//    std::swap(h1[p1], h1[p2]);
//    for(int k = 0; k < nkorpus; ++k)
//       std::swap(h1e[k][p1], h1e[k][p2]);
//    std::swap(h2[p1], h2[p2]);
//    for(int p = 0; p < ntaste; ++p)
//       std::swap(h2[p][p1], h2[p][p2]);
//
//    if(trigramme){
//       std::swap(h3[p1], h3[p2]);
//       for(int pi = 0; pi < ntaste; ++pi)
//          std::swap(h3[pi][p1], h3[pi][p2]);
//       for(int pi = 0; pi < ntaste; ++pi)
//          for(int pj = 0; pj < ntaste; ++pj)
//             std::swap(h3[pi][pj][p1], h3[pi][pj][p2]);
//    }
// }

void Haeufigkeit::swap(int p1, int p2){
   for(int e1 = 0; e1 < nebene; ++e1){
      const haeufigkeit_t t = h1[p1][e1];
      h1[p1][e1] = h1[p2][e1];
      h1[p2][e1] = t;
   }
   for(int k = 0; k < nkorpus; ++k)
      for(int e1 = 0; e1 < nebene; ++e1){
         const haeufigkeit_t t = h1e[k][p1][e1];
         h1e[k][p1][e1] = h1e[k][p2][e1];
         h1e[k][p2][e1] = t;
      }

   for(int p = 0; p < ntaste; ++p)
      for(int e1 = 0; e1 < nebene; ++e1)
         for(int e2 = 0; e2 < nebene2; ++e2){
            const haeufigkeit_t t = h2[p1][p][e1][e2];
            h2[p1][p][e1][e2] = h2[p2][p][e1][e2];
            h2[p2][p][e1][e2] = t;
         }
   for(int p = 0; p < ntaste; ++p)
      for(int e1 = 0; e1 < nebene; ++e1)
         for(int e2 = 0; e2 < nebene2; ++e2){
            const haeufigkeit_t t = h2[p][p1][e1][e2];
            h2[p][p1][e1][e2] = h2[p][p2][e1][e2];
            h2[p][p2][e1][e2] = t;
         }
   if(trigramme){
      for(int pi = 0; pi < ntaste; ++pi)
         for(int pj = 0; pj < ntaste; ++pj)
            for(int e1 = 0; e1 < nebene; ++e1){
               const haeufigkeit_t t = h3[p1][pi][pj][e1];
               h3[p1][pi][pj][e1] = h3[p2][pi][pj][e1];
               h3[p2][pi][pj][e1] = t;
            }
      for(int pi = 0; pi < ntaste; ++pi)
         for(int pj = 0; pj < ntaste; ++pj)
            for(int e1 = 0; e1 < nebene; ++e1){
               const haeufigkeit_t t = h3[pi][p1][pj][e1];
               h3[pi][p1][pj][e1] = h3[pi][p2][pj][e1];
               h3[pi][p2][pj][e1] = t;
            }
      for(int pi = 0; pi < ntaste; ++pi)
         for(int pj = 0; pj < ntaste; ++pj)
            for(int e1 = 0; e1 < nebene; ++e1){
               const haeufigkeit_t t = h3[pi][pj][p1][e1];
               h3[pi][pj][p1][e1] = h3[pi][pj][p2][e1];
               h3[pi][pj][p2][e1] = t;
            }
   }
}

void Haeufigkeit::
akkumuliere(int korpus, double gewicht,
            const zaehl_t* uh1,  const zaehl_t* uh2, const zaehl_t* uh3,
            const zaehl_t* uh11, const zaehl_t* uh21, const zaehl_t* uh22,
            const zaehl_t* uh1l, const zaehl_t* uh1s,
            const zaehl_t* uh2l, const zaehl_t* uh2s,
            zaehl_t ll, zaehl_t ls, zaehl_t ss,
            const std::unordered_map<char32_t, zaehl_t>& unbekannt,
            std::unordered_map<char32_t, zaehl_t>& summe_unbekannt)
{
   zaehl_t total = 0, geshiftet = 0, mitfix = 0;

   bool shift_variabel = 0;
   for(int i = ntaste; i < ntaste+nshift; ++i)
      if(!tastatur.finger_fix(tastatur.finger_index(i))) shift_variabel = true;

   for(int i = 0; i < ntaste; ++i){
      for(int ei = 0; ei < nebene; ++ei){
         const int oi = index_ein_flach(i, ei);
         if(!tastatur.finger_fix(tastatur.finger_index(i))) total += uh1[oi];
         mitfix += uh1[oi];
         if(ei && shift_variabel) geshiftet += uh1[oi];
      }
   }

   const zaehl_t htot_k = total+geshiftet;
   const double w = gewicht/total, w2 = w*w;
   const double wf = 1./htot_k, wf2 = wf*wf, wfw = wf*w;
   const double mitfix_w = gewicht/mitfix;

   for(int i = 0; i < ntaste; ++i){
      for(int ei = 0; ei < nebene; ++ei){
         const int oi = index_ein_flach(i, ei);
         ein(i,ei) += uh1[oi]*w;
         ein_k(korpus,i,ei) = uh1[oi]*wf;

         for(int j = 0; j < ntaste; ++j){
            for(int ej = 0; ej < nebene2; ++ej){
               const int oj = index_ein_flach(j, ej);
               const int oij = index_bi_flach(oi, oj);
               bi(i,ei,j,ej) += uh2[oij]*w;

               // Zu dieser speziellen Summation siehe den Kommentar in
               // Aufwandstabelle::Aufwandstabelle
               if(uh3 && ej == 0){
                  for(int k = 0; k < ntaste; ++k){
                     for(int ek = 0; ek < nebene; ++ek){
                        const int ok = index_ein_flach(k, ek);
                        const int oijk = index_tri21_flach(oij, ok);
                        tri(i,j,k,ek) += uh3[oijk]*w;
                     }
                  }
               }
            }
         }
      }
   }
   if(uh11){
      const double wk = 1./total;
      varianzen_anlegen();
      for(int oi = 0; oi < groesse1; ++oi){
         const haeufigkeit_t hi = uh1[oi]*wk, hi_f = uh1[oi]*wf;
         for(int oj = 0; oj < groesse1; ++oj){
            const haeufigkeit_t hj = uh1[oj]*wk, hj_f = uh1[oj]*wf;
            const zaehl_t uhij = uh11[sym_index(oi, oj)];
            ein_ein_k(korpus, oi, oj) =
               (uhij-hj_f*uh1s[oi]-hi*uh1l[oj]+ls*hi*hj_f)*wfw;

            if(oj > oi) continue;
            ein_ein(oi, oj) +=
               (uhij-hj*uh1l[oi]-hi*uh1l[oj]+ll*hi*hj)*w2;
            ein_k_ein_k(korpus, oi, oj) =
               (uhij-hj_f*uh1s[oi]-hi_f*uh1s[oj]+ss*hi_f*hj_f)*wf2;
         }
      }
      for(int oij = 0; oij < groesse2; ++oij){
         const haeufigkeit_t hij = uh2[oij]*wk;
         for(int ok = 0; ok < groesse1; ++ok){
            const haeufigkeit_t hk = uh1[ok]*wk, hk_f = uh1[ok]*wf;
            const zaehl_t uhijk = uh21[index_tri21_flach(oij, ok)];
            ein_bi(ok, oij) +=
               (uhijk-hij*uh1l[ok]-hk*uh2l[oij]+ll*hij*hk)*w2;
            ein_k_bi(korpus, ok, oij) =
               (uhijk-hij*uh1l[ok]-hk_f*uh2s[oij]+ls*hij*hk_f)*wfw;
         }
         for(int okl = 0; okl <= oij; ++okl){
            const haeufigkeit_t hkl = uh2[okl]*wk;
            bi_bi(oij, okl) += (uh22[sym_index(oij, okl)]-hij*uh2l[okl]
                                -hkl*uh2l[oij]+ll*hij*hkl)*w2;
         }
      }
   }

   for(const auto& zh : unbekannt){
      summe_unbekannt[zh.first] += mitfix_w*zh.second;
   }
}

void
Haeufigkeit::statistik() const {
   const Haeufigkeit& h = *this;
   haeufigkeit_t htot = 0, h2tot = 0, doppelt = 0, gross = 0;
   for(int i = 0; i < ntaste; ++i){
      htot += h(i,0)+h(i,1);
      gross += h(i,1);
      for(int ei = 0; ei < nebene; ++ei){
         for(int ej = 0; ej < nebene2; ++ej){
            doppelt += h(i,ei,i,ej);
            for(int j = 0; j < ntaste; ++j)
               h2tot += h(i,ei,j,ej);
         }
      }
   }

   right(std::cout);  fixed(std::cout);
   std::cout << std::setprecision(3)
             << SPRACHE("Zeichenh" strAe "ufigkeit klein/gross:\n\n",
                        "Symbol frequencies upper/lower case:\n\n");
   for(int i = 0; i < ntaste; ++i){
      if(i && i%4 == 0) std::cout << "\n";
      std::cout << kodierung.txt(i, 0) << kodierung.txt(i, 1) << " "
                << std::setw(6) << 100.*h(i,0)/htot << "/"
                << std::setw(5) << 100.*h(i,1)/htot << "   ";
   }

   std::cout << SPRACHE("\n\nGrossbuchstaben:  ", "\n\nCapital letters:  ")
             << std::setw(5) << 100*gross/htot
             << SPRACHE(" %\nDoppeltanschl" strAe "ge: ",
                        " %\nDouble strokes:   ")
             << std::setw(5) << 100*doppelt/h2tot << " %\n" << std::endl;
}

//--------------- src/Tastatur.cc ---------------
//#include "Tastatur.hh"

//#include "konstanten.hh"
//#include "utfhilfe.hh"
#include <cassert>
#include <cmath>
#include <iostream> // für Fehlerausgabe, besser wäre Exception

bool
Tastatur::istDaumen(finger_t i)
{ return std::abs(i) <= finger_t::DaumenRechts; }

bool
Tastatur::istKleinfinger(finger_t i)
{ return std::abs(i) == finger_t::KleinfingerRechts; }

const char*
Tastatur::kategorie_str(int kategorie){
   const char* kategorie_s[] = {
      SPRACHE("Handwechsel", "Hand alternation"),
      SPRACHE("Kollision", "Same finger repetition"),
      SPRACHE("Doppeltanschlag", "Double stroke"),
      SPRACHE("Ausw" strAe "rts", "Outwards"),
      SPRACHE("Einw" strAe "rts", "Inwards"),
      SPRACHE("Mit undefiniertem Daumen", "With undefined thumb") };
   return kategorie_s[kategorie];
}

std::string
Tastatur::kategorie_lang(int i, int j) const {
   assert(i < ntaste+nshift && j < ntaste+nshift);
   const int dzeile = std::abs(zeile(i)-zeile(j));
   const int fi = finger(i), fj = finger(j);
   std::string typ = kategorie_str(kategorie(i, j));
   if(istHandwiederholung(i,j)){
      if(std::abs(fi-fj) == 1)
         typ += SPRACHE(", Nachbarfinger", ", adjacent finger");
      if(dzeile == 1)
         typ += SPRACHE(", Zeilensprung", ", row jump");
      if(dzeile == 2)
         typ += SPRACHE(", doppelter Zeilensprung", ", double row jump");
   }
   return typ;
}

int
Tastatur::spalte(int i) const {
   assert(i < ntaste+nshift);
   return spalten[i];
}

int
Tastatur::zeile(int i) const {
   assert(i < ntaste+nshift);
   return zeilen[i];
}

finger_t
Tastatur::finger(int i) const {
   assert(i < ntaste+nshift);
   return fingertab[i];
}

int
Tastatur::finger_index(int i) const {
   assert(i < ntaste+nshift);
   return fingerind[i];
}

int
Tastatur::shifttaste(int i) const {
   assert(i < ntaste+nshift);
   return shifttast[i];
}

int
Tastatur::shift_finger_index(int i) const {
   assert(i < ntaste+nshift);
   return sfingerind[i];
}

int
Tastatur::grundposition(int i) const {
   assert(i < ntaste+nshift);
   return grundpos[i];
}

int
Tastatur::taste(int z, int s) const {
   if(z < 0 || z >= nzeile || s < 0 || s >= nspalte) return -1;
   return tastennr[z][s];
}

int
Tastatur::taste(int seq_taste) const {
   return seq[seq_taste];
}

const std::u32string&
Tastatur::name(int i) const {
   assert(i < ntaste+nshift);
   return namen[i];
}

int
Tastatur::taste(const std::u32string& n) const {
   const auto i = namennr.find(n);
   return i == namennr.end() ? -1 : i->second;
}

kategorie_t
Tastatur::kategorie(int i, int j) const {
   assert(i < ntaste+nshift && j < ntaste+nshift);
   return tastenkategorie[i][j];
}

const std::vector<int>&
Tastatur::benutzerkategorie(const std::vector<int>& i) const {
   static const std::vector<int> leerervektor;
   const auto p = benutzerkat.find(i);
   return p == benutzerkat.end() ? leerervektor : p->second;
}

const std::string&
Tastatur::benutzerkategorie_name(int i) const
{ return kategorie_liste[i]; }

bool
Tastatur::istHandwiederholung(int i, int j) const
{ return kategorie(i,j) != kategorie_t::Handwechsel &&
      kategorie(i,j) != kategorie_t::MitUndefDaumen; }

bool
Tastatur::istWippe(int i, int j, int k) const {
   return (kategorie(i, j) == kategorie_t::Einwaerts &&
           kategorie(j, k) == kategorie_t::Auswaerts)
      || (kategorie(i, j) == kategorie_t::Auswaerts &&
          kategorie(j, k) == kategorie_t::Einwaerts);
}

koordinate_t
Tastatur::tastenkoord(int i) const
{ return koordinate_t{x[i], y[i]}; }

double
Tastatur::distanz(int i, int j) const {
   const auto ki = tastenkoord(i), kj = tastenkoord(j);
   return std::hypot(ki.x-kj.x, ki.y-kj.y);
}

bool
Tastatur::finger_fix(int i) const
{ return fix[i]; }

int
Tastatur::nvariabel() const
{ return nvar; }

double
Tastatur::lageaufwand(int i) const
{
   assert(i < ntaste+nshift);
   return aufwand[i];
}

Tastatur::Tastatur(const std::vector<taste_t>& tasten,
                   const std::unordered_set<std::u32string>& fixe_tasten){
   nvar = ntaste-fixe_tasten.size();
   if(nvar < 2){
      std::cerr << SPRACHE("Alle Tasten sind fix belegt.",
                           "All keys have a fixed mapping.")
                << std::endl;
      exit(1);
   }

   for(auto& i : tastennr) for(auto& j : i) j = -1;

   int gpos[nfinger+1];
   for(auto& i: gpos) i = -1;
   int ifix = nvar, ivar = 0;
   for(int j = 0; j < ntaste+nshift; ++j){
      const taste_t& t = tasten[j];
      const bool istFix = fixe_tasten.find(t.name) != fixe_tasten.end();
      const int i = istFix ? ifix++ : (j < ntaste ? ivar++ : j);
      seq[j] = i;
      namen[i] = t.name;
      namennr[t.name] = i;
      spalten[i] = t.spalte;
      zeilen[i] = t.zeile;
      // Wenn meherere Tasten in derselben Spalte und Zeile sind "gewinnt"
      // die erste nicht fixe.
      if(tastennr[t.zeile][t.spalte] == -1){
         tastennr[t.zeile][t.spalte] = i;
      }else{
         if(tastennr[t.zeile][t.spalte] >= nvar && !istFix)
            tastennr[t.zeile][t.spalte] = i;
      }

      fingertab[i] = t.finger;
      fingerind[i] = (t.finger < 0)
         ? t.finger+5
         : (t.finger > 0 ? t.finger+4 : nfinger);

      if(t.istGrundposition) gpos[fingerind[i]] = i;
      x[i] = t.x;  y[i] = t.y;
      aufwand[i] = t.aufwand;

      shifttast[i] = t.seite ? ntaste : ntaste+(nshift-1);
   }

   for(int i = 0; i < ntaste+nshift; ++i){
      sfingerind[i] = finger_index(shifttaste(i));
      const int fi = finger_index(i);
      grundpos[i] = gpos[fi] < 0 ? i : gpos[fi];
   }

   for(int i = 0; i < ntaste+nshift; ++i){
      const int finger_i = finger(i);
      for(int j = 0; j < ntaste+nshift; ++j){
         const int finger_j = finger(j);

         if(finger_i == finger_t::EinerDerDaumen ||
            finger_j == finger_t::EinerDerDaumen){
            tastenkategorie[i][j] = kategorie_t::MitUndefDaumen;
         }else if((finger_i^finger_j) < 0){
            tastenkategorie[i][j] = kategorie_t::Handwechsel;
         }else{
            if(spalte(i) == spalte(j) && zeile(i) == zeile(j)){
               tastenkategorie[i][j] = kategorie_t::Doppeltanschlag;
            }else if(finger_i == finger_j){
               tastenkategorie[i][j] = kategorie_t::Kollision;
            }else{
               const bool auswaerts =
                  (std::abs(finger_j) > std::abs(finger_i));
               tastenkategorie[i][j] = auswaerts
                  ? kategorie_t::Auswaerts
                  : kategorie_t::Einwaerts;
            }
         }
      }
   }

   bool mehrere_shift = false;
   for(auto& i : fix) i = true;
   for(int i = 0; i < nvar; ++i){
      fix[finger_index(i)] = false;
      if(shift_finger_index(i) != shift_finger_index(0))
         mehrere_shift = true;
   }
   if(mehrere_shift)
      for(int i = 0; i < nvar; ++i) fix[shift_finger_index(i)] = false;
}

void
Tastatur::neue_kategorien(const std::vector<n_gramm_t>& benutzerkategorie){
   for(const auto& i : benutzerkategorie){
      const std::string n = utf32_in_ausgabe(i.name);
      size_t p = 0;
      for(; p < kategorie_liste.size(); ++p)
         if(kategorie_liste[p] == n) break;
      if(p == kategorie_liste.size())
         kategorie_liste.push_back(n);
      std::vector<int> ngramm = { i.t1, i.t2 };
      if(i.t3 >= 0) ngramm.push_back(i.t3);
      benutzerkat[ngramm].push_back(p);
   }
}

//--------------- src/Kodierung.cc ---------------
//#include "Kodierung.hh"

//#include "Unicode.hh"
//#include "utfhilfe.hh"
#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>

Kodierung::Kodierung(const std::vector<std::u32string>& klartext,
                     const std::vector<std::u32string>& ersatzstring,
                     const std::vector<std::u32string>& glyph,
                     char32_t platzhalter)
   : platzhalterstr(utf32_in_ausgabe(platzhalter))
{
   // DEL, dann kommen die hohen CTRLs, die können wir alle belegen.
   unsigned char freiercode = 127;
   if(klartext.size() != ntaste){
      std::cerr << SPRACHE("Die Anzahl der der Zeichen ist ",
                           "The number of symbols is ")
                << klartext.size()
                << SPRACHE(
                   ", sollte jedoch gleich der Anzahl der Zeichentasten ",
                   ", but it should be equal to the number of symbol keys ")
                << ntaste << SPRACHE(" sein.", ".") << std::endl;
      exit(1);
   }
   for(int i = 0; i < ntaste; ++i){
      for(int e = 0; e < nebene; ++e) chars[i][e] = 0;
      psenc[i] = 0;
      for(int e = 0; e < nebene; ++e){
         const char32_t c =
            klartext[i].length() < nebene ? klartext[i][0] : klartext[i][e];
         if(c == platzhalter) continue;
         chars[i][e] = c;
         if(c < 256 &&
            (psenc[i] == 0 || psenc[i] == Unicode::get().kleinbuchstabe(c))){
            psenc[i] = c;
         }
         strings[i][e] = utf32_in_ausgabe(c);
         if(invers.find(c) == invers.end()) invers[c] = std::make_pair(i, e);
      }
      for(size_t e = nebene; e < klartext[i].length(); ++e){
         const char32_t extra = klartext[i][e];
         if(psenc[i] == 0 && extra < 256) psenc[i] = extra;
         if(invers.find(extra) == invers.end())
            invers[extra] = std::make_pair(i, nebene-1);
      }

      glyphname[i] = utf32_in_ausgabe(glyph[i]);
      if(psenc[i] == 0){
         psenc[i] = freiercode;
         if(freiercode == 9 || freiercode == 12){
            freiercode += 2;  // CR, LF übergehen
         }else if(freiercode == 159){
            // 160 ist nichtumbrechendes Leerzeichen, das überschreiben wir
            // nicht.  Fülle dann den CTRL-Bereich.
            freiercode = 1;
         }else ++freiercode;

         if(glyphname[i].length() == 0){
            const int codepoint =
               (chars[i][0] == 0 ||
                Unicode::get().kleinbuchstabe(chars[i][1]) == chars[i][0])
               ? chars[i][1] : chars[i][0];
            const int laenge = codepoint < 0x10000 ? 4 : 5;
            const std::string u = codepoint < 0x10000 ? "uni" : "u";
            std::ostringstream os;
            os << u << std::setw(laenge) << std::setfill('0')
               << std::uppercase << std::hex << codepoint;
            glyphname[i] = os.str();
         }
      }
   }

   for(const auto& i : ersatzstring){
      if(i.length() == 0) continue;
      std::vector<std::pair<int, int>> ersatzliste;
      const char32_t n = i[0];
      std::u32string fehlt;
      for(size_t j = 1; j < i.length(); ++j){
         const auto p = position(i[j]);
         if(p.first < 0){
            const auto* pe = ersatz(i[j]);
            if(pe){
               for(const auto& pk : *pe) ersatzliste.push_back(pk);
            }else fehlt.push_back(i[j]);
         }else ersatzliste.push_back(p);
      }
      if(fehlt.size() == 0){
         inversstr[n] = ersatzliste;
      }else{
         std::cerr << "Ersatz '" << utf32_in_ausgabe(i) << "': "
                   << SPRACHE("Warnung: Das Zeichen '",
                              "Warning: The character '")
                   << utf32_in_ausgabe(n)
                   << SPRACHE(
                      "' kann nicht ersetzt werden weil das/die Zeichen '",
                      "' cannot be substituted because the character(s) '")
                   << utf32_in_ausgabe(fehlt)
                   << SPRACHE("' nicht in der Belegung sind.",
                              "' are not in the layout.")
                   << std::endl;
      }
   }
}
bool
Kodierung::ist_platzhalter(int i, int e) const
{ assert(i < ntaste && e < nebene); return strings[i][e].length() == 0; }

const std::string&
Kodierung::txt(int i, int e) const
{ return ist_platzhalter(i,e) ? platzhalterstr : strings[i][e]; }

char32_t
Kodierung::uchar(int i, int e) const
{ assert(i < ntaste && e < nebene); return chars[i][e];}

const std::string&
Kodierung::bevorzugt(int i) const {
   assert(i < ntaste);
   return strings[i][0].length() ? strings[i][0] : strings[i][1];
}
const std::string&
Kodierung::txt(int ie) const
{ assert(ie < ntaste*nebene);  return strings[ie/nebene][ie%nebene]; }

std::pair<int, int>
Kodierung::position(char32_t z) const {
   const auto i = invers.find(z);
   if(i == invers.end()) return std::make_pair(-1, -1);
   return i->second;
}

const std::vector<std::pair<int,int>>*
Kodierung::ersatz(char32_t z) const {
   const auto i = inversstr.find(z);
   return i == inversstr.end() ? nullptr : &i->second;
}

int
Kodierung::psencoding(int i) const
{ assert(i < ntaste); return psenc[i]; }

std::string
Kodierung::psencstr(int i) const {
   std::string str;
   if(psenc[i] == '(' || psenc[i] == ')' || psenc[i] == '\\')
      str.push_back('\\');
   str.push_back(psenc[i]);
   return str;
}

const std::string&
Kodierung::psglyphname(int i) const
{ assert(i < ntaste); return glyphname[i]; }

//--------------- src/schreibe_belegung.cc ---------------
//#include "schreibe_belegung.hh"

//#include "Aufwandstabelle.hh"
//#include "Haeufigkeit.hh"
//#include "Kodierung.hh"
//#include "Statistik.hh"
//#include "Tastatur.hh"
//#include "konstanten.hh"
//#include "utfhilfe.hh"
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#define SP1 std::setprecision(1)
#define SP2 std::setprecision(2)
#define SP3 std::setprecision(3)
#define SP4 std::setprecision(4)
#define SW2 std::setw(2)
#define SW4 std::setw(4)
#define SW5 std::setw(5)
#define SW6 std::setw(6)
#define SW7 std::setw(7)
#define SW8 std::setw(8)

namespace {

std::string
decodiere(const std::string& seq, const Kodierung& kodierung)
{
   std::string resultat;
   for(const auto j : seq) resultat += kodierung.txt(j);
   return resultat;
}

void
handeinsaetze(const belegung_t b, const Tastatur& tastatur,
              const Kodierung& kodierung,
              const std::unordered_map<std::string, haeufigkeit_t>& wortliste,
              const akkumuations_t limit)
{
   bool seite[ntaste];
   for(int i = 0; i < ntaste; ++i) seite[b[i]] = (tastatur.finger(i) < 0);

   std::unordered_map<std::string, akkumuations_t> handeinsatz;
   akkumuations_t summe = 0, summe2 = 0, tot = 0, stot[2] = { 0, 0 };
   std::vector<akkumuations_t> prolaenge[2];
   for(auto& i : wortliste){
      const std::string& wort = i.first;
      const haeufigkeit_t h = i.second;

      bool warseite = seite[wort[0]/nebene];
      size_t start = 0;
      for(size_t j = 1; j <= wort.size(); ++j){
         if(j == wort.size() || warseite != seite[wort[j]/nebene]){
            const size_t len = j-start;

            handeinsatz[wort.substr(start, len)] += h;

            stot[warseite] += h;
            while(prolaenge[0].size() <= len){
               prolaenge[0].push_back(0);
               prolaenge[1].push_back(0);
            }
            assert(prolaenge[0].size() == prolaenge[1].size());
            prolaenge[warseite][len] += h;

            summe +=  len*h;
            summe2 += len*len*h;
            tot += h;

            warseite = !warseite;
            start = j;
         }
      }
   }

   std::multimap<akkumuations_t, std::string> sortiert;
   size_t laengster = 0;
   akkumuations_t hlaengster = 0;
   std::string tlaengster;
   for(const auto& i : handeinsatz){
      sortiert.insert(std::make_pair(i.second, i.first));
      size_t len = i.first.size();
      if(len > laengster || (len == laengster && hlaengster < i.second)){
         laengster = len;
         tlaengster = i.first;
         hlaengster = i.second;
      }
   }

   akkumuations_t mittel = summe/tot;
   std::cout << SPRACHE("\nInsgesamt ", "\nIn total ") << sortiert.size()
             << SPRACHE(
                " verschiedene Handeins" strAe "tze, mittlere L" strAe "nge ",
                " different one-handed sequences, average length ")
             << SP3 << mittel
             << SPRACHE("\n  Standardabweichung ",
                        "\n  standard deviation ")
             << SP3 << std::sqrt(summe2/tot-mittel*mittel)
             << SPRACHE(", maximale L" strAe "nge ",
                        ", maximum length ") << laengster
             << SPRACHE(" f" strUe "r ",
                        " for ") << decodiere(tlaengster, kodierung)
             << SPRACHE("\n\n#  % links  % summiert    % rechts % summiert    "
                        "% beide  % summiert\n",
                        "\n\n#  % left   % cumulative  % right  % cumulative  "
                        "% both   % cumulative\n");

   akkumuations_t procum[2] = { 0, 0 };
   for(size_t i = 1; i+1 < prolaenge[0].size(); ++i){
      procum[0] += prolaenge[0][i];
      procum[1] += prolaenge[1][i];
      std::cout << SW2 << i
                << SP4 << SW8 << 100*(prolaenge[1][i]/stot[1]) << " ("
                << SP4 << SW8 << 100*(procum[1]/stot[1]) << ")    "
                << SP4 << SW8 << 100*(prolaenge[0][i]/stot[0]) << " ("
                << SP4 << SW8 << 100*(procum[0]/stot[0]) << ")    "
                << SP4 << SW8 << 100*((prolaenge[0][i]+prolaenge[1][i])/tot)
                << " ("
                << SP4 << SW8 << 100*((procum[0]+procum[1])/tot) << ")\n";
   }

   if(limit > 0){
      std::cout << SPRACHE("\n#    %       % summiert\n",
                           "\n#    %       % cumulative\n");
      akkumuations_t sum = 0;
      int n = 0;
      for(auto i = sortiert.crbegin(); i != sortiert.crend(); ++i){
         if(sum > limit) break;
         n++;
         const akkumuations_t h = 100*i->first/tot;
         sum += h;
         std::cout << SW4 << n
                   << SP3 << SW7 << h << " ("
                   << SP3 << SW7 << sum << ")  "
                   << decodiere(i->second, kodierung) << "\n";
      }
   }
}

}

void schreibe_belegung(const belegung_t b, const Tastatur& tastatur,
                       const Kodierung& kodierung, const Haeufigkeit& h,
                       const Aufwandstabelle& a, const akkumuations_t A,
                       const std::u32string& name,
                       const double ngrammakkumlimit[3],
                       const std::unordered_map<std::string, haeufigkeit_t>&
                          wortliste,
                       const akkumuations_t handeinsatzlimit,
                       bool als_fixeszeichen)
{
   std::string str[nzeile];
   for(int z = 0; z < nzeile; ++z){
      for(int s = 0; s < nspalte; ++s){
         const int t = tastatur.taste(z, s);
         str[z] += (t < 0 || t >= ntaste)
            ? " " : kodierung.bevorzugt(b[t]);
      }
   }

   Statistik S(b, tastatur, kodierung, h, a, ngrammakkumlimit);

   const std::string lspalte(nspalte, ' ');

   std::cout << utf32_in_ausgabe(name);
   for(int i = name.length(); i < nspalte+1; ++i) std::cout << " ";
   right(std::cout);  fixed(std::cout);

   std::cout << SP3 << SW7 << 100*A
             << SPRACHE(" Gesamtaufwand  "," total effort   ")
             << SW7 << 100*S.aeinzel
             << SPRACHE(" Lageaufwand        links rechts\n",
                        " positional effort    left right\n");
   std::cout << str[0] << " "
             << SP3 << SW7 << S.hk[kategorie_t::Kollision]
             << SPRACHE(" Kollisionen    ", " same finger rp ")
             << SW7 << S.hs[kategorie_t::Kollision]
             << SPRACHE(" Shift-Kollisionen  ob ", " shift same finger top ")
             << SP1 << SW4 << S.hpos[0][zeilen_t::Obere_Zeile] << " "
             << SW4 << S.hpos[1][zeilen_t::Obere_Zeile] << "\n";
   std::cout << str[1] << " "
             << SP3 << SW7 << S.hk[kategorie_t::Handwechsel]
             << SPRACHE(" Handwechsel    ", " hand alternat. ")
             << SW7 << S.hs[kategorie_t::Handwechsel]
             << SPRACHE(" Shift-Handwechsel  mi ", " shift hand alter. mid ")
             << SP1 << SW4 << S.hpos[0][zeilen_t::Mittelzeile] << " "
             << SW4 << S.hpos[1][zeilen_t::Mittelzeile] << "\n";
   std::cout << str[2] << " "
             << SP3 << SW7
             << S.hk[kategorie_t::Einwaerts]/S.hk[kategorie_t::Auswaerts]
             << SPRACHE(" Ein-/Ausw" strAe "rts  ", " inward/outward ")
             << SP3 << SW7 << (S.hk[kategorie_t::Einwaerts]+
                               S.hk[kategorie_t::Auswaerts])
             << SPRACHE(" Ein- oder ausw" strAe "rts un ",
                        " inward or outward bot ")
             << SP1 << SW4 << S.hpos[0][zeilen_t::Untere_Zeile] << " "
             << SW4 << S.hpos[1][zeilen_t::Untere_Zeile] << "\n";
   std::cout << str[3] << " " << SP3  << SW7 << S.hnachbar
             << SPRACHE(" benachbart     ", " adjacent       ")
             // Die Normierung von hnachbar ist fragwürdig.
             << SW7 << S.hsnachbar
             << SPRACHE(" Shift-benachbart  sum", " shift adjacent    sum")
             << SP1 << SW5 << S.hlinks << SW5 << S.hrechts << "\n";
   if(h.mit_trigrammen()){
      std::cout << str[4] << " " << SP3  << SW7 << S.hkeinhw
                << SPRACHE(" kein Handwechs.", " no hand altern.")
                << SP3 << SW7 << S.hdoppelhw
                << SPRACHE(" zwei Handwechsel", " two hand altern.")
                << "\n " << lspalte
                << SP3  << SW7 << S.hwippe
                << SPRACHE(" Wippe          ", " seesaw         ")
                << SP3 << SW7 << S.hi2[kategorie_t::Kollision]
                << SPRACHE(" IndirKollision", " indir same finger")
                << "\n";
      str[4] = lspalte;
   }

   for(const auto& i : S.hk_benutzer){
      std::cout << str[4] << " " << SP3 << SW7 << i.second << " "
                << tastatur.benutzerkategorie_name(i.first) << "\n";
      str[4] = lspalte;
   }
   for(const auto& i : S.hs_benutzer){
      std::cout << str[4] << " " << SP3 << SW7 << i.second << " Shift-"
                << tastatur.benutzerkategorie_name(i.first) << "\n";
      str[4] = lspalte;
   }
   for(const auto& i : S.ht_benutzer){
      std::cout << str[4] << " " << SP3 << SW7 << i.second << " "
                << tastatur.benutzerkategorie_name(i.first) << "\n";
      str[4] = lspalte;
   }

   std::cout << str[4];
   for(int i = 0; i < nfinger; ++i){
      if(S.mitfinger[i])
         std::cout << " " << SW4 << SP1 << S.hfinger[i];
      else
         std::cout << " --.-";
   }
   std::cout << " Sh" << SW5 << S.hslinks << SW5 << S.hsrechts << "\n";

   if(ngrammakkumlimit[0] >= 0){
      std::cout << SPRACHE("  Kollision/Fi. ", "  same fing/fi. ");
      for(int i = 0; i < nfinger; ++i){
         if(S.mitfinger[i])
            std::cout << " " << SP2 << SW4 << S.hkollision1[i];
         else
            std::cout << "     ";
      }
      std::cout << " Sh" << SP2;
      for(int s = 0; s < nshift; ++s) std::cout << SW5 << S.hskollision1[s];

      std::cout << SPRACHE("\n  \" \" Sprung>=2 ", "\n  \" \" jump >= 2 ");
      for(int i = 0; i < nfinger; ++i){
         if(S.mitfinger[i])
            std::cout << SP2 << SW5 << S.hkollision2[i];
         else
            std::cout << "     ";
      }
      std::cout << " Sh" << SP2;
      for(int s = 0; s < nshift; ++s) std::cout << SW5 << S.hskollision2[s];

      std::cout << SPRACHE("\n  benachbart/F.paar", "\n  adjacent/fin.pair");
      for(int i = 0; i < nfinger-1; ++i){
         if(S.mitfinger[i] && S.mitfinger[i+1])
            std::cout << SP2 << SW5 << S.hnachbar1[i];
         else
            std::cout << "     ";
      }
      std::cout << "   Sh" << SP2;
      for(int s = 0; s < nshift; ++s) std::cout << SW5 << S.hsnachbar1[s];

      std::cout << SPRACHE("\n  \" \" Ze.sprung>=2 ",
                           "\n  \" \" row jump >=2 ");
      for(int i = 0; i < nfinger-1; ++i){
         if(S.mitfinger[i] && S.mitfinger[i+1])
            std::cout << SP2 << SW5 << S.hnachbar2[i];
         else
            std::cout << "     ";
      }
      std::cout << "   Sh" << SP2;
      for(int s = 0; s < nshift; ++s) std::cout << SW5 << S.hsnachbar2[s];
      std::cout << "\n";
   }

   const haeufigkeit_t htot[3] = { S.h2tot, S.hs2tot, S.h3tot };
   for(int t = 0; t < 3; ++t){
      if(ngrammakkumlimit[t] <= 0) continue;
      for(int s = 0; s < 2; ++s){
         const auto& hwb = S.ngramm[t][s];
         if(hwb.size() == 0) continue;
         double sum = 0;
         std::cout << SPRACHE("\n  % alle  % ", "\n  % all   % ")
                   << (t < 2
                       ? (s ? SPRACHE("rechts","right ")
                            : SPRACHE("links "," left "))
                       : (s ? SPRACHE("2 H.w.","2 h.a.")
                            : SPRACHE("0 H.w.","0 h.a.")))
                   << SPRACHE(" summiert\n", " cumulative\n");
         for(auto i = hwb.crbegin();
             i != hwb.crend() && sum < S.hrel[t][s]*ngrammakkumlimit[t]; ++i){
            const double aa = 100*i->first;
            sum += i->first;
            std::cout << SP3 << SW7 << aa/htot[t] << " "
                      << SP3 << SW7 << aa/S.hrel[t][s]
                      << " ("<< SP3 << SW7 << 100*sum/S.hrel[t][s]
                      <<"): " << i->second << "\n";
         }
      }
   }

   if(handeinsatzlimit >= 0 && wortliste.size())
      handeinsaetze(b, tastatur, kodierung, wortliste, handeinsatzlimit);
   std::cout << std::endl;

   if(als_fixeszeichen){
      const std::string einfach = "'", doppelt = "\"";

      for(int i = 0; i < tastatur.nvariabel(); ++i){
         const std::string z0 = kodierung.txt(b[i], 0);
         const std::string z1 = kodierung.txt(b[i], 1);
         const std::string anf = z0 != einfach && z1 != einfach
            ? einfach
            : (z0 != doppelt && z1 != doppelt ? doppelt : "!");
         std::cout << "FixesZeichen " << utf32_in_ausgabe(tastatur.name(i))
                   << " " << anf << z0 << z1 << anf << std::endl;
      }
      std::cout << std::endl;
   }
}

void schreibe_belegung(const belegung_t b, double A, const Tastatur& tastatur,
                       const Kodierung& kodierung, const std::u32string* name)
{
   constexpr int zbreite = 9;
   char bewertung[zbreite+ntaste*4+2];
   sprintf(bewertung, "%#8.7g", 100*A);
   char* s = bewertung+(zbreite-1);
   *s++ = ' ';
   for(int i = 0; i < ntaste; ++i){
      const int seq = tastatur.taste(i);
      const std::string& z = kodierung.bevorzugt(b[seq]);
      for(const char* j = z.c_str(); *j;) *s++ = *j++;
   }
   if(name){
      *s = 0;
      std::cout << bewertung;
      std::cout << "   " << utf32_in_ausgabe(*name) << std::endl;
   }else{
      *s++ = '\n'; *s = 0;
      std::cout << bewertung;
   }
}

void
schreibe_zyklen(const belegung_t b1, const belegung_t b2,
                const Kodierung& kodierung)
{
   belegung_t zyklus;

   for(int i = 0; i < ntaste; ++i){
      const unsigned char z1 = b1[i];
      zyklus[z1] = b2[i];
   };

   for(int i = 0; i < ntaste; ++i){
      const unsigned char z1 = zyklus[i];
      if(z1 == ntaste) continue; // Wurde schon abgehandelt.
      if(z1 == i) continue; // triviale Zyklen ignorieren.
      std::cout << "  ";
      unsigned char z = z1;
      do{
         const std::string& txt = kodierung.bevorzugt(z);
         std::cout << (txt == strNBS ? "_" : txt.c_str());
         const unsigned char z2 = zyklus[z];
         zyklus[z] = ntaste;
         z = z2;
      }while(z != z1);
   }
}

//--------------- src/vollkorpus.cc ---------------
//#include "vollkorpus.hh"

//#include "trennen.hh"
//#include "utfhilfe.hh"
#include <fstream>

void lies_vollkorpus(const std::string& name, const std::string* trennmuster,
                     std::unordered_map<std::u32string, zaehl_t>& wl,
                     std::unordered_map<uint64_t, zaehl_t> uh[3])
{
   bool utf8 = true, geaendert = false;
   do{
      for(size_t i = 0; i < 3; ++i) uh[i].clear();
      wl.clear();

      std::unique_ptr<Naechstes_zeichen> nz
         (trennmuster ? new hole_mit_trennung(name, *trennmuster, utf8)
                      : new Naechstes_zeichen(name, utf8));

      std::u32string w;  w.reserve(20);
      uint64_t z1 = 0, z2 = 0;
      while(const char32_t c = nz->get()){
         if(c < U' ' || c == U'\u00ad'){
            if(w.length()) ++wl[w];
            w.clear();
            z1 = z2 = 0;
            continue;
         }else if(c == U' '){
            if(w.length()) ++wl[w];
            w.clear();
         }else w += c;

         const uint64_t z3 = utf_maske & c;
         ++uh[0][z3];
         if(z2){
            z2 = (z2<<21) | z3;
            ++uh[1][z2];
            if(z1) ++uh[2][(z1<<21) | z3];
            z1 = z2;
         }
         z2 = z3;
      }
      if(w.length()) ++wl[w];

      geaendert = nz->encoding_geaendert();
      utf8 = false;
   }while(geaendert);
}

//--------------- src/wortliste.cc ---------------
//#include "wortliste.hh"

//#include "Eingabestream.hh"
//#include "Haeufigkeit.hh"
//#include "Kodierung.hh"
//#include "utfhilfe.hh"
#include <iostream>

namespace {

void wort_in_wortliste(
   const std::string wort, unsigned h,
   std::unordered_map<std::string, haeufigkeit_t>& wortliste)
{
   if(!wort.size()) return;
   const auto i = wortliste.find(wort);
   if(i == wortliste.end())
      wortliste[wort] = h;
   else i->second += h;
}

}

void lies_wortliste(const std::string& name,
                    std::unordered_map<std::u32string, zaehl_t>& wl,
                    bool muss_existieren){
   bool utf8 = true, geaendert = false;
   do{
      wl.clear();
      Eingabestream f(name, utf8, muss_existieren);
      while(f.echte_neuezeile()){
         const zaehl_t h = hole_zahl(f, 0, 1e15);
         const bool zwischen = f.ist_zwischenraum();
         const size_t rest = f.restzeichen();
         if(!(zwischen && rest > 1)){
            std::cerr << SPRACHE("Nach der Zahl werden ein Leerzeichen und "
                                 "mindestens ein weiteres Zeichen erwartet.",
                                 "After the number, one space character and at "
                                 "least one more character are expected.")
                      << std::endl;
            f.fehler();
         }

         f.uebergehen();
         std::u32string idx; idx.reserve(rest-1);
         for(size_t i = 1; i < rest; ++i)
            idx.push_back(f.lies_in_zeile());
         wl[idx] += h;
      }
      geaendert = f.encoding_geaendert();
      utf8 = false;
   }while(geaendert);
}

void lies_wortliste(const std::string name,
                    std::unordered_map<std::string, haeufigkeit_t>& wortliste,
                    const Kodierung& kodierung)
{
   std::unordered_map<std::u32string, zaehl_t> wl;
   lies_wortliste(name, wl, true);
   for(const auto& i : wl){
      const haeufigkeit_t h = i.second;
      std::string konvertiert;
      for(const char32_t z : i.first){
         const bool zwischenraum = ist_zwischenraum(z);
         const auto c = kodierung.position(z);
         if(c.first < 0 || zwischenraum){
            wort_in_wortliste(konvertiert, h, wortliste);
            konvertiert.erase();
         }else konvertiert
                  .push_back(Haeufigkeit::index_ein_flach(c.first, c.second));
      }
      wort_in_wortliste(konvertiert, h, wortliste);
   }
}

//--------------- src/Aufwandstabelle.cc ---------------
//#include "Aufwandstabelle.hh"

//#include "Kodierung.hh"
//#include "Konfiguration.hh"
//#include "Tastatur.hh"
//#include "utfhilfe.hh"
#include <iomanip>
#include <iostream>
#include <string>

Aufwandstabelle::
Aufwandstabelle(bool mittrigrammen, const Tastatur& tast,
                const Konfiguration& A)
   : aunbekannt(A.unbekannt()), mit_trigrammen(mittrigrammen), tastatur(tast)
{
   // werden Trigramme mit berücksichtigt, steigt der Gesamtaufwand.  Daher
   // sollte das Gewicht für die Fingerbelastung auch entsprechend grösser
   // gewählt werden:
   const double mult_mf_tri = 1+A.indirekt(1.);
   double fh_tot = 0;
   // Wenn einem Finger nur fest belegte Tasten zugeordnet sind legen wir für
   // ihn keine Zielhäufigkeit fest.
   for(int i = 0; i < nfinger; ++i){
      if(!tastatur.finger_fix(i)){
         fh_tot += A.zielhaeufigkeit(i);
      }
   }
   for(int i = 0; i < nfinger; ++i){
      finger_zielhaeufigkeit[i] = A.zielhaeufigkeit(i)/fh_tot;
      multfinger[i] = mit_trigrammen
         ? mult_mf_tri*A.multfinger(i) : A.multfinger(i);
      if(tastatur.finger_fix(i)){
         finger_zielhaeufigkeit[i] = multfinger[i] = 0;
      }
   }

   for(int i = 0; i < ntaste; ++i){
      // Basisaufwand für Kleinbuchstaben:
      a1[i][0] = tastatur.lageaufwand(i);
      // Für Grossbuchstaben muss zusätzlich noch Shift gedrückt werden:
      const int shift_i = tastatur.shifttaste(i);
      a1[i][1] = a1[i][0]+tastatur.lageaufwand(tastatur.shifttaste(i))
         +A.bigrammaufwand(shift_i, i, tastatur);
   }

   // Wir bewerten n-Gramme bis zu drei Tasten, dabei wird Shift als Taste
   // mitgezählt.  Deshalb fallen Tasten-n-Gramme nicht mit Zeichen-n-Grammen
   // zusammen, und die Zählung wird verwickelt.  Shift ist zudem speziell: Zum
   // einen, weil die Shifttaste durch die zu shiftende Taste festgelegt ist,
   // zum anderen, weil Shift vor der zu shiftenden Taste angeschlagen und
   // gehalten wird, bis die zu shiftende Taste angeschlagen wurde.
   //
   // Schauen wir uns die Eingabe von drei Zeichen an.  Für jedes Zeichen muss
   // eine Zeichentaste (b) und gegebenenfalls Shift (s) angeschlagen werden.
   // Es gibt also acht Möglichkeiten, die man in die darin enthaltenen
   // Trigramme aufspalten kann:
   //
   // T1:   b   b   b  = bbb
   // T2: s-b   b   b  = sbb + bbb
   // T3:   b s-b   b  = bsb + sbb
   // T4: s-b s-b   b  = sbs + bsb + sbb
   // T5:   b   b s-b  = bbs + bsb
   // T6: s-b   b s-b  = sbb + bbs + bsb
   // T7:   b s-b s-b  = bsb + sbs + bsb
   // T8: s-b s-b s-b  = sbs + bsb + sbs + bsb
   //
   // Nur für bbb (in T1 und T2) und bbs (in T5 und T6) müssen wir tatsächlich
   // die Häufigkeiten von Zeichentrigrammen betrachten.  Für sbb, bsb und sbs
   // genügt die Betrachtung von Zeichenbigrammen.  Wir handeln sie daher mit
   // den Bigrammen ab.  Für Zeichenbigramme gibt es vier Möglichkeiten, und
   // wir können sie in die enthaltenen Tastenbi- und trigramme aufteilen:
   //
   // B1:   b   b = bb
   // B2: s-b   b = sbb + bb
   // B3:   b s-b = bsb + bs
   // B4: s-b s-b = sbs + bsb + bs
   //
   // wobei die Bigramme sb bereits weggelassen sind, denn das sind
   // verkappte 1-Gramme (Grossbuchstaben).
   //
   // Wenn wir die Zeichentrigrammhäufigkeiten für Trigramme der Form bbb
   // und bbs genauer betrachten sehen wir:
   // - Es spielt keine Rolle, ob der erste Buchstabe gross oder klein
   //   ist, denn ein allfälliges Shift liegt vor (also ausserhalb) des
   //   Trigramms.  Wir können die entsprechenden Häufigkeiten summieren.
   // - Der zweite Buchstabe ist immer klein.
   // - Für bbb ist der dritte Buchstabe klein, für bbs gross.
   //
   // Die Zeichenbigrammhäufigkeiten haben wir nach den vier
   // Klein/Gross-Kombinationen aufgespalten.
   //
   // Für die Bewertung ist noch zu bemerken, dass sbb und sbs nicht mit bbb,
   // bbs und bsb gleichzusetzen sind, weil das s am Anfang gehalten werden
   // muss, bis das folgende b getippt ist.  Wir nennen nennen Tastentrigramme
   // der Art sbb und sbs «Shift-Bigramme».

   // Fülle die Bigrammtabellen.
   for(int i = 0; i < ntaste; ++i){
      const int shift_i = tastatur.shifttaste(i);
      for(int j = 0; j < ntaste; ++j){
         // Basisbewertung: Bigramme zwischen normalen Tasten
         a2[i][j][0][0] = A.bigrammaufwand(i, j, tastatur);// bb

         // Aufwand, falls das erste Zeichen im Bigramm zusammen mit Shift
         // gedrückt wird.
         const double sa1 = A.bigrammaufwand(shift_i, j, tastatur);
         const double sf1 = A.shiftindirekt(sa1);
         a2[i][j][1][0]  = sf1*sa1                         // sbb
            +A.bigrammaufwand(i, j, tastatur);             // bb

         // Aufwand, falls das zweite Zeichen im Bigramm zusammen mit Shift
         // gedrückt wird.
#ifndef OHNE2SHIFT
         const int shift_j = tastatur.shifttaste(j);
         a2[i][j][0][1] =
            A.trigrammaufwand(i, shift_j, j, tastatur)    // bsb
            +A.bigrammaufwand(i, shift_j, tastatur);      // bs

         // Aufwand wenn beide Zeichen mit Shift gedrückt werden.
         const double sa12 = A.bigrammaufwand(shift_i, shift_j, tastatur);
         const double sf12 = A.shiftindirekt(sa12);
         a2[i][j][1][1] = sa12*sf12                       // sbs
            +A.trigrammaufwand(i, shift_j, j, tastatur)   // bsb
            +A.bigrammaufwand(i, shift_j, tastatur);      // bs
#endif
      }
   }

   bool mit_verwechslungspotenzial = false;
   mit_vorlieben = mit_aehnlichkeit = false;
   for(int i = 0; i < ntaste; ++i){
      for(int j = 0; j < ntaste; ++j){
         va2[i][j] = A.verwechslungspotenzial(i, j, tastatur);
         if(va2[i][j] != 0){
            mit_verwechslungspotenzial = true;
         }
         ae[i][j] = A.aehnlichkeit(i, j);
         if(ae[i][j] != 0){
            mit_aehnlichkeit = true;
         }
         vl[i][j] = A.vorliebe(i, j);
         if(vl[i][j] != 0){
            mit_vorlieben = true;
         }
      }
   }
   mit_aehnlichkeit &= mit_verwechslungspotenzial;
   vl_knick = -A.vorliebenknick();

   if(!mit_trigrammen){
      return;
   }
   // Fülle die Trigrammtabellen.
   for(int k = 0; k < ntaste; ++k){
      const int shift_k = tastatur.shifttaste(k);
      for(int i = 0; i < ntaste; ++i){
         for(int j = 0; j < ntaste; ++j){
            a3[i][j][k][0] =
               A.trigrammaufwand(i,j,k,tastatur);                    // bbb
            a3[i][j][k][1] =
               A.trigrammaufwand(i,j,shift_k,tastatur);              // bbs
         }
      }
   }
}

void
Aufwandstabelle::
anzeigen(const Kodierung& kodierung, const Konfiguration& A) const
{
   // Keine Übersetzung ins Englische, die Wörter hier sind Schlüsselwörter.
   const std::string einfachesanfuehrungszeichen = "'";
   for(int i = 0; i < ntaste+nshift; ++i){
      const std::string ni = utf32_in_ausgabe(tastatur.name(i));
      for(int j = 0; j < ntaste+nshift; ++j){
         const std::string nj = utf32_in_ausgabe(tastatur.name(j));
         const double b = A.bigrammaufwand(i, j, tastatur);
         if(b != 0.){
            std::cout << "Bigramm " << ni << " " << nj << " "
                      << std::setprecision(16) << b << "\n";
         }

         if(mit_trigrammen){
            for(int k = 0; k < ntaste+nshift; ++k){
               const double t = A.trigrammaufwand(i, j, k, tastatur);
               if(t != 0.){
                  std::cout << "Trigramm " << ni << " " << nj << " "
                            << utf32_in_ausgabe(tastatur.name(k)) << " "
                            << std::setprecision(16) << t << "\n";
               }
            }
         }

         if(i < ntaste && j < ntaste){
            const double v = verwechslungspotenzial(i, j);
            if(v != 0.){
               std::cout << "Verwechslungspotenzial " << ni << " " << nj << " "
                         << std::setprecision(16) << v << "\n";
            }
            if(mit_vorlieben && vorliebe(i, j) != 0.){
               const std::string& z = kodierung.bevorzugt(i);
               const std::string a = z == einfachesanfuehrungszeichen
                  ? "\"" : einfachesanfuehrungszeichen;
               std::cout << "Vorliebe " << a << z << a << " "
                         << -vorliebe(i, j) << " " << nj << "\n";
            }
         }
      }
   }
#ifdef EXPERIMENTELL
   if(mit_vorlieben && A.vorliebenknick() > -1e15){
      std::cout << "VorliebeKnick " << A.vorliebenknick() << "\n";
   }
#endif // EXPERIMENTELL
}

//--------------- src/ngramme.cc ---------------
//#include "ngramme.hh"

//#include "Eingabestream.hh"
//#include "typen.hh"
//#include "utfhilfe.hh"
//#include "vollkorpus.hh"
//#include "wortliste.hh"
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <map>
#include <unordered_map>

namespace {

void lies_ngramme(const std::string& name, size_t N,
                  std::unordered_map<uint64_t, zaehl_t>& uh){
   bool utf8 = true, geaendert = false;
   do{
      uh.clear();
      Eingabestream tabelle(name, utf8, true);
      while(tabelle.echte_neuezeile()){
         const zaehl_t h = hole_zahl(tabelle, 0, 1e15);
         pruefe_leer_dann_N(tabelle, N);
         tabelle.uebergehen();
         uint64_t k = 0;
         for(size_t i = 0; i < N; ++i){
            k = (k<<21) | (tabelle.lies_in_zeile() & utf_maske);
         }
         uh[k] += h;
      }
      geaendert = tabelle.encoding_geaendert();
      utf8 = false;
   }while(geaendert);
}

template <typename S>
void ngramme_ausgeben(const std::unordered_map<S, zaehl_t>& uh,
                      const std::string& name)
{
   if(uh.size() == 0) return;
   std::ofstream o(name.c_str(), std::ios_base::out);
   std::multimap<zaehl_t, std::string> g;
   for(const auto& i : uh){
      if(i.second){
         g.insert(std::make_pair(i.second, utf32_in_utf8(i.first)));
      }
   }
   for(auto i = g.crbegin(); i != g.crend(); ++i){
      o << std::setprecision(16) << i->first << " " << i->second << "\n";
   }
}

}

void erzeuge_ngrammtabellen(const std::vector<std::string>& namen)
{
   std::unordered_map<std::u32string, zaehl_t> wl;
   std::unordered_map<uint64_t, zaehl_t> uh[3];
   const std::string ext[] = { ".1", ".2", ".3", ".wl" };

   if(namen.size() < 3){
      if(namen.size() == 1){
         lies_vollkorpus(namen[0], nullptr, wl, uh);
      }else{
         lies_vollkorpus(namen[1], &namen[0], wl, uh);
      }
   }else{
      for(size_t i = 0; i+1 < namen.size(); ++i){
         for(int j = 0; j < 3; ++j){
            std::unordered_map<uint64_t, zaehl_t> uhj;
            lies_ngramme(namen[i]+ext[j], j+1, uhj);
            if(uh[j].size()){
               for(const auto& k : uhj){
                  uh[j][k.first] += k.second;
               }
            }else{
               uh[j].swap(uhj);
            }
         }
         std::unordered_map<std::u32string, zaehl_t> wli;
         lies_wortliste(namen[i]+ext[3], wli, false);
         if(wl.size()){
            for(const auto& k : wli){
               wl[k.first] += k.second;
            }
         }else{
            wl.swap(wli);
         }
      }
   }

   const std::string base = namen.back();
   for(int j = 0; j < 3; ++j){
      ngramme_ausgeben(uh[j], base+ext[j]);
   }
   ngramme_ausgeben(wl, base+ext[3]);
}

//--------------- src/html_markup.cc ---------------
//#include "html_markup.hh"

//#include "Eingabestream.hh"
//#include "Kodierung.hh"
//#include "Naechstes_zeichen.hh"
//#include "Tastatur.hh"
//#include "string_in_belegung.hh"
//#include "utfhilfe.hh"
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

namespace {

void add_bigramm_markup(int f, int vf, bool shift,
                        std::string& stil)
{
   char c = 0;
   if(vf == f){
      c = 'K';
   }else if(std::abs(vf-f) == 1){
      c = 'N';
   }else if(vf*f > 0){
      c = (std::abs(f) > std::abs(vf)) ? 'A' : 'E';
   }
   if(c){
      if(stil.length()) stil.push_back(' ');
      stil.push_back(shift ? ('a'-'A')+c : c);
   }
}

class Hole_mit_ersetzen {
   Naechstes_zeichen nz;
   const Kodierung& kodierung;
   const std::vector<std::pair<int, int>>* ersatz = nullptr;
   size_t ersatz_i = 0;
public:
   Hole_mit_ersetzen(const std::string& name, const Kodierung& kodierung,
                     bool utf8)
      : nz(name, utf8), kodierung(kodierung){}

   std::pair<char32_t, std::pair<int,int>> get() {
      if(ersatz && ersatz_i < ersatz->size()){
         const auto& ie = (*ersatz)[ersatz_i++];
         return std::make_pair(kodierung.uchar(ie.first, ie.second), ie);
      }
      const char32_t z = nz.get();
      const auto ie = kodierung.position(z);
      if(ie.first < 0){
         ersatz = kodierung.ersatz(z);
         ersatz_i = 0;
         if(ersatz) return get();
      }
      return std::make_pair(z, ie);
   }

   bool encoding_geaendert() const { return nz.encoding_geaendert(); }
};

}

void html_markup(const std::string& textfile, const Kodierung& kodierung,
                 const Tastatur& tastatur, const std::string& referenztastatur)
{
   const std::map<char32_t, std::string> escape =
      {{ U'<', "&lt;" },{ U'>', "&gt;" },{ U'&', "&amp;" },{ U'\u00A0', " " } };

   bool utf8 = true, nochmal;
   do{
      nochmal = false;
      Eingabestream liste(referenztastatur, true);
      std::ofstream html(textfile+".html");
      html << "<!DOCTYPE html><html><head>\n"
         "<meta http-equiv=\"Content-Type\" content=\"text/html; "
         "charset=utf-8\" />\n"
         "<style type=\"text/css\">\n"
         ".K{background-color: #FCC;}\n"
         ".k{text-decoration: underline; text-decoration-color: #F00;}\n"
         ".N{background-color: #FFA;}\n"
         ".n{text-decoration: underline; text-decoration-color: #A80;}\n"
         ".A{background-color: #DDF;}\n"
         ".a{text-decoration: underline; text-decoration-color: #00F;}\n"
         ".E{background-color: #DFD;}\n"
         ".e{text-decoration: underline; text-decoration-color: #0F0;}\n"
         ".L{color: #008;}\n"
         ".R{color: #080;}\n"
         ".G{font-weight: 600;}\n"
         ".B{font-family: monospace; background-color: #FFF; color: #000;}\n"
         "</style>\n"
         "</head><body>\n"
         SPRACHE("<p class=\"B\">"
                 "<a class=\"K\">Kollision</a>, "
                 "<a class=\"k\">Shift-Kollision</a>, "
                 "<a class=\"N\">Nachbaranschlag</a>, "
                 "<a class=\"n\">Shift-Nachbaranschlag</a>, "
                 "<a class=\"A\">Auswärts</a>, "
                 "<a class=\"a\">Shift-Auswärts</a>, "
                 "<a class=\"E\">Einwärts</a>, "
                 "<a class=\"e\">Shift-Einwärts</a>.\n"
                 ,
                 "<p class=\"B\">"
                 "<a class=\"K\">Same finger repetition</a>, "
                 "<a class=\"k\">Shift-same finger repetition</a>, "
                 "<a class=\"N\">adjacent finger stroke</a>, "
                 "<a class=\"n\">Shift-adjacent finger stroke</a>, "
                 "<a class=\"A\">outwards</a>, "
                 "<a class=\"a\">Shift-outwards</a>, "
                 "<a class=\"E\">inwards</a>, "
                 "<a class=\"e\">Shift-inwards</a>.\n"
            ) "</p>";
      while(liste.echte_neuezeile()){
         std::u32string bs, uname;
         if(!liste.hole_wort(bs) || !liste.hole_wort(uname)){
            std::cerr << SPRACHE("Fehlerhaft formatiertes Belegungsfile ",
                                 "Incorrectly formatted layout file ")
                      << referenztastatur << std::endl;
            liste.fehler();
         }

         if(!bs.size()) continue;
         bool fest[ntaste]; // Dieses Resultat wird nicht verwendet.
         belegung_t b, ib;
         string_in_belegung(bs, b, fest, tastatur, kodierung);
         for(int i = 0; i < ntaste; ++i) ib[b[i]] = i;

         html << "<h1>" << utf32_in_utf8(uname) << "</h1>\n<p class=\"B\">";
         Hole_mit_ersetzen nz(textfile, kodierung, utf8);
         int vorige_pos = -1, vorige_ebene = -1;
         std::string voriger_stil;
         do{
            const auto zie = nz.get();
            const auto zeichen = zie.first;
            nochmal = nz.encoding_geaendert();
            if(!zeichen || nochmal) break;
            const auto i = zie.second.first, e = zie.second.second;
            std::string stil;
            if(i >= 0 && e >= 0){
               const auto pos = ib[i];
               const auto shift = tastatur.shifttaste(pos);
               const auto f = tastatur.finger(pos), sf = tastatur.finger(shift);
               if(vorige_pos >= 0){
                  const auto vshift = tastatur.shifttaste(vorige_pos);
                  const auto vf = tastatur.finger(vorige_pos);
                  const auto vsf = tastatur.finger(vshift);
                  if(vorige_pos != pos) add_bigramm_markup(f, vf, false, stil);
                  if(vorige_ebene > 0){
                     // Shift-Buchstabentaste oder Shift-Shift?
                     const auto nt = e ? shift : pos;
                     if(vshift != nt)
                        add_bigramm_markup(e ? sf : f, vsf, true, stil);
                  }
               }
               vorige_pos = pos;  vorige_ebene = e;
            }else{
               vorige_pos = vorige_ebene = -1;
            }
            if(stil != voriger_stil){
               if(voriger_stil.size()) html << "</a>";
               if(stil.size()) html << "<a class=\"" << stil << "\">";
               voriger_stil.swap(stil);
            }
            const auto iesc = escape.find(zeichen);
            if(iesc == escape.end()){
               html << utf32_in_utf8(zeichen);
            }else{
               html << iesc->second;
            }
         }while(true);
         if(voriger_stil.size()) html << "</a>";
         html << "</p>\n";
      }
      html << "</body></html>\n" << std::endl;
      utf8 = false;
   }while(nochmal);
}

//--------------- src/trennen.cc ---------------
//#include "trennen.hh"

//#include "Eingabestream.hh"
//#include "Unicode.hh"
//#include "utfhilfe.hh"
#include <cassert>
#include <memory>

namespace {

size_t
lies_trennmuster(const std::string& name,
                 std::unordered_map<std::u32string, std::vector<char>>& tabelle)
{
   size_t maxlen;
   bool utf8ein = true, nochmal = false;
   do{
      tabelle.clear();
      maxlen = 0;
      Eingabestream liste(name, utf8ein);
      while(liste.echte_neuezeile()){
         std::u32string muster;
         std::vector<char> trennstellen;
         bool warziffer = false;
         while(const char32_t zi = liste.lies_in_zeile()){
            if(ist_ziffer(zi)){
               trennstellen.push_back(zi-U'0');
               warziffer = true;
            }else{
               if(!warziffer) trennstellen.push_back(0);
               muster.append(1, zi);
               warziffer = false;
            }
         }
         if(!warziffer) trennstellen.push_back(0);
         assert(trennstellen.size() == muster.size()+1);
         tabelle[muster] = trennstellen;
         if(muster.size() > maxlen) maxlen = muster.size();
      }
      nochmal = liste.encoding_geaendert();
      utf8ein = false;
   }while(nochmal);
   return maxlen;
}

void
trenne(const std::u32string& wort1,
       const std::unordered_map<std::u32string, std::vector<char>>& tabelle,
       size_t maxlen, std::vector<bool>& resultat)
{
   static const size_t min_silbenlaenge = 2;

   // Die Punkte in der Trennmustertabelle markieren Wortanfang und Wortende.
   std::u32string wort(wort1.size()+2, U'.');
   for(size_t i = 0; i < wort1.size(); ++i)
      wort[i+1] = Unicode::get().kleinbuchstabe(wort1[i]);
   std::vector<char> trennstellen(wort.length()+1, 0);
   for(size_t i = 0; i < wort.size()-min_silbenlaenge; ++i){
      const size_t minlaenge = i ? min_silbenlaenge : min_silbenlaenge+1;
      const size_t len = wort.size()-i;
      const size_t maxlaenge = len > maxlen ? maxlen : len;
      // Effizienz spielt keine Rolle...
      std::u32string muster(wort, i, minlaenge-1);
      muster.reserve(maxlaenge);
      for(size_t j = minlaenge; j <= maxlaenge; ++j){
         muster.append(1, wort[i+j-1]);
         const auto p = tabelle.find(muster);
         if(p != tabelle.end()){
            const std::vector<char>& t = p->second;
            for(size_t k = 0; k < t.size(); ++k){
               if(t[k] > trennstellen[i+k]) trennstellen[i+k] = t[k];
            }
         }
      }
   }

   if(resultat.size() < wort1.size()) resultat.resize(wort1.size());
   for(size_t i = 2; i < trennstellen.size()-2; ++i){
      const bool innen = (i > min_silbenlaenge) &&
         (i < trennstellen.size()-1-min_silbenlaenge);
      resultat[i-2] = innen && (trennstellen[i] & 1);
   }
   resultat[wort1.size()-1] = true;
}

}


char32_t
hole_mit_trennung::fuelle_buffer(){
   wort_i = 0;
   wort.erase();
   char32_t c = Naechstes_zeichen::get();
   while(Unicode::get().ist_buchstabe(c)){
      wort.append(1, c);
      c = Naechstes_zeichen::get();
   }
   if(wort.size()){
      const auto i = cache.find(wort);
      if(i != cache.end())
         trennstellen = i->second;
      else{
         trenne(wort, trennmuster, maxlen, trennstellen);
         cache[wort] = trennstellen;
      }
   }
   return c;
}

hole_mit_trennung::
hole_mit_trennung(const std::string& name, const std::string& tmfile,
                  bool utf8)
   : Naechstes_zeichen(name, utf8)
{
   maxlen = lies_trennmuster(tmfile, trennmuster);
   naechste_ist_trennung = false;
   rest = fuelle_buffer();
}


char32_t
hole_mit_trennung::
get(){
   const char32_t trenner = U'\u00ad';
   if(naechste_ist_trennung){
      naechste_ist_trennung = false;
      return trenner;
   }

   if(wort_i < wort.size()){
      naechste_ist_trennung = trennstellen[wort_i];
      const char32_t c = wort[wort_i];
      wort_i++;
      return c;
   }

   if(rest){
      const char32_t alterrest = rest;
      rest = fuelle_buffer();
      naechste_ist_trennung = true;
      return alterrest;
   }
   return 0;
}


void markiere_alle_trennstellen(const std::string& ein, const std::string& aus,
                                const std::string& trennmuster)
{
   bool utf8 = true, nochmal = false;
   do{
      std::unique_ptr<Naechstes_zeichen> nz
         (new hole_mit_trennung(ein, trennmuster, utf8));
      std::ofstream ausgabe(aus.c_str(), std::ios_base::out);
      while(char32_t c3 = nz->get()) ausgabe << utf32_in_utf8(c3);
      nochmal = nz->encoding_geaendert();
      utf8 = false;
   }while(nochmal);
}

//--------------- src/Grafik.cc ---------------
//#include "Grafik.hh"

//#include "Haeufigkeit.hh"
//#include "Kodierung.hh"
//#include "Konfiguration.hh"
//#include "Tastatur.hh"
#include <cmath>

Grafik::
Grafik(const std::string& name, const Tastatur& tast,
       const Kodierung& kod, const Haeufigkeit& h,
       const Konfiguration& konfiguration)
   : grafik(name.c_str(), std::ios_base::out), seite(0),
     ungerade(false), tastatur(tast), kodierung(kod)
{
   constexpr double sk = 1e6;
   const std::string& Beschreibungsfont = konfiguration.beschreibungsfont();
   const std::string& Zeichenfont = konfiguration.zeichenfont();

   // Vorspann
   grafik <<
      "%!PS-Adobe-3.0\n"
      "%%BoundingBox: 0 0 842 595\n"
      "%%DocumentFonts: " << Beschreibungsfont;
   if(Beschreibungsfont != Zeichenfont) grafik << " " << Zeichenfont;
   grafik <<
"\n%%DocumentData: Clean8Bit\n"
"%%EndComments\n"
"%%BeginProlog\n"
"/transparenz true def  % Verwende Transparenz (nach Konversion in PDF)\n"
"/alpha 0.5 def         % Grad der Transparenz (1: intranspanent)\n"
"/mitmini true def      % Zeige Minitastatur unter Buchstaben\n"
"/mitZahlen true def    % Zeige Haeufigkeiten zusaetzlich als Zahlenwerte an\n"
"/mitSummen true def    % Zeige summierte Haeufigkeiten mit an\n"
"/minFuerKurve 0.01 def % Mindestbigrammhaeufigkeit, ab der Kurve gemalt wird\n"
"/maxkurvendicke 0.25 def   % Maximale Dicke der Kurven\n"
"/differenzen false def % Zeige Haeufigkeitsdiffenzen fuer Belegungspaare\n"
"/fhkaestchen 0.25 def % Fingerhaeufigkeit: Haeufigkeit, die Kaestchen fuellt\n"
"/ghkaestchen 0.4 def  % Gruppenhaeufigkeit ''\n"
"/bikaestchen 0.02 def % Bigrammhaeufigkeit ''\n"
"/zhkaestchen 0.2 def  % Zeilenspruenge     ''\n"
"/kollschritt 1.25 sqrt def % Sprungdistanz-Einteilung bei Kollisionen\n"
"/beschrx 0.3 def      % X-Position der Beschreibung\n"
"/beschry -0.7 def     % X-Position der Beschreibung\n"
"/beschrgr 0.3 def     % Groesse der Beschreibung\n"
"%\n"
"/b{def}bind def/B{bind b}bind b"
"/h1a[";

   // Variabler Teil: Häufigkeiten, Zeichen, Tasten
   for(int i = 0; i < tastatur.nvariabel(); ++i)
      grafik << std::floor(0.5+(h(i,0)+h(i,1))*sk)
             << (i+1 < tastatur.nvariabel() ? " " : "]");
   grafik << "B/h1s[";
   for(int i = 0; i < tastatur.nvariabel(); ++i)
      grafik << std::floor(0.5+h(i,1)*sk)
             << (i+1 < tastatur.nvariabel() ? " " : "]");
   grafik << "B/h2a[";
   for(int i = 0; i < tastatur.nvariabel(); ++i){
      grafik << "[";
      for(int j = 0; j < tastatur.nvariabel(); ++j){
         double h2 = 0.;
         for(int ej = 0; ej < nebene2; ++ej)
            h2 += h(i,0,j,ej)+h(i,1,j,ej);
         grafik << std::floor(0.5+h2*sk)
                << (j+1 < tastatur.nvariabel() ? " " : "]");
      }
   }
   grafik << "]B/h2s[";
   for(int i = 0; i < tastatur.nvariabel(); ++i){
      grafik << "[";
      for(int j = 0; j < tastatur.nvariabel(); ++j){
         double h2 = 0.;
         for(int ej = 0; ej < nebene2; ++ej)
            h2 += h(i,1,j,ej);
         grafik << std::floor(0.5+h2*sk)
                << (j+1 < tastatur.nvariabel() ? " " : "]");
      }
   }
   grafik << "]B/klartext{(";
   for(int i = 0; i < tastatur.nvariabel(); ++i)
      grafik << kodierung.psencstr(i);
   grafik << ")}B/zeilen[";
   int maxzeile = 0, minzeile = 99;
   double maxx = -100, minx = 100, maxy = -100, miny = 100;
   double maxx1 = -100, minx1 = 100, maxy1 = -100, miny1 = 100;
   for(int i = 0; i < ntaste+nshift; ++i){
      if(i < ntaste && i >= tastatur.nvariabel()) continue;

      const auto k = tastatur.tastenkoord(i);
      const int zeile = zeilen_t::Leerzeichenzeile-tastatur.zeile(i);

      if(zeile > maxzeile) maxzeile = zeile;
      if(zeile < minzeile) minzeile = zeile;

      if(k.x > maxx) maxx = k.x;
      if(k.x < minx) minx = k.x;
      if(k.y > maxy) maxy = k.y;
      if(k.y < miny) miny = k.y;

      if(i < ntaste){
         if(k.x > maxx1) maxx1 = k.x;
         if(k.x < minx1) minx1 = k.x;
         if(k.y > maxy1) maxy1 = k.y;
         if(k.y < miny1) miny1 = k.y;
      }

      grafik << zeile;
      if(i+1 < ntaste+nshift) grafik << " ";
   }

   // Für die globale Skalierung:
   const double weite = maxx-minx+1.5;
   const double hoehe = zeilen_t::Leerzeichenzeile-miny;
   const double skala = std::min(12.5/weite, std::min(4./hoehe, 1.));
   // Für die Skalierung der Minitastaturen:
   const double miniweite = maxx1-minx1+1;
   const double minihoehe = maxy1-miny1+1;
   const double minixoff  = minx1-minx;
   const double miniyoff  = zeilen_t::Leerzeichenzeile-maxy1;

   grafik << "]B/koordinaten[";
   for(int i = 0; i < ntaste+nshift; ++i){
      if(i < ntaste && i >= tastatur.nvariabel()) continue;
      const auto k = tastatur.tastenkoord(i);
      double x = k.x-minx;
      // Verschiebe Daumentasten der Platzeinteilung zuliebe.
      if(zeilen_t::Leerzeichenzeile == tastatur.zeile(i)){
         if(tastatur.spalte(i) == 5){
            x -= 4;
         }else if(tastatur.spalte(i) == 9){
            x += 4;
         }
      }
      grafik << "[" << x << " " << zeilen_t::Leerzeichenzeile-k.y << "]";
   }
   grafik << "]B/fingertab[";
   for(int i = 0; i < ntaste+nshift; ++i){
      if(i < ntaste && i >= tastatur.nvariabel()) continue;
      grafik << tastatur.finger(i);
      if(i+1 < ntaste+nshift) grafik << " ";
   }
   grafik << "]B/shift[";
   for(int i = 0; i < tastatur.nvariabel(); ++i){
      grafik << tastatur.shifttaste(i)+tastatur.nvariabel()-ntaste;
      if(i+1 < tastatur.nvariabel()) grafik << " ";
   }
   grafik << "]B/minzeile " << minzeile
          << " b/maxzeile " << maxzeile
          << " b/skala " << skala
          << " b/minx " << minx
          << " b/minixoff " << -minixoff
          << " b/miniyoff " << -miniyoff
          << " b/minihoehe " << minihoehe
          << " b/miniweite " << miniweite
          << " b";

   std::string meinencoding = "ISOLatin1Encoding";
   bool umcodieren = false;
   for(int i = 0; i < tastatur.nvariabel(); ++i)
      if(kodierung.psglyphname(i).size()) umcodieren = true;
   if(umcodieren){
      meinencoding = "meinencoding";
      grafik << "/meinencoding[ISOLatin1Encoding aload pop]B\n";
      for(int i = 0; i < tastatur.nvariabel(); ++i){
         if(kodierung.psglyphname(i).size()){
            const int n = kodierung.psencoding(i);
            grafik << meinencoding << " " << n << "/"
                   << kodierung.psglyphname(i) << " put\n";
         }
      }
   }
   grafik <<
      "/schrift{findfont 0.3 scalefont setfont}B"
      "/neuerfont{findfont dup length dict begin{b}forall/Encoding "
          << meinencoding << " b currentdict end definefont pop}B"
      "/BSchrift/" << Beschreibungsfont << " neuerfont\n";
   if(Beschreibungsfont != Zeichenfont)
      grafik <<"/ZSchrift/" << Zeichenfont << " neuerfont\n";

   grafik <<
"/n " << tastatur.nvariabel() << " b/N " << tastatur.nvariabel()+nshift << " b"
"/zz 5 b" // zeilenzahl
"/mf 2 b" // innerster Finger
"/xf 5 b" // äusserster Finger
"/tmp 2 array B"
"/rot{3 2 roll}B"
"/-rot{3 1 roll}B"
"/over{1 index}B"
"/2dup{2 copy}B"
"/nip{exch pop}B"
"/tuck{exch over}B"
"/2drop{pop pop}B"
"/3drop{2drop pop}B"
"/2swap{4 2 roll}B"
"/l{1 sub 0 1 rot}B"
"/L{length l}B"
"/R{grestore}B"
"/S{gsave}B"
"/T{translate}B"
"/@{exch get}B"
"/?{ifelse}B"
"/max{2dup gt{pop}{nip}?}B"
"/min{2dup gt{nip}{pop}?}B"
"/hypot{dup mul exch dup mul add sqrt}B"
"/lf{dup L{over exch 0 put}for pop}B" // lösche Feld
"/lF{dup L{over @ lf}for pop}B"       // lösche Felder
"/arraymax{tuck L{2 index @ max}for nip}B"
"/arraysumme{tuck L{2 index @ add}for nip}B"
"/addinsarray{2 index 2 index get add put}B"
"/addarrays{dup L{2dup get 3 index -rot addinsarray}for 2drop}B"
"/submaxvonarray{2 index 2 index get exch sub 0 max put}B"
"/submaxarrays{dup L{2dup get 3 index -rot submaxvonarray}for 2drop}B"
"/submaxmatrizen{dup L{2dup get 3 index rot get exch submaxarrays}for 2drop}B"
"/switch{@ exec}B"
"/t{transparenz{"
    "/Multiply .setblendmode "
    "alpha .setfillconstantalpha "
    "alpha .setstrokeconstantalpha "
"}if}B"
"/buchstabe{over length over le{2drop(Sh)}{1 getinterval}?}B"
"/zeile{zeilen @}B/finger{fingertab @}B"
"/buchstabentab[256{-1}repeat]B "
// Berechnet buchstabentab[buchstabe] = i
"n l{klartext over get buchstabentab -rot exch put}for"
"/koordinate{koordinaten @ aload pop}B"
"/gruppe{dup finger 0 lt{0}{zz}? exch zeile add}B"
"/klassifiziere{" // taste2 taste1--
    "2dup eq"
    "{2drop 1}"
    "{"
       "finger exch finger "
       "2dup mul 0 lt"
       "{2drop 0}"
       "{"
           "2dup eq"
           "{2drop 6}"
           "{"
               "abs exch abs 2dup lt"
               "{sub abs 1 eq{3}{5}?}"
               "{sub abs 1 eq{2}{4}?}?"
           "}?"
       "}?"
    "}?"
"}B"
"/klassifikationstab["
    "N l{"
       "[exch N l{over klassifiziere exch}for pop]"
    "}for"
"]B"
"/bigrammklasse{" // taste1 taste2--
    "exch klassifikationstab @ @"
"}B"
"/einmax 0 h1a arraymax b"
"/bimax 0 h2a L{h2a @ arraymax}for b"
"/einsumme 0 h1a arraysumme b"
"/bisumme 0 h2a L{h2a @ arraysumme}for h2s L{h2s @ arraysumme}for b"
"/ph1a N array B"
"/ph1A N array B"
"/ph1s n array B"
"/ph2a[N{n array}repeat]B"
"/ph2A[N{n array}repeat]B"
"/ph2s[n{n array}repeat]B"
// Weist TOS gemäss der Belegung permutiertes TOS+1 zu
"/perm1{" // belegung htab1 ptab1--
    "n l{"
       "over exch 4 index over get buchstabentab @ 4 index @ put"
    "}for 3drop"
"}B"
// Weist TOS gemäss der Belegung permutiertes TOS+1 zu
"/perm2{" // belegung htab2 ptab2
    "n l{"
       "3 index over get buchstabentab @ 3 index @ "
       "2 index rot get 4 index -rot perm1"
    "}for 3drop"
"}B"
"/setze{" // belegung 1a 2a--
    "rot 2 index 2 index "
    "2 index h2s ph2s perm2 "
    "2 index h2a rot perm2 "
    "over h1s ph1s perm1 "
    "h1a exch perm1 "
    "n 1 over " << nshift-1 << " add{"
       "2 index over 0 put "
       "over @ lf"
    "}for "
    "h1s L{" // 1a 2a i
       "shift over get 2dup exch "
       "ph1s @ 5 index -rot addinsarray "
       "2 index @ exch ph2s @ addarrays"
    "}for 2drop"
"}B"
"/zentriert{dup stringwidth pop -0.5 mul 0 rmoveto show}B"
"/zzahl{mitZahlen{(000000000000)cvs zentriert}{pop}?}B"
"/zeigebuchstabe{" // belegungsstring tastenindex
    "S "
    "pop 0.5 0.5 moveto "
   << ((Beschreibungsfont != Zeichenfont)
       ? "S/ZSchrift schrift zentriert R"
       : "zentriert")
   <<
" 0.2 dup scale "
    "tmp 1 get 2.5 2.1 moveto zzahl "
    "R"
"}B"
"/skgrau{sqrt 0.95 mul 0.95 exch sub setgray}B"
"/skrgb{"
    "4 3 roll sqrt 0.95 mul 0.05 add 4 1 roll "
    "3{1 sub 3 index mul 1 add rot}repeat "
    "setrgbcolor pop"
"}B"
"/Q{"
    "0.1 0.1 0.8 0.8 rectfill "
    "0 setgray tmp 1 get "
    "0.5 0.12 moveto zzahl"
"}B"
"/K{"
    "0.05 setlinewidth newpath "
    "0.1 0.1 moveto 0.9 0.9 lineto "
    "0.1 0.9 moveto 0.9 0.1 lineto stroke"
"}B"
"/minitastatur{" // t1 htab
    "S t 0.03 0 0.94 dup minihoehe mul miniweite div rectfill "
    "R "
    "dup L{"
       "S 0.03 0 T "
       "0.94 miniweite div dup scale "
       "minixoff miniyoff T "
       "2dup get dup tmp 1 rot put "
       "bimax div "
       "over koordinate T "
       "3 index rot bigrammklasse"
       "["
           "{0 0.7 0 skrgb Q}"
           "{0 0.7 0 skrgb K}"
           "{0.9 0.7 0 skrgb Q}"
           "{0.9 0.9 0 skrgb Q}"
           "{0.2 0.9 0 skrgb Q}"
           "{0.2 0.9 0 skrgb Q}"
           "{0.9 0 0 skrgb Q}"
       "]switch "
       "R"
    "}for 2drop"
"}B"
"/taste{" // tastenindex belegung
    "over buchstabe over ph1a @ "
    "dup tmp 1 rot put "
    "einmax div skgrau "
    "over zeigebuchstabe "
    "mitmini{dup ph2a @ minitastatur}{pop}?"
"}B"
"/diffpkt{rot sub -rot exch sub exch}B"
"/skpunkt{rot over mul -rot mul}B"
"/kurve{"
    "0.4 -0.2 2dup hypot 0.15 exch div skpunkt newpath moveto "
    "dup 0.4 -0.2 rot skpunkt rot "
    "dup 0.6 -0.2 rot skpunkt rot "
    "0.4 -0.2 2dup hypot 0.15 exch div skpunkt -rot sub "
    "tmp 1 2 index put"
    "/Pattern setcolorspace"
    "<<"
       "/PatternType 2"
       "/Shading<<"
           "/ShadingType 2"
           "/ColorSpace/DeviceRGB"
           "/Coords[0 0 tmp 1 get 0]"
           "/Extend[true true]"
           "/Function<<"
               "/Domain[0 1]"
               "/FunctionType 0"
               "/Range[0 1 0 1 0 1]"
               "/DataSource tmp 0 get"
               "/BitsPerSample 8"
               "/Size[2]"
           ">>"
       ">>"
    ">>matrix makepattern setcolor "
    "exch curveto stroke"
"}B"
"/linien{" // htab tastenindex
    "S dup koordinate T 1 setlinecap t "
    "over L{"
       "2dup eq"
       "{pop}"
       "{"
           "2dup bigrammklasse"
           "[<FFFFFF00FFFF>"
            "<FFFFFFFFFFFF>"
            "<FFFFFF8000FF>"
            "<FFFFFF4000FF>"
            "<FFFFFF0080FF>"
            "<FFFFFF00E0FF>"
            "<FFFFFFFF00FF>"
           "]over get tmp 0 rot put "
           "0 eq"
           "{pop}"
           "{"
               "2 index over get bimax div "
               "dup minFuerKurve gt"
               "{"
                   "maxkurvendicke mul setlinewidth "
                   "over koordinate rot koordinate diffpkt 2dup exch atan "
                   "S "
                       "rotate hypot over finger 0 lt{1 -1 scale}if kurve "
                   "R"
               "}"
               "{2drop}?"
           "}?"
       "}?"
    "}for "
    "2drop R"
"}B"
"/gruppenzahl 2 zz mul b"
"/fsum 11 array B"
"/fsumA 11 array B"
"/gsum gruppenzahl array B"
"/gsumA gruppenzahl array B"
"/bsum[11{21 array}repeat]B"
"/bsumA[11{21 array}repeat]B"
"/zsum[gruppenzahl{zz array}repeat]B"
"/zsumA[gruppenzahl{zz array}repeat]B"
"/fingerhaeufigkeit{" // 1a fs
    "dup lf "
    "over L{"
       "2dup finger 5 add "
       "rot 4 index @ addinsarray"
    "}for 2drop"
"}B"
"/gruppenhaeufigkeit{" // 1a gs
    "dup lf "
    "over L{2dup gruppe rot 4 index @ addinsarray}for 2drop"
"}B"
"/unterklasse{" // t1 t2
    "2dup bigrammklasse dup 3 mul 4 1 roll"
    "["
       "{2drop 0}dup"
       "{zeile exch zeile sub abs}dup"
       "{2drop 0}dup"
       "{koordinate rot koordinate diffpkt hypot "
        "kollschritt div floor 2 min 0 max cvi}"
    "]switch"
"}B"
"/bigrammsumme1{" // bs 2a-1 t1
    "over L{"
       "2dup unterklasse "
       "4 index 4 3 roll get "
       "-rot over add 1 exch{"
           "over 5 index -rot addinsarray"
       "}for pop"
    "}for 3drop"
"}B"
"/bigrammhaeufigkeit{" // 2a gs
    "dup lF "
    "over L{"
       "2dup finger 5 add get "
       "over 4 index @ "
       "rot bigrammsumme1"
    "}for 2drop"
"}B"
"/zssumme1{" // zs-gr 2a-1 t1
    "finger "
    "over L{"
       "2dup finger mul 0 gt"
       "{2 index over get exch zeile 4 index -rot exch addinsarray}"
       "{pop}?"
    "}for "
    "3drop"
"}B"
"/zshaeufigkeit{" // 2a zs
    "dup lF "
    "over L{"
       "2dup gruppe get over 4 index @ rot zssumme1"
    "}for 2drop"
"}B"
"/umrandung{0.25 setgray 0.003 setlinewidth 0 0 0.8 0.12 rectstroke}B"
"/zahlimkaestchen{"
    "S "
    "0.25 setgray 0.4 0.017 moveto 0.4 dup scale zzahl "
    "R"
"}B"
"/hkaestchen{"
    "0.75 setgray "
    "over exch einsumme mul div 0.8 mul 0.12 0 0 2swap rectfill "
    "umrandung zahlimkaestchen"
"}B"
"/bigrammfarben["
    "[0 1 0][0 0 1][1 0 0]"
    "[]dup dup"
    "[1 1 0.5][1 0.85 0.25][1 0.7 0]"
    "[1 1 0.5][1 0.85 0.25][1 0.7 0]"
    "[]dup 2dup 2dup"
    "[1 0.75 0.75][1 0.4 0.4][1 0 0]"
"]B"
"/bkaestchen{" // haeufigkeiten a b
    "over 4 1 roll "
    "1 exch{"
       "bigrammfarben over get aload pop setrgbcolor "
       "over @ "
       "bisumme bikaestchen mul div 0.8 mul 0.12 0 0 2swap rectfill"
    "}for "
    "umrandung @ zahlimkaestchen"
"}B"
"/zsfarbe["
    "[[0.5 0.5 0.5][0.5 0.5 0.5][0.5 0.5 0.5][0.5 0.5 0.5][0.5 0.5 0.5]]"
    "[[0.5 0.5 0.5][0 1 0][1 0.7 0][1 0 0][1 0 0]]"
    "[[0.5 0.5 0.5][1 0.5 0][0 1 0][1 0.7 0][1 0 0]]"
    "[[0.5 0.5 0.5][1 0 0][1 0.5 0][0 1 0][1 0.7 0]]"
    "[[0.5 0.5 0.5][1 0 0][1 0 0][1 0.7 0][0 1 0]]"
"]B"
"/zkaestchen{" // haeufigkeiten farbtab
    "minzeile 1 maxzeile{"
       "2dup get aload pop setrgbcolor "
       "2 index over get bisumme zhkaestchen mul div 0.8 mul "
       "0.12 0 0 2swap rectfill "
       "umrandung 2 index @ zahlimkaestchen "
       "0.8 0.07 T"
    "}for 2drop"
"}B"
"/anzeigen{" // beschreibung belegung
    "S "
       "beschrx beschry moveto beschrgr dup scale"
       "{S show R 0 -0.27 rmoveto}forall "
    "R "
    "ph1a L{"
       "S "
       "0.4 -0.55 T ph2a over get exch linien "
       "R"
    "}for "
    "ph1a L{"
       "S "
       "dup koordinate 1 sub T 0.8 dup scale over taste "
       "R"
    "}for pop "
    "mitSummen{"
       "fsum L{"
           "dup 5 sub abs dup mf ge exch xf le and{"
               "S "
               "dup dup 5 lt{2}{1}? minx sub add -0.3 T "
               "dup fsum @ fhkaestchen hkaestchen "
               "0.15 -0.1 T 0.625 dup scale "
               "dup bsum @ dup 18 20 bkaestchen "
               "over dup 5 lt{0.4}{-0.4}? -0.15 T "
               "5 sub abs dup mf ne{over 9 11 bkaestchen}if "
               "rot 5 lt{-0.8}{0.8}? -0.08 T "
               "xf ne{6 8 bkaestchen}{pop}? "
               "R"
           "}{pop}?"
       "}for "
       "gsum L{"
           "dup zz mod dup minzeile ge exch maxzeile le and{"
               "S "
               "dup zz idiv " << weite << " mul 0.25 sub "
               "over zz mod 1 sub T "
               "90 rotate "
               "dup gsum @ ghkaestchen hkaestchen "
               "-0.1 -0.14 T 1.25 maxzeile minzeile sub 1 add div dup scale "
               "zsum over get exch zz mod zsfarbe @ zkaestchen "
               "R"
           "}{pop}?"
       "}for"
    "}if"
"}B"
"/adirekt{" // belegung beschreibung--
    "over ph1a ph2a setze "
    "mitSummen{"
       "ph1a fsum fingerhaeufigkeit "
       "ph1a gsum gruppenhaeufigkeit "
       "ph2a bsum bigrammhaeufigkeit "
       "ph2a zsum zshaeufigkeit"
    "}if "
    "anzeigen"
"}B"
"/adiff{"
    "ph1A ph2A setze over ph1a ph2a setze "
    "mitSummen{"
       "ph1a fsum fingerhaeufigkeit ph1a gsum gruppenhaeufigkeit "
       "ph2a bsum bigrammhaeufigkeit ph2a zsum zshaeufigkeit "
       "ph1A fsumA fingerhaeufigkeit ph1A gsumA gruppenhaeufigkeit "
       "ph2A bsumA bigrammhaeufigkeit ph2A zsumA zshaeufigkeit "
       "fsum fsumA submaxarrays gsum gsumA submaxarrays "
       "bsum bsumA submaxmatrizen zsum zsumA submaxmatrizen "
       "ph1a ph1A submaxarrays ph2a ph2A submaxmatrizen"
    "}if "
    "anzeigen"
"}B"
// Zwei Belegungen.  (belegung1 beschr1 belegung2 beschr2 --)
"/an{S adirekt 0 4 skala div T adirekt showpage R}B"
// Differenz zweier Belegungen (belegung1 beschr1 belegung2 beschr2 --)
"/ad{S over exch 4 index adiff 0 4 skala div T adiff showpage R}B"
// Belegung und Differenz (belegung1 beschr1 belegung2 beschr2 --)
"/M{S 3 index adiff 0 4 skala div T adirekt showpage R}B"
// Zwei Belegungen oder ihre Differenz.
"/A{differenzen{ad}{an}?}B"
// Einzelne Belegung
"/E{S 0 2 T adirekt showpage grestore}B\n"
"%%EndProlog\n"
"%%BeginSetup\n"
"<</PageSize[842 595]>> setpagedevice "
"65 skala mul dup scale 0.5 1.7 T"
"/BSchrift schrift\n"
"%%EndSetup" << std::endl;
};

void
Grafik::
ausgabe(const belegung_t b){
   ungerade = !ungerade;
   if(ungerade){
      seite++;
      grafik << "%%Page: " << seite << " " << seite << "\n";
   }
   grafik << "(";
   for(int i = 0; i < tastatur.nvariabel(); ++i)
      grafik << kodierung.psencstr(b[i]);
   grafik << ")[]";

   if(!ungerade) grafik << "A" << std::endl;
}

Grafik::
~Grafik(){
   if(ungerade) grafik << "E\n";
   grafik << "%%EOF" << std::endl;
}

//--------------- src/string_in_belegung.cc ---------------
//#include "string_in_belegung.hh"

//#include "Kodierung.hh"
//#include "Tastatur.hh"
//#include "utfhilfe.hh"
#include <iostream>

void string_in_belegung(const std::u32string& z, belegung_t b, bool* fest,
                        const Tastatur& tastatur, const Kodierung& kodierung)
{
   char32_t gabs[ntaste];
   for(auto& i : gabs) i = 0;
   for(int i = 0; i < ntaste; ++i) b[i] = i;

   const int l = z.size(), nv = tastatur.nvariabel();
   const bool mit_fixen = l > nv;
   const int gebraucht = mit_fixen ? ntaste : nv;
   if(l < gebraucht){
      std::cerr << SPRACHE("Die Belegung '", "The layout '")
                << utf32_in_ausgabe(z)
                << SPRACHE("' hat ", "' has ")
                << l << SPRACHE(" Zeichen, erwartet werden ",
                                " symbols, but expected are ")
                <<  gebraucht << "." << std::endl;
      exit(1);
   }

   for(int seq = 0; seq < gebraucht; ++seq){
      const int i = mit_fixen ? tastatur.taste(seq) : seq;
      const auto p = kodierung.position(z[seq]);
      if(p.first == -1){
         if(i >= nv){
            std::cerr << SPRACHE("Die Belegung '", "The layout '")
                      << utf32_in_ausgabe(z)
                      << SPRACHE("' enth" strAe "lt das unbekannte Zeichen '",
                                 "' contains the unknown symbol '")
                      << utf32_in_ausgabe(z[seq]) << "'." << std::endl;
            exit(1);
         }
      }else if(gabs[p.first]){
         std::cerr << SPRACHE("Die Belegung  '", "The layout '")
                   << utf32_in_ausgabe(z)
                   << SPRACHE("' enth" strAe "lt '", "' contains '")
                   << utf32_in_ausgabe(z[seq])
                   << SPRACHE("' mehrfach. ", "' multiple times.");
         if(z[seq] != gabs[p.first]){
            std::cerr << SPRACHE(" Vorher wurde das gleichwertige Zeichen '",
                                 " Previously, the equivalent symbol '")
                      << utf32_in_ausgabe(gabs[p.first])
                      << SPRACHE("' verwendet.", "' has been used.");
         }
         std::cerr << std::endl;
         exit(1);
      }else{
         gabs[p.first] = z[seq];
      }
      if(i < nv){
         b[i] = p.first;
         fest[i] = (p.second != 0) && kodierung.txt(p.first, 0).length();
      }else{
         fest[i] = true;
         if(p.first >= 0 && p.first < nv){
            std::cerr<< SPRACHE("Die Belegung  '", "The layout '")
                   << utf32_in_ausgabe(z)
                      << SPRACHE("' enth" strAe "lt das Zeichen '",
                                 "' contains the symbol '")
                     << utf32_in_ausgabe(z[seq])
                     << SPRACHE("' an einer 'FixesZeichen'-Position.",
                                "' at a 'FixesZeichen' position.")
                     << std::endl;
            exit(1);
         }
      }
   }
}

//--------------- src/Eingabestream.cc ---------------
//#include "Eingabestream.hh"

//#include "konstanten.hh"
//#include "utfhilfe.hh"
#include <iostream>

namespace {

constexpr char32_t cp1252extra[] =
   U"\u20ac \u201a\u0192\u201e\u2026\u2020\u2021\u02c6\u2030\u0160"
   U"\u2039\u0152 \u017d  \u2018\u2019\u201c\u201d\u2022\u2013\u2014"
   U"\u02dc\u2122\u0161\u203a\u0153 \u017e\u0178";

char32_t cp1252_in_utf32(const char*& s){
   if(*s == 0) return 0;
   const char32_t c = *s++ & 0xff;
   if(c < 0x80 || c >= 0xA0) return c;
   return cp1252extra[c-0x80];
}

double zehnhoch(unsigned n){
   double fak = 1., mult = 10.;
   while(n){
      if(n & 1) fak *= mult;
      n >>= 1;
      mult *= mult;
   }
   return fak;
}

constexpr char32_t BOM = U'\uFEFF', CR = U'\u000D';
}


Eingabestream::
Eingabestream(const std::string& nam, bool utf8,
              bool muss_existieren)
   : name(nam), l(0), p(0), maxl(0), izeile(0), errpos(-1),
     utf8ein(utf8), geaendert(false){
   f.open(name.c_str());
   if(muss_existieren && !f){
      std::cerr << "'" << name
                << SPRACHE("' ist nicht lesbar.", "' is not readable")
                << std::endl;
      exit(1);
   }
}

Eingabestream::
~Eingabestream()
{ f.close(); }

// Lies nächstes Zeichen (einschliesslich Zeilenenden '\n'); falls Eingabe
// erschöpft ist gibt Null zurück.
char32_t
Eingabestream::lieszeichen(){
   if(p < l) return buffer[p++];
   if(!fuellen()) return 0;
   return U'\n';
}

// Falls aus der aktuellen Zeile schon Zeichen gelesen wurden, fange eine
// neue an.  Gib false zurück, wenn die Eingabe erschöpft ist.
bool
Eingabestream::neuezeile(){
   errpos = -1;
   if(!p && l) return true;
   p = l;
   return fuellen();
}
// Wie neuezeile, übergeht aber Kommentar- und Leerzeilen.
bool
Eingabestream::echte_neuezeile(){
   while(neuezeile()){
      zwischenraum_uebergehen();
      if(p == l) continue;
      if(ist(U'#')){ p = l; continue; }
      return true;
   }
   return false;
}

size_t
Eingabestream::restzeichen() const
{ return l-p; }

bool
Eingabestream::ist_zwischenraum() const
{ return p < l && ::ist_zwischenraum(buffer[p]); }

bool
Eingabestream::ist_ziffer() const
{ return p < l && ::ist_ziffer(buffer[p]); }

// Ist aktuelles Zeichen in aktueller Zeile gleich dem übergebenen?
bool Eingabestream::ist(char32_t c) const { return p < l && c == buffer[p]; }

void
Eingabestream::zwischenraum_uebergehen()
{ while(ist_zwischenraum()) ++p; }

void
Eingabestream::uebergehen()
{ if(p < l) ++p; }

char32_t
Eingabestream::lies_in_zeile()
{ return p < l ?  buffer[p++] : 0; }

bool
Eingabestream::zeilenende(){
   zwischenraum_uebergehen();
   errpos = p;
   return p == l;
}

bool
Eingabestream::hole_wort(std::u32string& wert){
   wert.resize(0);
   zwischenraum_uebergehen();
   errpos = p;
   if(p >= l) return false;
   if(ist(U'#')){ p = l; return false; }
   while(p < l && !ist_zwischenraum()) wert.push_back(buffer[p++]);
   return true;
}

bool
Eingabestream::hole_flag(bool& wert){
   zwischenraum_uebergehen();
   errpos = p;
   if(p >= l) return false;
   wert = ist(U'+');
   if(wert || ist(U'-')){ ++p; return p == l || ist_zwischenraum(); }
   return false;
}

bool
Eingabestream::hole_string(std::u32string& wert){
   wert.resize(0);
   zwischenraum_uebergehen();
   errpos = p;
   if(p+1 >= l) return false;
   const char32_t ende = buffer[p++];
   while(p < l && buffer[p] != ende) wert.push_back(buffer[p++]);
   if(p >= l) return false;
   ++p;
   return true;
}

bool Eingabestream::encoding_geaendert() const { return geaendert; }

[[noreturn]] void
Eingabestream::fehler(size_t off) const {
   const std::string o = name+SPRACHE(", Zeile ", ", row ")+
      std::to_string(izeile)+": ";
   std::cerr << o << zeile << std::endl;
   if(errpos >=0){
      size_t n = o.length()+errpos+off;
      for(size_t i = 0; i < n; ++i)
         std::cerr << ".";
      std::cerr << "^" << std::endl;
   }
   exit(1);
}

size_t
Eingabestream::aktuelle_zeile() const
{ return izeile; }

const std::string&
Eingabestream::aktuelles_file() const
{ return name; }

bool
Eingabestream::hole_zahl(double& wert){
   zwischenraum_uebergehen();
   errpos = p;
   wert = 0;
   if(p >= l) return false;

   double sign = 1.;
   if(buffer[p] == U'-'){
      sign = -1.; ++p;
   }else if(buffer[p] == U'+') ++p;

   int nk = 0, ev = 0, es = 1;
   bool nachkomma = false, exponent = false, zf = false;
   for(; p < l && !ist_zwischenraum(); ++p){
      if(ist_ziffer()){
         const double val = buffer[p]-U'0';
         if(exponent){
            ev = ev*10+val;
         }else{
            wert = wert*10.+val;
            if(nachkomma) ++nk;
            zf = true;
         }
      }else if(buffer[p] == U'.' || buffer[p] == U','){
         if(exponent || nachkomma) return false;
         nachkomma = true;
      }else if(buffer[p] == U'e' || buffer[p] == U'E'){
         if(exponent) return false;
         if(p+1 == l) return false;
         if(buffer[p+1] == '-'){
            es = -1.; ++p;
         }else if(buffer[p+1] == '+') ++p;
         if(p+1 == l) return false;
         ++p;
         if(!ist_ziffer()) return false;
         ev =  buffer[p]-U'0';
         exponent = true;
      }else{
         return false;
      }
   }
   ev = es*ev-nk;
   const double e10 = sign*zehnhoch(std::abs(ev));
   wert = ev < 0 ? wert/e10 : wert*e10;
   return zf;
}

bool
Eingabestream::fuellen(){
   if(p < l) return true; // Buffer nicht leer.
   p = l = 0;
   buffer.resize(0);  buffer.reserve(maxl);
   if(!f || f.rdstate() & std::ios::eofbit) return false;
   // Das \n am Zeilenende ist nicht enthalten.
   std::getline(f, zeile);
   ++izeile;

   const char* cp = zeile.c_str();
   if(utf8ein){
      bool reinterpret = false;
      while(*cp){
         const char32_t c = utf8_in_utf32(cp, reinterpret);
         if(reinterpret){
            l = 0; buffer.resize(0);  buffer.reserve(maxl);
            geaendert = true;
            return false;
         }
         if(c != BOM && c != CR){  // wegen CP/M und dergleichen
            buffer.push_back(c);
            ++l;
         }
      }
   }else{
      while(*cp){
         buffer.push_back(cp1252_in_utf32(cp));
         ++l;
      }
   }

   if(l > maxl) maxl = l;
   return true;
}

double
hole_zahl(Eingabestream& f, double rmin, double rmax){
    double w;
    if(f.hole_zahl(w)){
       if(w < rmin || w > rmax){
          std::cerr << w
                    << SPRACHE(" ist ausserhalb des erlaubten Wertebereichs [",
                               " is not within the allowed range of values [")
                    << rmin << ", " << rmax << "]." << std::endl;
       }else return w;
    }else{
       std::cerr << SPRACHE("Eine Zahl wurde erwartet, jedoch nicht gefunden.",
                            "A number has been expected, but none was found.")
                  << std::endl;
    }
    f.fehler();
}

void
pruefe_leer_dann_N(Eingabestream& f, size_t N){
   const bool rest = f.restzeichen() == N+1;
   const bool zwischen = f.ist_zwischenraum();
   if(zwischen && rest) return;

   std::cerr << SPRACHE("Nach der Zahl werden ein Leerzeichen und genau ",
                        "After the number, one space character and exactly ")
             << N << SPRACHE(" weitere Zeichen erwartet.",
                             " additional characters are expected.")
             << std::endl;
   f.fehler();
}

//--------------- src/Statistik.cc ---------------
//#include "Statistik.hh"

//#include "Aufwandstabelle.hh"
//#include "Haeufigkeit.hh"
//#include "Kodierung.hh"
//#include "Tastatur.hh"

Statistik::
Statistik(const belegung_t b, const Tastatur& tastatur,
          const Kodierung& kodierung, const Haeufigkeit& h,
          const Aufwandstabelle& a, const double ngrammakkumlimit[3])
{
   for(int i = 0; i < ntaste; ++i){
      const int fi  = tastatur.finger(i);
      const int zi  = b[i], sti = tastatur.shifttaste(i);
      const int fx  = tastatur.finger_index(i);
      const int s   = fi > 0;
      const int sfi = tastatur.finger(sti);
      const int ss  = sfi > 0;
      const int sfx = tastatur.finger_index(sti);
      const int z   = tastatur.zeile(i);
      const int sz  = tastatur.zeile(sti);

      mitfinger[fx] = mitfinger[sfx] = true;

      haeufigkeit_t h01 = 0;
      for(int ei = 0; ei < nebene; ++ei){
         aeinzel += a(i,ei)*h(zi,ei);
         if(fx < nfinger) h01 += h(zi,ei);
      }
      const haeufigkeit_t h1 = sfx < nfinger ? h(zi,1) : 0;
      h1tot         += h01+h1;
      hpos[s][z]    += h01;
      hpos[ss][sz]  += h1;
      hfinger[fx]   += h01;
      hfinger[sfx]  += h1;
      hlinks        += (1-s)*h01+(1-ss)*h1;
      hslinks       += (1-ss)*h1;
      hrechts       += ss*h1+s*h01;
      hsrechts      += ss*h1;

      for(int j = 0; j < ntaste; ++j){
         if(tastatur.kategorie(i,j) == kategorie_t::MitUndefDaumen) continue;
         const int zj = b[j];
         const int stj= tastatur.shifttaste(j), sfy= tastatur.finger_index(stj);
         const int fy  = tastatur.finger_index(j);
         const int fj = tastatur.finger(j);
         const int f2[2] = { fj, tastatur.finger(stj) };
         const int t2[2] = { j, stj }, fi2[2] = { fy, sfy };

         // sbb und sbs (siehe den ganz langen Kommentar oben) sind ein
         // Mittelding zwischen Bigrammen und Trigammen.  Für die Ausgabe
         // weisen wir sie getrennt und mit der Vorsatz "Shift-" aus.
         for(int ej = 0; ej < nebene2; ++ej){
            // Shift-Bigramme (sbb und sbs)
            const haeufigkeit_t h2 = h(zi,1,zj,ej);
            hs[tastatur.kategorie(sti,t2[ej])] += h2;
            hs2tot += h2;

            const std::vector<int> sbi = { sti, t2[ej] };
            for(const int ub : tastatur.benutzerkategorie(sbi))
               hs_benutzer[ub] += h2;

            if(std::abs(sfi-f2[ej]) == 1){
               hsnachbar += h2;
               hsnachbar1[sti-ntaste] += h2;
               if(std::abs(sz-tastatur.zeile(t2[ej])) > 1)
                  hsnachbar2[sti-ntaste] += h2;
            }else if(tastatur.kategorie(sti,t2[ej]) == kategorie_t::Kollision){
               hskollision1[sti-ntaste] += h2;
               if(tastatur.distanz(sti,t2[ej]) >= 2)
                  hskollision2[sti-ntaste] += h2;
            }

            if(h2 > 0 && ngrammakkumlimit[1] > 0 &&
               tastatur.istHandwiederholung(sti,t2[ej])){
               std::string bg_typ = kodierung.txt(zi,1)+kodierung.txt(zj,ej)
                  +" : Shift-"+tastatur.kategorie_lang(sti, t2[ej]);
               hrel[1][ss] += h2;
               const std::pair<haeufigkeit_t, std::string> hw(h2, bg_typ);
               ngramm[1][ss].insert(hw);
            }

            for(int ei = 0; ei < nebene; ++ei){
               // Bigramme: bb in B1 und B2 (ej == 0), bs in B3 und B4 (ej == 1)
               const haeufigkeit_t h2 = h(zi,ei,zj,ej);
               hk[tastatur.kategorie(i,t2[ej])] += h2;
               h2tot+= h2;
               if(std::abs(f2[ej]-fi) == 1){
                  hnachbar += h2;
                  hnachbar1[(fx+fi2[ej]-1)/2] += h2;
                  if(std::abs(z-tastatur.zeile(t2[ej])) > 1)
                     hnachbar2[(fx+fi2[ej]-1)/2] += h2;
               }else if(tastatur.kategorie(i,t2[ej]) == kategorie_t::Kollision){
                  assert(fx < nfinger);
                  hkollision1[fx] += h2;
                  if(tastatur.distanz(i,t2[ej]) >= 2) hkollision2[fx] += h2;
               }

               const std::vector<int> bi = {i, t2[ej] };
               for(const int ub : tastatur.benutzerkategorie(bi))
                  hk_benutzer[ub] += h2;

               if(ej){
                  // bsb, vom Tippen her Trigramme.
                  const haeufigkeit_t h3 = h(zi,ei,zj,ej);
                  // Wir interessieren uns nicht für einfache Handwechsel, und
                  // kein Handwechsel kann hier nicht vorkommen.
                  if(tastatur.kategorie(i,stj) == kategorie_t::Handwechsel){
                     hi2[tastatur.kategorie(i,j)] += h3; // bsb in B3 und B4
                     hdoppelhw += h3;
                  }
                  h3tot += h3;

                  const std::vector<int> tri = {i, stj, j };
                  for(const int ub : tastatur.benutzerkategorie(tri))
                     ht_benutzer[ub] += h2;
               }
            }

            // bb und bs.  Die hatten wir oben schonmal, wir müssen hier aber
            // darauf achten, dass wir nicht unterscheiden, ob das erste b ein
            // Grossbuchstabe ist oder nicht, denn in beiden Fällen handelt es
            // sich um dasselbe Tastenbigramm (das unterscheidende Shift liegt
            // vor diesem).
            const haeufigkeit_t hij = h(zi,0,zj,ej)+h(zi,1,zj,ej);
            if(hij > 0 && ngrammakkumlimit[0] > 0 &&
               tastatur.istHandwiederholung(i, t2[ej])){
               // Für ej == 1 steht der Grossbuchstabe in diesem Bigramm für
               // die Shifttaste die man zu seine Eingabe braucht.
               std::string bg_typ = kodierung.txt(zi,0)+kodierung.txt(zj,ej)
                  +" : "+tastatur.kategorie_lang(i, t2[ej]);
               hrel[0][s] += hij;
               const std::pair<haeufigkeit_t, std::string> hw(hij, bg_typ);
               ngramm[0][s].insert(hw);
            }
         }

         if(h.mit_trigrammen()){
            for(int k = 0; k < ntaste; ++k){
               if(tastatur.finger(k) == finger_t::EinerDerDaumen) continue;
               const int zk = b[k];

               // Die normalen Trigramme.
               const int stk = tastatur.shifttaste(k), t3[2] = { k, stk };
               const haeufigkeit_t h3[2] = { h.tri(zi,zj,zk,0),
                                             h.tri(zi,zj,zk,1) };
               h3tot += h3[0]+h3[1];

               for(int ek = 0; ek < 2; ++ek){
                  if(tastatur.kategorie(i,j) == kategorie_t::Handwechsel){
                     // ek == 0: bbb in T1 und T2; ek == 1: bbs in T5 und T6
                     hi2[tastatur.kategorie(i,t3[ek])] += h3[ek];
                  }else{
                     if(tastatur.kategorie(i,t3[ek]) !=
                        kategorie_t::Handwechsel){
                        if(tastatur.istWippe(i,j,t3[ek])) hwippe += h3[ek];
                        hi0[tastatur.kategorie(i,t3[ek])] += h3[ek];
                     }
                  }

                  const std::vector<int> tri = {i, j, t3[ek] };
                  for(const int ub : tastatur.benutzerkategorie(tri))
                     ht_benutzer[ub] += h3[ek];

                  const int wdh =
                     (tastatur.kategorie(i,j) == kategorie_t::Handwechsel)+
                     (tastatur.kategorie(j,t3[ek]) == kategorie_t::Handwechsel);
                  if(h3[ek] <= 0 || wdh == 1) continue;

                  if(wdh == 2) hdoppelhw += h3[ek];
                  else hkeinhw += h3[ek];

                  if(ngrammakkumlimit[2] > 0){
                     std::string tri_typ = kodierung.txt(zi,0)
                        +kodierung.txt(zj,0)+kodierung.txt(zk,ek)+" : ";
                     if(wdh == 2){
                        // doppelter Handwechsel: Charakterisiert durch erste
                        // und erste und letzte Taste
                        tri_typ += SPRACHE("Indirekt-", "indirect ")
                           +tastatur.kategorie_lang(i, t3[ek]);
                     }else{
                        // Sonst durch die beiden Bigramme charakterisiert.
                        tri_typ += tastatur.kategorie_lang(i, j)
                           +" + "+tastatur.kategorie_lang(j, t3[ek]);
                        if(tastatur.istWippe(i,j,t3[ek]))
                           tri_typ += SPRACHE(" (Wippe)", " (seesaw)");
                     }
                     const int s = wdh/2;
                     const std::pair<haeufigkeit_t, std::string> tri(h3[ek],
                                                                     tri_typ);
                     hrel[2][s] += h3[ek];
                     ngramm[2][s].insert(tri);
                  }
               }
            }
         }
      }
   }

   const akkumuations_t sk1 = 100./h1tot;
   hlinks *= sk1;
   hrechts *= sk1;
   hslinks *= sk1;
   hsrechts *= sk1;
   for(int s = 0; s < 2; ++s) for(auto& x : hpos[s]) x *= sk1;
   for(int i = 0; i < nfinger; ++i) hfinger[i] *= sk1;

   const akkumuations_t sk2 = 100./h2tot;
   hnachbar *= sk2;
   for(auto& x : hk)  x *= sk2;
   for(auto& i : hk_benutzer) i.second *= sk2;
   for(int i = 0; i < nfinger; ++i){
      hkollision1[i] *= sk2;
      hkollision2[i] *= sk2;
      hnachbar1[i]   *= sk2;
      hnachbar2[i]   *= sk2;
   }

   // ssk2 == 0 kommt durchaus vor; vemeide NaNs.
   const akkumuations_t ssk2 = hs2tot > 0 ? 100./hs2tot : 0;
   hsnachbar *= ssk2;
   for(auto& x : hs)  x *= ssk2;
   for(auto& i : hs_benutzer) i.second *= ssk2;
   for(int i = 0; i < nshift; ++i){
      hskollision1[i] *= ssk2;
      hskollision2[i] *= ssk2;
      hsnachbar1[i]   *= ssk2;
      hsnachbar2[i]   *= ssk2;
   }

   const akkumuations_t sk3 = h3tot > 0 ? 100./h3tot : 0;
   hkeinhw *= sk3;
   hdoppelhw *= sk3;
   hwippe *= sk3;
   for(auto& x : hi0) x *= sk3;
   for(auto& x : hi2) x *= sk3;
   for(auto& i : ht_benutzer) i.second *= sk3;
}

//--------------- src/Unicode.cc ---------------
//#include "Unicode.hh"

namespace {
// Um vom eingestellten Locale unabhängig zu werden, definieren wir unsere
// Zeichenklassifikation selber.
constexpr char32_t kleinbuchstaben[] =
   U"\u0061\u0062\u0063\u0064\u0065\u0066\u0067\u0068\u0069\u006A\u006B"
   U"\u006C\u006D\u006E\u006F\u0070\u0071\u0072\u0073\u0074\u0075\u0076"
   U"\u0077\u0078\u0079\u007A\u00E0\u00E1\u00E2\u00E3\u00E4\u00E5\u00E6"
   U"\u00E7\u00E8\u00E9\u00EA\u00EB\u00EC\u00ED\u00EE\u00EF\u00F0\u00F1"
   U"\u00F2\u00F3\u00F4\u00F5\u00F6\u00F8\u00F9\u00FA\u00FB\u00FC\u00FD"
   U"\u00FE\u0101\u0103\u0105\u0107\u0109\u010B\u010D\u010F\u0111\u0113"
   U"\u0115\u0117\u0119\u011B\u011D\u011F\u0121\u0123\u0125\u0127\u0129"
   U"\u012B\u012D\u012F\u0069\u0133\u0135\u0137\u013A\u013C\u013E\u0140"
   U"\u0142\u0144\u0146\u0148\u014B\u014D\u014F\u0151\u0153\u0155\u0157"
   U"\u0159\u015B\u015D\u015F\u0161\u0163\u0165\u0167\u0169\u016B\u016D"
   U"\u016F\u0171\u0173\u0175\u0177\u00FF\u017A\u017C\u017E\u0253\u0183"
   U"\u0185\u0254\u0188\u0256\u0257\u018C\u01DD\u0259\u025B\u0192\u0260"
   U"\u0263\u0269\u0268\u0199\u026F\u0272\u0275\u01A1\u01A3\u01A5\u0280"
   U"\u01A8\u0283\u01AD\u0288\u01B0\u028A\u028B\u01B4\u01B6\u0292\u01B9"
   U"\u01BD\u01C6\u01C9\u01CC\u01CE\u01D0\u01D2\u01D4\u01D6\u01D8\u01DA"
   U"\u01DC\u01DF\u01E1\u01E3\u01E5\u01E7\u01E9\u01EB\u01ED\u01EF\u01F3"
   U"\u01F5\u0195\u01BF\u01F9\u01FB\u01FD\u01FF\u0201\u0203\u0205\u0207"
   U"\u0209\u020B\u020D\u020F\u0211\u0213\u0215\u0217\u0219\u021B\u021D"
   U"\u021F\u019E\u0223\u0225\u0227\u0229\u022B\u022D\u022F\u0231\u0233"
   U"\u2C65\u023C\u019A\u2C66\u0242\u0180\u0289\u028C\u0247\u0249\u024B"
   U"\u024D\u024F\u0371\u0373\u0377\u03F3\u03AC\u03AD\u03AE\u03AF\u03CC"
   U"\u03CD\u03CE\u03B1\u03B2\u03B3\u03B4\u03B5\u03B6\u03B7\u03B8\u03B9"
   U"\u03BA\u03BB\u03BC\u03BD\u03BE\u03BF\u03C0\u03C1\u03C3\u03C4\u03C5"
   U"\u03C6\u03C7\u03C8\u03C9\u03CA\u03CB\u03D7\u03D9\u03DB\u03DD\u03DF"
   U"\u03E1\u03E3\u03E5\u03E7\u03E9\u03EB\u03ED\u03EF\u03B8\u03F8\u03F2"
   U"\u03FB\u037B\u037C\u037D\u0450\u0451\u0452\u0453\u0454\u0455\u0456"
   U"\u0457\u0458\u0459\u045A\u045B\u045C\u045D\u045E\u045F\u0430\u0431"
   U"\u0432\u0433\u0434\u0435\u0436\u0437\u0438\u0439\u043A\u043B\u043C"
   U"\u043D\u043E\u043F\u0440\u0441\u0442\u0443\u0444\u0445\u0446\u0447"
   U"\u0448\u0449\u044A\u044B\u044C\u044D\u044E\u044F\u0461\u0463\u0465"
   U"\u0467\u0469\u046B\u046D\u046F\u0471\u0473\u0475\u0477\u0479\u047B"
   U"\u047D\u047F\u0481\u048B\u048D\u048F\u0491\u0493\u0495\u0497\u0499"
   U"\u049B\u049D\u049F\u04A1\u04A3\u04A5\u04A7\u04A9\u04AB\u04AD\u04AF"
   U"\u04B1\u04B3\u04B5\u04B7\u04B9\u04BB\u04BD\u04BF\u04CF\u04C2\u04C4"
   U"\u04C6\u04C8\u04CA\u04CC\u04CE\u04D1\u04D3\u04D5\u04D7\u04D9\u04DB"
   U"\u04DD\u04DF\u04E1\u04E3\u04E5\u04E7\u04E9\u04EB\u04ED\u04EF\u04F1"
   U"\u04F3\u04F5\u04F7\u04F9\u04FB\u04FD\u04FF\u0501\u0503\u0505\u0507"
   U"\u0509\u050B\u050D\u050F\u0511\u0513\u0515\u0517\u0519\u051B\u051D"
   U"\u051F\u0521\u0523\u0525\u0527\u0529\u052B\u052D\u052F\u0561\u0562"
   U"\u0563\u0564\u0565\u0566\u0567\u0568\u0569\u056A\u056B\u056C\u056D"
   U"\u056E\u056F\u0570\u0571\u0572\u0573\u0574\u0575\u0576\u0577\u0578"
   U"\u0579\u057A\u057B\u057C\u057D\u057E\u057F\u0580\u0581\u0582\u0583"
   U"\u0584\u0585\u0586\u2D00\u2D01\u2D02\u2D03\u2D04\u2D05\u2D06\u2D07"
   U"\u2D08\u2D09\u2D0A\u2D0B\u2D0C\u2D0D\u2D0E\u2D0F\u2D10\u2D11\u2D12"
   U"\u2D13\u2D14\u2D15\u2D16\u2D17\u2D18\u2D19\u2D1A\u2D1B\u2D1C\u2D1D"
   U"\u2D1E\u2D1F\u2D20\u2D21\u2D22\u2D23\u2D24\u2D25\u2D27\u2D2D\uAB70"
   U"\uAB71\uAB72\uAB73\uAB74\uAB75\uAB76\uAB77\uAB78\uAB79\uAB7A\uAB7B"
   U"\uAB7C\uAB7D\uAB7E\uAB7F\uAB80\uAB81\uAB82\uAB83\uAB84\uAB85\uAB86"
   U"\uAB87\uAB88\uAB89\uAB8A\uAB8B\uAB8C\uAB8D\uAB8E\uAB8F\uAB90\uAB91"
   U"\uAB92\uAB93\uAB94\uAB95\uAB96\uAB97\uAB98\uAB99\uAB9A\uAB9B\uAB9C"
   U"\uAB9D\uAB9E\uAB9F\uABA0\uABA1\uABA2\uABA3\uABA4\uABA5\uABA6\uABA7"
   U"\uABA8\uABA9\uABAA\uABAB\uABAC\uABAD\uABAE\uABAF\uABB0\uABB1\uABB2"
   U"\uABB3\uABB4\uABB5\uABB6\uABB7\uABB8\uABB9\uABBA\uABBB\uABBC\uABBD"
   U"\uABBE\uABBF\u13F8\u13F9\u13FA\u13FB\u13FC\u13FD\u10D0\u10D1\u10D2"
   U"\u10D3\u10D4\u10D5\u10D6\u10D7\u10D8\u10D9\u10DA\u10DB\u10DC\u10DD"
   U"\u10DE\u10DF\u10E0\u10E1\u10E2\u10E3\u10E4\u10E5\u10E6\u10E7\u10E8"
   U"\u10E9\u10EA\u10EB\u10EC\u10ED\u10EE\u10EF\u10F0\u10F1\u10F2\u10F3"
   U"\u10F4\u10F5\u10F6\u10F7\u10F8\u10F9\u10FA\u10FD\u10FE\u10FF\u1E01"
   U"\u1E03\u1E05\u1E07\u1E09\u1E0B\u1E0D\u1E0F\u1E11\u1E13\u1E15\u1E17"
   U"\u1E19\u1E1B\u1E1D\u1E1F\u1E21\u1E23\u1E25\u1E27\u1E29\u1E2B\u1E2D"
   U"\u1E2F\u1E31\u1E33\u1E35\u1E37\u1E39\u1E3B\u1E3D\u1E3F\u1E41\u1E43"
   U"\u1E45\u1E47\u1E49\u1E4B\u1E4D\u1E4F\u1E51\u1E53\u1E55\u1E57\u1E59"
   U"\u1E5B\u1E5D\u1E5F\u1E61\u1E63\u1E65\u1E67\u1E69\u1E6B\u1E6D\u1E6F"
   U"\u1E71\u1E73\u1E75\u1E77\u1E79\u1E7B\u1E7D\u1E7F\u1E81\u1E83\u1E85"
   U"\u1E87\u1E89\u1E8B\u1E8D\u1E8F\u1E91\u1E93\u1E95\u00DF\u1EA1\u1EA3"
   U"\u1EA5\u1EA7\u1EA9\u1EAB\u1EAD\u1EAF\u1EB1\u1EB3\u1EB5\u1EB7\u1EB9"
   U"\u1EBB\u1EBD\u1EBF\u1EC1\u1EC3\u1EC5\u1EC7\u1EC9\u1ECB\u1ECD\u1ECF"
   U"\u1ED1\u1ED3\u1ED5\u1ED7\u1ED9\u1EDB\u1EDD\u1EDF\u1EE1\u1EE3\u1EE5"
   U"\u1EE7\u1EE9\u1EEB\u1EED\u1EEF\u1EF1\u1EF3\u1EF5\u1EF7\u1EF9\u1EFB"
   U"\u1EFD\u1EFF\u1F00\u1F01\u1F02\u1F03\u1F04\u1F05\u1F06\u1F07\u1F10"
   U"\u1F11\u1F12\u1F13\u1F14\u1F15\u1F20\u1F21\u1F22\u1F23\u1F24\u1F25"
   U"\u1F26\u1F27\u1F30\u1F31\u1F32\u1F33\u1F34\u1F35\u1F36\u1F37\u1F40"
   U"\u1F41\u1F42\u1F43\u1F44\u1F45\u1F51\u1F53\u1F55\u1F57\u1F60\u1F61"
   U"\u1F62\u1F63\u1F64\u1F65\u1F66\u1F67\u1FB0\u1FB1\u1F70\u1F71\u1F72"
   U"\u1F73\u1F74\u1F75\u1FD0\u1FD1\u1F76\u1F77\u1FE0\u1FE1\u1F7A\u1F7B"
   U"\u1FE5\u1F78\u1F79\u1F7C\u1F7D\u03C9\u006B\u00E5\u214E\u2184\u2C30"
   U"\u2C31\u2C32\u2C33\u2C34\u2C35\u2C36\u2C37\u2C38\u2C39\u2C3A\u2C3B"
   U"\u2C3C\u2C3D\u2C3E\u2C3F\u2C40\u2C41\u2C42\u2C43\u2C44\u2C45\u2C46"
   U"\u2C47\u2C48\u2C49\u2C4A\u2C4B\u2C4C\u2C4D\u2C4E\u2C4F\u2C50\u2C51"
   U"\u2C52\u2C53\u2C54\u2C55\u2C56\u2C57\u2C58\u2C59\u2C5A\u2C5B\u2C5C"
   U"\u2C5D\u2C5E\u2C5F\u2C61\u026B\u1D7D\u027D\u2C68\u2C6A\u2C6C\u0251"
   U"\u0271\u0250\u0252\u2C73\u2C76\u023F\u0240\u2C81\u2C83\u2C85\u2C87"
   U"\u2C89\u2C8B\u2C8D\u2C8F\u2C91\u2C93\u2C95\u2C97\u2C99\u2C9B\u2C9D"
   U"\u2C9F\u2CA1\u2CA3\u2CA5\u2CA7\u2CA9\u2CAB\u2CAD\u2CAF\u2CB1\u2CB3"
   U"\u2CB5\u2CB7\u2CB9\u2CBB\u2CBD\u2CBF\u2CC1\u2CC3\u2CC5\u2CC7\u2CC9"
   U"\u2CCB\u2CCD\u2CCF\u2CD1\u2CD3\u2CD5\u2CD7\u2CD9\u2CDB\u2CDD\u2CDF"
   U"\u2CE1\u2CE3\u2CEC\u2CEE\u2CF3\uA641\uA643\uA645\uA647\uA649\uA64B"
   U"\uA64D\uA64F\uA651\uA653\uA655\uA657\uA659\uA65B\uA65D\uA65F\uA661"
   U"\uA663\uA665\uA667\uA669\uA66B\uA66D\uA681\uA683\uA685\uA687\uA689"
   U"\uA68B\uA68D\uA68F\uA691\uA693\uA695\uA697\uA699\uA69B\uA723\uA725"
   U"\uA727\uA729\uA72B\uA72D\uA72F\uA733\uA735\uA737\uA739\uA73B\uA73D"
   U"\uA73F\uA741\uA743\uA745\uA747\uA749\uA74B\uA74D\uA74F\uA751\uA753"
   U"\uA755\uA757\uA759\uA75B\uA75D\uA75F\uA761\uA763\uA765\uA767\uA769"
   U"\uA76B\uA76D\uA76F\uA77A\uA77C\u1D79\uA77F\uA781\uA783\uA785\uA787"
   U"\uA78C\u0265\uA791\uA793\uA797\uA799\uA79B\uA79D\uA79F\uA7A1\uA7A3"
   U"\uA7A5\uA7A7\uA7A9\u0266\u025C\u0261\u026C\u026A\u029E\u0287\u029D"
   U"\uAB53\uA7B5\uA7B7\uA7B9\uA7BB\uA7BD\uA7BF\uA7C1\uA7C3\uA794\u0282"
   U"\u1D8E\uA7C8\uA7CA\uA7D1\uA7D7\uA7D9\uA7F6\uFF41\uFF42\uFF43\uFF44"
   U"\uFF45\uFF46\uFF47\uFF48\uFF49\uFF4A\uFF4B\uFF4C\uFF4D\uFF4E\uFF4F"
   U"\uFF50\uFF51\uFF52\uFF53\uFF54\uFF55\uFF56\uFF57\uFF58\uFF59\uFF5A"
   U"\U00010428\U00010429\U0001042A\U0001042B\U0001042C\U0001042D\U0001042E"
   U"\U0001042F\U00010430\U00010431\U00010432\U00010433\U00010434\U00010435"
   U"\U00010436\U00010437\U00010438\U00010439\U0001043A\U0001043B\U0001043C"
   U"\U0001043D\U0001043E\U0001043F\U00010440\U00010441\U00010442\U00010443"
   U"\U00010444\U00010445\U00010446\U00010447\U00010448\U00010449\U0001044A"
   U"\U0001044B\U0001044C\U0001044D\U0001044E\U0001044F\U000104D8\U000104D9"
   U"\U000104DA\U000104DB\U000104DC\U000104DD\U000104DE\U000104DF\U000104E0"
   U"\U000104E1\U000104E2\U000104E3\U000104E4\U000104E5\U000104E6\U000104E7"
   U"\U000104E8\U000104E9\U000104EA\U000104EB\U000104EC\U000104ED\U000104EE"
   U"\U000104EF\U000104F0\U000104F1\U000104F2\U000104F3\U000104F4\U000104F5"
   U"\U000104F6\U000104F7\U000104F8\U000104F9\U000104FA\U000104FB\U00010597"
   U"\U00010598\U00010599\U0001059A\U0001059B\U0001059C\U0001059D\U0001059E"
   U"\U0001059F\U000105A0\U000105A1\U000105A3\U000105A4\U000105A5\U000105A6"
   U"\U000105A7\U000105A8\U000105A9\U000105AA\U000105AB\U000105AC\U000105AD"
   U"\U000105AE\U000105AF\U000105B0\U000105B1\U000105B3\U000105B4\U000105B5"
   U"\U000105B6\U000105B7\U000105B8\U000105B9\U000105BB\U000105BC\U00010CC0"
   U"\U00010CC1\U00010CC2\U00010CC3\U00010CC4\U00010CC5\U00010CC6\U00010CC7"
   U"\U00010CC8\U00010CC9\U00010CCA\U00010CCB\U00010CCC\U00010CCD\U00010CCE"
   U"\U00010CCF\U00010CD0\U00010CD1\U00010CD2\U00010CD3\U00010CD4\U00010CD5"
   U"\U00010CD6\U00010CD7\U00010CD8\U00010CD9\U00010CDA\U00010CDB\U00010CDC"
   U"\U00010CDD\U00010CDE\U00010CDF\U00010CE0\U00010CE1\U00010CE2\U00010CE3"
   U"\U00010CE4\U00010CE5\U00010CE6\U00010CE7\U00010CE8\U00010CE9\U00010CEA"
   U"\U00010CEB\U00010CEC\U00010CED\U00010CEE\U00010CEF\U00010CF0\U00010CF1"
   U"\U00010CF2\U000118C0\U000118C1\U000118C2\U000118C3\U000118C4\U000118C5"
   U"\U000118C6\U000118C7\U000118C8\U000118C9\U000118CA\U000118CB\U000118CC"
   U"\U000118CD\U000118CE\U000118CF\U000118D0\U000118D1\U000118D2\U000118D3"
   U"\U000118D4\U000118D5\U000118D6\U000118D7\U000118D8\U000118D9\U000118DA"
   U"\U000118DB\U000118DC\U000118DD\U000118DE\U000118DF\U00016E60\U00016E61"
   U"\U00016E62\U00016E63\U00016E64\U00016E65\U00016E66\U00016E67\U00016E68"
   U"\U00016E69\U00016E6A\U00016E6B\U00016E6C\U00016E6D\U00016E6E\U00016E6F"
   U"\U00016E70\U00016E71\U00016E72\U00016E73\U00016E74\U00016E75\U00016E76"
   U"\U00016E77\U00016E78\U00016E79\U00016E7A\U00016E7B\U00016E7C\U00016E7D"
   U"\U00016E7E\U00016E7F\U0001E922\U0001E923\U0001E924\U0001E925\U0001E926"
   U"\U0001E927\U0001E928\U0001E929\U0001E92A\U0001E92B\U0001E92C\U0001E92D"
   U"\U0001E92E\U0001E92F\U0001E930\U0001E931\U0001E932\U0001E933\U0001E934"
   U"\U0001E935\U0001E936\U0001E937\U0001E938\U0001E939\U0001E93A\U0001E93B"
   U"\U0001E93C\U0001E93D\U0001E93E\U0001E93F\U0001E940\U0001E941\U0001E942"
   U"\U0001E943";

constexpr char32_t grossbuchstaben[] =
   U"\u0041\u0042\u0043\u0044\u0045\u0046\u0047\u0048\u0049\u004A\u004B"
   U"\u004C\u004D\u004E\u004F\u0050\u0051\u0052\u0053\u0054\u0055\u0056"
   U"\u0057\u0058\u0059\u005A\u00C0\u00C1\u00C2\u00C3\u00C4\u00C5\u00C6"
   U"\u00C7\u00C8\u00C9\u00CA\u00CB\u00CC\u00CD\u00CE\u00CF\u00D0\u00D1"
   U"\u00D2\u00D3\u00D4\u00D5\u00D6\u00D8\u00D9\u00DA\u00DB\u00DC\u00DD"
   U"\u00DE\u0100\u0102\u0104\u0106\u0108\u010A\u010C\u010E\u0110\u0112"
   U"\u0114\u0116\u0118\u011A\u011C\u011E\u0120\u0122\u0124\u0126\u0128"
   U"\u012A\u012C\u012E\u0130\u0132\u0134\u0136\u0139\u013B\u013D\u013F"
   U"\u0141\u0143\u0145\u0147\u014A\u014C\u014E\u0150\u0152\u0154\u0156"
   U"\u0158\u015A\u015C\u015E\u0160\u0162\u0164\u0166\u0168\u016A\u016C"
   U"\u016E\u0170\u0172\u0174\u0176\u0178\u0179\u017B\u017D\u0181\u0182"
   U"\u0184\u0186\u0187\u0189\u018A\u018B\u018E\u018F\u0190\u0191\u0193"
   U"\u0194\u0196\u0197\u0198\u019C\u019D\u019F\u01A0\u01A2\u01A4\u01A6"
   U"\u01A7\u01A9\u01AC\u01AE\u01AF\u01B1\u01B2\u01B3\u01B5\u01B7\u01B8"
   U"\u01BC\u01C4\u01C7\u01CA\u01CD\u01CF\u01D1\u01D3\u01D5\u01D7\u01D9"
   U"\u01DB\u01DE\u01E0\u01E2\u01E4\u01E6\u01E8\u01EA\u01EC\u01EE\u01F1"
   U"\u01F4\u01F6\u01F7\u01F8\u01FA\u01FC\u01FE\u0200\u0202\u0204\u0206"
   U"\u0208\u020A\u020C\u020E\u0210\u0212\u0214\u0216\u0218\u021A\u021C"
   U"\u021E\u0220\u0222\u0224\u0226\u0228\u022A\u022C\u022E\u0230\u0232"
   U"\u023A\u023B\u023D\u023E\u0241\u0243\u0244\u0245\u0246\u0248\u024A"
   U"\u024C\u024E\u0370\u0372\u0376\u037F\u0386\u0388\u0389\u038A\u038C"
   U"\u038E\u038F\u0391\u0392\u0393\u0394\u0395\u0396\u0397\u0398\u0399"
   U"\u039A\u039B\u039C\u039D\u039E\u039F\u03A0\u03A1\u03A3\u03A4\u03A5"
   U"\u03A6\u03A7\u03A8\u03A9\u03AA\u03AB\u03CF\u03D8\u03DA\u03DC\u03DE"
   U"\u03E0\u03E2\u03E4\u03E6\u03E8\u03EA\u03EC\u03EE\u03F4\u03F7\u03F9"
   U"\u03FA\u03FD\u03FE\u03FF\u0400\u0401\u0402\u0403\u0404\u0405\u0406"
   U"\u0407\u0408\u0409\u040A\u040B\u040C\u040D\u040E\u040F\u0410\u0411"
   U"\u0412\u0413\u0414\u0415\u0416\u0417\u0418\u0419\u041A\u041B\u041C"
   U"\u041D\u041E\u041F\u0420\u0421\u0422\u0423\u0424\u0425\u0426\u0427"
   U"\u0428\u0429\u042A\u042B\u042C\u042D\u042E\u042F\u0460\u0462\u0464"
   U"\u0466\u0468\u046A\u046C\u046E\u0470\u0472\u0474\u0476\u0478\u047A"
   U"\u047C\u047E\u0480\u048A\u048C\u048E\u0490\u0492\u0494\u0496\u0498"
   U"\u049A\u049C\u049E\u04A0\u04A2\u04A4\u04A6\u04A8\u04AA\u04AC\u04AE"
   U"\u04B0\u04B2\u04B4\u04B6\u04B8\u04BA\u04BC\u04BE\u04C0\u04C1\u04C3"
   U"\u04C5\u04C7\u04C9\u04CB\u04CD\u04D0\u04D2\u04D4\u04D6\u04D8\u04DA"
   U"\u04DC\u04DE\u04E0\u04E2\u04E4\u04E6\u04E8\u04EA\u04EC\u04EE\u04F0"
   U"\u04F2\u04F4\u04F6\u04F8\u04FA\u04FC\u04FE\u0500\u0502\u0504\u0506"
   U"\u0508\u050A\u050C\u050E\u0510\u0512\u0514\u0516\u0518\u051A\u051C"
   U"\u051E\u0520\u0522\u0524\u0526\u0528\u052A\u052C\u052E\u0531\u0532"
   U"\u0533\u0534\u0535\u0536\u0537\u0538\u0539\u053A\u053B\u053C\u053D"
   U"\u053E\u053F\u0540\u0541\u0542\u0543\u0544\u0545\u0546\u0547\u0548"
   U"\u0549\u054A\u054B\u054C\u054D\u054E\u054F\u0550\u0551\u0552\u0553"
   U"\u0554\u0555\u0556\u10A0\u10A1\u10A2\u10A3\u10A4\u10A5\u10A6\u10A7"
   U"\u10A8\u10A9\u10AA\u10AB\u10AC\u10AD\u10AE\u10AF\u10B0\u10B1\u10B2"
   U"\u10B3\u10B4\u10B5\u10B6\u10B7\u10B8\u10B9\u10BA\u10BB\u10BC\u10BD"
   U"\u10BE\u10BF\u10C0\u10C1\u10C2\u10C3\u10C4\u10C5\u10C7\u10CD\u13A0"
   U"\u13A1\u13A2\u13A3\u13A4\u13A5\u13A6\u13A7\u13A8\u13A9\u13AA\u13AB"
   U"\u13AC\u13AD\u13AE\u13AF\u13B0\u13B1\u13B2\u13B3\u13B4\u13B5\u13B6"
   U"\u13B7\u13B8\u13B9\u13BA\u13BB\u13BC\u13BD\u13BE\u13BF\u13C0\u13C1"
   U"\u13C2\u13C3\u13C4\u13C5\u13C6\u13C7\u13C8\u13C9\u13CA\u13CB\u13CC"
   U"\u13CD\u13CE\u13CF\u13D0\u13D1\u13D2\u13D3\u13D4\u13D5\u13D6\u13D7"
   U"\u13D8\u13D9\u13DA\u13DB\u13DC\u13DD\u13DE\u13DF\u13E0\u13E1\u13E2"
   U"\u13E3\u13E4\u13E5\u13E6\u13E7\u13E8\u13E9\u13EA\u13EB\u13EC\u13ED"
   U"\u13EE\u13EF\u13F0\u13F1\u13F2\u13F3\u13F4\u13F5\u1C90\u1C91\u1C92"
   U"\u1C93\u1C94\u1C95\u1C96\u1C97\u1C98\u1C99\u1C9A\u1C9B\u1C9C\u1C9D"
   U"\u1C9E\u1C9F\u1CA0\u1CA1\u1CA2\u1CA3\u1CA4\u1CA5\u1CA6\u1CA7\u1CA8"
   U"\u1CA9\u1CAA\u1CAB\u1CAC\u1CAD\u1CAE\u1CAF\u1CB0\u1CB1\u1CB2\u1CB3"
   U"\u1CB4\u1CB5\u1CB6\u1CB7\u1CB8\u1CB9\u1CBA\u1CBD\u1CBE\u1CBF\u1E00"
   U"\u1E02\u1E04\u1E06\u1E08\u1E0A\u1E0C\u1E0E\u1E10\u1E12\u1E14\u1E16"
   U"\u1E18\u1E1A\u1E1C\u1E1E\u1E20\u1E22\u1E24\u1E26\u1E28\u1E2A\u1E2C"
   U"\u1E2E\u1E30\u1E32\u1E34\u1E36\u1E38\u1E3A\u1E3C\u1E3E\u1E40\u1E42"
   U"\u1E44\u1E46\u1E48\u1E4A\u1E4C\u1E4E\u1E50\u1E52\u1E54\u1E56\u1E58"
   U"\u1E5A\u1E5C\u1E5E\u1E60\u1E62\u1E64\u1E66\u1E68\u1E6A\u1E6C\u1E6E"
   U"\u1E70\u1E72\u1E74\u1E76\u1E78\u1E7A\u1E7C\u1E7E\u1E80\u1E82\u1E84"
   U"\u1E86\u1E88\u1E8A\u1E8C\u1E8E\u1E90\u1E92\u1E94\u1E9E\u1EA0\u1EA2"
   U"\u1EA4\u1EA6\u1EA8\u1EAA\u1EAC\u1EAE\u1EB0\u1EB2\u1EB4\u1EB6\u1EB8"
   U"\u1EBA\u1EBC\u1EBE\u1EC0\u1EC2\u1EC4\u1EC6\u1EC8\u1ECA\u1ECC\u1ECE"
   U"\u1ED0\u1ED2\u1ED4\u1ED6\u1ED8\u1EDA\u1EDC\u1EDE\u1EE0\u1EE2\u1EE4"
   U"\u1EE6\u1EE8\u1EEA\u1EEC\u1EEE\u1EF0\u1EF2\u1EF4\u1EF6\u1EF8\u1EFA"
   U"\u1EFC\u1EFE\u1F08\u1F09\u1F0A\u1F0B\u1F0C\u1F0D\u1F0E\u1F0F\u1F18"
   U"\u1F19\u1F1A\u1F1B\u1F1C\u1F1D\u1F28\u1F29\u1F2A\u1F2B\u1F2C\u1F2D"
   U"\u1F2E\u1F2F\u1F38\u1F39\u1F3A\u1F3B\u1F3C\u1F3D\u1F3E\u1F3F\u1F48"
   U"\u1F49\u1F4A\u1F4B\u1F4C\u1F4D\u1F59\u1F5B\u1F5D\u1F5F\u1F68\u1F69"
   U"\u1F6A\u1F6B\u1F6C\u1F6D\u1F6E\u1F6F\u1FB8\u1FB9\u1FBA\u1FBB\u1FC8"
   U"\u1FC9\u1FCA\u1FCB\u1FD8\u1FD9\u1FDA\u1FDB\u1FE8\u1FE9\u1FEA\u1FEB"
   U"\u1FEC\u1FF8\u1FF9\u1FFA\u1FFB\u2126\u212A\u212B\u2132\u2183\u2C00"
   U"\u2C01\u2C02\u2C03\u2C04\u2C05\u2C06\u2C07\u2C08\u2C09\u2C0A\u2C0B"
   U"\u2C0C\u2C0D\u2C0E\u2C0F\u2C10\u2C11\u2C12\u2C13\u2C14\u2C15\u2C16"
   U"\u2C17\u2C18\u2C19\u2C1A\u2C1B\u2C1C\u2C1D\u2C1E\u2C1F\u2C20\u2C21"
   U"\u2C22\u2C23\u2C24\u2C25\u2C26\u2C27\u2C28\u2C29\u2C2A\u2C2B\u2C2C"
   U"\u2C2D\u2C2E\u2C2F\u2C60\u2C62\u2C63\u2C64\u2C67\u2C69\u2C6B\u2C6D"
   U"\u2C6E\u2C6F\u2C70\u2C72\u2C75\u2C7E\u2C7F\u2C80\u2C82\u2C84\u2C86"
   U"\u2C88\u2C8A\u2C8C\u2C8E\u2C90\u2C92\u2C94\u2C96\u2C98\u2C9A\u2C9C"
   U"\u2C9E\u2CA0\u2CA2\u2CA4\u2CA6\u2CA8\u2CAA\u2CAC\u2CAE\u2CB0\u2CB2"
   U"\u2CB4\u2CB6\u2CB8\u2CBA\u2CBC\u2CBE\u2CC0\u2CC2\u2CC4\u2CC6\u2CC8"
   U"\u2CCA\u2CCC\u2CCE\u2CD0\u2CD2\u2CD4\u2CD6\u2CD8\u2CDA\u2CDC\u2CDE"
   U"\u2CE0\u2CE2\u2CEB\u2CED\u2CF2\uA640\uA642\uA644\uA646\uA648\uA64A"
   U"\uA64C\uA64E\uA650\uA652\uA654\uA656\uA658\uA65A\uA65C\uA65E\uA660"
   U"\uA662\uA664\uA666\uA668\uA66A\uA66C\uA680\uA682\uA684\uA686\uA688"
   U"\uA68A\uA68C\uA68E\uA690\uA692\uA694\uA696\uA698\uA69A\uA722\uA724"
   U"\uA726\uA728\uA72A\uA72C\uA72E\uA732\uA734\uA736\uA738\uA73A\uA73C"
   U"\uA73E\uA740\uA742\uA744\uA746\uA748\uA74A\uA74C\uA74E\uA750\uA752"
   U"\uA754\uA756\uA758\uA75A\uA75C\uA75E\uA760\uA762\uA764\uA766\uA768"
   U"\uA76A\uA76C\uA76E\uA779\uA77B\uA77D\uA77E\uA780\uA782\uA784\uA786"
   U"\uA78B\uA78D\uA790\uA792\uA796\uA798\uA79A\uA79C\uA79E\uA7A0\uA7A2"
   U"\uA7A4\uA7A6\uA7A8\uA7AA\uA7AB\uA7AC\uA7AD\uA7AE\uA7B0\uA7B1\uA7B2"
   U"\uA7B3\uA7B4\uA7B6\uA7B8\uA7BA\uA7BC\uA7BE\uA7C0\uA7C2\uA7C4\uA7C5"
   U"\uA7C6\uA7C7\uA7C9\uA7D0\uA7D6\uA7D8\uA7F5\uFF21\uFF22\uFF23\uFF24"
   U"\uFF25\uFF26\uFF27\uFF28\uFF29\uFF2A\uFF2B\uFF2C\uFF2D\uFF2E\uFF2F"
   U"\uFF30\uFF31\uFF32\uFF33\uFF34\uFF35\uFF36\uFF37\uFF38\uFF39\uFF3A"
   U"\U00010400\U00010401\U00010402\U00010403\U00010404\U00010405\U00010406"
   U"\U00010407\U00010408\U00010409\U0001040A\U0001040B\U0001040C\U0001040D"
   U"\U0001040E\U0001040F\U00010410\U00010411\U00010412\U00010413\U00010414"
   U"\U00010415\U00010416\U00010417\U00010418\U00010419\U0001041A\U0001041B"
   U"\U0001041C\U0001041D\U0001041E\U0001041F\U00010420\U00010421\U00010422"
   U"\U00010423\U00010424\U00010425\U00010426\U00010427\U000104B0\U000104B1"
   U"\U000104B2\U000104B3\U000104B4\U000104B5\U000104B6\U000104B7\U000104B8"
   U"\U000104B9\U000104BA\U000104BB\U000104BC\U000104BD\U000104BE\U000104BF"
   U"\U000104C0\U000104C1\U000104C2\U000104C3\U000104C4\U000104C5\U000104C6"
   U"\U000104C7\U000104C8\U000104C9\U000104CA\U000104CB\U000104CC\U000104CD"
   U"\U000104CE\U000104CF\U000104D0\U000104D1\U000104D2\U000104D3\U00010570"
   U"\U00010571\U00010572\U00010573\U00010574\U00010575\U00010576\U00010577"
   U"\U00010578\U00010579\U0001057A\U0001057C\U0001057D\U0001057E\U0001057F"
   U"\U00010580\U00010581\U00010582\U00010583\U00010584\U00010585\U00010586"
   U"\U00010587\U00010588\U00010589\U0001058A\U0001058C\U0001058D\U0001058E"
   U"\U0001058F\U00010590\U00010591\U00010592\U00010594\U00010595\U00010C80"
   U"\U00010C81\U00010C82\U00010C83\U00010C84\U00010C85\U00010C86\U00010C87"
   U"\U00010C88\U00010C89\U00010C8A\U00010C8B\U00010C8C\U00010C8D\U00010C8E"
   U"\U00010C8F\U00010C90\U00010C91\U00010C92\U00010C93\U00010C94\U00010C95"
   U"\U00010C96\U00010C97\U00010C98\U00010C99\U00010C9A\U00010C9B\U00010C9C"
   U"\U00010C9D\U00010C9E\U00010C9F\U00010CA0\U00010CA1\U00010CA2\U00010CA3"
   U"\U00010CA4\U00010CA5\U00010CA6\U00010CA7\U00010CA8\U00010CA9\U00010CAA"
   U"\U00010CAB\U00010CAC\U00010CAD\U00010CAE\U00010CAF\U00010CB0\U00010CB1"
   U"\U00010CB2\U000118A0\U000118A1\U000118A2\U000118A3\U000118A4\U000118A5"
   U"\U000118A6\U000118A7\U000118A8\U000118A9\U000118AA\U000118AB\U000118AC"
   U"\U000118AD\U000118AE\U000118AF\U000118B0\U000118B1\U000118B2\U000118B3"
   U"\U000118B4\U000118B5\U000118B6\U000118B7\U000118B8\U000118B9\U000118BA"
   U"\U000118BB\U000118BC\U000118BD\U000118BE\U000118BF\U00016E40\U00016E41"
   U"\U00016E42\U00016E43\U00016E44\U00016E45\U00016E46\U00016E47\U00016E48"
   U"\U00016E49\U00016E4A\U00016E4B\U00016E4C\U00016E4D\U00016E4E\U00016E4F"
   U"\U00016E50\U00016E51\U00016E52\U00016E53\U00016E54\U00016E55\U00016E56"
   U"\U00016E57\U00016E58\U00016E59\U00016E5A\U00016E5B\U00016E5C\U00016E5D"
   U"\U00016E5E\U00016E5F\U0001E900\U0001E901\U0001E902\U0001E903\U0001E904"
   U"\U0001E905\U0001E906\U0001E907\U0001E908\U0001E909\U0001E90A\U0001E90B"
   U"\U0001E90C\U0001E90D\U0001E90E\U0001E90F\U0001E910\U0001E911\U0001E912"
   U"\U0001E913\U0001E914\U0001E915\U0001E916\U0001E917\U0001E918\U0001E919"
   U"\U0001E91A\U0001E91B\U0001E91C\U0001E91D\U0001E91E\U0001E91F\U0001E920"
   U"\U0001E921";
}

Unicode::Unicode(){
   for(size_t i = 0; i < sizeof(grossbuchstaben)/sizeof(char32_t)-1; ++i){
      gross_in_klein[grossbuchstaben[i]] = kleinbuchstaben[i];
      buchstaben.insert(grossbuchstaben[i]);
      buchstaben.insert(kleinbuchstaben[i]);
   }
}

char32_t
Unicode::kleinbuchstabe(char32_t c) const {
   const auto i = gross_in_klein.find(c);
   return i == gross_in_klein.end() ? c : i->second;
}
bool
Unicode::ist_buchstabe(char32_t c) const {
   return buchstaben.find(c) != buchstaben.end();
}

//--------------- src/utfhilfe.cc ---------------
//#include "utfhilfe.hh"

#include <vector>

namespace {
   int
   utf8_laenge(const char* s, bool& fehler)
   {
      if(*s == 0) return 0;
      const unsigned c = *s & 0xff;

      if(c < 0x80) return 1;
      if(c >= 0xc2){
         if(c < 0xe0) return 2;
         if(c < 0xf0) return 3;
         if(c < 0xf5) return 4; // nicht alle 4-Zeichen-Sequenzen sind erlaubt.
         // 5- und 6-Zeichen-Sequenzen sind nicht erlaubt.
      }
      fehler = true;
      return 0;
   }

   char*
   utf32_in_utf8_l(char32_t c, char* acc) {
      if(c < 0x80){
         *acc = c & 0x7f;
         return acc;
      }

      char32_t hochwertig = 0x80, add = 0x40;
      do{
         char r = c & 0x3f;
         c = (c-r) >> 6;
         *acc-- = r+0x80;
         hochwertig = hochwertig+add;
         add = add >> 1;
      }while(c >= add);
      *acc = hochwertig+c;
      return acc;
   }

   inline char32_t
   folgezeichen(const char c, bool& fehler){
      const char32_t z = c & 0xff;
      if(z < 0x80 || z >= 0xc0) fehler = true;
      return z;
   }
}

std::string
utf32_in_utf8(char32_t c) {
   char acc[5], *e = acc+4, *p = utf32_in_utf8_l(c, acc+3);
   *e = 0;
   return std::string(p, e-p);
}

std::string
utf32_in_utf8(const std::u32string& s){
   const size_t l = s.length();
   std::vector<char> res(4*l+1);
   char *e = &res[4*l], *p = e;
   *e = 0;
   for(size_t i = l; i-- > 0;) p = utf32_in_utf8_l(s[i], --p);
   return std::string(p, e-p);
}
std::string
utf32_in_utf8(uint64_t u){
   char res[17], *e = res+16, *p = e;
   *e = 0;
   while(u){
      p = utf32_in_utf8_l(u & utf_maske, --p);
      u >>= 21;
   }
   return std::string(p, e-p);
}

char32_t
utf8_in_utf32(const char*& s, bool& fehler){
   if(*s == 0) return 0;
   const int l = utf8_laenge(s, fehler);
   char32_t sum = *s++ & 0xff;
   if(l < 2) return sum;

   sum &= (1 << (7-l))-1;
   for(int i = 1; !fehler && i < l; ++i)
      sum = (sum << 6)+(folgezeichen(*s++, fehler)-0x80);

   return sum;
}

std::u32string zahl_in_utf32(int z){
   const std::string s = std::to_string(z);
   const size_t l = s.length();
   std::u32string u;
   for(size_t i = 0; i < l; ++i) u.push_back(s[i] & 0xff);
   return u;
}


#ifdef AUSGABE_8BIT

std::string
utf32_in_ausgabe(char32_t c){
   char acc[2]; acc[0] = c; acc[1] = 0;
   return std::string(acc, 1);
}

std::string
utf32_in_ausgabe(const std::u32string& s){
   const size_t l = s.length();
   std::string res; res.reserve(l);
   for(size_t i = 0; i < l; ++i) res.push_back(s[i]);
   return res;
}

#else

std::string utf32_in_ausgabe(char32_t c)
{ return utf32_in_utf8(c); }

std::string utf32_in_ausgabe(const std::u32string& s)
{ return utf32_in_utf8(s);}

#endif // !AUSGABE_8BIT

bool ist_ziffer(char32_t c){ return c >= U'0' && c <= U'9'; }
bool ist_zwischenraum(char32_t c){ return c == U' ' || c == U'\t'; }
