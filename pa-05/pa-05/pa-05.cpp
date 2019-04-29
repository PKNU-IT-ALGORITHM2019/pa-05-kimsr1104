#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DICT {
	char *word;
	char *classes;
	char *description;
}DICT;

typedef struct BinaryTree {
	char *word_data;
	char *word_classes;
	char *word_description;
	struct BinaryTree *left;
	struct BinaryTree *right;
	struct BinaryTree *parent;
}BinaryTree;

BinaryTree *root = NULL;

BinaryTree* newNode(char *word, char *classes, char *description)
{
	BinaryTree* node = (BinaryTree*)malloc(sizeof(BinaryTree));
	node->word_data = word;
	node->word_classes = classes;
	node->word_description = description;
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	return node;
}

BinaryTree* Successor(BinaryTree* p)
{
	if (p->right == NULL)
		return p;
	return Successor(p->right);
}

void Start();
void Find();
void Add(BinaryTree *p, BinaryTree *addnode);
void Delete();
void DeleteAll();
void DeleteTree(char *target);
BinaryTree* CreateBST(BinaryTree* p, DICT dict);
BinaryTree* SearchTree(BinaryTree* root, char *target);


DICT dict[200000];

char *cont = NULL;
char buffer[1000];
int count = 0;
int size = 1;

int main()
{
	Start();

	return 0;
}

void Start()
{
	char *token[3];
	int i = 1;

	FILE *file = fopen("shuffled_dict.txt", "rt");
	if (file == NULL)
	{
		printf("파일을 찾을 수 없습니다.\n");
		fclose(file);
	}

	while (!feof(file))
	{
		fgets(buffer, sizeof(buffer), file);
		token[0] = strtok(buffer, "(");
		token[1] = strtok(cont, " ");
		token[2] = strtok(cont, "\n");

		dict[count].word = strdup(token[0]);
		dict[count].classes = strdup(token[1]);
		dict[count].description = strdup(token[2]);
		count++;
	}
	fclose(file);
	BinaryTree *root = newNode(dict[0].word, dict[0].classes, dict[0].description);

	while (dict[size].word != NULL)
	{
		CreateBST(root, dict[size]);
		size++;
	}

	int process;
	char dataW[20], dataC[10], dataD[100];
	BinaryTree *addNode;
	while (1)
	{
		printf("\n\n");
		printf("========= Dictionary <Binary Search Tree Version> =========\n");
		printf("1. Size  2. Find  3. Add  4. Delete  5. DeleteAll  6. Exit\n");
		printf("===========================================================\n");
		printf("수행할 작업을 입력하세요. ");
		scanf("%d", &process);
		printf("\n");

		if (process == 6)
		{
			printf("프로그램을 종료합니다.\n");
			break;
		}

		switch (process)
		{
		case 1:
			printf("%d\n\n", size);
			break;
		case 2:
			Find();
			break;
		case 3:
			printf("추가할 단어의 정보를 입력하세요.\n\n");
			printf("word : ");
			scanf("%s", &dataW);
			printf("class : ");
			scanf("%s", &dataC);
			printf("meaning : ");
			scanf("%s", &dataD);
			addNode = newNode(dataW, dataC, dataD);
			Add(root, addNode);
			break;
		case 4:
			Delete();
			break;
		case 5:
			DeleteAll();
			break;
		default:
			printf("유효하지 않은 명령입니다.\n");
			break;
		}
	}
}

BinaryTree* CreateBST(BinaryTree *p, DICT dict)
{
	BinaryTree *Node = newNode(dict.word, dict.classes, dict.description);
	int compare;

	if (p == NULL)
	{
		return Node;
	}
	else
	{
		compare = strcmp(dict.word, p->word_data);

		if (compare < 0)
		{
			p->left = CreateBST(p->left, dict);
		}
		else if (compare >= 0)
		{
			p->right = CreateBST(p->right, dict);
		}
		return p;
	}
}

BinaryTree* SearchTree(BinaryTree* root, char *target)
{
	int compare;

	compare = strcmp(target, root->word_data);

	if (compare == 0)
		return root;

	else if (compare < 0)
	{
		if (root->left == NULL)
			return NULL;
		return SearchTree(root->left, target);
	}

	if (compare > 0)
	{
		if (root->right == NULL)
			return NULL;
		return SearchTree(root->right, target);
	}
}

