
![image](https://github.com/user-attachments/assets/51167072-4cf3-46ca-9ce0-8ef2ded2a911)

| **Article Overview** |  |
|------|---------|
| 🖮 **Core Concept** | What makes Anymak different |
| ⏱️ **Quick Start** | Trying anymak:END without learning |
| 🗺️ **Layers** | Layer overview |
| 📊 **Analysis** | Numerical/graphical evaluation |
| ⚙️ **Setup** | Implementation Options |
| 🔗 **More** | Resources & Related projects |


**Anymak** is a keyboard layout approach designed to significantly enhance typing comfort and efficiency. By rearranging keys and utilizing one-shot layer access and bottom-row mods, Anymak makes it easier to access common keys like Shift, symbols, and navigation shortcuts, while minimizing finger stretching. A major design goal is that the same comfortable fingering works on both a standard ANSI/ISO keyboard and a split columnar-stagger ergonomic keyboard — so you can use your programmable keyboard at your desk and your laptop on the go, with no meaningful adjustment needed.


Anymak is a versatile concept, compatible to be adapted to any alphanumeric layout. You can continue using QWERTY, Colemak, Graphite, or any other layout you prefer. My own implementation pairs the Anymak layer concept with a fully optimized multi-language alpha layout called "END" — optimized for English, German, and Dutch, with good support for French, Spanish, and Nordic languages. Together these form anymak:END. A ready-to-run Kanata configuration for Windows, Linux, and macOS is available for download here.

## What Makes Anymak Different

Most alternative layouts optimize only the alpha keys — treating Shift, symbols, modifiers, and navigation as afterthoughts, if at all. Anymak addresses all of these as a unified system.


<img width="1500" height="1233" alt="anymak-concept_what_others_miss" src="https://github.com/user-attachments/assets/21381176-2fcf-42b6-b2de-806b11ae1e6e" />



The following six design decisions characterize the holistic Anymak concept:

- **No held modifiers for text input**
  
  Shift and the symbol layer are both accessed via dedicated one-shot keys. You tap the layer key, then tap the character — no holding or hand synchronization is required. Text input is where speed really matters, and held modifiers hurt most. Using one-shot layer switching is less error-prone, more comfortable, and is a little bit faster even. To work seamlessly it is important that both Shift and Symbol layer keys are in easy to reach positions on the keyboard, which Anymak takes care of.

> Note: In contrast timed approaches such as auto-shift might seem a good solution and indeed they are comfortable to use. But they disrupt the typing flow, especially for fast typists, and they must be tuned per person and per typing speed.

- **Navigation always within reach**

  Holding the Space key activates a full navigation and shortcut layer, accessible with either thumb from the natural rest position. No jumping around between small thumb-cluster keys. The layer includes arrow keys, Enter, Backspace, Delete, word-jump, Cut/Copy/Paste, Undo/Redo, tab management, Alt-tab, F-keys, and more. Because navigation is deliberate and slower than text typing by nature, the slight cost of a held key is not relevant here.
  
   A single wide Space bar — present on every keyboard from laptop to split ergonomic — is all that is needed. This avoids overloading the thumb with multiple small 1u keys, which has [well-documented ergonomic risks](https://getreuer.info/posts/keyboards/thumb-ergo/index.html).

- **Modifier and layer keys always on the opposite hand**
  
   Shift, the symbol layer key, and the navigation layer on the Space key are each available symmetrically on both sides. This means the modifier key is always pressed by the opposite hand — maximizing hand alternation, eliminating same-hand conflicts, and giving the other hand time to get into position for the next keystroke.


- **Bottom-row mods that actually work**
   
    Ctrl, Alt, and Win/OS are on the bottom row as hold-keys. Because these are only triggered deliberately — never during fast text input — the timing conflicts that plague home-row mods simply do not arise. Shift is a dedicated key in the same row, making Shift+Ctrl and similar combos straightforward.

- **Identical fingering on any keyboard**

   The default layout is designed for a standard 4×6 layout with one main thumb key per side being sufficient; the concept also works on standard ANSI/ISO keyboards, and can be adapted to 3x6 or 3×5 with minor changes. The symmetrical hand position on the bottom row and the deliberate exclusion on a standard keyboard of the B-key position (QWERTY location) mean that the same comfortable finger positions translate directly between a standard row-stagger keyboard and a columnar-stagger keyboard — without any mental adjustment when switching between laptop and desk setup. 

- **Holistic alpha layout optimization**
   
    The END alpha layout was developed with Shift and symbol layer key positions included as part of the optimization — something most layout optimizers and layouts do not consider. The result is that the full typing system, including layer access, navigation  and modifier keys from the home-row position, is optimized together rather than in isolation.


# anymak:END — Multi-Lingual Keyboard Layout
END is my alpha layout developed to integrate into the Anymak concept. END stands for the 3 languages it was designed for ⇒ E = English, N = Nederlands, D = Deutsch.

The name END also hints to marking the final destination of [my keyboard layout journey](https://rpnfan.github.io/keyboard-heaven/after-party/keyboard-journey/) :-)

**Note:** END works well for English-only use. But derivatives optimized for other language combinations can be created using the tools and approach outlined in [this article](https://kbd.news/END-my-final-keyboard-layout-2609.html). 

## What makes anymak:END feel good to type
To assure foremost a comfortable typing feel, rather optimizing for analyzers metrics in the first place, the layout was designed with the following in mind:
- Put most work on the strong index and middle finger.
- Very high amount of inward rolls in combination with many hand alternations.
> Most layouts optimise strongly for alternations or rolls; achieving a high level of both simultaneously is uncommon and contributes to a fluid typing feel at speed.
- Very low number of redirects and scissors, which reduce the awkward direction changes and uncomfortable cross-finger movements that cause fatigue over long sessions.

## Trying Out anymak:END Without Learning It

You can test anymak:END interactively by mapping it to your current keyboard layout. This helps you get a feel for the layout before committing to learning it.

Testdrive [**anymak:END**](https://keyboard-layout-try-out.pages.dev/?l0r0=q+w+e+r+t++y+u+i+o+p&l0r1=a+s+d+f+g++h+j+k+l+%3B+%27&l0r2=z+x+c+v+b++n+m+%2C+.+%2F&l1r0=q+k+o+u+y++v+d+c+l+f+&l1r1=h+a+e+i+%2C++g+t+r+n+s+j&l1r2=%2F+z+%27+.+x++b+p+m+w+%3B&z=z) here when you use QWERTY or a layout such as Colemak, Graphite or others. Choose 'Current layout' on the left side then! :-)

For QWERTZ users (German layout): [**anymak:END**](https://keyboard-layout-try-out.pages.dev/?l0r0=q+w+e+r+t++z+u+i+o+p+ü&l0r1=a+s+d+f+g++h+j+k+l+ö+ä&l0r2=z+x+c+v+b++n+m+%2C+.+-&l1r0=q+k+o+u+y++v+d+c+l+f+j&l1r1=h+a+e+i+%2C++g+t+r+n+s+&l1r2=%2F+z+%27+.+x++b+p+m+w+%3B&z=z) 

For a more systematic evaluation, a [word list and scoring spreadsheet](https://github.com/rpnfan/KeyDuel) can help you compare anymak:END against your current layout by rating how each word feels to type — a method that works for any two layouts you want to compare.


## The Four Layers of anymak:END

| Layer | Purpose | Access Method |
|-------|---------|---------------|
| Base | Alphanumeric input | Direct key press |
| Shift | Capital letters and common symbols | One-shot key |
| Symbol | Extended symbols and dead keys | One-shot key |
| Navigation | Arrows, shortcuts, F-keys, editing | Hold Space |

**Diacritics:** Umlauts, trema, and accents needed for German and Dutch are easily accessible (two keystrokes for `ä`). French and Spanish are supported as secondary languages. Other accents (`ñ`, `~`, `'`, `^`) are available as dead keys on the symbol layer (three keystrokes for `é`).


## Layer Visualizations

The layout works identically on both standard ANSI / ISO keyboards and split columnar-stagger keyboards — only the physical key positions differ, not the fingering. Here an overview of the base layer on standard keyboard:

<img width="2332" height="1082" alt="image" src="https://github.com/user-attachments/assets/d051405b-d941-4ace-877c-84e794864eb5" />

*Note the symmetrical bottom-row positioning. The B-key position is intentionally unused to maintain the same comfortable finger placement on any keyboard type.*

**For clarity, the complete layer set is shown on a split keyboard layout below:**

![image](https://github.com/user-attachments/assets/7c6a03a9-2f24-4b0a-9ea6-c7fa49fddeb4)

![image](https://github.com/user-attachments/assets/00decf95-202d-4a7b-ac61-3b326a359a00)

![image](https://github.com/user-attachments/assets/155a3021-b209-4422-9df6-600055914241)

![image](https://github.com/user-attachments/assets/4a2e5386-dffa-4c15-a07b-c6cb917dd96a)


# Numerical Evaluation

The metrics below show anymak:END is a well-balanced layout across multiple criteria. A key design goal was avoiding uncomfortable key positions, resulting in lower overall effort than comparable layouts.

**Trade-off:** Due to fewer keys being used, the same-finger bigram (SFB) count is slightly higher. However, these are predominantly favorable SFBs: strong fingers (index and middle) moving from top row back to home row — where the finger needs to land anyway.

**Advantages not shown in the nubers:** Analyzers do not credit the easy-to-reach one-shot shift and symbol layer keys — meaning anymak:END's real world effort is lower than the numbers show. That is compared to layout implementations where Shift is not as comfortable to reach. Also the navigation, shortcut and editing layer is a major benefit in everyday use, but not taken into account by any analyzer.

> **Note:** Color coding (green/red) is relative within each column, not absolute. Colors indicate ranking within the comparison group, not necessarily practical significance.

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

# Graphical Evaluation
The graphics not only show the movements of fingers on one hand, but also gives numbers of the frequency of the used keys, row jumps and so on. A line bowed to the top means inward movement of fingers. A line bowed to the bottom represents outward movement. The color coding of the graphic is as follows:

- **Pink:** Same-finger bigram
- **Purple:** Neighbor-finger bigram
- **Light blue:** Finger skip — inward movement (line curves upward; starting position shown in lighter color)
- **Dark blue:** Finger skip — outward movement (line curves downward)

## anymak:END
The graphs show that the typing flow is calm, largely on the home row, then on the top row and very few on the bottom row. You see that motions are largely inward rolls and no really ugly finger movements stand out. This is in stark contrast to QWERTY and to a lesser degree also compared to Colemak.

![image](https://github.com/user-attachments/assets/3b1d9df7-627a-4631-9995-d364c7cda539)

![image](https://github.com/user-attachments/assets/92c9a60e-8ec3-48b9-9224-140a456e5f24)

![image](https://github.com/user-attachments/assets/e2ce2194-eca1-4bd8-a3ac-234783f97589)

![image](https://github.com/user-attachments/assets/421a71c3-28c6-4e07-89e3-76b119f1cbd1)

## Common Layouts for Comparison
All layouts are shown with symmetrical fingering. Traditional fingering would slightly look different for some layouts. 

![image](https://github.com/user-attachments/assets/eacb8f29-38ff-4086-a1ad-085c9288cb4b)

![image](https://github.com/user-attachments/assets/3b6db9cb-1812-464c-aafc-fe0ea4f5a69e)

![image](https://github.com/user-attachments/assets/5d9606f9-29b0-430a-996f-aa1c4b1eb71c)

![image](https://github.com/user-attachments/assets/5d9c559b-19eb-4c44-8fd9-111545427a4f)

![image](https://github.com/user-attachments/assets/ab6ea731-0e27-4054-bf09-f6ef24659e6e)

![image](https://github.com/user-attachments/assets/2ab105a0-5a6b-481b-8ace-617766227ab8)

![image](https://github.com/user-attachments/assets/4685b817-4337-4e59-80b8-836c9f8a2856)


# Layout Evaluation for 20 Layouts and 14 Languages
For more languages and layout comparisons download or clone the repo. In the evaluation folder you find the detailed numerical and graphical evaluations for many common or popular layouts. Besides anymak:END you will find evaluations for: AdNW, BEAKL, Bone, Canary, Colemak, Colemak-DH, Dvorak, Engram, Focal, Gallium, Graphite, Hands-Down-Neu, KOU, KOY, Middlemak-NH, Neo, Noted, QWERTY and QWERTZ, and Sturdy.

Each layout is tested for these languages: Czech, Danish, Dutch, English, Finnish, French, German, Hungarian, Italian, Polish, Portuguese, Spanish, Swedish and Turkish. Read the documentation in the evaluation folder and [this article](https://kbd.news/END-my-final-keyboard-layout-2609.html) on kbd.news.



# Implementation Options

A custom keyboard layout like anymak:END can be implemented through:

| Option | What is needed | Advantage | Disadvantage |
|--------|-------------|----------|---------|
|1. **Keyboard (Language) setting in the Operating System** | Software [3] | Works consistently | Only alpha and symbol layer [1] |
|2. **Programmable Keyboard** | QMK, ZMK, Via, Vial, UHK ... | Works consistently | Extra keyboard needed|
|3. **Software Remapping** | Kanata, AutoHotkey ... | Works with laptop or any keyboard |Cases where it does not work [2] |

[1] An operating system wide keyboard (language) layout does not to support one-shot-keys, bottom-row-mods, tap-dance (held layer keys). Therefore this solution is limited and would need to be completed by a software remapping option.

[2] Known Limitations of LLHook based software remapping (Kanata and similar) on Windows

- Some applications may not recognize remapped keys for hotkey configuration (might work fine during normal use). Workaround: Temporarily disable Kanata when defining application hotkeys, then re-enable.
- Kanata requires admin rights to function in Windows Terminal with admin privileges

[3] Keyboard layout editor for Windows: [kbdedit](http://kbdedit.com/)

## Recommendation 1: Kanata + US International

For many users combining US International keyboard setting with the provided Kanata configuration is a great option. This works on Windows, Linux, and macOS with any keyboard. It is also a good way to quickly test and fine-tune layouts, without the need to update keyboard firmware. The Kanata config file is commented and should allow you to make adaptations according to your needs. Check the Kanata documentation on Github and the discussions there if you need help.

**Setup:**
1. Download [Kanata](https://github.com/jtroo/kanata)
2. Download the `kanata.kbd` config file from [Releases](../../releases)
3. Place both in the same folder and run Kanata
4. Activate "US International" keyboard layout in your OS

**Windows Setup Guide:** [Configure Kanata for Autostart](https://rpnfan.github.io/keyboard-heaven/how-to/kanata-autostart-windows/)

## Recommendation 2: Programmable Keyboard

In general using a programmable keyboard is the best option, if you have a chance to use one. This works reliable in all use cases, because no remapping in software is needed and the keyboard directly sends the correct wanted scancodes to the computer. 

There are just two drawbacks. First, programmable keyboards can be pricey and you cannot use a laptop or another keyboard instead. Second, you need to be able to configure the keyboard. That requires some knowledge on your side and also that the keyboard firmware supports all needed features. Some keyboards, for example the ones from ZSA, do not support all features at all or in an easy to implement way. ZSA's Oryx does not allow shift-override, which is a feature you will need to implement anymak:END. You will have to compile a QMK firmware instead to realize that.


# Resources
I have published three articles on kbd.news which go into much more detail why I went to search for "the keyboard solution" and what I found was missing and how I finally developed anymak:END. 

[A r-evolutionary approach to improve on the standard keyboard layout](https://kbd.news/A-r-evolutionary-approach-to-improve-on-the-standard-keyboard-layout-2559.html)

[Anymak - the compatible ergonomic keyboard layout](https://kbd.news/Anymak-the-compatible-ergonomic-keyboard-layout-2574.html)

[END - My final keyboard layout](https://kbd.news/END-my-final-keyboard-layout-2609.html)

The content of these articles will be carried over to my website [Keyboard Heaven](https://rpnfan.github.io/keyboard-heaven/) which aims to help you to find the path to your personal *Keyboard Heaven*. There you can discover tips, layouts, and optimization strategies that make typing feel natural, comfortable, and even fun. For the geeks it has some deep dive articles about optimizing keyboard layouts and technical background such as how the windows keyboard chain works.

# Related projects
If the full anymak:END implementation isn't exactly what you're looking for, these alternatives from the same ecosystem might fit better:

- [Spacemak](https://github.com/rpnfan/Spacemak/) -  A lighter alternative to Anymak for those who want to keep their existing alphanumeric layout unchanged. Spacemak adds the SpaceFN navigation layer, bottom-row-mods and a CapsLock-based symbol layer without requiring any changes to the alpha layout. A good entry point if you want most of the navigation and shortcut benefits with minimal disruption to your current setup.

- [EnDeu](https://github.com/rpnfan/EnDeu) -  An alternative alpha layout optimized for English and German. It uses essentially the same alpha layout as anymak:END, but retains the traditional B-key and Shift-key positions and includes German diacritics on the base layer. Derivatives for French, Dutch, and others are in progress there too.

# Star History
Please star this project if it helped you! Your feedback shows me how many people benefit from these resources, which motivates me making keyboard layout optimization accessible to everyone. :-)

<a href="https://www.star-history.com/?repos=rpnfan%2FAnymak&type=date&legend=top-left">
 <picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://api.star-history.com/image?repos=rpnfan/Anymak&type=date&theme=dark&legend=top-left" />
   <source media="(prefers-color-scheme: light)" srcset="https://api.star-history.com/image?repos=rpnfan/Anymak&type=date&legend=top-left" />
   <img alt="Star History Chart" src="https://api.star-history.com/image?repos=rpnfan/Anymak&type=date&legend=top-left" />
 </picture>
</a>



