// --------Lotka-Volterra Visualizer------- //

	author: Nathan Crock
	date: 12/20/2010 16:01:30
	website: http://mathnathan.com



You'll need OpenGL, glut, and OpenCV 2.1

DIRECTORY:

Lotka-Volterra-Visualizer
			|
			main.cpp - This is where the guts of the program are
			|
			util.h - Some headers and declarations for OpenGL
			|
			util.cpp - Definition of the createVBO function
			|
			texMapping.h - Declaration of texture mapping OpenGL functions
			|			   and openCV functions. There are also some silly
			|			   constants in there, I needed these for scaling my
			|			   clicks onto the graphs. They're ratios for scaling
			|			   the pixels to be between (0,5)
			texMapping.cpp - Definitions, the drawCvObjects function could
			|				 probably use some tweaking...
			FindGLEW.cmake - CMakeLists.txt calls this to find the appropriate headers
			|
			CMakeLists.txt - Does all the building for us. You may need to tweak where
			|				 CMake looks for the dependencies.
			|
			data - 
			|	 |
			|	 LVvectorField.png - Background for the OpenGL display
			|	 |
			|	 PvsT2.png - Background for the OpenCV display
			|
			build - Where the program is built


//--------------------Instructions----------------------------//

Once you've downloaded both the backgrounds and put them in the
data folder, all you do is make a build directory and call cmake

mkdir build
cd build
cmake ..

This calls CMakeLists.txt from the previous directory and builds
the binary files into the build folder.


// ---------------------- TO DO ------------------------------//

- I learned OpenGL for this program, so it's a little buggy and certainly not very clean.

- There are a bunch of constants I had to experimentally derive to scale the mouse clicks to the appropriate dimensions for both windows. I'm sure there is a better way to go about doing it.

- This could have eaily been written only in OpenCV, but then I wouldn't have had the excuse to learn OpenGL! =]

SPECIAL THANKS:

To my good friend and programming mentor Ian Johnson. http://enja.org
