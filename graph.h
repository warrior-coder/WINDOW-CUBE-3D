#pragma once

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

// Include using libs
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
using std::cout;
using std::endl;

// Define macroses
#define ABS(a) ( (a) < 0 ? -(a) : (a) )
#define MIN(a, b) ( (a) < (b) ? (a) : (b) )
#define MAX(a, b) ( (a) > (b) ? (a) : (b) )
#define PI 3.14159265358979323846
#define RAD(a) (a * 0.01745329251994329577)

// Define using types
typedef unsigned char BYTE;
typedef struct RGB
{
    BYTE R, G, B;
};
typedef struct RGB4
{
    BYTE B, G, R, A;
};
typedef struct VECT2
{
    int x, y;
};
typedef struct VECT3
{
    float x, y, z;
};
typedef struct EDGE
{
    int begin, end;
};

class FIGURE
{
public:
    VECT3* vertexes;
    VECT2* vert_proj;
    EDGE* edges;
    int num_vert;
    int num_edg;

    FIGURE(const char* fname);
    ~FIGURE();

    void rotate(float angle_x, float angle_y, float angle_z, float rot_x, float rot_y, float rot_z);
    void translate(float dx, float dy, float dz);
};

class FRAME
{
private:
    // Frame buffer
    RGB4* buffer = nullptr;

    // Handle to the window
    HWND hwnd = nullptr;

    // Handle to the device context
    HDC hdc = nullptr;
    HDC tmpDc = nullptr;

    // Handle to the bitmap
    HBITMAP hbm = nullptr;

    // Camera position
    VECT3 CAMERA;

    // Tangens of half field of veiw
    float TAN_HALF_FOV;

public:
    // Size of the frame
    short width;
    short height;

    // Pen color
    RGB pen_color;

    FRAME(short frameWidth, short frameHeight, HWND frameHwnd);
    ~FRAME();

    void clear(RGB color = { 255, 255, 255 });
    void set_rect(short x1, short y1, short x2, short y2);
    void set_circle(short x0, short y0, short R);
    void set_pixel(short x, short y);
    void set_line(short x1, short y1, short x2, short y2);
    void set_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
    void print();
    void save(const char* fname);
    VECT2 vect_projection(VECT3 vect3);
    void set_figure(FIGURE& figure);
};



void vect_rotate(VECT3& vect3, float angle_x, float angle_y, float angle_z, float rot_x, float rot_y, float rot_z);
void vect_translate(VECT3& vect3, float dx, float dy, float dz);

#endif
