char * readFileContent(char * path) {

	FILE * filePointer;
	long fileSize;

	//abre para leitura.

	filePointer = fopen(path, "r");

	if (!filePointer) {
		perror("Error reading file. Aborting...");
		exit(1);
	}

	//pega o tamnho do arquivo.

	fseek(filePointer, 0L, SEEK_END);
	fileSize = ftell(filePointer);
	rewind(filePointer);

	//Aloca o buffer para guardar conteudo.

	char * buffer = calloc(1, fileSize + 1);

	if (!buffer) {
		fclose(filePointer);
		free(buffer);
		fputs("Error allocation buffer.", stderr);
		exit(1);
	}

	//guarda o arquivo.

	if (fread(buffer, fileSize, 1, filePointer) != 1) {
		fclose(filePointer);
		free(buffer);
		fputs("Error reading file.", stderr);
		exit(1);
	}

	
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

	//Abre para escrever.

	filePointer = fopen(path, "w");

	if (!filePointer) {
		perror("Error opening file. Aborting...");
		exit(1);
	}

	//Escreve conteudo no arquivo.

	fprintf(filePointer, "%s", content);



	fclose(filePointer);
}
