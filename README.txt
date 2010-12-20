Lotka-Volterra Visualizer
author: Nathan Crock
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
			|
			|	//------------------------------------------------------------------//
			|		The data folder holds the two images for the background of both
			|		displays. You'll need to download them from my website
			|
			|				http://mathnathan.com/2010/12/20/lv-visual/ 
			|	//------------------------------------------------------------------//
			|
			build - Where the program is built


//--------------------Instructions----------------------------//

Once you've downloaded both the backgrounds and put them in the
data folder, all you do is cd into the build directory and type

cmake ..

This calls CMakeLists.txt from the previous directory and builds
the binary files into the build folder.

