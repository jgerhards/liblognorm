#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include "syntaxes.h"

int anonbits = 16;
enum mode {ZERO, RANDOMINT};
int cons = 0;
enum mode mode = ZERO;
union node {
	struct {
		union node* more;
		union node* less;
	} pointer;
	struct {
		char ip_high[16];
		char ip_low[16];
	} ips;
};
union node* Root = NULL;
#if 0
struct ipv4_etry{
	char ip_org[16];
	char ip_anon[16];
};
struct tree_etry{
	struct ipv4_etry ips;
	struct tree_etry *less;
	struct tree_etry *more;
};
struct tree_etry *Root = NULL;
struct tree_etry *Last = NULL;
int ippos;
struct node{
	struct ipv4_etry ips;
	struct node *next;
};
struct node* ListRoot = NULL;
struct node* ListLast = NULL;
long long ips_count = 0;
struct ipv4_etry *ips;


struct ipv4_etry* MvAddArray(struct ipv4_etry* array){
	int i;
	int j;
	struct ipv4_etry* NewArray;
	NewArray = (struct ipv4_etry*)malloc(sizeof(struct ipv4_etry)*(++ips_count));
	for(i = 0; i < ips_count - 1; i++){
		for(j = 0; j < 16; j++){
			NewArray[i].ip_org[j] = array[i].ip_org[j];
			NewArray[i].ip_anon[j] = array[i].ip_anon[j];
		}
	}
	if(ips_count - 1 != 0){
		free(array);
	}
	return NewArray;
}
#endif

static unsigned
ipv42num(const char *str)
{
	unsigned num[4] = {0, 0, 0, 0};
	unsigned value = -1;
	size_t len = strlen(str);
	int cyc = 0;
	int prevdot = 0;
	int startblank = 0;
	int endblank = 0;
	for(unsigned i = 0 ; i < len ; i++) {
		switch(str[i]){
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if(endblank == 1){
				printf("ipv42num error: wrong IP-Address format (invalid space(1))\n");
				goto done;
			}
			prevdot = 0;
			startblank = 0;
			num[cyc] = num[cyc]*10+(str[i]-'0');
			break;
		case ' ':
			prevdot = 0;
			if(i == 0 || startblank == 1){
				startblank = 1;
				break;
			}
			else{
				endblank = 1;
				break;
			}
		case '.':
			if(endblank == 1){
				printf("ipv42num error: wrong IP-Address format (inalid space(2))\n");
				goto done;
			}
			startblank = 0;
			if(prevdot == 1){
				printf("ipv42num error: wrong IP-Address format (two dots after one another)\n");
				goto done;
			}
			prevdot = 1;
			cyc++;
			if(cyc > 3){
				printf("ipv42num error: wrong IP-Address format (too many dots)\n");
				goto done;
			}
			break;
		default:
			printf("ipv42num error: wrong IP-Address format (invalid charakter)\n");
			goto done;
		}
	}
	if(cyc != 3){
		printf("ipv42num error: wrong IP-Address format (wrong number of dots)\n");
		goto done;
	}
	value = num[0]*256*256*256+num[1]*256*256+num[2]*256+num[3];
done:
	return(value);
}


unsigned
code_int(unsigned ip){
	long long tmp = ip;
	int random;
	//variable needed because of shift operation being unable to move unisgned 32 bits
	switch(mode){
	case(ZERO):
		tmp = ((tmp>>anonbits)<<anonbits);
		return (unsigned)tmp;
	case(RANDOMINT):
		tmp = ((tmp>>anonbits)<<anonbits);
		if(anonbits != 0){
			random = (int)((rand()/(double)RAND_MAX)*((1<<(anonbits-1))-1));
		}
		else{
			random = 0 ;
		}
		return (unsigned)tmp + random;
	}
}


int
num2ipv4(unsigned num, char *str) {
	int numip[4];
	size_t len;
	if(num > 4294967295) {
		printf("num2ipv4 invalid number(too big/negative); does not represent IPv4 address\n");
		len = snprintf(str, 16, "-1");
		goto done;
	}
	for(int i = 0 ; i < 4 ; i++){
		numip[i] = num % 256;
		num = num / 256;
	}
	len = snprintf(str, 16, "%d.%d.%d.%d", numip[3], numip[2], numip[1], numip[0]);
done:
	return len;
}

#if 0
char* findip(char* newIp){
	int i;
	for(i = 0; i < ips_count; i++){
		if(!strcmp(ips[i].ip_org, newIp)) {
			return ips[i].ip_anon;
		}
	}
	return NULL;
}


