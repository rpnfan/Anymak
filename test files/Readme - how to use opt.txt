Short instructions how to use the optimizer on Windows, with the supplied binaries.

Binaries are named accordingly the number of keys to be used. 

Example: 
opt33.exe - compiled for 33 keys to test, German version (output)
opte33.exe - dto, English version
... 
opte35.exe - compiled for 35 keys, English version
... and so on for the different versions

See "Anleitung Manual opt.pdf" for the full documentation (German and English).

In general follow these steps:

1. Unzip all files to a folder.

2. Open the terminal in that folder.

3. Set the encoding correct to UTF-8

on Windows:
chcp 65001

4. Run the optimizer, the results are saved in the file you specify at the end via >

On Windows you can use the supplied Powershell scripts to automate the testing.

a) Using the standard layout config file: Here an example for English. Change the file name of the language file accordingly for other languages, see the relevant language file names. You can run the optimizer for several languages at the same time (with weighting), see the documentation. 

opte35.exe -3 englisch.txt -r Layout_symmetrical.txt -b 10 -b 10 -b 10 -g symmetrical_english.ps -K symmetrical.cfg  >symmetrical_english.txt

b) Using the anymak-layer concept config file, here an example for three languages weighted a third each: 

opt33.exe -3 english.txt -3 deutsch.txt -3 dutch.txt -r Layout_anymak.txt -g anymak_en-nl-de.ps -K anymak.cfg >anymak_en-nl-de.txt

5. Convert the postscript output to PDF files, Ghostscript needs to be installed and in the path.

ps2pdf -dALLOWPSTRANSPARENCY symmetrical_english.ps symmetrical_english.pdf

ps2pdf -dALLOWPSTRANSPARENCY anymak_en-nl-de.ps anymak_en-nl-de.pdf

6. Output are text and PDF files with the numerical and graphical evaluation.

---
Note: You can adjust the config files according to your needs. See documentation.

 ======== adding description to the PDFs ============
 optional, before you convert the postscript PS files to PDF you can add a description to be displayed on the left lower bottom of each keymap
 
 Open the PS file and change the lines which look like
 
 %%Page: 1 1
(KUÜ.ÄVGCLJFHIEAODTRNSßXYÖ,QBPWMZ)[](K.O,YVGCLßZHAEIUDTRNSFXQÄÜÖBPWMJ)[]A
 
to
 
%%Page: 1 1
(KUÜ.ÄVGCLJFHIEAODTRNSßXYÖ,QBPWMZ)[(Aus der Neo-Welt)(Andreas Wettstein)]
(K.O,YVGCLßZHAEIUDTRNSFXQÄÜÖBPWMJ)[(KOY)(Ulf Bro)]A
 
Size and position of the text is defined in the preample of the file:
 
/beschrx 0.3 def      % X-Position description
/beschry -0.7 def     % X-Position  description
/beschrgr 0.7 def     % Size description (0.7 is a good starting point for a large one-line description)