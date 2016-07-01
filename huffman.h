#include <string.h>
#define MARRETA 1000000

static const int ASCII_TABLE_SIZE = 256;

//No.

struct node {
	struct node * left;
	struct node * right;
	char character;
	int frequency;
};

//frequencia.

struct frequencyData {
	int * frequencyHash;
	int totalCharacters;
	int totalDistinctCharacters;
};

//lista de caracteres.

struct characterList {
	struct node * list;
	int size;
};

struct huffmanData {
	char * codificationMapString;
	char * compressedContent;
	int bitCont;
};

char * newString(int size) {
	char * string = (char *)calloc(size,sizeof(char));
	return string;
}

void quickSortByFrequency(struct node * list, int left, int right) {
	int pivot, i, j, mid;

	i = left;
	j = right;

	mid = (i + j) / 2;
	pivot = list[mid].frequency;

	do {
		while (list[i].frequency > pivot) i = i + 1;
		while (list[j].frequency < pivot) j = j - 1;

		if (i <= j) {
			struct node aux = list[i];
			list[i] = list[j];
			list[j] = aux;
			i++; j--;
		}
	} while (j > i);

	if (left < j) quickSortByFrequency(list, left, j);
	if (i < right) quickSortByFrequency(list, i, right);
}

//constroi no

struct node * buildNode() {
	struct node * newNode = (struct node*) malloc(sizeof(struct node));

	newNode->left = NULL;
	newNode->right = NULL;
	newNode->character = '\0';
	newNode->frequency = 0;

	return newNode;
}
//constroi lista de caracteres
struct characterList * buildCharacterList() {
	struct characterList * charList = (struct characterList *) malloc(sizeof(struct characterList));
	charList->list = (struct node *)calloc(ASCII_TABLE_SIZE, sizeof(struct node));
	charList->size = 0;
	return charList;
}

void printCharList(struct characterList * charList) {
	int i;
	for (i = 0; i < charList->size; i++) {
		printf("'%c' => %d\n", charList->list[i].character, charList->list[i].frequency);
	}
	printf("\n");
	printf("Tamanho da lista de caracteres: %d\n", charList->size);
}

struct characterList * getCharacterList(struct frequencyData * data) {

	int character = 0;

	struct characterList * charList = buildCharacterList();

	for (character = 0; character < ASCII_TABLE_SIZE; character++) {
		int characterFrequency = data->frequencyHash[character];
		if (characterFrequency > 0) {
			int index = charList->size;
			charList->list[index] = *buildNode();
			charList->list[index].character = character;
			charList->list[index].frequency = characterFrequency;
			charList->size += 1;
		}
	}

	int i = 0;

	realloc(charList->list, charList->size * sizeof(struct node));
	quickSortByFrequency(charList->list, 0, charList->size - 1);

	return charList;
}

//ve se esta no final do arquivo

int isContent(char c) {
	return c != '\0' && c != EOF;
}

//cria o hash com as frequencias

void createFrequencyStructure(struct frequencyData ** data) {
	(*data) = (struct frequencyData *) malloc(sizeof(struct frequencyData));
	(*data)->frequencyHash = (int *)calloc(ASCII_TABLE_SIZE, sizeof(int));
	(*data)->totalCharacters = 0;
}

struct frequencyData * getFrequencyData(char * contents) {
	struct frequencyData * data;

	createFrequencyStructure(&data);

	char current; int i = 0;

	while (isContent(current = contents[i++])) {
		data->frequencyHash[current]++;
		data->totalCharacters += 1;
	}

	return data;
}

int isLeafNode(struct node * treeNode) {
	return treeNode->left == NULL && treeNode->right == NULL;
}

char * append(char * sequence, char c) {
	size_t len = strlen(sequence);

	char *appendedSequence = malloc(len + 1 + 1);
	strcpy(appendedSequence, sequence);
	appendedSequence[len] = c;
	appendedSequence[len + 1] = '\0';

	return appendedSequence;
}

void printCodMap(char ** codificationMap) {
	int i = 0;
	for (i = 1; i < ASCII_TABLE_SIZE; i++) {
		if (codificationMap[i]) {
			printf("'%c' eh ... %s\n", (char)i, codificationMap[i]);
		}
	}
	printf("\n");
}

void inOrder(struct node * x, char * sequence, char ** map) {
	if (x == NULL) return;

	inOrder(x->left, append(sequence, '1'), map);
	inOrder(x->right, append(sequence, '0'), map);

	if (isLeafNode(x)) {
		map[x->character] = sequence;
	}
}

/**
* codifica a arvore de huffman e retornar valores e chaves.
*/

char ** codifyTree(struct node * root) {
	char ** map = (char **)calloc(ASCII_TABLE_SIZE,sizeof(char *));
	inOrder(root, "\0", map);
	return map;
}

