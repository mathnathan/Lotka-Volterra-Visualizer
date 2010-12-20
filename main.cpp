/* Real-Time Lotka-Volterra Dynamic System visualizer
 *
 *
 * Forked from:
 * 		Adventures in OpenCL tutorial series Part 2
 *		by - Ian Johnson (http://enja.org)
 *
 *
 * author: Nathan Crock
 * http://mathnathan.com 
 */
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include <math.h>
# include <vector>
# include "cv.h"
# include "highgui.h"
# include "util.h"
# include "texMapping.h"


#define NUM_PARTICLES 100000

using namespace std;
using namespace cv;

//GL related variables
	int window_width = 1080;
	int window_height = 1014;
	int glutWindowHandle = 0;
	int glutWindowHandle2 = 0;
	float translate_x = -.5f;
	float translate_y = -.5f;
	float translate_z = -.5f;

// mouse controls
	int mouse_old_x, mouse_old_y;
	float respawn_x = 0, respawn_y = 0;
	int mouse_buttons = 0;
	bool has_clicked = false;

//main app helper functions
	void init_gl( int argc, char** argv );
	void appRender();
	void appDestroy();
	void timerCB( int ms );
	void appKeyboard( unsigned char key, int x, int y );
	void appMouse( int button, int state, int x, int y );
	void appMotion( int x, int y );
	void appReshape( int nw, int nh );
	void kernel();

// for use in generating tail of particles
	int colored_particle;
	int counter = 0;
	int tail_particles = 0;
	const int TAIL_LENGTH = 200;

// constants for the Lotka-Volterra ODE
	const double a1 = 2, a2 = 2, b1 = 1, b2 = 1, dt = .01;

//initialize our particle system with positions, velocities and color
	int num = NUM_PARTICLES;

// Create the vectors to store the position and color of each particle
	std::vector<Vec4> pos( num );
	std::vector<Vec4> color( num );

// Vertex buffer object, for drawing the particles
	GLuint p_vbo;
	GLuint c_vbo;
	
//-----------OpenCV Stuff-----------//
// Matrix to store the graph
	Mat img;
// Begining and end of line for population graph
	Point begLine( 42, 289 ), endLine( 1059, 289 );
	vector<float> koalas( endLine.x - begLine.x );
	vector<float> bums( endLine.x - begLine.x );


//----------------------------------------------------------------------
// quick random function to grab random colors, or positions
float rand_float( float mn, float mx ) {

    float r = random() / (float) RAND_MAX;
    return mn + ( mx-mn )*r;
}


//----------------------------------------------------------------------
int main( int argc, char** argv ) {

	//-----------------Initialize OpenGL-----------------------//
    //Setup our GLUT window and OpenGL related things
    //glut callback functions are setup here too
    init_gl( argc, argv );

	//-------------Initialize OpenGL Textures------------------//
	setupGLTextures ();

	//-----------------Initialize OpenCV-----------------------//
	loadCVimage( img );
	init_Vectors( koalas, bums );
	namedWindow( "Population vs. Time", CV_WINDOW_AUTOSIZE );
	
	
	// initialize all the particles with position and color
	for( int i = 0; i < num; i++ ) {	
		// store the particles out of sight
		pos[i] = Vec4( 0, 0, 0, 1 );
		color[i] = Vec4( rand_float( 0, 0.3 ), rand_float( 0, 0.3 ), rand_float( 0, 0.3), 1 );
	}
	
	// create the particle buffer and color buffer objects
		p_vbo = createVBO(&pos[0], sizeof(Vec4)*num, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
		c_vbo = createVBO(&color[0], sizeof(Vec4)*num, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

    //this starts the GLUT program, from here on out everything we want
    //to do needs to be done in glut callback functions
    	glutMainLoop();

}

//----------------------------------------------------------------------
void appRender() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //this updates the particle system by calling the kernel
	glBindBuffer( GL_ARRAY_BUFFER, p_vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof(Vec4)*num, &pos[0], GL_DYNAMIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, c_vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof(Vec4)*num, &color[0], GL_DYNAMIC_DRAW );
	kernel();

    //render the particles from VBOs
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_POINT_SMOOTH );
    glPointSize( 5. );
    
    glBindBuffer( GL_ARRAY_BUFFER, c_vbo );
    glColorPointer( 4, GL_FLOAT, 0, 0 );

    glBindBuffer( GL_ARRAY_BUFFER, p_vbo );
    glVertexPointer( 4, GL_FLOAT, 0, 0 );

    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );

    //Need to disable these for blender
    glDisableClientState( GL_NORMAL_ARRAY );

    glDrawArrays( GL_POINTS, 0, num );

    glDisableClientState( GL_COLOR_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );
    
    glutSwapBuffers();

}


