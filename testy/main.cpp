#include <iostream>
#include <cmath>
#include <iomanip>
 
using namespace std;
 
void createFilter(double gKernel[][5])
{
    // set standard deviation to 1.0
    double sigma = 1.0;
    double r, s = 2.0 * sigma * sigma;
 
    // sum is for normalization
    double sum = 0.0;
 
    // generate 5x5 kernel
    for (int x = -1; x <= 0; x++)
    {
        for(int y = -2; y <= 2; y++)
        {
            r = sqrt(x*x + y*y);
            gKernel[x+1][y + 2] = (exp(-(r*r)/s))/(M_PI * s);
            sum += gKernel[x + 1][y + 2];
        }
    }
 
    // normalize the Kernel
    for(int i = 0; i < 1; ++i)
        for(int j = 0; j < 5; ++j)
            gKernel[i][j] /= sum;
 
}
 
int main()
{
    double gKernel[1][5];
    createFilter(gKernel);
    for(int i = 0; i < 1; ++i)
    {
        for (int j = 0; j < 5; ++j)
            cout<<gKernel[i][j]<<"\t";
        cout<<"X\n" <<endl;
        return 1;
    }
}
