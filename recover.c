#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//Criando um novo tipo conforme Hit do exercicio
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: ./recover forensicImage\n");
        return 1;
    }

    // Verificando se o arquivo existe conforme Hit do exercicio
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    BYTE buffer[512];
    int nova = 0;
    FILE *img = NULL;
    char name[8];

    //Laço de leitura
    while (fread(&buffer, 512, 1, input))
    {
        //Verificando se começou uma nova imagem
        if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff) && ((buffer[3] & 0xf0) == 0xe0))
        {
            //Fechando a imagem anterior
            if (nova > 0)
            {
                fclose(img);
            }

            //Função para colocar mascara no nome do arquivo
            sprintf(name, "%03i.jpg", nova);
            nova ++;

            img = fopen(name, "w");
        }

        //Copiando a imagem
        if (nova > 0)
        {
            fwrite(&buffer, 512, 1, img);
        }
    }

    // Close files
    fclose(input);
    fclose(img);

}