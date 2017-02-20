#include <cstdio>
#include <iostream>
using namespace std;
#define _USE_MATH_DEFINES
#include <cmath>

void GaussianKernel(const int sigma);
float** mypyrdown(float **origin, int mypr_height, int mypr_width);
float** scaledown(float **origin, int scale_height, int scale_width);
float** getlap(float **bigger, float **small, int lap_height, int lap_width);
void savepgm(float **save_matrix,int save_height, int save_width,string title);

static float kernel[5][5];
static int counter = 1;
char pic_type1,pic_type2;
int maxvalue;
static float** pic_src;
FILE *fileReader, *filewriter;

int main(){
	string filename;
	unsigned char pixel;
	int width, height;

	// input the image
	filename = "bubble.pgm";
	fileReader = fopen(filename.c_str(), "rb");
	fscanf(fileReader, "%c%c\n", &pic_type1, &pic_type2); //P5 binary grayline
	fscanf(fileReader, "%d %d\n", &width, &height);
	fscanf(fileReader, "%d\n", &maxvalue); //0:black 255:white

	//initialize the matrix
	pic_src = new float *[height];
	for(int i =0; i < height; i++){
		pic_src[i] = new float[width];
	}
	
	//put image to a matrix
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			fscanf(fileReader, "%c", &pixel);
			pic_src[i][j] = pixel *256;
		}
	}
	fclose(fileReader);
	
	//create Gaussian kernel in the 5*5 matrix with sigma = 1
    GaussianKernel(1);

	//create 5 layer Gaussian and Laplacian pyramid
	for(counter= 1; counter <6 ; counter++){
		pic_src = mypyrdown(pic_src,height,width);
		height /= 2;
		width /= 2;
	}
	
    return 0;
}


/*
Create a Gaussian filter
Input: sigma
Output: Gaussian Filter
*/ 
void GaussianKernel(int sigma){
	double sum = 0;
	for(int i = -2; i < 3; i++){
		for(int j = -2; j < 3; j++){
			//calculate each value of Gaussian kernel, and use 2.0 to casting the int to double
			double kernel_value = exp (-(i * i +j * j ) / (2.0 * sigma * sigma )) / (2 * M_PI * sigma * sigma);
			kernel[i+2][j+2] = kernel_value;
			sum += kernel_value;
		}
	}

	//make the sum of the matrix is 1
	for(int i = 0; i < 5; i++){
		cout << "[";
		for(int j = 0; j < 5; j++){
			kernel[i][j] = kernel[i][j] / sum;
			cout << kernel[i][j] << " ";
		}
		cout << "]" << endl;
	}
}

/*
Create next level of Gaussian and Laplacian pyramid.
intput: **origin: previos level of gaussian, mypr_height: height of gaussian, mypr_width: width of gaussain
output: next level of gaussian
*/
float** mypyrdown(float **origin, int mypr_height, int mypr_width){

	float **target = new float *[mypr_height];
	for(int i = 0 ; i < mypr_height ; i++){
		target[i] = new float[mypr_width];
		for(int j = 0 ; j < mypr_width; j++){
			target[i][j] = 0;
		}
	}

	//use gaussian filter
	for(int picture_row = 0; picture_row < mypr_height ; picture_row++){
		for(int picture_col = 0; picture_col < mypr_width; picture_col++){
			
			if(target[picture_row][picture_col] != target[picture_row][picture_col]){
				cout << picture_row << "  " << picture_col << target[picture_row][picture_col] << endl;
			}

			for(int kernel_row = -2; kernel_row < 3 ; kernel_row ++){
				int effect_row = picture_row + kernel_row;

				//reflect accross edge to modify the edge

				if(effect_row < 0) effect_row = -effect_row;
				if(effect_row >= mypr_height) effect_row = mypr_height - 1 -(effect_row- mypr_height);

				for(int kernel_col = -2; kernel_col < 3 ; kernel_col++){
					int effect_col =  picture_col + kernel_col;
					if(effect_col < 0) effect_col = -effect_col;
					if(effect_col >= mypr_width) effect_col = mypr_width - 1 - (effect_col - mypr_width);

					float cell = origin[effect_row][effect_col]* kernel[kernel_row+2][kernel_col+2];

					target[picture_row][picture_col] += cell ;
				}
			}
		}
	}

	//next level of gaussain
	float** gaus = scaledown(target,mypr_height/2,mypr_width/2 );
	//next level of laplacian
	float** lapl = getlap(target,gaus,mypr_height,mypr_width);
	return gaus;

}

/*
scale down the gaussianized picture
intput: **origin: previos level of gaussian, mypr_height: height of gaussian, mypr_width: width of gaussain
output: next level of gaussian
*/
float** scaledown(float **origin, int scale_height, int scale_width){
	float **small = new float *[scale_height];
	for(int i = 0 ; i < scale_height ; i++){
		small[i] = new float[scale_width];
		for(int j = 0 ; j < scale_width ; j++){
			small[i][j] = 0;
		}
	}

	for(int i =0; i < scale_height ;i++){
		for(int j =0 ; j < scale_width; j++){
			small[i][j] = origin[i*2][j*2];
		}
	}

	char counter_text[1];
	string title;
	sprintf(counter_text, "%d", counter);
	title = "gaussian_";
	title += counter_text;
	title +=".pgm";
	
	savepgm(small, scale_height, scale_width,title);

	return small;
}

/*
get the next level of laplacian
intput: **bigger: previos level of gaussian, **small: next level of gaussian, lap_height: height of laplacian, lap_width: width of laplacian
output: next level of laplacian
*/
float** getlap(float **bigger, float **small, int lap_height, int lap_width){
	float **lap = new float *[lap_height];
	for(int i = 0 ; i < lap_height ; i++){
		lap[i] = new float[lap_width];
		for(int j = 0 ; j < lap_width ; j++){
			lap[i][j] = 0;
		}		
	}

	for(int i =0; i < lap_height ;i++){
		for(int j =0 ; j < lap_width; j++){
			lap[i][j] = bigger[i][j] - small[i/2][j/2];
		}
	}

	char counter_text[1];
	string title;
	sprintf(counter_text, "%d", counter);
	title = "laplacian_";
	title += counter_text;
	title +=".pgm";
	
	savepgm(lap, lap_height, lap_width,title);

	return lap;

}

/*
save the picture
intput: **save_matrix: picture, save_height: height of laplacian, save_width: width of laplacian, title: file name
*/
void savepgm(float **save_matrix,int save_height, int save_width,string title){
	filewriter = fopen(title.c_str(), "wb");
	fprintf(filewriter, "%c%c\n", pic_type1, pic_type2);
	fprintf(filewriter, "%d %d\n", save_width, save_height);
	fprintf(filewriter, "%d\n", maxvalue);
	for(int i = 0 ; i < save_height; i++){
		for(int j = 0; j < save_width; j++ ){
			fprintf(filewriter, "%c",(int) save_matrix[i][j]/256);
			if(counter == 1 && i == 0){
				// cout << (int)save_matrix[i][j]/256 << ",";
			}
		}
	}
	fclose(filewriter);
}