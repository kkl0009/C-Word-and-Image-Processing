/*
* Name: Kollin Labowski
* Date: October 27, 2020
* Course: CS 350
* Description: This program takes in an input image and outputs 6 images. The first 3 of these images use edge detection
* and the latter 3 use a threshold to binarize the images. See code below for more thorough explanation on the functionality
* of each section.
*/

//Include statements
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Constant for determining array sizes
const int MASK_SIZE = 3;

int SIZE, TYPE;

typedef unsigned char *image_ptr;

//Function prototypes
image_ptr readImage(char *fName, int *size, int *type);
int getNum(FILE *myPtr);
void writeImage(image_ptr imgPtr, char *fName, int size, int keyNum);
int maskComputeH(int arr[][3]);
int maskComputeV(int arr[][3]);

/*
* Main method: This method finds the file names to read/write to and performs necessary processing
* using some additional functions
*/
void main(int argc, char **argv)
{
	//Determine whether the correct amount of arguments are input
	if(argc != 8)
	{
		printf("Incorrect input format: use %s imageIn.ppm outEh.ppm outEv.ppm outE.ppm outBh.ppm outBv.ppm outB.ppm", argv[0]);
		return;
	}

	int size, type;

	//READ INPUT IMAGE

	image_ptr imagePtr = readImage(argv[1], &size, &type);

	unsigned char imageA[size][size];

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			int temp[MASK_SIZE][MASK_SIZE];
			for(int k = 0; k < MASK_SIZE; k++)
			{
				if(i - 1 < 0 || i + 1 >= size || j - 1 < 0 || j + 1 >= size)
					break;

				for(int l = 0; l < MASK_SIZE; l++)
				{
					temp[k][l] = imagePtr[(i - 1 + k) * size + (j - 1 + l)];
				}
			}
			int tempInt = maskComputeH(temp);
			imageA[i][j] = tempInt;
		}
	}

	//BEGIN FIRST OUTPUT
	//Edge detection horizontally

	image_ptr imagePtr2 = (image_ptr) imageA;

	writeImage(imagePtr2, argv[2], size, type);

	unsigned char imageB[size][size];

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			int temp[MASK_SIZE][MASK_SIZE];
			for(int k = 0; k < MASK_SIZE; k++)
			{
				if(i - 1 < 0 || i + 1 >= size || j - 1 < 0 || j + 1 >= size)
					break;

				for(int l = 0; l < MASK_SIZE; l++)
				{
					temp[k][l] = imagePtr[(i - 1 + k) * size + (j - 1 + l)];
				}
			}
			int tempInt = maskComputeV(temp);
			imageB[i][j] = tempInt;
		}
	}

	//BEGIN SECOND OUTPUT
	//Edge detection vertically

	image_ptr imagePtr3 = (image_ptr) imageB;

	writeImage(imagePtr3, argv[3], size, type);

	unsigned char imageC[size][size];

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			int tempInt = imageA[i][j] + imageB[i][j];
			imageC[i][j] = tempInt;
		}
	}

	//BEGIN THIRD OUTPUT
	//Edge detection combined AND gather information for images 4-6

	image_ptr imagePtr4 = (image_ptr) imageC;

	writeImage(imagePtr4, argv[4], size, type);

	//Used for binarized images
	int avgI = 0;
	int avgA = 0;
	int avgB = 0;
	int avgC = 0;

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			avgI += imagePtr[(i * size) + j];
			avgA += imageA[i][j];
			avgB += imageB[i][j];
			avgC += imageC[i][j];
		}
	}

	avgI /= (size * size);
	avgA /= (size * size);
	avgB /= (size * size);
	avgC /= (size * size);

	//Used for binarized images
	int stdevI = 0;
	int stdevA = 0;
	int stdevB = 0;
	int stdevC = 0;

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			stdevI += ((imagePtr[i * size + j] - avgI) * (imagePtr[i * size + j] - avgI));
			stdevA += ((imageA[i][j] - avgA) * (imageA[i][j] - avgA));
			stdevB += ((imageB[i][j] - avgB) * (imageB[i][j] - avgB));
			stdevC += ((imageC[i][j] - avgC) * (imageC[i][j] - avgC));
		}
	}

	stdevI /= ((size * size) - 1);
	stdevI = (int)sqrt(stdevI);
	stdevA /= ((size * size) - 1);
	stdevA = (int)sqrt(stdevA);
	stdevB /= ((size * size) - 1);
	stdevB = (int)sqrt(stdevB);
	stdevC /= ((size * size) - 1);
	stdevC = (int)sqrt(stdevC);

	int tA = avgA + stdevA;
	int tB = avgB + stdevB;
	int tC = avgC + stdevC;

	//BEGIN OUTPUTS 4-6
	//Binarized images that are horizontal, vertical, and combined

	unsigned char imageD[size][size];
	unsigned char imageE[size][size];
	unsigned char imageF[size][size];

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			if(imageA[i][j] >= tA)
				imageD[i][j] = 232;
			else
				imageD[i][j] = 0;
			if(imageB[i][j] >= tB)
				imageE[i][j] = 232;
			else
				imageE[i][j] = 0;
			if(imageC[i][j] >= tC)
				imageF[i][j] = 232;
			else
				imageF[i][j] = 0;
		}
	}

	image_ptr imagePtr5 = (image_ptr) imageD;
	image_ptr imagePtr6 = (image_ptr) imageE;
	image_ptr imagePtr7 = (image_ptr) imageF;

	writeImage(imagePtr5, argv[5], size, type);
	writeImage(imagePtr6, argv[6], size, type);
	writeImage(imagePtr7, argv[7], size, type);

	printf("\nInput Average: %d, Input Standard Deviation: %d\n", avgI, stdevI);
	printf("Edge Response H Average: %d, Edge Response H Standard Deviation: %d\n", avgA, stdevA);
	printf("Edge Response V Average: %d, Edge Response V Standard Deviation: %d\n", avgB, stdevB);
	printf("Edge Response Combined Average: %d, Edge Response Combined Standard Deviation: %d\n", avgC, stdevC);
}
//End main method

