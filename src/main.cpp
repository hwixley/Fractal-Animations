#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>

using namespace std;

int findMandelBrot(double cr, double ci, int max_iters)
{
    int i = 0;
    double zr = 0.0, zi = 0.0;
    while (i < max_iters && zr * zr + zi * zi < 4.0)
    {
        double temp = zr * zr - zi * zi + cr;
        zi = 2.0 * zr * zi + ci;
        zr = temp;
        i++;
    }
    return i;
}

double mapToReal(int x, int imageWidth, double minR, double maxR)
{
    double range = maxR - minR;
    return x * (range / imageWidth) + minR;
}

double mapToImaginary(int y, int imageHeight, double minI, double maxI)
{
    double range = maxI - minI;
    return y * (range / imageHeight ) + minI;
}

int main()
{
    // Get the required input values from file...
    ifstream fin("input.txt");
    int imageWidth, imageHeight, maxN;
    double minR, maxR, minI, maxI;

    if (!fin)
    {
        cout << "Could not open file!" << endl;
        cin.ignore();
        return 0;
    }
    fin >> imageWidth >> imageHeight >> maxN;
    fin >> minR >> maxR >> minI >> maxI;
    fin.close();

    // Open the output file, write the PPM header
    ofstream fout("output_image.ppm");
    fout << "P3" << endl; // PPM magic number
    fout << imageWidth << " " << imageHeight << endl; // Image dimensions
    fout << "256" << endl; // Max RGB value

    // For each pixel
    for (int y = 0; y < imageHeight; y++)
    {
        for (int x = 0; x < imageWidth; x++)
        {
            //  1)  Find the real and imaginary parts of c, corresponding
            //      to that x,y pixel in the image
            double cr = mapToReal(x, imageWidth, minR, maxR);
            double ci = mapToImaginary(y, imageHeight, minI, maxI);

            //  2)  Find the number of iterations in the Mandelbrot formula
            //      using said c
            int n = findMandelBrot(cr, ci, maxN);

            //  3)  Map the resulting number to an RGB value
            int r = ((int)(n * sinf(n)) % 256);
            int g = ((n * n) % 256);
            int b = (n % 256);

            //  4)  Output it to the image
            fout << r << " " << g << " " << b << " ";
        }
        fout << endl;   
    }
    fout.close();
    cout << "Finished!" << endl;
    return 0;
}