char* addip(char* newIP){
	unsigned num;
	unsigned cint;
	char caddress[16];

	ips = MvAddArray(ips);
	strcpy(ips[ips_count-1].ip_org, newIP);
	num = ipv42num(newIP);
	cint = code_int(num);
	num2ipv4(cint, caddress);
	strcpy(ips[ips_count-1].ip_anon, caddress);
	return ips[ips_count-1].ip_anon;
	
}
#endif

void getipv4(char *start, size_t end, char *address){
	int i;
	for(i = 0; i < end; i++){
		address[i] = *(start+i);
	}
	address[i] = '\0';
}

#if 0
char* addip(char *newIP){
	unsigned num;
	struct node *NewNode;
	NewNode = (struct node*)malloc(sizeof(struct node));
	if(ListLast == NULL){
		if(ListRoot == NULL){
			ListRoot = NewNode;
		}
	}else{
		ListLast->next = NewNode;
	}
	ListLast = NewNode;
	NewNode->next = NULL;

	strcpy(NewNode->ips.ip_org, newIP);
	num = ipv42num(newIP);
	num = code_int(num);
	num2ipv4(num, newIP);
	strcpy(NewNode->ips.ip_anon, newIP);
	return NewNode->ips.ip_anon;
}


char* findip(const char* IP){
	struct node* node = ListRoot;
	while(node != NULL){
		if(strcmp(node->ips.ip_org, IP) == 0){
			return node->ips.ip_anon;
		}else{
			node = node->next;
		}
	}
	return NULL;
}


char* findip(const char* IP) {
	struct tree_etry *current = Root;
	int cmp;

	while(1){
		if(current == NULL){
			return NULL;
		}
		cmp = strcmp(IP, current->ips.ip_org);
		if(cmp == 0) {
			return current->ips.ip_anon;
		} else if(cmp < 0) {
			Last = current;
			current = current->less;
			ippos = 0;
		} else {
			Last = current;
			current = current->more;
			ippos = 1;
		}
	}
}


char* addip(char* newIP) {
	struct tree_etry *new;
	unsigned num;

	new = (struct tree_etry*)malloc(sizeof(struct tree_etry));
	strcpy(new->ips.ip_org, newIP);
	num = ipv42num(newIP);
	num = code_int(num);
	num2ipv4(num, newIP);
	strcpy(new->ips.ip_anon, newIP);
	new->less = NULL;
	new->more = NULL;

	if(Root != NULL){
		if(ippos == 0){
			Last->less = new;
		} else if(ippos == 1){
			Last->more = new;
		}
	} else {
		Root = new;
	}
	return new->ips.ip_anon;		
}


void dumpips(){
	free(ips);
	ips_count = 0;
}


void dumpips() {
	struct node* current = ListRoot;
	struct node* next = current->next;
	while(next != NULL){
		next = current->next;
		free (current);
		current = next;
	}
	free(current);
	ListRoot = NULL;
	ListLast = NULL;
}
#endif
#if 0
void dumpips(){
	struct tree_etry *current = Root;
	struct tree_etry *prior;
	int MoreLess;

	while(Root->less != NULL || Root->more != NULL){
		current = Root;
		while(1){
			printf("current more: %p, less: %p\n", current->more, current->less);
			if(current->more != NULL){
				MoreLess = 0;
				prior = current;
				current = current->more;
			} else if(current->less != NULL){
				MoreLess = 1;
				prior = current;
				current = current->less;
			} else{
				printf("free\n");
				if(MoreLess == 1) {
					prior->less = NULL;
				} else {
					prior->more = NULL;
				}
				free(current);
				break;
			}
		}
	}
	free(Root);
	Root = NULL;
	Last = NULL;
}


void DelTree(struct tree_etry* node){
	if(node == NULL){
		return;
	}
	DelTree(node->more);
	DelTree(node->less);
	free(node);
}


void dumpips(){
	DelTree(Root);
	Root = NULL;
	Last = NULL;
}
#endif

void DelTree(union node* node, const int layer){
	//printf("layer: %d, address: %p\n", layer, node);
	if(node == NULL){
		return;
	}
	if(layer == 31){
		free(node);
	} else {
		DelTree(node->pointer.more, layer + 1);
		DelTree(node->pointer.less, layer + 1);
		free(node);
	}
}


void WriteTree(union node* node, const int layer, FILE* tmp, const char* nodename){
	//printf("layer: %d, address: %p\n", layer, node);
	if(node == NULL){
		return;
	}
	if(layer == 31){
		fprintf(tmp,"n%p [label=\"%s, %s\"]\n", node, node->ips.ip_high, node->ips.ip_low);
	} else {
		fprintf(tmp, "n%p [label=\"%s\"]\n", node, nodename);
		if(node->pointer.less != NULL)
			fprintf(tmp, "n%p->n%p [label=\"0\"]\n", node, node->pointer.less);
		if(node->pointer.more != NULL)
			fprintf(tmp, "n%p->n%p [label=\"1\"]\n", node, node->pointer.more);
		WriteTree(node->pointer.more, layer + 1, tmp, "");
		WriteTree(node->pointer.less, layer + 1, tmp, "");
	}
}