/*
* This method reads in an image from a given file. It is used once by the main method to
* read the image at the input file name and prepare it for processing
*/
image_ptr readImage(char *fName, int *size, int *type)
{
	FILE *myPtr;

	if((myPtr = fopen(fName, "rb")) == NULL)
	{
		printf("Unable to open file: %s\n", fName);
		exit(1);
	}

	char charOne, charTwo;

	charOne = getc(myPtr);
	charTwo = getc(myPtr);

	if(charOne != 'P')
	{
		printf("Input file is NOT a PPM file\n");
		exit(1);
	}

	*size = getNum(myPtr);
	*type = charTwo - '0';

	SIZE = *size;
	TYPE = *type;

	int maxValue;
	float scale;

	switch(charTwo)
	{
		case '4':
			scale = 0.125;
			maxValue = 1;
			break;
		case '5':
			scale = 1.0;
			maxValue = getNum(myPtr);
			break;
		case '6':
			scale = 3.0;
			maxValue = getNum(myPtr);
			break;
		default :
			printf("This is not a RAWBITS file\n");
			exit(1);
			break;
	}

	int rowSize = (*size) * scale;
	unsigned long totalSize = (unsigned long)(*size) * rowSize;

	image_ptr ptr = (image_ptr) malloc(totalSize);

	if(ptr == NULL)
	{
		printf("Unable to malloc for %lu bytes\n", totalSize);
		exit(1);
	}

	unsigned long totalBytes = 0;
	unsigned long offset = 0;

	for(int i = 0; i < (*size); i++)
	{
		totalBytes += fread(ptr + offset, 1, rowSize, myPtr);
		offset += rowSize;
	}

	if(totalSize != totalBytes)
	{
		printf("Failed to read %ld bytes\nRead %ld bytes\n", totalSize, totalBytes);
		exit(1);
	}

	fclose(myPtr);
	return ptr;
}

/*
* A helper function used by the above readImage function
*/
int getNum(FILE *myPtr)
{
	char ch;

	do
	{
		ch = getc(myPtr);
	} while((ch == ' ') || (ch == '\t') || (ch == '\n') || (ch == '\r'));

	if((ch < '0') || (ch > '9'))
	{
		if(ch == '#')
		{
			while(ch == '#')
			{
				while(ch != '\n')
					ch = getc(myPtr);
				ch = getc(myPtr);
			}
		}
		else
		{
			printf("ASCII fields contain garbage\n");
			exit(1);
		}
	}

	int ind = 0;

	do
	{
		ind = ind * 10 + (ch - '0');
		ch = getc(myPtr);
	} while((ch >= '0') && (ch <= '9'));

	return ind;
}

/*
* A method which writes an image to a file with an input file name. Called by the main method each time
* a new processed image has been created and needs to be added to a file.
*/
void writeImage(image_ptr imgPtr, char *fName, int size, int keyNum)
{
	float scale;
	switch(keyNum)
	{
		case 4:
			scale = 0.125;
			break;
		case 5:
			scale = 1.0;
			break;
		case 6:
			scale = 3.0;
			break;
		default :
			printf("Output dile is not a RAWBITS file\n");
			exit(1);
			break;
	}

	FILE *myPtr;

	if((myPtr = fopen(fName, "wb")) == NULL)
	{
		printf("Unable to open %s file to ouput\n", fName);
		exit(1);
	}

	fprintf(myPtr, "P%d\n%d %d\n", keyNum, size, size);
	if(keyNum != 4)
		fprintf(myPtr, "255\n");

	int rowSize = size * scale;
	long totalSize = (long) rowSize * size;
	long offset = 0;
	long totalBytes = 0;

	for(int i = 0; i < size; i++)
	{
		totalBytes += fwrite(imgPtr + offset, 1, rowSize, myPtr);
		offset += rowSize;
	}

	if(totalBytes != totalSize)
		printf("Tried to write %ld bytes but instead wrote %ld\n", totalSize, totalBytes);

	fclose(myPtr);
}

/*
* Used in the main method to compute the value at each point in the image matrix. This uses Sobel
* Edge Detection, with a horizontal mask which is shown below as maskH
*/
int maskComputeH(int arr[][3])
{
	int maskH[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
	int sum = 0;

	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			sum += (maskH[i][j] * arr[i][j]);
		}
	}

	return abs(sum);
}

/*
* Used in the main method to compute the value at each point in the image matrix. This uses Sobel
* Edge Detection, with a vertical mask which is shown below as maskV
*/
int maskComputeV(int arr[][3])
{
	int maskV[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
	int sum = 0;

	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			sum += (maskV[i][j] * arr[i][j]);
		}
	}

	return abs(sum);
}
//End of program