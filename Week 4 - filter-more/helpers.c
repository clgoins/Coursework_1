#include "helpers.h"
#include "math.h"

RGBTRIPLE calculateSobel(RGBTRIPLE box[3][3]);


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    double average;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            average = ((double)image[i][j].rgbtBlue + (double)image[i][j].rgbtGreen + (double)image[i][j].rgbtRed) / 3.0;

            average = round(average);

            image[i][j].rgbtBlue = (int)average;
            image[i][j].rgbtGreen = (int)average;
            image[i][j].rgbtRed = (int)average;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE reversedRow[width];

    for (int i = 0; i < height; i++)
    {
        //make a copy of the row but backwards
        for (int j = 0; j < width; j++)
        {
            reversedRow[width - j - 1] = image[i][j];
        }

        //write the backwards row into image[][]
        for (int j = 0; j < width; j++)
        {
            image[i][j] = reversedRow[j];
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //for each pixel in the image, we need the average of:
    // i-1, j-1;  i-1, j;  i-1, j+1;
    // i, j-1;    i, j;    i, j+1;
    // i+1, j-1;   i+1, j;   i+1, j+1;
    //for each channel; red, green, and blue


    double redAverage = 0;
    double blueAverage = 0;
    double greenAverage = 0;
    double pixelCount = 0;

    //buffer to write our new image into
    RGBTRIPLE newImage[height][width];

    //iterate through the entire image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //count the number of pixels in the box
            pixelCount = 0;

            //reset our averages at the start of each pixel calculation
            redAverage = 0;
            blueAverage = 0;
            greenAverage = 0;

            //first loop looks at i-1. if we're on the top row, skip this.
            if (i != 0)
            {
                //add the RGB components of j-1, j, and j+1 to their respective average variables
                for (int rowOffset = -1; rowOffset < 2; rowOffset++)
                {
                    //don't look at j-1 if we're at the far left, or j+1 if we're at the far right
                    if (j + rowOffset >= 0 && j + rowOffset < width)
                    {
                        redAverage +=   image[i - 1][j + rowOffset].rgbtRed;
                        blueAverage +=  image[i - 1][j + rowOffset].rgbtBlue;
                        greenAverage += image[i - 1][j + rowOffset].rgbtGreen;
                        pixelCount++;
                    }
                }
            }

            //this loop looks at i (center row of box)
            for (int rowOffset = -1; rowOffset < 2; rowOffset++)
            {

                if (j + rowOffset >= 0 && j + rowOffset < width)
                {
                    redAverage +=   image[i][j + rowOffset].rgbtRed;
                    blueAverage +=  image[i][j + rowOffset].rgbtBlue;
                    greenAverage += image[i][j + rowOffset].rgbtGreen;
                    pixelCount++;
                }
            }

            //final loop looks at i+1. if we're on the bottom row, skip this.
            if (i != height - 1)
            {
                for (int rowOffset = -1; rowOffset < 2; rowOffset++)
                {
                    if (j + rowOffset >= 0 && j + rowOffset < width)
                    {
                        redAverage +=   image[i + 1][j + rowOffset].rgbtRed;
                        blueAverage +=  image[i + 1][j + rowOffset].rgbtBlue;
                        greenAverage += image[i + 1][j + rowOffset].rgbtGreen;
                        pixelCount++;
                    }
                }
            }

            //calculate averages and update pixel with new values
            newImage[i][j].rgbtRed = round(redAverage / pixelCount);
            newImage[i][j].rgbtGreen = round(greenAverage / pixelCount);
            newImage[i][j].rgbtBlue = round(blueAverage / pixelCount);

        }
    }


    //copy our new blurred image into the original image array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = newImage[i][j];
        }
    }


    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //buffer to write our new image into
    RGBTRIPLE newImage[height][width];

    //our grid of 9 pixels we'll use to calculate a sobel value
    RGBTRIPLE box[3][3];

    //iterate through entire image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            //iterate through box of 9 pixels to send to sobel calculator
            for (int boxX = -1; boxX < 2; boxX++)
            {
                for (int boxY = -1; boxY < 2; boxY++)
                {
                    //if we're checking outside the bounds of the image, add a black pixel to the box
                    if (i + boxX < 0 || i + boxX > height - 1 || j + boxY < 0 || j + boxY > width - 1)
                    {
                        box[1 + boxX][1 + boxY].rgbtRed = 0;
                        box[1 + boxX][1 + boxY].rgbtGreen = 0;
                        box[1 + boxX][1 + boxY].rgbtBlue = 0;
                    }
                    //otherwise add the corresponding pixel from the image
                    else
                    {
                        box[1 + boxX][1 + boxY] = image[i + boxX][j + boxY];
                    }
                }
            }

            //calculate the sobel value and add it to our image
            newImage[i][j] = calculateSobel(box);

        }
    }


    //copy our new edge-detected image into the original image array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = newImage[i][j];
        }
    }


    return;
}

RGBTRIPLE calculateSobel(RGBTRIPLE box[3][3])
{
    RGBTRIPLE sobel;
    int sobelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int sobelY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    double xSum = 0;
    double ySum = 0;
    double newRed;
    double newBlue;
    double newGreen;

    //iterate through the provided box of pixels and calculate sobel values for red channel
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            xSum += box[i][j].rgbtRed * sobelX[i][j];
            ySum += box[i][j].rgbtRed * sobelY[i][j];
        }
    }

    newRed = sqrt((xSum * xSum) + (ySum * ySum));
    //round to nearest whole number, and clamp to a max of 255
    newRed = round(newRed);
    if (newRed > 0xFF)
    {
        newRed = 0xFF;
    }


    //repeat process for green channel
    xSum = 0;
    ySum = 0;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            xSum += box[i][j].rgbtGreen * sobelX[i][j];
            ySum += box[i][j].rgbtGreen * sobelY[i][j];
        }
    }

    newGreen = sqrt((xSum * xSum) + (ySum * ySum));
    newGreen = round(newGreen);
    if (newGreen > 0xFF)
    {
        newGreen = 0xFF;
    }



    //repeat once more for blue channel
    xSum = 0;
    ySum = 0;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            xSum += box[i][j].rgbtBlue * sobelX[i][j];
            ySum += box[i][j].rgbtBlue * sobelY[i][j];
        }
    }

    newBlue = sqrt((xSum * xSum) + (ySum * ySum));
    newBlue = round(newBlue);
    if (newBlue > 0xFF)
    {
        newBlue = 0xFF;
    }


    //update sobel with the new color values and return
    sobel.rgbtRed = newRed;
    sobel.rgbtGreen = newGreen;
    sobel.rgbtBlue = newBlue;

    return sobel;
}
