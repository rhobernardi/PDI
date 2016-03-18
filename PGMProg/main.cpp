
/*****************************************************************
*
*    SCC0251 - Processamento de Imagem
*    Tarefa 4 - Movimentacao de Objeto em imagem binaria
*
*    -Rodrigo das Neves Bernardi - 8066395
*
******************************************************************/

#include "pdi.hpp"


/**
 * Procurar objeto na imagem
 * Executar comando
 */



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
    char input[20], output[20], operation[2], complement[4];
    double result;

    strcpy(input, argv[1]);
    strcpy(operation, argv[2]);
    strcpy(complement, argv[3]);
    strcpy(output, "img_out.pgm");

    // Le a imagem de entrada
    readImage(&imgIn, &imgOut, input);

    // Execucao do programa para imagens em escala de cinza (PGM)
    if (imgIn.type[0] == 'P' && imgIn.type[1] == '2')
    {
        imgOut = findObject(&imgIn);

        char direction, angl[3];

        direction = complement[0];

        angl[0] = complement[1];
        angl[1] = complement[2];
        angl[2] = complement[3];

        cout << "Op: " << operation << "  Direction: " << direction << "  Angle: " << angl << endl;

        if(!strcmp(operation, "MH"))
        {
            printf("Moving...\n");
            //MH();
            printf("MH FUNCTION\n");
        }

        else if(!strcmp(operation, "MV"))
        {
            printf("Moving...\n");
            //MV();
            printf("MV FUNCTION\n");
        }

        else if(!strcmp(operation, "RO"))
        {
            printf("Rotating...\n");
            //RO();
            printf("RO FUNCTION\n");
        }

        else if(!strcmp(operation, "RH"))
        {
            printf("Rotating...\n");
            //RH();
            printf("RH FUNCTION\n");
        }

        else if(!strcmp(operation, "RV"))
        {
            printf("Rotating...\n");
            //RV();
            printf("RV FUNCTION\n");
        }

        else printf("== ERROR. INVALID OPERATION. USE \"MH\", \"MV\", \"RO\", \"RH\" OR \"RV\".\n");


        saveImage(&imgOut, output);
        printf("Done.\n");


        // printf("\nCoordinates(x y): ");
        // scanf("%d %d", &coordx, &coordy);

        // if((coordx < imgIn.width || coordx >= 0) && (coordy < imgIn.height || coordy >= 0))
        // {
        //     printf("\nColor value(0 <= val <= 255): ");
        //     scanf("%hhu", &replacement_tone);

        //     if(replacement_tone >= 0 && replacement_tone <= imgIn.maxVal)
        //     {
        //         printf("\nProcessing...\n");
                
        //         copyImage(&imgIn, &imgOut);

        //         floodFill(&imgOut, coordx, coordy, target_tone, replacement_tone);
                
        //         saveImage(&imgOut, output);
        //         printf("Done.\n");
        //     }

        //     else printf("== ERROR. FOR THIS IMAGE, REPLACEMENT TONE MUST BE BETWEEN '0' AND '%d' ==\n\n", imgIn.maxVal);
        // }

        // else printf("== ERROR. FOR THIS IMAGE, COORDINATE 'X' MUST BE BETWEEN '0' AND '%d', COORDINATE 'Y' BETWEEN '0' AND '%d' ==\n\n", imgIn.width-1, imgIn.height-1);
    }

    else printf("== IMAGE IS NOT IN PGM FORMAT (P2 format) ==\n\n");

    freeData(&imgIn);
    freeData(&imgOut);

    return 0;
}