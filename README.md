Coded by Steven Leal (2017)

## Build Notes:
* Originally compiled for Visual Studio 14.0 (also confusingly known as Visual Studio 2015)
	
## Setup Instructions:
	NOTE: In all of the following instructions, don't type the quotes (ex: "" or '')
		when entering information.
		
* Download and install Visual Studio 2015 (or some greater version)
	
* Download and install cmake using the windows-x64 installer (https://cmake.org/download/)
	* be sure to allow cmake to add its path variable during installation
		
* Download and extract the following libraries:
	1. GLM (http://glm.g-truc.net/)
		* Go to the downloads page (its a git repository) and grab the latest
				zip version (0.9.8.4 last time I checked)
	2. GLFW (http://www.glfw.org/)
	3. GLEW (http://glew.sourceforge.net/)
		* Download the windows 32/64-bit binaries

		NOTE: There is no need to build or compile anything with these libraries,
			just put the folders in a place where you can reference them later
			when making the environment variables.
			
* Setup Environment Variables
	* Under the user variables section set the following 3 environment variables
			to the root directory of the libraries.(AKA: the directories that contain
			items like include/, src/, doc/, lib/, readme, etc.)
		1. Set 'GLM_INCLUDE_DIR' to be the path/to/GLM
		2. Set 'GLFW_DIR' to be the path/to/GLFW
		3. Set 'GLEW_DIR' to be the path/to/GLEW

* To test your environment variables, open a new powershell and run the following commands:
		```powershell
		$env:GLM_INCLUDE_DIR
		$env:GLFW_DIR
		$env:GLEW_DIR
		```
		
* To build the Visual Studio Project:
	* Open a command prompt in the repository's root directory
			(contains src/, resources/, readme, etc.)
			
	* Enter the following commands to generate the project files:
			mkdir build
			cd build
			cmake ..
		
* At this point, you should find the PaintBot.sln project file in the new build/ folder
		
## Build/Run Instructions:
* Open the Visual Studio Project (PaintBot.sln)
* Right click PaintBot in the solutions explorer and click "Set as StartUp Project"
* Right click PaintBot again and open the properties window:
	* Under the Linker Tab:
		* In the General section, add '../src' to the
				'Additional Library Directories' property
		* In the Input section, append 'SOIL.lib;' to the beginning of the
				list of Additional Dependencies
	* Under the Debugging Tab:
		* In the command arguments, add ../resources
* press F5 to build and run
	