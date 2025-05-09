![image](https://github.com/user-attachments/assets/51167072-4cf3-46ca-9ce0-8ef2ded2a911)

Testdrive [**anymak:END**](https://keyboard-layout-try-out.pages.dev/?l0r0=q+w+e+r+t++y+u+i+o+p&l0r1=a+s+d+f+g++h+j+k+l+%3B+%27&l0r2=z+x+c+v+b++n+m+%2C+.+%2F&l1r0=q+k+o+u+y++v+d+c+l+f+&l1r1=h+a+e+i+%2C++g+t+r+n+s+j&l1r2=%2F+z+%27+.+x++b+p+m+w+%3B&z=z) here when you use QWERTY or a layout such as Colemak, Graphite or others. Choose 'Current layout' on the left side then! :-)

Probiere [**anymak:END**](https://keyboard-layout-try-out.pages.dev/?l0r0=q+w+e+r+t++z+u+i+o+p+ü&l0r1=a+s+d+f+g++h+j+k+l+ö+ä&l0r2=z+x+c+v+b++n+m+%2C+.+-&l1r0=q+k+o+u+y++v+d+c+l+f+j&l1r1=h+a+e+i+%2C++g+t+r+n+s+&l1r2=%2F+z+%27+.+x++b+p+m+w+%3B&z=z) als QWERTZ-Tipper! :-)
 
*Anymak* is a keyboard layout approach designed to largely enhance typing comfort. By  rearranging keys, and utilizing one-shot modifiers and bottom-row mods, *Anymak* makes it easier to access common keys like Shift, symbols and navigation shortcuts, while minimizing finger stretching. It maintains consistent key positions across both standard and ergonomic split keyboards. As a versatile keymap approach, *Anymak* is compatible with various layouts, allowing you to continue using QWERTY, Colemak, or any other layout you prefer. I myself use the *Anymak* concept with a fully optimized multi-language character layout, specifically designed for English, German, Dutch, and other European languages.  It is named anymak:END and probably also the last layout you might need.

The Anymak concept can be applied to any alphanumeric or language layout. A ready-to-run software implementation in Kanata for anymak:END can be downloaded here and works on Windows, Linux and macOS.

I have published a three part series about my thoughts on keyboard layouts.
1) [Introduction with general remarks about approaching a comfortable keyboard layout](https://kbd.news/A-r-evolutionary-approach-to-improve-on-the-standard-keyboard-layout-2559.html)
2) [Anymak -- the concept for a layerless and compatible key-arrangement](https://kbd.news/Anymak-the-compatible-ergonomic-keyboard-layout-2574.html)
3) [The anymak:END alphanumeric multi-language layout](https://kbd.news/END-my-final-keyboard-layout-2609.html) The last artcile explains how I developed the alphanumeric layout and how you can optimize a layout yourself to customize it to different languages or your personal wishes.


These articles will likely help you to choose or create your own personal "best" solution.

# anymak:END — multi-lingual keyboard layout
END stands for ⇒ E = English, N = Nederlands, D = Deutsch

It also marks the final destination of my keyboard layout journey :-)

 
 Umlauts, Trema and accents needed for German and Dutch are easily accesible (only 2 keystrokes needed), 
    while French and Spanish are also supported as second priority language,
    accents  ~ ` ^ realized as dead keys on symbol layer (3 key strokes needed).
  
 Anymak includes two additional non-standard layers:
   - Navigation layer on the space-key (space-fn concept), including shortcuts like copy/ paste and
     easy to reach positions for Enter, Backspace, Delete, Insert and more
   - an extra character layer for symbols and umlauts and more
 
Can be used with a programmable keyboard or a software solution. To be able to test or directly use the layout with any keyboard (even laptops) I offer a Kanata configuration file. Just extract [kanata](https://github.com/jtroo/kanata) (kanata_gui for Windows) and the config file 'kanata.kbd' you download here, in the same folder and run kanata. To use the layout activate the "US International" keyboard layout in Windows. (Linux and MacOS should work the same, but not tested, let me know if that works without changes).

![image](https://github.com/user-attachments/assets/7c6a03a9-2f24-4b0a-9ea6-c7fa49fddeb4)

![image](https://github.com/user-attachments/assets/00decf95-202d-4a7b-ac61-3b326a359a00)

![image](https://github.com/user-attachments/assets/155a3021-b209-4422-9df6-600055914241)

![image](https://github.com/user-attachments/assets/4a2e5386-dffa-4c15-a07b-c6cb917dd96a)

# Numerical evaluation
As you can see below anymak:END is a very balanced layout regarding several criteria. One design goal was to avoid uncomfortable key positions. This results in the lower overall effort than all other layouts. Due less keys being used naturally the number of same-finger-bigrams (SFBs) gets a bit higher on the other side. This is the price to pay for the overall comfort. In my opinion it is worth the price. Especially since the SFBs are not that high. And as important they are largely SFBs of the better type: from strong fingers (index and middle) from the top row back to the home row, where the finger needs to land anyways. This can be nicely seen in the graphics below.

The formatting of the tables in green for 'good' and 'red' for bad is automatically adjusted to each column of the table. The color does not necessarily reflect the practical importance of a parameter, but just serves as a relative reference point inside each comparison group.
## English
![image](https://github.com/user-attachments/assets/e6c66680-9889-48ae-8a97-bc89f4e4d502)
## German
![image](https://github.com/user-attachments/assets/9c9f1338-d214-498f-9a64-49e182100768)
## Dutch
![image](https://github.com/user-attachments/assets/66581f02-577d-4837-9144-8bd8fcf341ff)
## Spanish
![image](https://github.com/user-attachments/assets/ce2ead3b-0961-4300-b966-ba6e68ecc5ae)
## French
![image](https://github.com/user-attachments/assets/6fb36121-1e45-4d50-b36d-65d9f41c2d52)

# Graphical evaluation
The graphics not only show the movements of fingers on one hand, but also gives numbers of the frequency of the used keys, row jumps and so on. A line bowed to the top means inward movement of fingers. A line bowed to the bottom represents outward movement. The color coding of the graphic is as follows:

- pink: same finger bigram
    
- purple: neighbor finger
    
- light blue: finger skip - inwards movement (line to the top, start finger position in lighter color)
    
- dark blue: finger skip - outwards movement (line to the bottom)

## anymak:END
The graphs show that the typing flow is calm, largely on the home row, then on the top row and very few on the bottow row. You see that motions are largely inward rolls and no really ugly finger movements stand out. This is in stark contrast to QWERTY and to a lesser degree also compared to Colemak.

![image](https://github.com/user-attachments/assets/3b1d9df7-627a-4631-9995-d364c7cda539)

![image](https://github.com/user-attachments/assets/92c9a60e-8ec3-48b9-9224-140a456e5f24)

![image](https://github.com/user-attachments/assets/e2ce2194-eca1-4bd8-a3ac-234783f97589)

![image](https://github.com/user-attachments/assets/421a71c3-28c6-4e07-89e3-76b119f1cbd1)

## Common layouts for comparison
![image](https://github.com/user-attachments/assets/eacb8f29-38ff-4086-a1ad-085c9288cb4b)

![image](https://github.com/user-attachments/assets/3b6db9cb-1812-464c-aafc-fe0ea4f5a69e)

![image](https://github.com/user-attachments/assets/5d9606f9-29b0-430a-996f-aa1c4b1eb71c)

![image](https://github.com/user-attachments/assets/5d9c559b-19eb-4c44-8fd9-111545427a4f)

![image](https://github.com/user-attachments/assets/ab6ea731-0e27-4054-bf09-f6ef24659e6e)

![image](https://github.com/user-attachments/assets/2ab105a0-5a6b-481b-8ace-617766227ab8)

![image](https://github.com/user-attachments/assets/4685b817-4337-4e59-80b8-836c9f8a2856)


For more languages and layout comparisons download or clone the repo.

# Layout evaluation for 20 layouts and 14 languages
In the evaluation folder you find the detailed numerical and grahpical evalutionsa for many common or popular layouts. Besides anymak:END you will find evaluations for: AdNW, BEAKL, Bone, Canary, Colemak, Colemak-DH, Dvorak, Engram, Focal, Gallium, Graphite, Hands-Down-Neu, KOU, KOY, Middlemak-NH, Neo, Noted, QWERTY and QWERTZ, and Sturdy.

Each layout is tested for these languages: Czech, Danish, Dutch, English, Finnish, French, German, Hungarian, Italian, Polish, Portuguese, Spanish, Swedish and Turkish. Read the documentation in the evaluation folder and the third article (about anymak:END) on kbd.news.

# Star History
Was it worth not only to develop the layout for myself but spending many hours and days to document my approach and trying to make it accessible for others? You decide. Leaving a short "thanks" or starring the project will give me some feedbak. :)

[![Star History Chart](https://api.star-history.com/svg?repos=rpnfan/Anymak&type=Date)](https://star-history.com/#rpnfan/Anymak&Date)



