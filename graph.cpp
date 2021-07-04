#include "graph.h"

// -+-+-+-+-+-+-+-+-+-+- FRAME -+-+-+-+-+-+-+-+-+-+-
FRAME::FRAME(short frameWidth, short frameHeight, HWND frameHwnd)
{
    width = frameWidth;
    height = frameHeight;

    // Create buffer array
    int size = width * height;
    buffer = new RGB4[size];

    // Copy handle to the window
    hwnd = frameHwnd;

    // Create handle to the device context
    hdc = GetDC(hwnd);
    tmpDc = CreateCompatibleDC(hdc);

    pen_color = {};

    CAMERA = { 200.0f, 200.0f, 300.0f };
    TAN_HALF_FOV = width / 2 / CAMERA.z;
}
FRAME::~FRAME()
{
    delete[] buffer;
    DeleteDC(tmpDc);
    ReleaseDC(hwnd, hdc);
}

void FRAME::clear(RGB color)
{
    int i;

    for (short y = 0; y < height; y++)
    {
        for (short x = 0; x < width; x++)
        {
            i = y * width + x;
            buffer[i].R = color.R;
            buffer[i].G = color.G;
            buffer[i].B = color.B;
        }
    }
}
void FRAME::set_pixel(short x, short y)
{
    if (x > -1 && y > -1 && x < width && y < height)
    {
        int i = y * width + x;

        buffer[i].R = pen_color.R;
        buffer[i].G = pen_color.G;
        buffer[i].B = pen_color.B;
    }
}
void FRAME::set_rect(short x1, short y1, short x2, short y2)
{
    for (short y = y1; y <= y2; y++)
    {
        for (short x = x1; x <= x2; x++)
        {
            set_pixel(x, y);
        }
    }
}
void FRAME::set_circle(short x0, short y0, short R)
{
    for (short x = x0 - R; x < x0 + R; x++)
    {
        for (short y = y0 - R; y < y0 + R; y++)
        {
            if (((x - x0) * (x - x0) + (y - y0) * (y - y0) - R * R) < 0) set_pixel(x, y);
        }
    }
}
void FRAME::set_line(short x1, short y1, short x2, short y2)
{
    short dx = ABS(x2 - x1);        // Absolute delta X
    short dy = ABS(y2 - y1);        // Absolute delta Y
    short sx = (x2 >= x1) ? 1 : -1; // Step X
    short sy = (y2 >= y1) ? 1 : -1; // Step Y

    if (dx > dy)
    {
        short d = (dy << 1) - dx, d1 = dy << 1, d2 = (dy - dx) << 1;

        for (short x = x1 + sx, y = y1, i = 1; i < dx; i++, x += sx)
        {
            if (d > 0)
            {
                d += d2;
                y += sy;
            }
            else d += d1;

            set_pixel(x, y);
        }
    }
    else
    {
        short d = (dx << 1) - dy, d1 = dx << 1, d2 = (dx - dy) << 1;

        for (short x = x1, y = y1 + sy, i = 1; i < dy; i++, y += sy)
        {
            if (d > 0)
            {
                d += d2;
                x += sx;
            }
            else d += d1;

            set_pixel(x, y);
        }
    }

    set_pixel(x1, y1);
    set_pixel(x2, y2);
}
void FRAME::set_triangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    int left = MIN(MIN(x1, x2), x3);   // Rect left border
    int right = MAX(MAX(x1, x2), x3);  // Rect right border
    int top = MIN(MIN(y1, y2), y3);    // Rect top border
    int bottom = MAX(MAX(y1, y2), y3); // Rect bottom border

    int side12, side23, side31;

    for (int x = left; x <= right; x++)
    {
        for (int y = top; y <= bottom; y++)
        {
            side12 = (x - x1) * (y2 - y1) - (x2 - x1) * (y - y1);
            side23 = (x - x2) * (y3 - y2) - (x3 - x2) * (y - y2);
            side31 = (x - x3) * (y1 - y3) - (x1 - x3) * (y - y3);

            if (side12 <= 0 && side23 <= 0 && side31 <= 0 || side12 > 0 && side23 > 0 && side31 > 0) set_pixel((short)x, (short)y);
        }
    }
}
void FRAME::print()
{
    // Create Bitmap function creates a bitmap from array of pixels data and returns handle to the bitmap
    hbm = CreateBitmap(
        width,            // Bitmap width
        height,           // Bitmap height
        1,                // Number of color planes
        8 * sizeof(RGB4), // Number of bits required to identify the color 1 pixel
        buffer            // Pointer to array of pixels data
    );

    //  Selects Object into the specified device context (DC)
    SelectObject(
        tmpDc, // Handle to the DC
        hbm    // Handle to the object to be selected
    );

    // Bit Built transfers a rectangle of pixels from one DC into another
    BitBlt(
        hdc,    // Destination DC
        0,      // X begin of destination rect
        0,      // Y begin of destination rect
        width,  // Width of the source and destination rect
        height, // Height of the source and destination rect
        tmpDc,  // Sourse DC
        0,      // X begin of source rect
        0,      // Y begin of source rect
        SRCCOPY // Source copy mode
    );

    // Clear bitmap memory
    DeleteObject(hbm);
}
void FRAME::save(const char* fname)
{
    const short f_header_size = 14;
    BYTE f_header[f_header_size];

    const short f_info_size = 40;
    BYTE f_info[f_info_size];

    int p_size = (4 - (width * 3) % 4) % 4;

    int fsize = f_header_size + f_info_size + (width * height * 4) + (p_size * height);

    // File signature
    f_header[0] = 'B';
    f_header[1] = 'M';

    // File size
    f_header[2] = fsize;
    f_header[3] = fsize >> 8;
    f_header[4] = fsize >> 16;
    f_header[5] = fsize >> 24;

    // Reserved
    f_header[6] = 0;
    f_header[7] = 0;
    f_header[8] = 0;
    f_header[9] = 0;

    // Pixel data offset
    f_header[10] = f_header_size + f_info_size;
    f_header[11] = 0;
    f_header[12] = 0;
    f_header[13] = 0;

    // Header size
    f_info[0] = f_info_size;
    f_info[1] = 0;
    f_info[2] = 0;
    f_info[3] = 0;

    // Image width
    f_info[4] = width;
    f_info[5] = width >> 8;
    f_info[6] = width >> 16;
    f_info[7] = width >> 24;

    // Image height
    f_info[8] = height;
    f_info[9] = height >> 8;
    f_info[10] = height >> 16;
    f_info[11] = height >> 24;

    // Planes
    f_info[12] = 1;
    f_info[13] = 0;

    // Bits per pixel (RGB - 24 bits)
    f_info[14] = 24;
    f_info[15] = 0;

    // Compression (No compression)
    f_info[16] = 0;
    f_info[17] = 0;
    f_info[18] = 0;
    f_info[19] = 0;

    // Image size (No compression)
    f_info[20] = 0;
    f_info[21] = 0;
    f_info[22] = 0;
    f_info[23] = 0;

    // X pixels per meter
    f_info[24] = 0;
    f_info[25] = 0;
    f_info[26] = 0;
    f_info[27] = 0;

    // Y pixels per meter
    f_info[28] = 0;
    f_info[29] = 0;
    f_info[30] = 0;
    f_info[31] = 0;

    // Total colors (Not used)
    f_info[32] = 0;
    f_info[33] = 0;
    f_info[34] = 0;
    f_info[35] = 0;

    // Important colors (0 - all)
    f_info[36] = 0;
    f_info[37] = 0;
    f_info[38] = 0;
    f_info[39] = 0;

    // Write file
    FILE* fp;

    fopen_s(&fp, fname, "wb");
    if (fp)
    {
        // Write header
        fwrite(f_header, sizeof(BYTE), f_header_size, fp);

        // Write info
        fwrite(f_info, sizeof(BYTE), f_info_size, fp);

        // Write buffer
        BYTE padding = 0;

        for (int y = height - 1; y > -1; y--)
        {
            for (int x = 0; x < width; x++)
            {
                fwrite(&buffer[y * width + x].B, sizeof(BYTE), 1, fp);
                fwrite(&buffer[y * width + x].G, sizeof(BYTE), 1, fp);
                fwrite(&buffer[y * width + x].R, sizeof(BYTE), 1, fp);
            }
            fwrite(&padding, sizeof(BYTE), p_size, fp);
        }

        fclose(fp);
    }
}
VECT2 FRAME::vect_projection(VECT3 vect3)
{
    if (vect3.z == 0)
    {
        return { int(vect3.x), int(vect3.y) };
    }
    else if (vect3.z > 0)
    {
        VECT2 vect2;
        float k = (width / 2) / ((CAMERA.z + vect3.z) * TAN_HALF_FOV);

        vect2.x = CAMERA.x + (vect3.x - CAMERA.x) * k;
        vect2.y = CAMERA.y + (vect3.y - CAMERA.y) * k;

        return vect2;
    }
    else
    {
        VECT2 vect2;
        float k = (width / 2) / ((CAMERA.z - vect3.z) * TAN_HALF_FOV);

        vect2.x = CAMERA.x + (vect3.x - CAMERA.x) / k;
        vect2.y = CAMERA.y + (vect3.y - CAMERA.y) / k;

        return vect2;
    }
}
void FRAME::set_figure(FIGURE& figure)
{
    // Make projections on frame
    for (short i = 0; i < figure.num_vert; i++)
    {
        figure.vert_proj[i] = vect_projection(figure.vertexes[i]);
    }

    // Draw edges
    for (short i = 0; i < figure.num_edg; i++)
    {
        set_line(figure.vert_proj[figure.edges[i].begin].x, figure.vert_proj[figure.edges[i].begin].y, figure.vert_proj[figure.edges[i].end].x, figure.vert_proj[figure.edges[i].end].y);
    }

    // Draw vertexes
    for (short i = 0; i < figure.num_vert; i++)
    {
        set_circle(figure.vert_proj[i].x, figure.vert_proj[i].y, 5);
    }
}

