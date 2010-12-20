# include "highgui.h"
# include "cv.h"
# include <string>
# include "texMapping.h"
# include <stdio.h>

using namespace std;
using namespace cv;

void setupGLTextures (void)
{
    glEnable (GL_TEXTURE_2D);
}

void init_Vectors( vector<float>& koalas, vector<float>& bums ) {

	for( int i = 0; i < koalas.size(); i++ ) {
		koalas[i] = 0.0;
		bums[i] = 0.0;
	}

}

int loadTextures ( const string& filename )
{
	Mat image = imread( filename );

    glBindTexture (GL_TEXTURE_2D, 1);
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.data);
		printf("(Image.cols, Image.rows): (%d, %d)\n", image.cols, image.rows);

	return 0;
}

void drawTextureObjects (void) {

	glBindTexture (GL_TEXTURE_2D, 1);
	glBegin (GL_QUADS);
	glTexCoord2f (0.0, 0.0);
	glVertex3f (l_bound, t_bound, 0.0);
	glTexCoord2f (1.0, 0.0);
	glVertex3f (r_bound, t_bound, 0.0);
	glTexCoord2f (1.0, 1.0);
	glVertex3f (r_bound, b_bound, 0.0);
	glTexCoord2f (0.0, 1.0);
	glVertex3f (l_bound, b_bound, 0.0);
	glEnd ();
}

void loadCVimage( Mat& img, const string& filename ) {

	img = imread( filename );
	
}

void drawCvObjects( Mat& img, vector<float> koalas, vector<float> bums, const Point& begLine, const Point& endLine ) {

	loadCVimage( img );

	int b = 42;

	for( int i = 1; i < bums.size(); i++ ) {

	if( i == 1 ) {
		koalas[0] = 5 - ( (koalas[0]/5)*520 + 24 );
		bums[0] = 5 - ( (bums[0]/5)*520 + 24 );
	}

	koalas[i] = 586 - ( (koalas[i]/5)*520 + 24 );
	bums[i] = 586 - ( (bums[i]/5)*520 + 24 );

	line( img, Point(b, koalas[i-1]), Point(b+1, koalas[i]), CV_RGB(255,0,0), 1 ); 
	line( img, Point(b, bums[i-1]), Point(b+1, bums[i]), CV_RGB(0,0,255), 1 );  

	b++;

	}

	imshow( "Population vs. Time", img );
}
