#ifndef OPENCV_FUNCTIONS
#define OPENCV_FUNCTIONS

# include <string>
# include "util.h"
# include <cv.h>

using namespace std;
using namespace cv;

typedef struct Vec4
{
    float x,y,z,w;
    Vec4(){};
    //convenience functions
    Vec4(float xx, float yy, float zz, float ww):
        x(xx),
        y(yy),
        z(zz),
        w(ww)
    {}
    void set(float xx, float yy, float zz, float ww=1.) {
        x = xx;
        y = yy;
        z = zz;
        w = ww;
    }
} Vec4 __attribute__((aligned(16)));


// All for scaling and transforming the glutWindowHandle
const double d_scale = 5.0;
const double lr_perc = .129903704;
const double b_perc = .10378501;
const double t_perc = .069033531;
const double l_bound = -d_scale*lr_perc;
const double r_bound = d_scale+d_scale*lr_perc;
const double b_bound = -d_scale*b_perc;
const double t_bound = d_scale+d_scale*t_perc;

void setupGLTextures (void);

void init_Vectors( vector<float>& koalas, vector<float>& bums );

int loadTextures ( const string& filename = "../data/LVvectorField.png");

void loadCVimage( Mat& img, const string& filename = "../data/PvsT2.png" );

void drawTextureObjects (void);

void drawCvObjects ( Mat& img, vector<float> koalas, vector<float> bums, const Point& begLine, const Point& endLine ); 

#endif