// -+-+-+-+-+-+-+-+-+-+- GLOBAL FUNC -+-+-+-+-+-+-+-+-+-+-
void vect_rotate(VECT3& vect3, float angle_x, float angle_y, float angle_z, float rot_x, float rot_y, float rot_z)
{
    /*
    float Mx[3][3] = 
    {
        { 1,               0,                0 },
        { 0, cos(RAD(angle)), -sin(RAD(angle)) },
        { 0, sin(RAD(angle)),  cos(RAD(angle)) }
    }
    float My[3][3] =
    {
        {  cos(RAD(angle)), 0, sin(RAD(angle)) },
        {                0, 1,               0 },
        { -sin(RAD(angle)), 0, cos(RAD(angle)) }
    }
    float Mz[3][3] =
    {
        { cos(RAD(angle)), -sin(RAD(angle)), 0 },
        { sin(RAD(angle)), cos(RAD(angle)),  0 },
        { 0,               0,                1 }
    }
    */

    vect3.x -= rot_x;
    vect3.y -= rot_y;
    vect3.z -= rot_z;

    float x, y, z;

    if (angle_x)
    {
        angle_x = RAD(angle_x);
        y = vect3.y;
        z = vect3.z;

        vect3.y = cos(angle_x) * y - sin(angle_x) * z;
        vect3.z = sin(angle_x) * y + cos(angle_x) * z;
    }
    if (angle_y)
    {
        angle_y = RAD(angle_y);
        x = vect3.x;
        z = vect3.z;

        vect3.x =  cos(angle_y) * x + sin(angle_y) * z;
        vect3.z = -sin(angle_y) * x + cos(angle_y) * z;
    }
    if (angle_z)
    {
        angle_z = RAD(angle_z);
        x = vect3.x;
        y = vect3.y;

        vect3.x = cos(angle_z) * x - sin(angle_z) * y;
        vect3.y = sin(angle_z) * x + cos(angle_z) * y;
    }

    vect3.x += rot_x;
    vect3.y += rot_y;
    vect3.z += rot_z;
}
void vect_translate(VECT3& vect3, float dx, float dy, float dz)
{
    vect3.x += dx;
    vect3.y += dy;
    vect3.z += dz;
}

