//
//  main.c
//  Noise reduction using fourier transforms
//
//  Created by Sanjiv Soni on 13/09/16.
//  Copyright © 2016 Sanjiv Soni. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include "csv.h"
#include "fft.cpp"

#define TRUE 1
#define FALSE 0
#define ERROR -1;

#define MAX_FILE_LINES 100

int countLines(const char *fileName)
{
    FILE *file = fopen(fileName, "r");
    
    int lines = 0;
    char buffer[256];
    if(file)
    {
        while(fgets(buffer, sizeof(buffer), file) != NULL)
            lines++;
    
        return lines;
    }
    else
    {
        perror(fileName);
        return ERROR;
    }
}

int writeToFile(char const* fileName, double *data, int size)
{
    FILE *file = fopen(fileName, "w");
    
    if(file)
    {
        
        fprintf(file, "Noise Free \n");
        for (int i=0; i<size; i++)
        {
            fprintf(file, "%f\n", data[2*i+1]);
        }
        fclose(file);
        return 1;
    }
    else
    {
        perror(fileName);
        exit(0);
    }
}


double* readFile(const char* fileName, int *lines, double *dataArray, int column)
{
    FILE *file = fopen(fileName, "r");
    
    if(file)
    {
        // Valid File
        char buffer[1024];
        char *pendingString = NULL, **parsedData = NULL, **iterator;
        double data = 0.0;
        int lineIsValid = TRUE, i = 0,j=0, validColumn = FALSE, firstPass = FALSE;
        
        int NFFT = (int)pow(2.0, ceil(log((double)*lines)/log(2.0)));
        dataArray = (double *)malloc((NFFT*2+1)* sizeof(double));
        
        while(fgets(buffer, sizeof(buffer), file) != NULL)
        {
            //printf("%s\n",buffer);
            parsedData = parse_csv(buffer);
            lineIsValid = FALSE;
            
            for (iterator = parsedData; *iterator; iterator++)
            {
                data = strtod(*iterator, &pendingString);
                
                //printf("%f\n",data);
                // Valid Float
                if(*pendingString == NULL)
                {
                    j++;
                    
                    lineIsValid = TRUE;
  
                    if(j == column)
                    {
                        validColumn = TRUE;
                        dataArray[i++] = data;
                        //printf("%f\n",dataArray[i-1]);
                    }
                    
                }
            }
            // Invalid input
            if(validColumn == FALSE && firstPass == TRUE)
            {
                printf(" Column: %d out of bounds in file '%s'. Column should be in range 1 - %d \n",column,fileName, j);
                exit(0);
                
            }
            
            firstPass = TRUE;
            
            j=0;
        }
        
        // Free buffers
        free_csv_line(parsedData);
        fclose(file);
        
        *lines = i;
        return dataArray;
    }
    else
    {
        perror(fileName);
        return NULL;
    }
}

void printPaddedArray(double *data, int size)
{
    for(int i=0; i<size; i++)
    {
        printf("(%.6f + %.6f*%%i)\n", data[2*i+1], data[2*i+2]);
    }
    printf("\n");
}

void printPaddedArrayReal(double *data, int size)
{
    for(int i=0; i<size; i++)
        printf("(%.6f)\n", data[2*i+1]);

    printf("\n");
}

double* padZerosToInput(double *dataArray, int *size)
{
    int  i=0;
    int newSize = (int)pow(2.0, ceil(log((double)*size)/log(2.0)));
    
    double *temp = (double*)malloc(*size*sizeof(double));
    
    //printf(" New Size with padding  %d\n",newSize);
    
    for(i = 0; i < *size ; i++)
    {
        temp[i] = dataArray[i];
    }
    for(i = 0; i < *size ; i++)
    {
        dataArray[2*i+1] = temp[i];
        dataArray[2*i+2] = 0.0;
    }
    for(i=*size; i<newSize; i++)
    {
        dataArray[2*i+1] = 0.0;
        dataArray[2*i+2] = 0.0;
    }
    
    //(dataArray,newSize);
    *size = newSize;
    
    // Free temp
    realloc(temp, 0);
    return dataArray;
}

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        printf("Invalid Number of Arguments. Try again\n Proper Usage: noisefreefft.out signal.csv noise.csv noiseFree.csv 3\n");
        exit(0);
    }
    char *fileSignalName = argv[1];
    char *fileNoiseName = argv[2];
    char *outputFileName = argv[3];
    int column = atoi(argv[4]);

    double *signal, *noise;
    int i=0;
    
    int samples = countLines(fileSignalName);
    if(samples != countLines(fileNoiseName))
    {
        printf("Files Incompatible \n");
        exit(0);
    }
    else if(samples == 0)
    {
        printf("Input files Empty. Please add samples to process.\n");
        exit(0);
    }
    
    signal = readFile(fileSignalName, &samples, signal, column);
    noise = readFile(fileNoiseName, &samples, noise, column);


    signal = padZerosToInput(signal, &samples);
    noise = padZerosToInput(noise, &samples);
    
    four1(signal, samples, 1);
    
    //printf("\nFFT Signal\n");
    //printPaddedArray(signal,samples);
    
    four1(noise, samples, 1);

    // fft(signal) - fft(noise)
    for(i=0; i<samples ;i++)
    {    signal[2*i+1] = signal[2*i+1] - noise[2*i+1];
         signal[2*i+2] = signal[2*i+2] - noise[2*i+2];
    }
    
    // Normalize values
    for(i=0; i<samples; i++)
    {
        signal[2*i+1] /= samples;
        signal[2*i+2] /= samples;
    }

    //printf("ffr diff\n");
    //printPaddedArray(signal, samples);
    
    // ifft
    four1(signal, samples, -1);
    writeToFile(outputFileName, signal, samples);
    //printPaddedArrayReal(signal, samples);
    printf("Successfully written %d Noise free values for column %d to %s file\n", samples,column, outputFileName);
    return 1;
}