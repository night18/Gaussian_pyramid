#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <fstream>
using namespace std;
#define PI 3.14

vector <unsigned> ori;
vector <unsigned> gau_1;
vector <unsigned> gau_2;
vector <unsigned> gau_3;
vector <unsigned> lap;
int main(void)
{
	string filename, gau_out, lap_out;
	FILE *fr, *fw;
	char name[20];
	char magic_num1, magic_num2;
	int w, h, maxval;
	unsigned char pix;
	int z = 1;
	//start
	while(z<6)
	{
		// input the image
		filename = "camera.pgm";
		fr = fopen(filename.c_str(), "rb");
		fscanf(fr, "%c%c\n", &magic_num1, &magic_num2); //P5
		fscanf(fr, "%d %d\n", &w, &h);
		fscanf(fr, "%d\n", &maxval); //255
		cout << w*h <<endl;
		for (int i = 0; i < w*h; i++)
		{
			fscanf(fr, "%c", &pix);
			ori.push_back(pix * 256);
			gau_1.push_back(0);
			gau_2.push_back(0);
			gau_3.push_back(0);
			lap.push_back(0);
		}
		fclose(fr);
		// Get a Gaussian kernel and use it
		int sigma = 1;
		for (int i = 0; i < w; i++)
		{
			for (int j = 0; j < h; j++)
			{
				for (int m = -2; m <= 2; m++)
				{
					for (int n = -2; n <= 2; n++)
					{
						int k = i + m;
						if (k < 0) k = 0;
						if (k >= w) k = w - 1;
						int l = j + n;
						if (l < 0) l = 0;
						if (l >= h) l = h-1;
						gau_1[i * w + j] += (exp(-1.0 * (m * m + n * n) / (2 *
							sigma * sigma)) / (2 * PI * sigma * sigma)) * ori[k * w + l];
					}
				}
			}
		}
		// Downsample the image and get the next layer
		for (int i = 0; i < w / 2; i++)
		{
			for (int j = 0; j < h / 2; j++)
			{
				gau_2[i * w / 2 + j] = gau_1[(i * 2) * w + j * 2];
			}
		}
		// Upsampling the next coarser layer subtracting it from this layer of the Gaussian pyramid and get the first Laplacian layer
			for (int i = 0; i < w; i++)
			{
				for (int j = 0; j < h; j++)
				{
					gau_3[i * w + j] = gau_2[(i / 2)*(w / 2) + j/2];
					lap[i * w + j] = ori[i * w + j] - gau_3[i * w + j];
				}
			}
			//output the image
			sprintf(name, "%d", z);
			gau_out = name;
			gau_out += "_gau.pgm";
			lap_out = name;
			lap_out += "_lap.pgm";
			fw = fopen(gau_out.c_str(), "wb");
			fprintf(fw, "%c%c\n", magic_num1, magic_num2);
			fprintf(fw, "%d %d\n", w / 2, h / 2);
			fprintf(fw, "%d\n", maxval);
			for (int i = 0; i < w * h / 4; i++)
			{
				fprintf(fw, "%c", gau_2[i] / 256);
			}
			fclose(fw);
			fw = fopen(lap_out.c_str(), "wb");
			fprintf(fw, "%c%c\n", magic_num1, magic_num2);
			fprintf(fw, "%d %d\n", w, h);
			fprintf(fw, "%d\n", maxval);
			for (int i = 0; i < w * h; i++)
			{
				fprintf(fw, "%c", lap[i] / 256);
			}
			fclose(fw);
			z++;
	}
	system("pause");
	return 0;
}