void Find()
{
	char target[20];
	
	printf("검색할 단어를 입력하세요. ");
	scanf("%s", target, sizeof(target));
	SearchTree(root, target);

	if (SearchTree(root, target) == NULL)
	{
		printf("단어가 존재하지 않습니다.");
	}
	else
	{
		printf("%s", SearchTree(root, target)->word_description);
	}
}

void Add(BinaryTree *p, BinaryTree *addnode)
{
	int cmp1, cmp2;

	cmp1 = strcmp(p->word_data, addnode->word_data);
	cmp2 = strcmp(p->word_description, addnode->word_description);

	printf("%s %d %s\n", p->word_data, cmp1, addnode->word_data);

	if (cmp1 < 0)
	{
		if (p->right == NULL)
		{
			p->right = addnode;
			addnode->parent = p;
			printf("단어가 추가되었습니다.");
		}
		else
			Add(p->right, addnode);
	}
	else if (cmp1 > 0)
	{
		if (p->left == NULL)
		{
			p->left = addnode;
			addnode->parent = p;
			printf("단어가 추가되었습니다.");
		}
		else
			Add(p->left, addnode);
	}
	else if (cmp1 == 0)
	{
		if (cmp2 == 0)
		{
			printf("이미 같은 설명을 가진 동일 단어가 존재합니다.");
		}
		else
		{
			p->right = addnode;
			addnode->parent = p;
			printf("단어가 추가되었습니다.");
		}
	}
}

void Delete()
{
	char target[20];
	
	printf("삭제할 단어를 입력하세요. ");
	scanf("%s", target, sizeof(target));
	DeleteTree(target);
}

void DeleteAll()
{
	char *token;
	int count;
	FILE *file = fopen("to_be_deleted_words.txt", "rt");

	while (!feof(file))
	{
		fgets(buffer, sizeof(buffer), file);
		token = strtok(buffer, "\n");

		DeleteTree(token);
		count++;
	}
	fclose(file);

	printf("%d 개의 단어를 삭제했습니다.", count);
}

void DeleteTree(char *target)
{
	int compare;
	BinaryTree* D_Word = NULL;

	if (SearchTree(root, target) == NULL)
	{
		printf("단어가 존재하지 않습니다.");
	}
	else
	{
		D_Word = SearchTree(root, target);
		compare = strcmp(D_Word->word_data, D_Word->parent->word_data);

		if (D_Word->left == NULL && D_Word->right == NULL)
		{
			if (compare < 0)
				D_Word->parent->left = NULL;
			else
				D_Word->parent->right = NULL;
			printf("단어가 제거되었습니다.");
			free(D_Word);
		}
		else if (D_Word->left != NULL && D_Word->right == NULL)
		{
			D_Word->left->parent = D_Word->parent;
			D_Word->parent->left = D_Word->left;
			printf("단어가 제거되었습니다.");
			free(D_Word);
		}
		else if (D_Word->left == NULL && D_Word->right != NULL)
		{
			D_Word->right->parent = D_Word->parent;
			D_Word->parent->right = D_Word->right;
			printf("단어가 제거되었습니다.");
			free(D_Word);
		}
		else
		{
			BinaryTree* delete_successor(D_Word->left);
			BinaryTree* delete_left = D_Word->left;

			if (delete_left->left == NULL && delete_left->right == NULL)
			{
				D_Word->parent->left = delete_left;
				delete_left->right = D_Word->right;
				D_Word->right->parent = delete_left;
			}
			else if (delete_successor == NULL)
			{
				D_Word->left->parent = delete_successor;
				D_Word->right->parent = delete_successor;

				if (compare < 0)
					D_Word->parent->left = delete_successor;
				else
					D_Word->parent->right = delete_successor;

				delete_successor->parent = D_Word->parent;
				delete_successor->parent->left = NULL;
				delete_successor->parent->right = NULL;
				printf("단어가 제거되었습니다.");
				free(D_Word);
			}
			else
			{
				D_Word->left->parent = delete_successor;
				D_Word->right->parent = delete_successor;

				if (compare < 0)
					D_Word->parent->left = delete_successor;
				else
					D_Word->parent->right = delete_successor;
				
				delete_successor->parent->right = delete_successor->left;
				delete_successor->left->parent = delete_successor->parent;
				delete_successor->left = D_Word->left;
				delete_successor->right = D_Word->right;
				delete_successor->parent = D_Word->parent;
				printf("단어가 제거되었습니다.");
				free(D_Word);
			}
		}
	}
}