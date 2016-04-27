
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
    string filter, mask;

    if (argc != 5)
    {
        printf("\n== INVALID ARGUMENTS. USE '$ ./filtragem <image_in> <filter> <mask> <image_out>'. ==\n\n");
        return -1;
    }

    else
    {
        strcpy(input, argv[1]);
        filter =  argv[2];
        mask = argv[3];
        strcpy(output, argv[4]);

        //strcpy(output, "img_out.pgm");
        
        // converte o char de mask para um inteiro sem sinal
        unsigned int mask_dim = stringToInteger(mask);
        int tam = 0, bord = 0;

        if (!filter.compare("gauss") || !filter.compare("Gauss") || !filter.compare("GAUSS"))
        {
            tam = (6*mask_dim)+1;
        }

        else tam = mask_dim;

        // calcula borda
        bord = 2*floor(tam/2);

        // Le a imagem de entrada e a transforma com borda de mask
        readImage(&imgIn, &imgOut, input, bord);

        // copia a imagem original para a de saida
        copyImageBord(&imgIn, &imgOut, bord);

        // Execucao do programa para imagens em escala de cinza (PGM)
        if (imgIn.type[0] == 'P' && imgIn.type[1] == '2')
        {
            if (!filter.compare("media") || !filter.compare("Media") || !filter.compare("MEDIA"))
            {
                cout << "FILTER: " << filter << " MASK: " << mask << endl;
                mediaFilter(&imgOut, mask_dim, bord);
            }

            else if (!filter.compare("mediana") || !filter.compare("Mediana") || !filter.compare("MEDIANA"))
            {
                cout << "FILTER: " << filter << " MASK: " << mask << endl;
                medianaFilter(&imgOut, mask_dim, bord);
            }

            else if (!filter.compare("gauss") || !filter.compare("Gauss") || !filter.compare("GAUSS"))
            {
                cout << "FILTER: " << filter << " MASK: " << mask << endl;
                gaussFilter(&imgOut, mask_dim, bord);
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