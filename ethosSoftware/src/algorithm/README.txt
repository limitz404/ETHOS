################# HOW TO COMPILE AND RUN ATTITUDE DETERMINATION CODE #################
1) Copy the attitude determination code to the desired directory.
		ex)		cp -r /path/to/Algorithm/Directory /path/to/desired/directory
2) Change directories to the Algorithm directory
		ex)		cd /path/to/Algorithm/Directory/
3) Edit the envVariables.sh file to correspond to desired settings.
		ex)		vi ./envVariables.sh
4) Source envVariables.sh to add the environment variables to the current shell.
		ex)		source ./envVariables.sh
5) Run compileEdgeDetection.sh.
		ex)		./compileEdgeDetection.sh
6) Assuming it completed without errors, run EdgeDetection.
		ex)		./EdgeDetection