#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void atoB(char *c, int size, int bi2[][8])
{
    for (int i = 0; i < size; i++)
    {
        unsigned char temp = c[i];
        for (int j = 7; j >= 0; j--)
        {
            bi2[i][j] = temp % 2;
            temp /= 2;
        }
    }
}

int main()
{
    char *str;
    FILE *ptr1 = NULL;
    int length;
    int opt;
    char magicString[9] = "TheDefau";
    

    printf("Do you want to use the Default magicString or your own? \n1. Use Default \n2. Use your own Key\n");
    scanf("%d", &opt);
    getchar();
    if (opt == 2)
    {
        char magic[9];
        printf("Enter your magicString which should be of length 8: ");
        fgets(magic, sizeof(magic), stdin);
        magic[strcspn(magic, "\n")] = '\0';
        strncpy(magicString, magic, sizeof(magicString) - 1);
        magicString[sizeof(magicString) - 1] = '\0';
    }

    printf("Your magicString is: %s\n", magicString);

    ptr1 = fopen("message.txt", "r");
    if (ptr1 == NULL)
    {
        printf("Cannot open the message file.\n");
        return 0;
    }

    fseek(ptr1, 0, SEEK_END);
    length = ftell(ptr1);
    if (length == 0)
    {
        printf("There is no message!!!!!");
        fclose(ptr1);
        return 0;
    }
    fseek(ptr1, 0, SEEK_SET);

    str = (char *)malloc(length + 1);
    if (str == NULL)
    {
        perror("Memory allocation failed");
        return 1;
    }

    fread(str, 1, length, ptr1);
    str[length] = '\0';
    fclose(ptr1);
    int n = strlen(str);

    FILE *ptr = fopen("beautiful2.bmp", "rb");
    if (ptr == NULL)
    {
        printf("Error: File not found!\n");
        return 1;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, ptr);

    fseek(ptr, 0, SEEK_END);
    long fileSize = ftell(ptr);
    fseek(ptr, 54, SEEK_SET);

    long pixelDataSize = fileSize - 54;
    printf("%ld\n", pixelDataSize);
    unsigned char *pixelData = (unsigned char *)malloc(pixelDataSize);
    fread(pixelData, sizeof(unsigned char), pixelDataSize, ptr);
    fclose(ptr);

    if ((8 * 8) + (n * 8) > pixelDataSize)
    {
        printf("Error: Message is too large to fit in the image!\n");
        free(pixelData);
        return 1;
    }
    int magic[8][8];
    atoB(magicString, 8, magic);
    int pixelIndex = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int messageBit = magic[i][j];
            pixelData[pixelIndex] = (pixelData[pixelIndex] & 0xFE) | messageBit;
            pixelIndex++;
        }
    }
    int bi2[n][8];
    atoB(str, n, bi2);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int messageBit = bi2[i][j];
            pixelData[pixelIndex] = (pixelData[pixelIndex] & 0xFE) | messageBit;
            pixelIndex++;
        }
    }

    FILE *outputFile = fopen("test.bmp", "wb");
    fwrite(header, sizeof(unsigned char), 54, outputFile);
    fwrite(pixelData, sizeof(unsigned char), pixelDataSize, outputFile);
    fclose(outputFile);

    printf("Message successfully embedded into test.bmp!\n");

    free(pixelData);
    return 0;
}
