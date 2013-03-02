#include <stdio.h>
#include <sqlite3.h>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <Magick++.h>

#define NUMBER_OF_COLUMNS 18
#define LENGTH 16

using namespace std;
using namespace Magick;

// COMPILE: g++  -O2 -o example main.cpp -lsqlite3  `Magick++-config --cppflags --cxxflags --ldflags --libs`

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    for(int i = 0; i < argc; i++)
    {
        cout << azColName[i] << ": " << argv[i] << endl; 
    }
    
    cout << endl << endl;
    
    return 0;
}

static int callbackPlace(void *voidPixels, int agc, char **argv, char **azColName, int xOffset, int yOffset)
{
    bool **pixels = (bool**)voidPixels;

    for(int row = 0; row < NUMBER_OF_COLUMNS; row++)
    {
        if(azColName[row][0] == 'p') // if it starts with "p" (which will be pixel_x_y)
        {
            pixels[xOffset + (azColName[row][6] - 48)][yOffset + (azColName[row][8] - 48)] = (bool)(argv[row][0] - 48);
        }
    }
}

static int callbackFace(void *voidPixels, int argc, char **argv, char **azColName)
{
    callbackPlace(voidPixels, argc, argv, azColName, 4, 4);
    return 0;
}

static int callbackBody(void *voidPixels, int argc, char **argv, char **azColName)
{
    callbackPlace(voidPixels, argc, argv, azColName, 4, 8);
    return 0;
}

static int callbackArm(void *voidPixels, int argc, char **argv, char **azColName)
{
    callbackPlace(voidPixels, argc, argv, azColName, 0, 4);
    return 0;
}

static int callbackHand(void *voidPixels, int argc, char **argv, char **azColName)
{
    callbackPlace(voidPixels, argc, argv, azColName, 0, 0);
    return 0;
}

static int callbackHair(void *voidPixels, int argc, char **argv, char **azColName)
{
    callbackPlace(voidPixels, argc, argv, azColName, 4, 0);
    return 0;
}

static int callbackLeg(void *voidPixels, int argc, char **argv, char **azColName)
{
    callbackPlace(voidPixels, argc, argv, azColName, 0, 8);
    return 0;
}

static int callbackFoot(void *voidPixels, int argc, char **argv, char **azColName)
{
    callbackPlace(voidPixels, argc, argv, azColName, 0, 12);
    return 0;
}

static int callbackTail(void *voidPixels, int argc, char **argv, char **azColName)
{
    callbackPlace(voidPixels, argc, argv, azColName, 4, 12);
    return 0;
}

static int countCallback(void *voidCount, int argc, char **argv, char **azColName)
{
    if(argc > 0)
    {
        int *theCount = (int*)voidCount;
        *theCount = atoi(argv[0]);
    }
    
    return 0;
}

