
/*****************************************************************
*
*    SCC0251 - Processamento de Imagem
*    Tarefa 3 - Flood Fill em imagem.
*
*    -Rodrigo Bernardi
*
******************************************************************/

#include "pdi.hpp"


/**
 * Funcao Main
 * 
 * @param  argc     -
 * @param  argv     Nome do arquivo de imagem de entrada
 * @return          0
 */
int main(int argc, char const *argv[]) 
{
    Image imgIn, imgOut;

    uchar *inPixel, *outPixel;
    int size;
    char input[20], output[20];
    double result;

    strcpy(input, argv[1]);
    strcpy(output, "img_out.pgm");

    // Le a imagem de entrada
    readImage(&imgIn, &imgOut, input);

    size = imgIn.width * imgIn.height;

    // Execucao do programa para imagens em escala de cinza (PGM)
    if (imgIn.type[0] == 'P' && imgIn.type[1] == '2')
    {
        double average = 0, stand_dev = 0;
        double *percent_vec = (double *) malloc((imgIn.maxVal+1)*sizeof(double)); // 255 valores para pixel

        unsigned int coordx, coordy;
        uchar replacement_tone;

        int v = sqrt(2);

        printf("\nCoordinates(x y): ");
        scanf("%d %d", &coordx, &coordy);

        if((coordx < imgIn.width || coordx >= 0) && (coordy < imgIn.height || coordy >= 0))
        {
            printf("\nColor value(0 <= val <= 255): ");
            scanf("%hhu", &replacement_tone);

            if(replacement_tone >= 0 && replacement_tone <= imgIn.maxVal)
            {
                printf("\nProcessing...\n");
                
                copyImage(&imgIn, &imgOut);
                uchar target_tone = imgOut.pixel[(imgOut.width*coordy)+coordx];

                floodFill(&imgOut, coordx, coordy, target_tone, replacement_tone);
                
                saveImage(&imgOut, output);
                printf("Done.\n");
            }

            else printf("== ERROR. FOR THIS IMAGE, REPLACEMENT TONE MUST BE BETWEEN '0' AND '%d' ==\n\n", imgIn.maxVal);
        }

        else printf("== ERROR. FOR THIS IMAGE, COORDINATE 'X' MUST BE BETWEEN '0' AND '%d', COORDINATE 'Y' BETWEEN '0' AND '%d' ==\n\n", imgIn.width-1, imgIn.height-1);
    }

    else printf("== IMAGE IS NOT IN PGM FORMAT (P2 format) ==\n\n");

    freeData(&imgIn);
    freeData(&imgOut);

    return 0;
}
