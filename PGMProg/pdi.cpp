
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





void readImage (Image *imgIn, Image *imgOut, char *in) 
{
    FILE *input;
    char type[3];
    int maxVal, width, height;

    input = fopen(in, "r");

    if (input == NULL)
        printf("Can't read image file.\n");
    
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
            allocData(imgOut, type, width, height, maxVal);
            
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
    fprintf(output, "#imagem gerada pelo programa\n");
    fprintf(output, "%d %d\n", img->width, img->height);
    fprintf(output, "%d\n", img->maxVal);

    // escreve dados da imagem 

    if(img->type[0] == 'P' && img->type[1] == '2') 
    {
        for (int i = 0; i < img->height; ++i)
        {
            for (int j = 0; j < img->width; ++j)
            {
                fprintf(output, "%hhu\n", img->pixel[i][j]);
            }
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





double averageCalc (Image *img, double *percent_vec, int n)
{    
    double sum=0;
    
    for (int i = 0; i < img->maxVal; ++i)
    {
        if(percent_vec[i] > 0) 
        {
            sum += (i*percent_vec[i]);
        }
    }

    return sum / n;
}





double standardDeviation (Image *img, double *percent_vec, double *average, int n) 
{
    double dp=0;

    for(int i=0; i <= img->maxVal; ++i)
    {
        if(percent_vec[i] > 0)
            dp += pow((percent_vec[i]-(*average)), 2);
    }

    return sqrt(dp / (n-1));
}





void analyzeFrequency (Image *img, double *percent_vec, double *average, double *stand_dev) 
{    
    int total=0;

    // Contagem dos pixels da imagem
    for(int i=0; i<=img->maxVal; i++)
        percent_vec[i] = 0;

    for(int i = 0; i < img->height; ++i)
        for (int j = 0; j < img->width; ++j)
        {
            percent_vec[(int)img->pixel[i][j]]++;

            if(percent_vec[(int)img->pixel[i][j]] > 0)
                total++;
        }

    // Calcula media aritmetica dos valores dos pixels
    printf("\nCalculating average...\n");
    
    *average = averageCalc(img, percent_vec, total);
    printf("Done.\n");


    // Calcula desvio padrao dos valores dos pixels
    printf("\nCalculating standard deviation...\n");
    
    *stand_dev = standardDeviation(img, percent_vec, average, total);
    printf("Done.\n");


    // Calcula frequencia dos valores dos pixels
    printf("\nCalculating frequency...\n");

    for(int i = 0; i<=img->maxVal; i++)
        percent_vec[i] = (percent_vec[i] / total)*100;

    printf("Done.\n");
}





void floodFill (Image *img, unsigned int x, unsigned int y, uchar target_tone, uchar replacement_tone)
{
    unsigned int index; uchar this_pixel;

    // confere se o pixel existe, se nao retorna 
    if((x >= 0 && x < img->width) && (y >= 0 && y < img->height))
    {
        // indexa pixel
        //index = ((img->width*y)+x);
        this_pixel = img->pixel[y][x];

        // se ja estiver colorido com o tom de reposicao, retorna
        if(target_tone == replacement_tone) return;

        // se o pixel atual nao tem a mesma cor dos outros, retorna
        if(this_pixel != target_tone) return;
        
        colorPixel(&img->pixel[y][x], replacement_tone);
    
        floodFill(img, x,  y+1,     target_tone,   replacement_tone); // proximo pixel acima
        floodFill(img, x,  y-1,     target_tone,   replacement_tone); // proximo pixel abaixo
        floodFill(img, x+1,    y,   target_tone,   replacement_tone); // proximo pixel a direita
        floodFill(img, x-1,    y,   target_tone,   replacement_tone); // proximo pixel a esquerda
    }

    else return;

    return;
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





unsigned char **findObject (Image *img, int *obj_pos)
{
    uchar **obj, color = 255;

    obj_pos[0] = -1; // linha de inicio
    obj_pos[1] = -1; // linha de fim
    obj_pos[2] = -1; // coluna de inicio
    obj_pos[3] = -1; // coluna de fim
    
    int k = 0;

    cout << "\n\n1- k: " << k << "\nAux: " << " " << obj_pos[0] << " " << obj_pos[1] << " " << obj_pos[2] << " " << obj_pos[3] << endl;

    // marca inicio e fim do objeto na imagem no vetor auxiliar
    // -> extremo norte
    for (int i = 0; i < img->height; ++i)
    {
        for (int j = 0; j < img->width; ++j)
        {
            if (img->pixel[i][j] < color && obj_pos[0] < 0)
            {
                obj_pos[0] = i;
                break;
            }
        }

        if (obj_pos[0] != -1)
        {
            ++k;
            break;
        }
    }

    cout << "\n\n2- k: " << k << "\nAux: " << " " << obj_pos[0] << " " << obj_pos[1] << " " << obj_pos[2] << " " << obj_pos[3] << endl;

    // -> extremo sul
    for (int i = img->height-1; i >= 0; --i)
    {
        for (int j = img->width-1; j >= 0; --j)
        {
            if (img->pixel[i][j] < color && obj_pos[1] == -1)
            {
                obj_pos[1] = i;
                break;
            }
        }

        if (obj_pos[1] != -1)
        {
            ++k;
            break;
        }
    }

    cout << "\n\n3- k: " << k << "\nAux: " << " " << obj_pos[0] << " " << obj_pos[1] << " " << obj_pos[2] << " " << obj_pos[3] << endl;

    // -> extrema esquerda
    for (int i = 0; i < img->width; ++i)
    {
        for (int j = 0; j < img->height; ++j)
        {
            if (img->pixel[i][j] < color && obj_pos[2] == -1)
            {
                obj_pos[2] = i;
                break;
            }
        }

        if (obj_pos[2] != -1)
        {
            ++k;
            break;
        }
    }

    cout << "\n\n4- k: " << k << "\nAux: " << " " << obj_pos[0] << " " << obj_pos[1] << " " << obj_pos[2] << " " << obj_pos[3] << endl;

    // -> extrema direita
    for (int i = img->width-1; i >= 0; --i)
    {
        for (int j = img->height-1; j >= 0; --j)
        {
            if (img->pixel[i][j] < color && obj_pos[3] == -1)
            {
                obj_pos[3] = i;
                break;
            }
        }

        if (obj_pos[3] != -1)
        {
            ++k;
            break;
        }
    }    

    cout << "\n\n5- k: " << k << "\nAux: " << " " << obj_pos[0] << " " << obj_pos[1] << " " << obj_pos[2] << " " << obj_pos[3] << endl;

    for (int i = obj_pos[2]; i <= obj_pos[3]+1; ++i)
    {
        img->pixel[i][obj_pos[0]] = 150;
        img->pixel[i][obj_pos[1]] = 150;
    }

    for (int j = obj_pos[0]; j <= obj_pos[1]+1; ++j)
    {
        img->pixel[obj_pos[2]][j] = 150;
        img->pixel[obj_pos[3]][j] = 150;
    }

    saveImage(img, "obj_marked.pgm");

    int h = obj_pos[1]-obj_pos[0]+2;
    obj = (uchar **) malloc (h * sizeof(uchar *));


    int w = obj_pos[3]-obj_pos[2]+2;
    for (int i = 0; i <= h; ++i)
        obj[i] = (uchar *) malloc (w * sizeof(uchar));

    
    int count = 0;

    //copia o objeto em uma submatriz e retorna o objeto;
    if (obj != NULL)
    {
        for (int i = obj_pos[2]; i < obj_pos[3]; ++i)
        {
            for (int j = obj_pos[0]; j < obj_pos[1]; ++j)
            {
                obj[i-obj_pos[2]][j-obj_pos[0]] = img->pixel[i][j];
            }
        }
    }

    cout << "\n\nFINAL- count: " << count << " h: " << h << " w: " << w << " k: " << k 
    << "\nAux: " << " " << obj_pos[0] << " " << obj_pos[1] << " " << obj_pos[2] << " " << obj_pos[3] << endl;

    return obj;
}





void MH (Image *img, char direction, int pixels)
{
    uchar **obj;
    int *pos = (int *) malloc (4 * sizeof(int));

    obj = findObject(img, pos); // retorna o objeto da imagem e um vetor de posicoes

    /**
     * pos[0] = -1; // linha de inicio
     * pos[1] = -1; // linha de fim
     * pos[2] = -1; // coluna de inicio
     * pos[3] = -1; // coluna de fim
     */
    
    cout << "vector pos: " << pos[0] <<  " " << pos[1] << " " << pos[2] << " " << pos[3] << endl;

    if (obj != NULL)
    {
        if (pos[3]+pixels <= img->width && pos[2]+pixels >= 0)
        {
            printf("Moving...\n");
            // apaga imagem anterior
            for (int i = pos[0]; i <= pos[1]; ++i)
                for (int j = pos[2]; j <= pos[3]; ++j)
                    img->pixel[i][j] = 255;

            // reescreve imagem
            for (int i = pos[0]; i <= pos[1]; ++i)
                for (int j = pos[2]; j <= pos[3]; ++j)
                    img->pixel[i][j+pixels] = obj[i-pos[0]+2][j-pos[2]+2];
        }

        else printf("== ERROR. CAN'T MOVE THE TURTLE BEYOND THE IMAGE SIZE.\n");
    }

    // else if (obj != NULL)
    // {
    //     if (pos[2]+pixels >= 0)
    //     {
    //         int s = pos[2]+pixels;

    //         cout << "==== s: " << s << endl;

    //         printf("Moving to the left...\n");
    //         // apaga imagem anterior
    //         for (int i = pos[2]; i <= pos[3]; ++i)
    //             for (int j = pos[0]; j <= pos[1]; ++j)
    //                 img->pixel[i][j] = 255;

    //         // reescreve imagem
    //         for (int i = pos[0]; i <= pos[1]; ++i)
    //             for (int j = pos[2]; j <= pos[3]; ++j)
    //                 img->pixel[i][j+pixels] = obj[i-pos[0]][j-pos[2]];
    //     }

    //    else printf("== ERROR. CAN'T MOVE THE TURTLE BEYOND THE IMAGE SIZE.\n");
    // }

    else printf("\n== ERROR. OBJECT NOT FOUND.\n\n");

    return;
}





void MV (Image *img, char direction, int pixels)
{
    uchar **obj;
    int *pos = (int *) malloc (4 * sizeof(int));

    obj = findObject(img, pos);

    cout << "vector pos: " << pos[0] <<  " " << pos[1] << " " << pos[2] << " " << pos[3] << endl;

    if (direction == '+' && obj != NULL)
    {
        if (pos[0]-pixels >= 0)
        {
            // apaga imagem anterior
            for (int i = pos[2]; i < pos[3]; ++i)
                for (int j = pos[0]; j < pos[1]; ++j)
                    img->pixel[i][j] = 255;

            // reescreve imagem
            for (int i = pos[2]; i < pos[3]; ++i)
                for (int j = pos[0]; j < pos[1]; ++j)
                    img->pixel[i-pixels][j] = obj[i-pos[2]][j-pos[0]];
        }
    }

    else if (direction == '-' && obj != NULL)
    {
        if (pos[1]+pixels <= img->height)
        {
            // apaga imagem anterior
            for (int i = pos[2]; i < pos[3]; ++i)
                for (int j = pos[0]; j < pos[1]; ++j)
                    img->pixel[i][j] = 255;

            // reescreve imagem
            for (int i = pos[2]; i < pos[3]; ++i)
                for (int j = pos[0]; j < pos[1]; ++j)
                    img->pixel[i+pixels][j] = obj[i-pos[2]][j-pos[0]];
        }
    }

    else printf("== ERROR. CAN'T MOVE THE TURTLE BEYOND THE IMAGE SIZE.\n");

    return;
}





void RO (Image *img, char direction, int angle)
{

}





void RH (Image *imgIn, Image *imgOut)
{
    uchar **obj;
    int *pos = (int *) malloc (4 * sizeof(int));

    obj = findObject(imgIn, pos);

    cout << "vector pos: " << pos[0] <<  " " << pos[1] << " " << pos[2] << " " << pos[3] << endl;


    if (obj != NULL)
    {
        for (int i = pos[2]; i < pos[3]; ++i)
        {
            for (int j = pos[0]; j < pos[1]; ++j)
            {
                imgOut->pixel[i][j] = obj[i-pos[2]][j-pos[0]];
            }
        }
    }
}





void RV (Image *img)
{

}