int buildVirusImage(int buildIndex)
{
    // for the SQLite database connection
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char buffer[4];
    
    // seed rand
    
    
    // the image we will save
    Image image("256x256", "black");

    bool **pixels = new bool*[LENGTH/2];
    int *numberOfRows = new int(-1);
    
    for(int i = 0; i < LENGTH/2; i++)
        pixels[i] = new bool[LENGTH];
    
    for(int x = 0; x < LENGTH/2; x++)
        for(int y = 0; y < LENGTH; y++)
            pixels[x][y] = false;


    // connect to the database
    rc = sqlite3_open("virus-parts.db", &db);
    
    if(rc)
    {
        cout << "Cannot open the Database!!!\n";
        sqlite3_close(db);
        return 1;
    }
    
    
    
    /* BEGIN: build the face
     ********************************************************/
     
    rc = sqlite3_exec(db, "SELECT COUNT(*) AS count FROM parts WHERE type = \"face\"", countCallback, numberOfRows, &zErrMsg);
    
    if(*numberOfRows > 0)
    {
        stringstream out;
        out << "SELECT * FROM parts WHERE type = \"face\" LIMIT 1 OFFSET " << rand()%*numberOfRows;
        string query = out.str();
        rc = sqlite3_exec(db, query.c_str(), callbackFace, pixels, &zErrMsg);
    }
    
    /* END: build the face
     ********************************************************/
    
    
    
    
    
    /* build the body
     ********************************************************/
    
    rc = sqlite3_exec(db, "SELECT COUNT(*) AS count FROM parts WHERE type = \"body\"", countCallback, numberOfRows, &zErrMsg);
    
    if(*numberOfRows > 0)
    {
        stringstream out;
        out << "SELECT * FROM parts WHERE type = \"body\" LIMIT 1 OFFSET " << rand()%*numberOfRows;
        string query = out.str();
        rc = sqlite3_exec(db, query.c_str(), callbackBody, pixels, &zErrMsg);
    }
    
    /* END: build the body
     ********************************************************/
    
    
    
    
    
    /* build the tail
     ********************************************************/
    
    rc = sqlite3_exec(db, "SELECT COUNT(*) AS count FROM parts WHERE type = \"tail\"", countCallback, numberOfRows, &zErrMsg);
    
    if(*numberOfRows > 0)
    {
        stringstream out;
        out << "SELECT * FROM parts WHERE type = \"tail\" LIMIT 1 OFFSET " << rand()%*numberOfRows;
        string query = out.str();
        rc = sqlite3_exec(db, query.c_str(), callbackTail, pixels, &zErrMsg);
    }
    
    /* END: build the tail
     ********************************************************/
    
    
    
    
    
    /* build the hair
     ********************************************************/
    
    rc = sqlite3_exec(db, "SELECT COUNT(*) AS count FROM parts WHERE type = \"hair\"", countCallback, numberOfRows, &zErrMsg);
    
    if(*numberOfRows > 0)
    {
        stringstream out;
        out << "SELECT * FROM parts WHERE type = \"hair\" LIMIT 1 OFFSET " << rand()%*numberOfRows;
        string query = out.str();
        rc = sqlite3_exec(db, query.c_str(), callbackHair, pixels, &zErrMsg);
    }
    
    /* END: build the hair
     ********************************************************/
    
    
    
    
    
    /* build the Arm
     ********************************************************/
    
    rc = sqlite3_exec(db, "SELECT COUNT(*) AS count FROM parts WHERE type = \"arm\"", countCallback, numberOfRows, &zErrMsg);
    
    if(*numberOfRows > 0)
    {
        stringstream out;
        out << "SELECT * FROM parts WHERE type = \"arm\" LIMIT 1 OFFSET " << rand()%*numberOfRows;
        string query = out.str();
        rc = sqlite3_exec(db, query.c_str(), callbackArm, pixels, &zErrMsg);
    }
    
    /* END: build the Arm
     ********************************************************/
     
     
     
     
     
     /* build the Hand
     ********************************************************/
    
    rc = sqlite3_exec(db, "SELECT COUNT(*) AS count FROM parts WHERE type = \"hand\"", countCallback, numberOfRows, &zErrMsg);
    
    if(*numberOfRows > 0)
    {
        stringstream out;
        out << "SELECT * FROM parts WHERE type = \"hand\" LIMIT 1 OFFSET " << rand()%*numberOfRows;
        string query = out.str();
        rc = sqlite3_exec(db, query.c_str(), callbackHand, pixels, &zErrMsg);
    }
    
    /* END: build the Hand
     ********************************************************/
    
    
    
    
    
    /* build the Leg
     ********************************************************/
    
    rc = sqlite3_exec(db, "SELECT COUNT(*) AS count FROM parts WHERE type = \"leg\"", countCallback, numberOfRows, &zErrMsg);
    
    if(*numberOfRows > 0)
    {
        stringstream out;
        out << "SELECT * FROM parts WHERE type = \"leg\" LIMIT 1 OFFSET " << rand()%*numberOfRows;
        string query = out.str();
        rc = sqlite3_exec(db, query.c_str(), callbackLeg, pixels, &zErrMsg);
    }
    
    /* END: build the Leg
     ********************************************************/
     
     
     
     
     
     
     /* build the Foot
     ********************************************************/
    
    rc = sqlite3_exec(db, "SELECT COUNT(*) AS count FROM parts WHERE type = \"foot\"", countCallback, numberOfRows, &zErrMsg);
    
    if(*numberOfRows > 0)
    {
        stringstream out;
        out << "SELECT * FROM parts WHERE type = \"foot\" LIMIT 1 OFFSET " << rand()%*numberOfRows;
        string query = out.str();
        rc = sqlite3_exec(db, query.c_str(), callbackFoot, pixels, &zErrMsg);
    }
    
    /* END: build the Arm
     ********************************************************/
     
     
    bool teamColor = rand()%2;
    image.strokeColor(teamColor ? "red" : "blue");
    image.fillColor(teamColor ? ColorRGB(0.5,0,0) : ColorRGB(0,0,0.5));
    image.strokeWidth(1);

    //cout << "Final Image:\n";
    for(int y = 0; y < LENGTH; y++)
    {
        int realX = 0;
        for(int x = 0; x < LENGTH/2; x++)
        {
            //cout << (pixels[x][y] ? "\033[7;34m  \033[0m" : "  ");
            if(pixels[x][y]) image.draw(DrawableRectangle(x*16,y*16, x*16 + 16, y*16 + 16));
            realX++;
        }
        for(int x = LENGTH/2 - 1; x > -1; x--)
        {
            //cout << (pixels[x][y] ? "\033[7;34m  \033[0m" : "  ");
            if(pixels[x][y]) image.draw(DrawableRectangle(realX*16,y*16, realX*16 + 16, y*16 + 16));
            realX++;
        }
        //cout << endl;
    }
    
    stringstream filename;
    filename << "virus-sprite-proof-of-concept-" << buildIndex << ".png";
    image.write(filename.str());
    
    // free up everything
    sqlite3_close(db);
    
    for(int i = 0; i < LENGTH/2; i++)
        delete pixels[i];  
    delete pixels;
    
    delete numberOfRows;
}

int main(int /*argc*/, char ** argv)
{
    // initialize ImageMagick
    InitializeMagick(*argv);
    srand(time(NULL));
    
    cout << "Starting\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    
    for(int i = 0; i < 100; i ++)
    {
        buildVirusImage(i);
        cout << i << " ";
    }
    
    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\nDone!\n";
    
    return 0;
}
