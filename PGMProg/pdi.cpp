
/*****************************************************************
*
*    SCC0251 - Processamento de Imagem
*    Arquivo pdi.cpp - Funcoes implementadas
*
*    -Rodrigo das Neves Bernardi - 8066395
*
******************************************************************/

#include "pdi.hpp"



void allocData (Image *img, char* type, int width, int height, int maxVal) 
{
    strcpy(img->type, type);
    img->width = width;
    img->height = height;
    img->maxVal = maxVal;

    if(img->type[0] == 'P' && img->type[1] == '2') 
    {
        img->pixel = (uchar**) malloc(height * sizeof(uchar*));

        for (int i = 0; i < height; ++i)
        {
            img->pixel[i] = (uchar*) malloc(width * sizeof(uchar));
        }
    }
}





void freeData (Image *img) 
{    
    if (img->type[0] == 'P' && img->type[1] == '2') 
    {
        for (int i = 0; i < img->height; ++i)
        {
            free(img->pixel[i]);
        }

        free(img->pixel);
    }
}





void readImage (Image *imgIn, Image *imgOut, char *in, int bord) 
{
    FILE *input;
    char type[3];
    int maxVal, width, height;

    input = fopen(in, "r");

    if (input == NULL)
        printf("== ERROR. CAN'T READ IMAGE FILE ==\n\n");
    
    else 
    {
        // le tipo
        rewind(input);
        fscanf(input, "%s", type);

        // compara tipo para ver se é pgm (escala de cinza)
        if(type[0] == 'P' && type[1] == '2') 
        {
            // pula fim da linha
            while (getc(input) != '\n');

            // pula comentario da linha
            while (getc(input) == '#') 
            {
                while (getc(input) != '\n');
            }

            // volta um caracter
            fseek(input, -1, SEEK_CUR);

            // le dimensões da imagem e a escala das cores
            fscanf(input, "%d", &width);
            fscanf(input, "%d", &height);
            fscanf(input, "%d", &maxVal);

            // aloca as matrizes width x height das imagens de entrada e saída na memória
            allocData(imgIn, type, width, height, maxVal);
            allocData(imgOut, type, width + bord, height + bord, maxVal);
            
            // le dados do arquivo
            for(int i = 0; i < height ; ++i)
            {
                for (int j = 0; j < width; ++j)
                {
                    // fscanf(input, "%hhu %hhu %hhu", &(imgIn->data->r[i]), &(imgIn->data->g[i]), &(imgIn->data->b[i]));
                    fscanf(input, "%hhu\n", &(imgIn->pixel[i][j]));
                }
            }
        }
    }

    fclose(input);
}





void saveImage (Image *img, const char *out) 
{
    FILE *output;

    // escreve header contendo tipo, comentário, dimensões e escala
    output = fopen(out, "w");
    fprintf(output, "%s\n", img->type);
    fprintf(output, "# imagem gerada pelo programa\n");
    fprintf(output, "%d %d\n", img->width, img->height);
    fprintf(output, "%d\n", img->maxVal);

    // escreve dados da imagem 

    if(img->type[0] == 'P' && img->type[1] == '2') 
    {
        for (int i = 0; i < img->height; ++i)
        {
            for (int j = 0; j < img->width; ++j)
            {
                fprintf(output, "%hhu ", img->pixel[i][j]);
            }

            fprintf(output, "\n");
        }
    }

    fclose(output);
}





void copyImage (Image *imgIn, Image *imgOut)
{
    for(int i = 0; i < imgIn->height; ++i)
        for (int j = 0; j < imgIn->width; ++j)
        {
            imgOut->pixel[i][j] = imgIn->pixel[i][j];
        }
}





void colorPixel (uchar *pixel, uchar tone)
{
    (*pixel) = tone;
}





void processInversion (Image *imgIn, Image *imgOut)
{
    for (int i = 0; i < imgIn->height; ++i) 
        for (int j = 0; j < imgIn->width; ++j)
        {
            imgOut->pixel[i][j] = imgIn->pixel[i][j];
            colorPixel(&imgOut->pixel[i][j], imgIn->maxVal - imgOut->pixel[i][j]);
        }
}






void transposeImage (Image *img)
{
    int aux;

    for (int i = 0; i < img->height; ++i)
        for (int j = i+1; j < img->width; ++j)
            if (j != i)
            {
                aux = img->pixel[i][j];
                img->pixel[i][j] = img->pixel[j][i];
                img->pixel[j][i] = aux;
            }
}





int stringToInteger(string str)
{
    istringstream iss(str);
    int retorno;
    iss >> retorno;
    return retorno;
}




/*void copyImageBorder(Image *imgIn, Image *imgOut, int mask)
{
    for (int i = 0; i < imgIn->height; ++i)
    {
        for (int j = 0; j < imgIn->width; ++j)
        {
            imgOut = ;
        }
    }
}
*/




void mediaFilter(Image *img, unsigned int mask, int bord)
{
    int sum = 0, inck = 0, incl = 0;

    for (int i = 0; i < img->height-mask; ++i)
    {
        incl = 0;
        for (int j = 0; j < img->width-mask; ++j)
        {
            for (int k = i; k < mask+inck; ++k)
            {
                for (int l = j; l < mask+incl; ++l)
                {
                    sum += (int)img->pixel[k][l];
                }
            }

            img->pixel[i+bord/2][j+bord/2] = (int)sum/(mask*mask);

            incl++;
            sum = 0;
        }

        inck++;
    }
}





