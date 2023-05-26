#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <string.h>
#include<stdbool.h>

#define ALPHABET_SIZE (26)
#define len 4

struct Trie
{
	struct Trie* children[ALPHABET_SIZE];
	bool isLeaf;
};

/** Initialize your data structure here. */

Trie* trieCreate()
{
	Trie* newNode = (struct Trie*) malloc(sizeof(Trie));

	int i;
	for (i = 0; i < ALPHABET_SIZE; i++)
	{
		newNode->children[i] = NULL;
	}

	newNode->isLeaf = false;
	return newNode;
}

/** Inserts a word into the trie. */
void trieInsert(Trie* obj, char* word)
{
	int index;
	int length = strlen(word);
	int i;

	for (i = 0; i < length-1; i++)
	{
		index = word[i] - 'a';

		if (!obj->children[index])
		{
			obj->children[index] = trieCreate();
		}

		obj = obj->children[index];
	}

	obj->isLeaf = true;
}

/** Returns if the word is in the trie. */
bool trieSearch(Trie* obj, char* word)
{
	int index;

	for (int i = 0; i < strlen(word)-1; i++)
	{
		index = word[i] - 'a';
		if (!obj->children[index])
		{
			return false;
		}

		obj = obj->children[index];
	}

	return (obj != NULL && obj->isLeaf);
}

void trieFree(Trie* obj)
{
	free(obj);
}

/** Create your Word List here. */
void CreateWordList(Trie *t,char w[][len+1])
{
	int item = 0;
	char str[len + 1];
	FILE *fp = fopen("wordlist.txt", "r");

	if(fp == NULL)
	{
		perror("fopen");
		exit(1);
	}
	while (fgets(str, len+1, fp) != NULL) {
		//get \n
		fgetc(fp);
		for (int i = 0; i < len+1; i++) {
			w[item][i] = str[i];
		}
		trieInsert(t, str);
		item++;
	}

	fclose(fp);
}

/** Create your AlphabetTable here. */
void InitAlphabetTable(char al[])
{
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		al[i] = 'a' + i;
	}
}

/** Print the Alphabet Table. */
void PrintAlphabetTable(char al[])
{
	printf("\n===============================\n");
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if(al[i] == '0') printf("■ ");
		else printf("%2c ", al[i]);
		if (i % 10 == 9) printf("\n");
	}
	printf("\n===============================\n");
}

/** Delet letter in the Alphabet Table. */
void DeletLetter(char al[] ,char g)
{
	al[g - 'a'] = '/';
}

void MaskLetter(char al[], char g)
{
	al[g - 'a'] = '0';
}

/** Initialize your Mask here. */
void InitMask(int m[])
{
	for (int i = 0; i < len; i++) {
		m[i] = 0;
	}
}

/** Mark true all mask positions corresponding to guess. */
void MaskEachGuess(int m[], char a[], char g[])
{
	InitMask(m);
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len; j++) {
			if (a[i] == g[j]) {
				if (i == j || m[j] == 2) m[j] = 2;
				else m[j] = 1;
			}
		}
	}
}

/** Print word with ■s/□s for unguessed letters. */
void PrintResultGraphic(int m[], char al[], char g[])
{
	printf("Oh!No! You guessed wrong.");
	for (int i = 0; i < len; i++) {
		if (m[i]) {
			if (m[i] > 1) {
				printf("■");
				MaskLetter(al, g[i]);
			}else printf("□");
		}
		else
		{
			printf("?");
			DeletLetter(al, g[i]);
		}
	}
	printf("\n");
	PrintAlphabetTable(al);
}

/** Return TRUE if this word has one non-lowercase. */
bool NonLowercase(char g[])
{
	bool low = true;
	for (int i = 0; i < len; i++) {
		//not in [a-z]
		if (g[i] > 'z' || g[i] < 'a')
			low = false;
	}
	if (low)
		return false;
	else
		return true;
}

/** Set an answer in Word list[0,426]. */
void SetAnswer(char a[], char w[][len + 1])
{
	// Creare a random integer in [0,426]
	srand(time(NULL));
	int min = 0;
	int max = 426;
	unsigned seed = rand() % (max - min + 1) + min;

	for (int i = 0; i < len+1; i++) {
		a[i] = w[seed][i];
		printf("%c\n",a[i]);
	}
}

/** Return TRUE if it is a reasonable guess. */
bool ReasonableGuess(char g[], Trie *t)
{
	fflush(stdout);
	gets(g);
	//Non-four letters words
	if(strlen(g) != len) {
		printf("\nOh!No! Not four letters words.\n請再次輸入4個【小寫】英文字母\nThe word is :");
		return false;
	}
	//Non lowercase words
	else if(NonLowercase(g)) {
		printf("\nOh!No! Not lowercase letter.\n請再次輸入4個【小寫】英文字母\nThe word is :");
		return false;
	}
	//Not in word list
	else if(!trieSearch(t, g)) {
		printf("\nOh!No! Not in word list.\n請再次輸入4個【小寫】英文字母\nThe word is :");
		return false;
	}else
		return true;
}

int main(void)

{
	char wordlist[427][len+1];
	Trie *tr = trieCreate();
	CreateWordList(tr, wordlist);

	char alph[ALPHABET_SIZE];
	InitAlphabetTable(alph);

	char answer[len + 1];
	SetAnswer(answer, wordlist);

	int mask[len];
	int loop10 = 10;
	int run = 0;
	int gameover = 0;

	printf("=====>猜字遊戲(Word Game)，限制10次 加油!<=====\n\n\n");
	while (!gameover) {

		// Get player's next guess word
		char guess[20] = {'\0'};
		printf("請輸入4個【小寫】英文字母，還有%d次機會\nThe word is :", loop10 - run);
		while (!ReasonableGuess(guess, tr)) {}
		MaskEachGuess(mask, answer, guess);
		PrintResultGraphic(mask,alph, guess);
		++run;

		// Determine whether the player has won!
		gameover = 1;
		for (int i = 0; i < len; ++i) {
			if (run < loop10 && mask[i] < 2) {
				gameover = 0;
				break;
			}
		}
	}

	// Print win/lose message!
	if(run > loop10-1)
		printf("\n************************************************************\nYou Lose!\nYou Lose!\nYou Lose!\nThe word is \"%s\".\n", answer);
	else
		printf("\n************************************************************\nYou Win! The word is \"%s\".\n", answer);

	//clean trie
	trieFree(tr);
	return 0;

}