void freeTree(struct node * x) {
	if (x == NULL) return;

	freeTree(x->left);
	freeTree(x->right);

	printf("Liberando...\n");
	free(x);
}

struct node * cloneNode(struct node * node) {
	struct node * clone = (struct node *) malloc(sizeof(struct node));

	clone->left = node->left;
	clone->right = node->right;
	clone->character = node->character;
	clone->frequency = node->frequency;

	return clone;
}

char * translate(char * contents, char ** translation) {
	char currentChar;
	char * binary = newString(MARRETA);
	binary[0] = '\0';

	int i = 0; int bitCont = 0;

	while (isContent(currentChar = contents[i])) {
		char * charCode = translation[currentChar];
		strcat(binary, charCode);
		bitCont += strlen(charCode);
		i++;
	}
//atualiza o total de bits do texto

	translation[0] = newString(100);

	//transforma bitCont em string e salva no mapa.

	itoa(bitCont, translation[0], 10);

	return binary;
}

char bitsToByte(const char bits[8]) {
	char ret = 0;
	int i; int pow;
	for (i = 7, pow = 128; i >= 0; --i, pow /= 2)
		if (bits[i] == '1') ret += pow;
	return ret;
}

char * initBitArray() {
	char * bits = newString(8);
	int i = 0;
	while (i < 8) {
		bits[i] = '0';
		i++;
	}
	return bits;
}


char * toBytes(char * contentBits, int * contBytes) {
	int i = 0; int j; char bit;
	char * contentBytes = newString(MARRETA);
	while (isContent(contentBits[i])) {
		char * bits = initBitArray();
		j = i;
		while (isContent(contentBits[i]) && i < j + 8) {
			bit = contentBits[i];
			bits[i % 8] = bit;
			i++;
		}
		contentBytes[*contBytes] = bitsToByte(bits);
		(*contBytes)++;
	}

	realloc(contentBytes, sizeof(char) * (*contBytes));
	return contentBytes;
}

struct huffmanData * createHuffmanData(char ** codificationMap, char * compressedContent) {

	struct huffmanData * huffmanData = (struct huffmanData*)malloc(sizeof(struct huffmanData));

	//mapa de codificacao para string
	char * c = newString(1);
	char * map = newString(MARRETA);
	map[0] = '\0';
	int i = 0;

	for (i = 0; i < ASCII_TABLE_SIZE; i++) {
		if (codificationMap[i]) {
			*c = (char)i;
			strcat(map, c);
			strcat(map, codificationMap[i]);
			strcat(map, "\n");
		}
	}

	strcat(map, "\0");

	free(c);

	huffmanData->codificationMapString = map;
	huffmanData->compressedContent = compressedContent;
	return huffmanData;
}

void analyzeCompressionRate(float compressionRate) {
	printf("Taxa de compressao: %.2f%%\n", compressionRate);

	if (compressionRate >= 75) {
		printf("Tamanho reduzido drasticamente! *___*");
	}
	else if (compressionRate >= 50) {
		printf("Otimo, reducao satisfatoria! :O");
	}
	else if (compressionRate >= 25) {
		printf("Ha casos melhores, mas eh algo. :D");
	}
	else {
		printf("Pouca vantagem. :S");
	}
}

//cria a arvore de huffman e retorna o ponteiro para a raiz

struct node * buildHuffmanTree(struct characterList * charList) {
	int n;
	while ((n = charList->size) > 1) {
		struct node * x = cloneNode(&charList->list[n - 1]);
		struct node * y = cloneNode(&charList->list[n - 2]);
		struct node * father = buildNode();

		father->left = x;
		father->right = y;
		father->frequency = x->frequency + y->frequency;
		charList->list[n - 2] = *father;
		charList->size -= 1;
		quickSortByFrequency(charList->list, 0, charList->size - 1);
	}

	return &charList->list[0];
}

struct huffmanData * compress(char * toCompress) {

	printf("Pegando a frequencia...\n\n");

	struct frequencyData * data = getFrequencyData(toCompress);
	struct characterList * charList = getCharacterList(data);

	printCharList(charList);

	printf("\nContruindo a arvore de huffman e codificando...\n\n");

	struct node * treeRoot = buildHuffmanTree(charList);


	char ** codificationMap = codifyTree(treeRoot);
	
	char * binary = translate(toCompress, codificationMap);

	printCodMap(codificationMap);

	printf("\nBinary content: \n%s\n", binary);

	int byteCont = 0;

	char * compressedContent = toBytes(binary, &byteCont);
	float compressionRate = (1 - (float)byteCont / data->totalCharacters)*100.0;

	analyzeCompressionRate(compressionRate);

	return createHuffmanData(codificationMap, compressedContent);
}

