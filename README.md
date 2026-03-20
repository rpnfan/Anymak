![image](https://github.com/user-attachments/assets/51167072-4cf3-46ca-9ce0-8ef2ded2a911)
 
**Anymak** is a keyboard layout approach designed to *largely enhance typing comfort*. By rearranging keys and utilizing one-shot modifiers and bottom-row mods, Anymak makes it easier to access common keys like Shift, symbols, and navigation shortcuts, while minimizing finger stretching. A major design goal is that the same comfortable fingering works on both a standard ANSI/ISO keyboard and a split columnar-stagger ergonomic keyboard — so you can use your programmable keyboard at your desk and your laptop on the go, with no meaningful adjustment needed.

Anymak is a versatile concept, compatible to be adapted to any alphanumeric layout. You can continue using QWERTY, Colemak, Graphite, or any other layout you prefer. My own implementation pairs the Anymak layer concept with a fully optimized multi-language alpha layout called "END" — optimized for English, German, and Dutch, with good support for French, Spanish, and Nordic languages. Together these form anymak:END. A ready-to-run Kanata configuration for Windows, Linux, and macOS is available for download here.

## What Makes Anymak Different

Most alternative keyboard layouts — and most layer schemes — solve only part of the problem how to optimize the keyboard experience. They optimize the alpha keys but treat Shift as an afterthought and ignore how symbols are accessed. Often they require a specific key count or thumb arrangement. This most often results in forcing different finger positions when switching between a split ergonomic keyboard and a laptop. Or they used timed methods such as auto-shift, which is convenient but disrupts the typing flow. Last not least they mostly do not take navigation and modifier keys into account. Anymak addresses all of these as a unified system with a small number of targeted changes to any existing alphanumeric layout.

- **No held modifiers for text input**
  
  Shift and the symbol layer are both accessed via dedicated one-shot keys. You tap the layer key, then tap the character — no holding, no timing, no synchronization required. This is where speed matters most, and held modifiers hurt most. One-shot keys are less error-prone, more comfortable, and potentially faster.

- **Navigation always within reach**

  Holding the Space key activates a full navigation and shortcut layer, accessible with either thumb from the natural rest position. No jumping around between small thumb-cluster keys. The layer includes arrow keys, Enter, Backspace, Delete, word-jump, Cut/Copy/Paste, Undo/Redo, tab management, F-keys, and more. Because navigation is deliberate and slower than text typing by nature, the slight cost of a held key is not relevant here.

- **Bottom-row mods that actually work**
   
    Ctrl, Alt, and Win/OS are on the bottom row as hold-keys. Because these are only triggered deliberately — never during fast text input — the timing conflicts that plague home-row mods simply do not arise. Shift is a dedicated key in the same row, making Shift+Ctrl and similar combos straightforward.

- **Identical fingering on any keyboard**
   
   The symmetrical hand position, also on the bottom row (sometimes called angle-mod), and the deliberate exclusion of the standard keyboard B-key position mean that the same comfortable finger positions translate directly between a standard row-stagger, such as on laptop, and a columnar-stagger keyboard. No mental adjustment when switching between them.

- **Works on any keyboard**

   A standard 3×6 layout with one main thumb key per side is sufficient. Works on 4×6 and larger keyboards, split keyboards, and standard ANSI/ISO keyboards. Even 3×5 keyboards can be accommodated with minor adaptation.

- **Holistic alpha layout optimization**
   
    The END alpha layout was developed with Shift and symbol layer key positions included as part of the optimization — something most layout optimizers and layouts do not consider. The result is that the full typing system, including layer access, navigation  and modifier keys from the home-row position, is optimized together rather than in isolation.


## Trying out anymak:END without learning it
There is a cool website which allows you to mimic a new layout by "translating" it to the key positions of your current layout you are using. This allows to get a first idea if you like a specific layout and it might be worth learning.

Testdrive [**anymak:END**](https://keyboard-layout-try-out.pages.dev/?l0r0=q+w+e+r+t++y+u+i+o+p&l0r1=a+s+d+f+g++h+j+k+l+%3B+%27&l0r2=z+x+c+v+b++n+m+%2C+.+%2F&l1r0=q+k+o+u+y++v+d+c+l+f+&l1r1=h+a+e+i+%2C++g+t+r+n+s+j&l1r2=%2F+z+%27+.+x++b+p+m+w+%3B&z=z) here when you use QWERTY or a layout such as Colemak, Graphite or others. Choose 'Current layout' on the left side then! :-)

For QWERTZ users (German layout): [**anymak:END**](https://keyboard-layout-try-out.pages.dev/?l0r0=q+w+e+r+t++z+u+i+o+p+ü&l0r1=a+s+d+f+g++h+j+k+l+ö+ä&l0r2=z+x+c+v+b++n+m+%2C+.+-&l1r0=q+k+o+u+y++v+d+c+l+f+j&l1r1=h+a+e+i+%2C++g+t+r+n+s+&l1r2=%2F+z+%27+.+x++b+p+m+w+%3B&z=z) 


# anymak:END — multi-lingual keyboard layout
END is my alpha layout developed to integrate into the Anymak concept. END stands for the 3 languages it was designed for ⇒ E = English, N = Nederlands, D = Deutsch.

The name END also hints to marking the final destination of [my keyboard layout journey](https://rpnfan.github.io/keyboard-heaven/after-party/keyboard-journey/) :-)

END works well for English-only use. But derivates optimized for other language combinations can be created using the tools and approach outlined in [this article](https://kbd.news/END-my-final-keyboard-layout-2609.html). 

## The four layers of anymak:END
1. Base layer - alphanumeric
2. Shift layer - capitals and a few symbols
3. Symbol layer
4. Navigation and Shortcut layer

 
**Umlauts, Trema and accents** needed for German and Dutch are easily accessible (two keystrokes needed to type *ä*), while French and Spanish are also supported as second priority language. Other **accents**, for example ~ ` ^ are realized as dead keys on the symbol layer (three key strokes needed to type *é*).
  
## Layer overview
A major advantage is that you can use any type of keyboard with anymak:END. The following graphic shows the base layer on a standard ANSI / ISO keyboard and which fingers to use. Note that the bottom left is symmetrical to the right. This is important for the ergonomic use of a standard keyboard. The B-key position (in QWERTY) is not easy to reach with a symmetrical hand position. Therefore this key-position on an ANSI/ ISO keyboard is not used for the layout itself.

<img width="2332" height="1082" alt="image" src="https://github.com/user-attachments/assets/d051405b-d941-4ace-877c-84e794864eb5" />

For better readability the complete layout is presented in the following for a split symmetrical keyboard. But all layers are applied to an ANSI/ ISO keyboard in exactly the same way. Use the above image as a reference which finger to use for traditional keyboards. 

![image](https://github.com/user-attachments/assets/7c6a03a9-2f24-4b0a-9ea6-c7fa49fddeb4)

![image](https://github.com/user-attachments/assets/00decf95-202d-4a7b-ac61-3b326a359a00)

![image](https://github.com/user-attachments/assets/155a3021-b209-4422-9df6-600055914241)

![image](https://github.com/user-attachments/assets/4a2e5386-dffa-4c15-a07b-c6cb917dd96a)


# Numerical evaluation
As you can see below anymak:END is a very balanced layout regarding several criteria. One design goal was to avoid uncomfortable key positions. This results in the lower overall effort than all other layouts. Due less keys being used naturally the number of same-finger-bigrams (SFBs) gets a bit higher on the other side. This is the price to pay for the overall comfort. In my opinion it is worth the price. Especially since the SFBs are not that high. And as important they are largely SFBs of the better type: from strong fingers (index and middle) from the top row back to the home row, where the finger needs to land anyways. This can be nicely seen in the graphics below.

The formatting of the tables in green for 'good' and 'red' for bad is relative within each column and not absolute. That means the color does not necessarily reflect the practical importance of a parameter, but just serves as a relative reference point inside each comparison group.
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
The graphs show that the typing flow is calm, largely on the home row, then on the top row and very few on the bottom row. You see that motions are largely inward rolls and no really ugly finger movements stand out. This is in stark contrast to QWERTY and to a lesser degree also compared to Colemak.

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


# Layout evaluation for 20 layouts and 14 languages
For more languages and layout comparisons download or clone the repo. In the evaluation folder you find the detailed numerical and graphical evaluations for many common or popular layouts. Besides anymak:END you will find evaluations for: AdNW, BEAKL, Bone, Canary, Colemak, Colemak-DH, Dvorak, Engram, Focal, Gallium, Graphite, Hands-Down-Neu, KOU, KOY, Middlemak-NH, Neo, Noted, QWERTY and QWERTZ, and Sturdy.

Each layout is tested for these languages: Czech, Danish, Dutch, English, Finnish, French, German, Hungarian, Italian, Polish, Portuguese, Spanish, Swedish and Turkish. Read the documentation in the evaluation folder and [this article](https://kbd.news/END-my-final-keyboard-layout-2609.html) on kbd.news.


# Practical implementation options
Anymak:END can be used with a programmable keyboard or a software solution. 

In general the layout can be realized by either one of the options below or a combination thereof: 
1. Keyboard (Language) setting in the Operating System
2. Programmable keyboard mapped to the relevant keys and functions (QMK, ZMK, Via, Vial, UHK...)
3. Software key-remapping (Kanata, Autohotkey, ...)

Because I want to use the layout not only on my desktop computer, but also when using my laptop with Windows or Linux I personally use the combination of 1. Keyboard setting to "US International" and 3. the Kanata script available here. This is the recommended approach for most. 

If you are strictly using an external keyboard option 2 might be the best. In the beginning I used the UHK60 keyboard with its remapping software, which was an easy and quick way to develop the layout and make changes as needed.

Option 1 (Keyboard - language setting) alone is not possible. It would first require to create a keyboard layout for the operating system which implements the anymak:END layers. I might provide one for Windows in the future. But this approach can only cover the majority of changes, but would need option 2 or 3 in addition for some features, which are not supported by a plain keyboard layout. Such as one-shot modifiers or hold-keys...

## Using the Kanata script
To be able to test or directly use the layout with any keyboard (even laptops) I offer a Kanata configuration file. Download [Kanata](https://github.com/jtroo/kanata) and put the config file 'kanata.kbd' you download here, in the same folder and run kanata. See here [How to configure Kanata for Windows](https://rpnfan.github.io/keyboard-heaven/how-to/kanata-autostart-windows/). To use the layout activate the "US International" keyboard layout in Windows. Similar on Linux (tested) and macOS (untested by me, but should work similar).

**Adapting Kanata to your personal wishes**

The Kanata config file is commented and should allow you to make adaptions according your needs. Check the Kanata documentation on Github and the discussions there if you need help.

**Potential Cons of using Kanata**

Setting up the Kanata file takes more knowledge (time and reading). But after the script is in place it actually is faster and easier to update then many other options. When you decide to use the anymak:END layout as is, the configuration work is done for you and you just need to download Kanata and place the 'kanata.kbd' file in the same folder and run Kanata. On Windows another downside is that Kanata is not as deeply integrated and some applications might not recognize the remapped keys. This is very seldom a problem in daily use, but especially when defining hotkeys in Windows programs they often check the scan code (key position) and do not get the remapped key code. Interestingly most programs recognize the remapped hotkeys fine, when the program is used. Just the input field to define the hotkey behaves differently. A workaround can be to deactivate Kanata to define the hotkeys and then turn it back on. Finally when Kanata needs to work in a Windows Terminal with admin rights, Kanata needs to run with admin rights as well. 

# Resources
I have published three articles on kbd.news which go into much more detail why I went to search for "the keyboard solution" and what I found was missing and how I finally developed anymak:END. 

[A r-evolutionary approach to improve on the standard keyboard layout](https://kbd.news/A-r-evolutionary-approach-to-improve-on-the-standard-keyboard-layout-2559.html)

[Anymak - the compatible ergonomic keyboard layout](https://kbd.news/Anymak-the-compatible-ergonomic-keyboard-layout-2574.html)

[END - My final keyboard layout](https://kbd.news/END-my-final-keyboard-layout-2609.html)

The content of these articles will be carried over to my website [Keyboard Heaven](https://rpnfan.github.io/keyboard-heaven/) which aims to help you to find the path to your personal *Keyboard Heaven*. There you can discover tips, layouts, and optimization strategies that make typing feel natural, comfortable, and even fun. For the geeks it has some deep dive articles about optimizing keyboard layouts and technical background such as how the windows keyboard chain works.

# Star History
Please star this project if it helped you! Your feedback shows me how many people benefit from these resources, which motivates me making keyboard layout optimization accessible to everyone. :-)

[![Star History Chart](https://api.star-history.com/svg?repos=rpnfan/Anymak&type=Date)](https://star-history.com/#rpnfan/Anymak&Date)



