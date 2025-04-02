# Steganography in BMP Images

This project demonstrates how to hide and extract secret messages within a BMP image using simple steganography techniques. The message is embedded in the least significant bits of the image pixels.

## Features

- Embed a secret message into a BMP image.
- Extract the hidden message from the BMP image.
- Uses an 8-character magic string for verification.
- Supports custom magic strings for added security.

## Files in the Project

- `Encode.c`: Contains the code to embed a message into a BMP image.
- `Decode.c`: Contains the code to extract a hidden message from a BMP image.
- `message.txt`: The file containing the message to be hidden.
- `beautiful2.bmp`: The BMP image used for embedding.
- `test.bmp`: The output BMP image containing the hidden message.

## How to Use

### Embedding a Message

1. Ensure you have `message.txt` with the text you want to hide.
2. Compile the `embed.c` file:
   ```bash
   gcc Encode.c -o Encode
   ```
3. Run the program:
   ```bash
   ./Encode
   ```
4. The program will generate `test.bmp` containing the hidden message.

### Extracting a Message

1. Compile the `extract.c` file:
   ```bash
   gcc Decode.c -o Decode
   ```
2. Run the program:
   ```bash
   ./Decode
   ```
3. Enter the magic string used during embedding.
4. The hidden message will be displayed if the magic string matches.

## Notes

- The magic string must be 8 characters long.
- Ensure the image is large enough to store the message.
- The message should end with `#END#` to indicate termination.

## Requirements

- GCC compiler
- A 24-bit BMP image

## License

This project is open-source and available for modification and use under the MIT License.

