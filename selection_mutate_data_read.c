#include<stdio.h>

typedef struct Chrom{
	char s[60];
	float fit;
}chrom;

typedef struct Pair
{
    chrom first;
    chrom second;
}pair;

chrom data[10000] , inp[10000];

pair selection(chrom data[] , int n){

	int sel1 = _rand()%n , sel2 = _rand()%n ;

	while(sel2 == sel1)
		sel2 = _rand()%n;

	pair p1;
	p1.first = data[sel1];
	p1.second = data[sel2];

	return p1;
}

pair selectionRW(chrom data[] , int n){

	int i;
	float sum = 0 , temp_sum = 0 ;

	for(i = 0; i < n ; i++)
		sum += data[i].fit;

	float sel1 = _rand()%(int)sum;
	temp_sum = sel1;

    pair p1;

	for(i=0;i<n;i++){
		if(sel1 <= 0){
			p1.first = data[i];
			break;
		}else
			sel1 -= data[i].fit;
	}

	float sel2 = _rand()%(int)sum;

	while(sel2 == temp_sum)
		sel2 = rand()%(int)sum;

	for(i=0;i<n;i++){
		if(sel2 <= 0){
			p1.second = data[i];
			break;
		}else
			sel2 -= data[i].fit;
	}

	return p1;
}

void mutate(chrom *ch){

	int len = 0,i=0;

	for(i=0;ch->s[i] != '\0' ;i++)
		len++;

	int flip_pos = _rand()%len;

	if(ch->s[flip_pos] == '0')
		ch->s[flip_pos] = '1';
	else
		ch->s[flip_pos] = '0';

}

void dataReadAndEncode(chrom data[] , chrom inp[]){

	char ch ;
	FILE *myfile = fopen("Rule Set.txt" , "r");
	int cur = 0, i = 0;
	do{
		ch = fgetc(myfile);
		if(ch == '\n'){
			data[i].s[cur] = '\0';
			inp[i].s[cur] = '\0';
			data[i].fit = 0;
			inp[i].fit = 0;
			strcpy(data[i].s,encoding(data[i].s));
			printf("%s\n",data[i].s);
			i++;
			cur = 0;
		}else{
			if(ch == ',')
				continue;
			if(ch =='*')
				ch = '0';
			data[i].s[cur] = ch;
			inp[i].s[cur] = ch;
			cur++;
		}
	}while(ch != EOF);

	fclose(myfile);
}

int main(){
	dataReadAndEncode(data,inp);
}
