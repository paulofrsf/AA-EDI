#include <stdio.h>
#include <stdlib.h>
#include "fileutils.h"
#include "huffman.h"

#define DECOMPRESS 1
#define COMPRESS 2

struct options {
	char originPath[100];
	char targetPath[100];
};

struct options * createOptions() {
	return (struct options *) malloc(sizeof(struct options));
}


struct huffmanData * parseHuffmanData(char * filePath) {
	char * treePath = (char *)malloc(sizeof(char) * 100);
	strcpy(treePath, filePath);
	strcat(treePath, "_tree");

	// treePath: caminho do arquivo da árvore de huffman.
	// filePath: caminho do arquivo comprimido.

	struct huffmanData * huffmanData = (struct huffmanData *)malloc(sizeof(struct huffmanData));
	huffmanData->compressedContent = rawRead(filePath);
	huffmanData->codificationMapString = rawRead(treePath);

	printf("Compressed content: \n\n%s\n\n", huffmanData->compressedContent);

	return huffmanData;
}

int main(int argc, char *argv[]) {

	struct options * options = createOptions();

	int numberOfArgs = argc - 1;

	switch (numberOfArgs) {
	case COMPRESS:
		strcpy(options->originPath, argv[1]);
		strcpy(options->targetPath, argv[2]);

		printf("Huffman Algorithm (compression) \n\n");
		printf("Origin path... %s\n", options->originPath);
		printf("Destination path... %s\n\n", options->targetPath);

		char * originContent = readFileContent(options->originPath);
		struct huffmanData * results = compress(originContent);

		//Output.

		char * targetTreeFile = (char *)malloc(sizeof(char) * 100);

		strcpy(targetTreeFile, options->targetPath);
		strcat(targetTreeFile, "_tree");

		printf("\n\nCompressed content: \n\n%s\n\n", results->compressedContent);

		writeToFile(options->targetPath, results->compressedContent);

		//Save huffman tree for future decompression.

		writeToFile(targetTreeFile, results->codificationMapString);

		break;
	case DECOMPRESS:
		strcpy(options->originPath, argv[1]);

		printf("Huffman Algorithm (decompression) \n\n");
		printf("Origin path... %s\n", options->originPath);

		char * decompressionResults = decompress(parseHuffmanData(options->originPath));

		writeToFile(strcat(options->originPath, "_decompressed.txt"), decompressionResults);

		break;
	default:
		printf("Invalid command line parameters. Aborting...");
		break;
	}

	getch();
	return 0;
}