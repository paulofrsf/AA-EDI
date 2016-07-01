char * readFileContent(char * path) {

	FILE * filePointer;
	long fileSize;

	//Open file in reading mode.

	filePointer = fopen(path, "r");

	if (!filePointer) {
		perror("Error reading file. Aborting...");
		exit(1);
	}

	//Get file size.

	fseek(filePointer, 0L, SEEK_END);
	fileSize = ftell(filePointer);
	rewind(filePointer);

	//Allocate buffer to store file contents.

	char * buffer = calloc(1, fileSize + 1);

	if (!buffer) {
		fclose(filePointer);
		free(buffer);
		fputs("Error allocation buffer.", stderr);
		exit(1);
	}

	//Store file content.

	if (fread(buffer, fileSize, 1, filePointer) != 1) {
		fclose(filePointer);
		free(buffer);
		fputs("Error reading file.", stderr);
		exit(1);
	}

	//Close file to avoid memory leaks.

	fclose(filePointer);

	return buffer;
}

char * rawRead(char * filePath) {
	FILE * file = fopen(filePath, "rb+");
	char c; int cont = 0;
	char * content = (char *)calloc(100000, sizeof(char));
	while ((c = fgetc(file)) != EOF) {
		content[cont] = c;
		cont++;
	}
	realloc(content, sizeof(char) * (cont + 1));
	content[cont] = '\0';
	fclose(file);
	return content;
}

void writeToFile(char * path, char * content) {

	FILE * filePointer;

	//Open file in writing mode.

	filePointer = fopen(path, "w");

	if (!filePointer) {
		perror("Error opening file. Aborting...");
		exit(1);
	}

	//Write content to file.

	fprintf(filePointer, "%s", content);

	//Shutdown file stream.

	fclose(filePointer);
}