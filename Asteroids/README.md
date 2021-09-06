# Asteroids!

Copyright (c) 2019-2021 Keith Dobbelaere. keith77mn77@gmail.com

## About the Game
___

I got the inspiration for this game from a YouTuber named [FamTrinli](https://www.youtube.com/user/FamTrinli/about).
He created a quick Asteroids! clone as part of the series
'Let's Make 16 Classic Games in C++/SFML!' and I thought I could continue the concept a bit further.  So here it is.  I've added quite a bit; added game-play features, a state machine for switching between menus, a High Score manager complete with security features (don't cheat, lol.)  It also has a demo mode that loads when the title screen is left on for more than 20 seconds, complete with a little bit of AI I wrote, sort of like an arcade.

## Controls
___

The controls are pretty basic, and can be remapped in the Custom Controls Menu:

* Left/A - Rotate Left
* Right/D - Rotate Right
* Up/W - Thrust forward
* Down/S - Reverse direction
* Space - Fire weapon
* Shift - Fire Special

## Install
___

Just unzip the file [Asteroids.zip](https://drive.google.com/drive/folders/1MMj2JpFcc9Lobh-euJyEXehgp8LgyUeD?usp=sharing), and run setup.exe.

## Credits
___

### Code

This game was written in C++ using [SFML](https://www.sfml-dev.org/).  These are very simple to use, multi-platform, OpenGL libraries for windows, graphics, audio, networking, etc.
If you want to get something up and running quickly, I recommend them.

To learn more about it, check out: 

  * The [official tutorials](https://www.sfml-dev.org/tutorials/)
  * The [online API documentation](https://www.sfml-dev.org/documentation/)
  * The [community wiki](https://github.com/SFML/SFML/wiki/)
  * The [community forum](https://en.sfml-dev.org/forums/) ([French](https://fr.sfml-dev.org/forums/))

Original inspiration from [FamTrinli](https://www.youtube.com/user/FamTrinli/about) on YouTube, thank you.

I used an audio fader routine by [Glocke](https://github.com/cgloeckner/), from his SFMLExt project, so thanks go out to him.

I used routines for creating SHA-256 hashes by [Olivier Gay](http://www.ouah.org/ogay/sha2/).

### Audio

Main song is '[Nearly There..](https://www.youtube.com/watch?v=eclMFa0mD1c&t=5s)' originally by [Jochen Hippel](https://en.wikipedia.org/wiki/Jochen_Hippel) for the Atari ST version of the game 'A Prehistoric Tale' from 1990, remake by Jogeir Liljedahl (1991).

Title music is '[Stardust Memories](https://www.youtube.com/watch?v=GLMhBE99byM)' by [Jester/Sanity](https://modarchive.org/index.php?request=view_profile&query=69138) (1992).

_Ah, the days of Mod Tracker music..._

### Font

Barcade by Daniel Zadorozny, [Iconian Fonts](http://www.iconian.com/).