void dumpips(){
	FILE* tmp;
	tmp = fopen("tmp.dot", "w+");
	fprintf(tmp, "digraph\n{\n");
	WriteTree(Root, 0, tmp, "Root");
	fprintf(tmp,"\n}");
	fclose(tmp);
	DelTree(Root, 0);
	Root = NULL;
}


char* findip(char* address){
	int i;
	unsigned num;
	union node* current;
	union node* Last;
	int MoreLess;
	char* CurrentCharPtr;

	current = Root;
	num = ipv42num(address);
	for(i = 0; i < 31; i++){
		if(Root == NULL) {
			current = (union node*)malloc(sizeof(union node));
			current->pointer.more = NULL;
			current->pointer.less = NULL;
			Root = current;
		}
		Last = current;
		if((num >> (31 - i)) & 1){
			current = current->pointer.more;
			MoreLess = 1;
		} else {
			current = current->pointer.less;
			MoreLess = 0;
		}
		if(current == NULL){
			current = (union node*)calloc(1, sizeof(union node));
			if(MoreLess == 1){
				Last->pointer.more = current;
			} else {
				Last->pointer.less = current;
			}
		}
	}
	if(num & 1){
		CurrentCharPtr = current->ips.ip_high;
	} else {
		CurrentCharPtr = current->ips.ip_low;
	}
	if(CurrentCharPtr[0] != '\0'){
		return CurrentCharPtr;
	} else {
		//current = (union node*)calloc(1, sizeof(union node));
		num = code_int(num);
		num2ipv4(num, address);
		strcpy(CurrentCharPtr, address);
		return CurrentCharPtr;
	}
}


char* process_IPv4 (char* address){
	char* current;
	unsigned num;

	if(cons){
		current = findip(address);
		return current;
	}else {
		num = ipv42num(address);
		num = code_int(num);
		num2ipv4(num, address);
		return address;
	}
}


int processFile(char *str){
	FILE *file;
	char buffer[10*1024];
	size_t len;
	int line = 1;
	size_t offset = 0;
	size_t iplen;
	char address[16];
	char* caddress;

	file = fopen(str, "r");
	if(file == NULL){
		perror(str);
		return 0;
	}
	while(1) {
		if(fgets(buffer, 10*1024, file) == NULL)
			break;
		len = strlen(buffer);
		for(offset = 0; offset + 1 < len; offset ++) {
			if(syntax_ipv4(buffer + offset, len, NULL, &iplen)) {
				getipv4(buffer+offset, iplen, address);
				offset = offset + iplen -1;
				caddress = process_IPv4(address);
				printf("%s", caddress);
			} else {
				printf("%c", buffer[offset]);
			}
		}
		printf("\n");
		line++;
	}
	return 1;
}


int main (int argc, char **argv) {

	srand(time(NULL));
	int opt;	
	static struct option options[] = {
		{"anonbits", required_argument, 0, 'a'},
		{"anonmode", required_argument, 0, 'm'},
		{"difffile", no_argument, 0, 'd'},
		{0, 0, 0, 0}
	};
	int DiffFile = 0;

	while(1){
		opt = getopt_long(argc, argv, "a, m", options, NULL);
		if(opt == -1)
			break;

		switch(opt){
		case('a'):
			anonbits = atoi(optarg);
			break;
		case('m'):
			if(strcmp(optarg, "random-consistent") == 0){
				mode = RANDOMINT;
				cons = 1;
				break;
			}
			if(strcmp(optarg, "random") == 0){
				mode = RANDOMINT;
				break;
			}
			else if(strcmp(optarg, "zero") == 0){
				mode = ZERO;
				break;
			}
			else{
				printf("invalid argument to anonmode option\n");
				break;
			}
		case('d'):
			DiffFile = 1;
			break;
		default:
			printf("\n\noption not recognised\n");
		}
	}

	printf("\n\nanonbits: %d, anonmode: %d\n", anonbits, mode);
	if(0 <= anonbits && anonbits <= 32){
		for(int i = optind ; i < argc ; i++){
			printf("\n\n\n%s\n\n", argv[i]);
			processFile(argv[i]);
			if(DiffFile){
				dumpips();
			}
		}
	}
	else{
	printf("error: invalid number of anonymized bits\n");
	}
	return EXIT_SUCCESS;
}