char ** processCodificationMapString(char * _codificationMapString) {
	char ** _codificationMap = (char **)calloc(ASCII_TABLE_SIZE, sizeof(char *));
	char currentChar; int i = 0; char it;

	//Pula primeira linha (numero de bits).

	while ((currentChar = _codificationMapString[i++]) != '\n');

	while(isContent(currentChar = _codificationMapString[i])) {
		i++;
		char * code = newString(100);
		code[0] = '\0';
		while((it = _codificationMapString[i]) != '\n') {
			strcpy(code,append(code, it));
			i++;
		}
		if(code[0] != '\0') _codificationMap[currentChar] = code;
		i++;
	}

	return _codificationMap;
}

char * getBinaryContent(char * compressedContent, int totalBits) {
	int i = 0; char c;
	int bitCont = 0;
	char * binaryContent = newString(totalBits + 1);
	int stop = 0;
	while (1){
		c = compressedContent[i];
		int j;
		for (j = 0; j<8; j++) {
			if (((c >> j) & 0x01) == 1) {
				binaryContent[bitCont] = '1';
				bitCont++;
			}
			else if (((c >> j) & 0x01) == 0) {
				binaryContent[bitCont] = '0';
				bitCont++;
			}

			if (bitCont == totalBits) {
				binaryContent[totalBits] = '\0';
				return binaryContent;
			}
		}
		i++;
		if (c == '\0') break;
	}

	return "";
}

int getTotalBitFromMapString(char * codificationMapString) {
	char c; int i = 0;
	char * totalBitString = newString(100);
	
	while((c = codificationMapString[i]) != '\n') {
		totalBitString[i] = c;
		i++;
	}

	totalBitString[i] = '\0';

	return atoi(totalBitString);
}

void preOrder(struct node * x) {
	if (x == NULL) return;

	if(isLeafNode(x)) printf("Leaf: %c\n", x->character);
	else printf("Node: %c\n", x->character);

	preOrder(x->left);
	preOrder(x->right);
}

struct node * buildDecompressionTree(char ** codificationMap) {
	struct node * root = buildNode();

	//Guarda apontador pra raiz.

	struct node ** aux = (struct node **)malloc(sizeof(struct node *));
	*aux = root;

	int i;
	for (i = 1; i < ASCII_TABLE_SIZE; i++) {
		if (codificationMap[i]) {
			char * code = codificationMap[i];
			char c; int j = 0;
			while (isContent(c = code[j])) {
				switch (c) {
					case '0': {
						if (root->right == NULL) {
							struct node * newRightNode = buildNode();
							newRightNode->character = (char)i;
							root->right = newRightNode;
						}
						root = root->right;
						break;
					}
					case '1': {
						if (root->left == NULL) {
							struct node * newLeftNode = buildNode();
							newLeftNode->character = (char)i;
							root->left = newLeftNode;
						}
						root = root->left;
						break;
					}
					default: {
						break;
					}
				}
				j++;
			}
			//Aponta pra raiz denovo.
			
			root = *aux;
		}
	}

	return root;
}

char * translateFromBinary(char * binaryContent, struct node * root) {
	struct node ** aux = (struct node **)malloc(sizeof(struct node *));
	*aux = root;

	char c; int i = 0;
	char * translation = newString(MARRETA);
	translation[0] = '\0';

	while (1) {
		c = binaryContent[i];

		if (isLeafNode(root)) {
			translation = append(translation, root->character);
			root = *aux;
		}
		
		switch (c) {
			case '0': {
				if (root->right == NULL) {
					printf("Erro ao descomprimir arquivo: código não reconhecido.");
					exit(1);
				}
				root = root->right;
				i++;
				break;
			}
			case '1': {
				if (root->left == NULL) {
					printf("Erro ao descomprimir arquivo: código não reconhecido.");
					exit(1);
				}
				root = root->left;
				i++;
				break;
			}
			default: {
				break;
			}
		}

		if (c == '\0'||c== EOF) break;
	}

	return translation;
}

char * decompress(struct huffmanData * huffmanData) {

	char ** codificationMap = processCodificationMapString(huffmanData->codificationMapString);
	printCodMap(codificationMap);

	int totalBits = getTotalBitFromMapString(huffmanData->codificationMapString);
	printf("totalBits: %d\n", totalBits);

	char * binaryContent = getBinaryContent(huffmanData->compressedContent,totalBits);

	printf("\n\nConteudo binario: %s\n", binaryContent);

     
	struct node * treeRoot = buildDecompressionTree(codificationMap);
    
     
	printf("\nArvore construida\n");
	
	char * decompressedContent = translateFromBinary(binaryContent, treeRoot);

	printf("\nSucesso descomprimindo! :) \n\n%s\n", decompressedContent);
	return decompressedContent;
}
