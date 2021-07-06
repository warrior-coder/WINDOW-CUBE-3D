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
typedef struct TRI
{
    int one, two, three;
};

class FIGURE_SURFACE
{
public:
    VECT3* vertexes;
    VECT2* vert_proj;
    TRI* tris;
    int* tri_index;
    float* tri_min_z;
    int num_vert;
    int num_tri;
    VECT3 NORM_VECT;

    FIGURE_SURFACE(const char* fname);
    ~FIGURE_SURFACE();

    void rotate(float angle_x, float angle_y, float angle_z, float rot_x, float rot_y, float rot_z);
    void translate(float dx, float dy, float dz);
    float is_show_surface(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
};

class FIGURE_WIREFRAME
{
public:
    VECT3* vertexes;
    VECT2* vert_proj;
    EDGE* edges;
    int num_vert;
    int num_edg;

    FIGURE_WIREFRAME(const char* fname);
    ~FIGURE_WIREFRAME();

    void rotate(float angle_x, float angle_y, float angle_z, float rot_x, float rot_y, float rot_z);
    void translate(float dx, float dy, float dz);
};

class FRAME
{
private:
    RGB4* buffer = nullptr; HWND hwnd = nullptr; HDC hdc = nullptr; HDC tmpDc = nullptr; HBITMAP hbm = nullptr;

    // Camera
    VECT3 CAMERA;

    // Tangent of half field of veiw
    float TAN_HALF_FOV, HALF_FOV, MIN_NORM_C;

public:
    short width; short height; RGB pen_color;

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
    void set_figure_wireframe(FIGURE_WIREFRAME& figure);
    void set_figure_surface(FIGURE_SURFACE& figure);
};



void vect_rotate(VECT3& vect3, float angle_x, float angle_y, float angle_z, float rot_x, float rot_y, float rot_z);
void vect_translate(VECT3& vect3, float dx, float dy, float dz);

#endif