//----------------------------------------------------------------------
void init_gl( int argc, char** argv ) {

	// initilizations 
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( window_width, window_height );
    glutInitWindowPosition ( glutGet( GLUT_SCREEN_WIDTH )/2 - window_width/2, 
                            glutGet( GLUT_SCREEN_HEIGHT )/2 - window_height/2 );

	// make window    
    std::stringstream ss;
    ss << "Real-Time Lotka-Volterra Solutions: Using " << NUM_PARTICLES << " Particles" << std::ends;
    glutWindowHandle = glutCreateWindow( ss.str().c_str() );

	// assign the callbacks
    glutDisplayFunc( appRender ); //main rendering function
	// the times here need to be the same as in the openCV waitKey function, in the kernel
    glutTimerFunc( 10, timerCB, 10 ); //determin a minimum time between frames
    glutKeyboardFunc( appKeyboard );
    glutMouseFunc( appMouse );

    glewInit();
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glDisable( GL_DEPTH_TEST );

    // projection
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
	gluOrtho2D( l_bound, r_bound, b_bound, t_bound );

    // set view matrix
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
	
    // viewport
    glViewport( 0, 0, window_width, window_height );

	// call some opencv functions to load an image and put it in a buffer
	loadTextures ();


}

//----------------------------------------------------------------------
void appReshape( int new_width, int new_height ) {

    window_width = new_width;
    window_height = new_height;
    glViewport( 0, 0, window_width, window_height );
	glutPostRedisplay();
}

//----------------------------------------------------------------------
void appDestroy() {

    if( glutWindowHandle ) glutDestroyWindow( glutWindowHandle );
    printf( "about to exit!\n" );

    exit(0);
}


//----------------------------------------------------------------------
void timerCB( int ms ) {

    //this makes sure the appRender function is called every ms miliseconds
    glutTimerFunc( ms, timerCB, ms );
    glutPostRedisplay();
}


//----------------------------------------------------------------------
void appKeyboard( unsigned char key, int x, int y ) {

    //this way we can exit the program cleanly
    switch( key )
    {
        case '\033': // escape quits
        case '\015': // Enter quits    
        case 'Q':    // Q quits
        case 'q':    // q (or escape) quits
            // Cleanup up and quit
            appDestroy();
            break;
    }
}


//----------------------------------------------------------------------
void appMouse( int button, int state, int x, int y ) {

	// If we're out of particles leave before we break things
	if( counter >= num )
		return;
	

	tail_particles = 0;

    //handle mouse interaction for rotating/zooming the view
    if ( state == GLUT_DOWN ) {
        mouse_buttons |= 1<<button;

		// get window width and height for scaling mouse clicks
    	float ww = glutGet( GLUT_WINDOW_WIDTH );
    	float wh = glutGet( GLUT_WINDOW_HEIGHT );

		// show the gobal click values
        printf( "x: %d, y:%d\n", x, y );

		// percents calculated by counting pixels in the texture image
		const double lr_percm = .103703704;
		const double b_percm = .08678501;

		// Translate the mouse clicks, need to be within the graph boundaries.
		// This simplifies calculations
        float xx = (float)((x-ww*lr_percm)/ww)*(r_bound - l_bound);
        float yy = (float)((wh-y-wh*b_percm)/wh)*(t_bound - b_bound);


			// move a particle to the mouse click location
			pos[counter].x = xx;
			pos[counter].y = yy;

			init_Vectors( koalas, bums );

			// add this new position to the end of the population vectors
			koalas.erase( koalas.begin() );
			bums.erase( bums.begin() );
			koalas.push_back( yy );
			bums.push_back( xx );

		// show the scaled click values
        printf("koalas: %f, beach bums:%f\n", xx, yy);

			// store this particles color so every particle in the tail
			// can have the same color
			colored_particle = counter++;

			// store the spawn location t respawn new particles for the tail
			respawn_x = xx;
			respawn_y = yy;

    } else if (state == GLUT_UP) {
        mouse_buttons = 0;
    }

    mouse_old_x = x;
    mouse_old_y = y;

	has_clicked = true;
}


void kernel() {

	// evaluate Lotka-Volterra system
	for(int i = 0; i <= counter; i++) {
		pos[i].x += (a1*pos[i].x - b1*pos[i].y*pos[i].x)*dt;
		pos[i].y += (-a2*pos[i].y + b2*pos[i].y*pos[i].x)*dt;
}
		// update the next population for the p vs. t graph
		if(has_clicked && tail_particles <= TAIL_LENGTH) {
			koalas.erase( koalas.begin() );
			bums.erase( bums.begin() );
			koalas.push_back( pos[counter-tail_particles].y );
			bums.push_back( pos[counter-tail_particles].x );
		}
		else {
			koalas.erase( koalas.begin() );
			bums.erase( bums.begin() );
			koalas.push_back( pos[counter-TAIL_LENGTH].y );
			bums.push_back( pos[counter-TAIL_LENGTH].x );
		}	


	// create a trail of particles after each click
	if(has_clicked && tail_particles <= TAIL_LENGTH && counter < num)  {
		pos[counter].x = respawn_x;
		pos[counter].y = respawn_y;
		color[counter++] = color[colored_particle];
	}

	// draw the texture to the quad (background) every loop
	drawTextureObjects ();


	// draw the population vs. time graph 
	drawCvObjects( img, koalas, bums, begLine, endLine );
	// this needs to be the same time as in timerCB
	waitKey( 10 );


	// tail_particles counts the number of particles in the tail
	tail_particles++;
}
