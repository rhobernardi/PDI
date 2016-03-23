
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
 * Funcao Main
 * 
 * @param  argc     -
 * @param  argv     Nome do arquivo de imagem de entrada e comando
 * @return          0
 */
int main(int argc, char const *argv[]) 
{
    Image imgIn, imgOut;

    int size;
    char input[20], output[20], operation[2], complement[4];
    double result;

    strcpy(input, argv[1]);
    strcpy(operation, argv[2]);
    
    if(argv[3] != NULL)
        strcpy(complement, argv[3]);

    strcpy(output, "img_out.pgm");

    // Le a imagem de entrada
    readImage(&imgIn, &imgOut, input);

    // Execucao do programa para imagens em escala de cinza (PGM)
    if (imgIn.type[0] == 'P' && imgIn.type[1] == '2')
    {
        copyImage(&imgIn, &imgOut); 

        char direction;
        int angl, npixels;

        npixels = stringToInteger(complement);
        angl = stringToInteger(complement);

        cout << "\nOp: " << operation << "  Value: " << npixels << endl;

        if(!strcmp(operation, "MH"))
        {
            MH(&imgOut, npixels);
            //printf("MH FUNCTION DONE\n");
            saveImage(&imgOut, "MH");
        }

        else if(!strcmp(operation, "MV"))
        {
            MV(&imgOut, npixels);
            //printf("MV FUNCTION DONE\n");
            saveImage(&imgOut, "MV");
        }

        else if(!strcmp(operation, "RO"))
        {
            RO(&imgOut, angl);
            //printf("RO FUNCTION DONE\n");
            saveImage(&imgOut, "RO");
        }

        else if(!strcmp(operation, "RH"))
        {
            RH(&imgOut);
            //printf("RH FUNCTION DONE\n");
            saveImage(&imgOut, "RH");
        }

        else if(!strcmp(operation, "RV"))
        {
            RV(&imgOut);
            //printf("RV FUNCTION DONE\n");
            saveImage(&imgOut, "RV");
        }

        else printf("== ERROR. INVALID OPERATION. USE \"MH\", \"MV\", \"RO\", \"RH\" OR \"RV\".\n");

        //saveImage(&imgOut, output);
        printf("Done.\n");
    }

    else printf("== IMAGE IS NOT IN PGM FORMAT (P2 format) ==\n\n");

    freeData(&imgIn);
    freeData(&imgOut);

    return 0;
}