
/*****************************************************************
*
*    SCC0251 - Processamento de Imagem
*    Tarefa 5 - Filtros
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
    char input[20], output[20]; 
    string filter, smask;

    if (argc != 5)
    {
        printf("\n== INVALID ARGUMENTS. USE '$ ./filtragem <image_in> <filter> <mask> <image_out>'. ==\n\n");
        return -1;
    }

    else
    {
        strcpy(input, argv[1]);
        filter =  argv[2];
        smask = argv[3];
        strcpy(output, argv[4]);

        //strcpy(output, "img_out.pgm");

        // Le a imagem de entrada
        readImage(&imgIn, &imgOut, input);

        // Execucao do programa para imagens em escala de cinza (PGM)
        if (imgIn.type[0] == 'P' && imgIn.type[1] == '2')
        {
            int mask = stringToInteger(smask);
            //copyImageBorder(&imgIn, &imgOut, mask);

            if (filter.compare("media") == 0)
            {
                cout << "FILTER: " << filter << " MASK: " << mask << endl;
                mediaFilter();
            }

            else if (filter.compare("mediana") == 0)
            {
                cout << "FILTER: " << filter << " MASK: " << mask << endl;
                medianaFilter();
            }

            else if (filter.compare("gauss") == 0)
            {
                cout << "FILTER: " << filter << " MASK: " << mask << endl;
                gaussFilter();
            }

            else 
            {
                printf("\n== ERROR. USE 'media', 'mediana' OR 'gauss' AS TYPE OF FILTER ==\n\n");
                return -1;
            }

            /*code here*/

            saveImage(&imgOut, output);
            printf("Done.\n");
        }

        else
        {
            printf("\n== IMAGE IS NOT IN PGM FORMAT (P2 format) ==\n\n");
            return -1;
        } 

        freeData(&imgIn);
        freeData(&imgOut);

        return 0;
    }
}