#include "common.h"
#include <string>

using namespace std;

// COMMON GLOBALS
const int NUM_ROWS = 128;
const int NUM_COLS = 162;
const int NUM_REGS = 12;
int parseLog(struct attitude *logAtt);

// ALGORITHM GLOBALS
float THRESHOLD_FACTOR = atof(getenv("THRESHOLDFACTOR"));
int BUFFER = atoi(getenv("BUFFER"));
int SUBPIX = atoi(getenv("SUBPIX"));
float ALTITUDE = atof(getenv("ALTITUDE"));
float PIXEL_PITCH = atof(getenv("PIXELPITCH"));
int SEARCH_RANGE = atoi(getenv("SEARCHRANGE"));
float F_LENGTH = atof(getenv("FLENGTH"));
int VISUALIZER = atoi(getenv("VISUALIZER"));
string IMAGE_PATH = getenv("IMAGEPATH");
int LATENCY = atoi(getenv("LATENCY"));
int LOOK_FORWARD = atoi(getenv("LOOKFORWARD"));
int CALIBRATE = atoi(getenv("CALIBRATE"));

// BITBANG GLOBALS
