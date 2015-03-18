#include <iostream>
#include "attitudeDetermination.h"
#include "dataStructures.h"
#include "common.h"

extern "C" {
    #include "bitbang.h"
}

#define NUMROWS 128
#define NUMCOLS 162

using namespace std;

int main()
{
    int image[NUMROWS][NUMCOLS];
    doBitBang(image);
    for (int i=0; i<128; i++){
        cout << image[i][1] << endl;
    }
    attitude finalAtt = determineAttitude(image);
    cout << "Roll = " << finalAtt.roll << endl;
    cout << "Pitch = " << finalAtt.pitch << endl;
    cout << "DONE" << endl;

    return 0;
}