// -+-+-+-+-+-+-+-+-+-+- FIGURE -+-+-+-+-+-+-+-+-+-+-
FIGURE::FIGURE(const char* fname)
{
    FILE* fp;

    // Open file
    fopen_s(&fp, fname, "rt");

    if (fp)
    {
        // Read vertexes
        fscanf_s(fp, "%d", &num_vert);

        vertexes = new VECT3[num_vert];
        vert_proj = new VECT2[num_vert];

        for (short i = 0; i < num_vert; i++)
        {
            fscanf_s(fp, "%f %f %f", &vertexes[i].x, &vertexes[i].y, &vertexes[i].z);
        }

        // Read edges
        fscanf_s(fp, "%d", &num_edg);

        edges = new EDGE[num_edg];

        for (short i = 0; i < num_edg; i++)
        {
            fscanf_s(fp, "%d %d", &edges[i].begin, &edges[i].end);
        }

        // Close file
        fclose(fp);
        printf("%s readed.", fname);
    }
    else
    {
        printf("%s read error.", fname);
        num_vert = num_edg = 0;
    }
}
FIGURE::~FIGURE()
{
    delete[] vertexes;
    delete[] vert_proj;
    delete[] edges;
}

void FIGURE::rotate(float angle_x, float angle_y, float angle_z, float rot_x, float rot_y, float rot_z)
{
    for (short i = 0; i < num_vert; i++)
    {
        vect_rotate(vertexes[i], angle_x, angle_y, angle_z, rot_x, rot_y, rot_z);
    }
}
void FIGURE::translate(float dx, float dy, float dz)
{
    for (short i = 0; i < num_vert; i++)
    {
        vect_translate(vertexes[i], dx, dy, dz);
    }
}