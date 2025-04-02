#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE* ptr = fopen("test.bmp", "rb");
    if (ptr == NULL) {
        printf("Error: The file is not opening.\n");
        return 1;
    }

    unsigned char header[54];
    if (fread(header, sizeof(unsigned char), 54, ptr) != 54) {
        printf("Error reading header.\n");
        fclose(ptr);
        return 1;
    }

    fseek(ptr, 0, SEEK_END);
    long fileSize = ftell(ptr);
    fseek(ptr, 54, SEEK_SET);

    long pixelDataSize = fileSize - 54;
    unsigned char *pixelData = (unsigned char*)malloc(pixelDataSize);
    if (pixelData == NULL) {
        printf("Memory allocation failed.\n");
        fclose(ptr);
        return 1;
    }


    size_t bytesRead = fread(pixelData, sizeof(unsigned char), pixelDataSize, ptr);
    fclose(ptr);
    
    if (bytesRead != pixelDataSize) {
        printf("Error reading pixel data.\n");
        free(pixelData);
        return 1;
    }

    char extractedMagicString[9] = {0};
    for (int i = 0; i < 8; i++) {
        unsigned char temp = 0;
        for (int j = 0; j < 8; j++) {
            int pixelIndex = i * 8 + j;
            temp = (temp << 1) | (pixelData[pixelIndex] & 1);
        }
        extractedMagicString[i] = temp;
    }
    extractedMagicString[8] = '\0';

    char expectedMagicString[9];
    printf("Enter your magicString which should be of length 8: ");
    fgets(expectedMagicString, sizeof(expectedMagicString), stdin);
    expectedMagicString[strcspn(expectedMagicString, "\n")] = '\0';

    if (strcmp(extractedMagicString, expectedMagicString) != 0) {
        printf("Magic string does not match! Decoding aborted.\n");
        free(pixelData);
        return 1;
    }

    printf("Magic string verified: %s\n", extractedMagicString);

    int maxPossibleChars = (pixelDataSize - 64) / 8; 
    char *message = (char*)malloc(maxPossibleChars + 1);
    if (message == NULL) {
        printf("Memory allocation failed.\n");
        free(pixelData);
        return 1;
    }

    int charCount = 0;
    int foundEnd = 0;
    for (int i = 8; i < maxPossibleChars + 8; i++) { 
        unsigned char temp = 0;
        for (int j = 0; j < 8; j++) {
            int pixelIndex = (i * 8) + j;
            temp = (temp << 1) | (pixelData[pixelIndex] & 1);
        }
        message[charCount++] = temp;

        if (charCount >= 5 && strncmp(&message[charCount - 5], "#END#", 5) == 0) {
            message[charCount - 5] = '\0'; // Remove the #END#
            foundEnd = 1;
            break;
        }
    }

    if (!foundEnd) {
        message[charCount] = '\0';
    }

    printf("Extracted Message: %s\n", message);

    free(pixelData);
    free(message);
    return 0;
}
