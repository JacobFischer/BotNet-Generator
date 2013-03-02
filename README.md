#BotNet-Generator

This is a simple c++ program that will generate 100 "BotNet" virus sprites. This was the (basic) way that I generated random but persistent sprites for each team in ACM SIG-GAME's MegaMinerAI 8: BotNet game.

## How does it work?

Basically, there is a database with "parts" of a virus. It randomly pairs the parts up, and then outputs an image of that virus to be used. In our actual implimentation in MegaMinerAI 8: BotNet we actually built the sprite in OpenGL instead of saving an image.

## Compiling

'''g++  -O2 -o example main.cpp -lsqlite3  `Magick++-config --cppflags --cxxflags --ldflags --libs`

You'll need sqlite3 and Image Magick libraries to use this.