void medianaFilter(Image *img, unsigned int mask, int bord)
{
    vector<int> values;
    int value = 0, inck = 0, incl = 0;

    for (int i = 0; i < img->height-mask; ++i)
    {
        incl = 0;
        for (int j = 0; j < img->width-mask; ++j)
        {
            for (int k = i; k < mask+inck; ++k)
            {
                for (int l = j; l < mask+incl; ++l)
                {
                    values.push_back((int)img->pixel[k][l]);
                }
            }

            incl++;

            sort(values.begin(), values.end());
            value = values[floor(values.size()/2)];

            img->pixel[i+bord/2][j+bord/2] = (int)value;

            values.clear();
        }

        inck++;
    }
}





void gaussFilter(Image *img, unsigned int sigma, int bord)
{
    // tamanho da mascara
    unsigned int maskSize = (6*sigma)+1;
    double sum = 0;
    double pi = 3.14159265;

    double **kernel = (double **) calloc(maskSize, sizeof(double*));
    for (int i = 0; i < maskSize; ++i)
    {
        kernel[i] = (double *) calloc(maskSize, sizeof(double));
    }

    int average = maskSize/2;

    for (int i = 0; i < maskSize; ++i)
    {
        for (int j = 0; j < maskSize; ++j)
        {
            kernel[i][j] = exp( -0.5 * ( pow((i-average)/maskSize, 2.0) + pow((j-average)/maskSize, 2.0)) ) / ( 2 * pi * (maskSize*maskSize) );
            sum += kernel[i][j];
        }
    }

    for (int i = 0; i < maskSize; ++i)
    {
        for (int j = 0; j < maskSize; ++j)
        {
            kernel[i][j] /= sum;
        }
    }

    cout << "maskSize: " << maskSize << endl;
    cout << "bord: " << bord << endl;

    int inck = 0, incl = 0, x = 0, y = 0;
    sum = 0;

    for (int i = 0; i < img->height-maskSize; ++i)
    {
        incl = 0;
        for (int j = 0; j < img->width-maskSize; ++j)
        {
            for (int k = i; k < maskSize+inck; ++k)
            {
                for (int l = j; l < maskSize+incl; ++l)
                {
                    if (x > maskSize-1)
                    {
                        x = 0;
                    }

                    if (y > maskSize-1)
                    {
                        y = 0;
                    }

                    sum += (double)img->pixel[k][l] * kernel[x][y];

                    y++;
                }

                x++;
            }

            img->pixel[i+bord/2][j+bord/2] = (int)sum;

            incl++;
            sum = 0;
        }

        inck++;
    }

}





void saveImageBord (Image *img, const char *out, int bord) 
{
    FILE *output;

    // escreve header contendo tipo, comentário, dimensões e escala
    output = fopen(out, "w");
    fprintf(output, "%s\n", img->type);
    fprintf(output, "# imagem gerada pelo programa\n");
    fprintf(output, "%d %d\n", img->width, img->height);
    fprintf(output, "%d\n", img->maxVal);

    // escreve dados da imagem 
    if(img->type[0] == 'P' && img->type[1] == '2') 
    {
        for (int i = 0 /*bord/2*/; i < img->height/*+(bord/2)*/; ++i)
        {
            for (int j = 0/*bord/2*/; j < img->width/*+(bord/2)*/; ++j)
            {
                fprintf(output, "%hhu ", img->pixel[i][j]);
            }

            fprintf(output, "\n");
        }
    }

    fclose(output);
}





void copyImageBord (Image *imgIn, Image *imgOut, int bord)
{
    for(int i = 0; i < imgIn->height; ++i)
        for (int j = 0; j < imgIn->width; ++j)
        {
            imgOut->pixel[i+(bord/2)][j+(bord/2)] = imgIn->pixel[i][j];

            if (i == 0)
            {
                for (int k = 1; k <= bord/2; ++k)
                {
                    imgOut->pixel[i+(bord/2)-k][(j+(bord/2))] = imgIn->pixel[i][j];
                }
            }

            if (i == imgIn->height-1)
            {
                for (int k = 1; k <= bord/2; ++k)
                {
                    imgOut->pixel[i+(bord/2)+k][(j+(bord/2))] = imgIn->pixel[i][j];
                }   
            }

            if (j == 0)
            {
                for (int k = 1; k <= bord/2; ++k)
                {
                    imgOut->pixel[i+(bord/2)][j+(bord/2)-k] = imgIn->pixel[i][j];
                }
            }

            if (j == imgIn->width-1)
            {
                for (int k = 1; k <= bord/2; ++k)
                {
                    imgOut->pixel[i+(bord/2)][j+(bord/2)+k] = imgIn->pixel[i][j];
                }
            }

            if (i == 0 && j == 0)
            {
                for (int k = 1; k <= bord/2; ++k)
                {
                    for (int l = 0; l <= bord/2; ++l)
                        imgOut->pixel[i+(bord/2)-k][j+(bord/2)-l] = imgIn->pixel[i][j];
                }
            }

            if (i == 0 && j == imgIn->width-1)
            {
                for (int k = 1; k <= bord/2; ++k)
                {
                    for (int l = 0; l <= bord/2; ++l)
                        imgOut->pixel[i+(bord/2)-k][j+(bord/2)+l] = imgIn->pixel[i][j];
                }
            }

            if (i == imgIn->height-1 && j == 0)
            {
                for (int k = 1; k <= bord/2; ++k)
                {
                    for (int l = 0; l <= bord/2; ++l)
                        imgOut->pixel[i+(bord/2)+k][j+(bord/2)-l] = imgIn->pixel[i][j];
                }
            }

            if (i == imgIn->height-1 && j == imgIn->width-1)
            {
                for (int k = 1; k <= bord/2; ++k)
                {
                    for (int l = 0; l <= bord/2; ++l)
                    {
                        imgOut->pixel[i+(bord/2)+k][j+(bord/2)+l] = imgIn->pixel[i][j];
                    }
                }
            }
        }
}