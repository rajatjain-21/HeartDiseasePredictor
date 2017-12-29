#include<stdio.h>
#include<string.h>
#include<math.h>
#include<conio.h>
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */
#include<stdbool.h>

 #define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#define MAX_CHARS 520
#define MAX_ATTRIBUTE_LEN 120

typedef struct Accuracy{
    int hits;
    int misses;
    int id;
}Accuracy_t;

void tokenizei(char line[], char tokenList[][MAX_ATTRIBUTE_LEN]);
Accuracy_t getAccuracy(char *rulesFileName, char *testFileName, int tokens);

int size_train=10;

typedef struct Chrom
{
    char s[60];
    float fit;
}chrom;

/*
typedef struct Data
{
    chrom d[10];
}data;
*/

typedef struct Pair
{
    chrom first;
    chrom second;
}pair;

void _init(chrom *chromes[],int n)
{
    int i=0;
    for(i=0;i<n;i++)
    {
        chromes[i]->s[0]='\0';
        chromes[i]->fit=0;
    }
}

int val_dec(char *d)
{
    int dec=0;
    while(*d!='\0')
    {
        dec=dec*10+(int)(*d-'0');
        d++;
    }
    return dec;
}


int val_bin(char *b)
{
    int bin=0;
    while(*b!='\0')
    {
        bin=bin*10+(int)(*b-'0');
        b++;
    }
    return bin;
}

char* encoding(char* d)
{
    //printf("Inside Encoding\n");
    char s[60];
    char* b=s;
    while(*d!='\0')
    {
        int dec=(int)(*d-'0');
        d++;
        if(dec==0)
        {
            *b='0';
            b++;
            *b='0';
            b++;
            *b='0';
            b++;
        }
        else if(dec==1)
        {
            *b='1';
            b++;
            *b='0';
            b++;
            *b='0';
            b++;
        }
        else if(dec==2)
        {
            *b='0';
            b++;
            *b='1';
            b++;
            *b='0';
            b++;
        }
        else if(dec==3)
        {
            *b='1';
            b++;
            *b='1';
            b++;
            *b='0';
            b++;
        }
        else if(dec==4)
        {
            *b='0';
            b++;
            *b='0';
            b++;
            *b='1';
            b++;
        }
        else if(dec==5)
        {
            *b='1';
            b++;
            *b='0';
            b++;
            *b='1';
            b++;
        }
        else if(dec==6)
        {
            *b='0';
            b++;
            *b='1';
            b++;
            *b='1';
            b++;
        }
        else if(dec==7)
        {
            *b='1';
            b++;
            *b='1';
            b++;
            *b='1';
            b++;
        }
    }
    *b='\0';
    char *ret = s;
    return ret;
}


char* decoding(char* b)
{
    char s[60];
    char* d=s;
    while(*b!='\0')
    {
        int c=0;
        int dec=0;
        int j=0;
        for(j=0;j<3;j++)
        {
            dec=dec+(int)(*b-'0')*pow(2,c);
            b++;
            c++;
        }
        if(dec==0)
        {
            *d='0';
            d++;
        }
        else if(dec==1)
        {
            *d='1';
            d++;
        }
        else if(dec==2)
        {
            *d='2';
            d++;
        }
        else if(dec==3)
        {
            *d='3';
            d++;
        }
        else if(dec==4)
        {
            *d='4';
            d++;
        }
        else if(dec==5)
        {
            *d='5';
            d++;
        }
        else if(dec==6)
        {
            *d='6';
            d++;
        }
        else if(dec==7)
        {
            *d='7';
            d++;
        }
    }
    *d='\0';
    char *ret = s;
    return ret;
}

long long _rand(){

	static const char filename[] = "Random Number.txt";
	FILE *file = fopen(filename, "r");

	if(file == NULL){
        printf("Unable to Open Random Number File !!!");
	}
	int count = 0;
	static int lineNumber = 0;
	long long rett = -1;
    //printf("\nLine : %d \n" , lineNumber);
	if ( file != NULL ){
    	char line[256]; /* or other suitable maximum line size */
    	while (fgets(line, sizeof(line), file) != NULL){
    		if(count == lineNumber){
    			long long ret = 0 , i;
	        	for(i=0;line[i] != '\0';i++){
    	    		ret = ret*10 + (line[i]-'0');
        		}
        		lineNumber = (lineNumber + 1)%10000;
        		rett = ret;
        		break;
    		}else{
    			count++;
    		}
	    }
	    fclose(file);
	}
	else{
    	printf("Unable to access file\n");
    	fclose(file);
	}
	return rett;
}


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


pair single_pt(chrom ch1,chrom ch2)
{
    int len=strlen(ch1.s);
    int random=_rand()%len+1;
    int i=0;
    chrom ch1_d,ch2_d; // duplicate chromes of ch1 & ch2
    for(i=0;i<random;i++)
    {
        ch1_d.s[i]=ch1.s[i];
        ch2_d.s[i]=ch2.s[i];
    }
    for(i=random;i<len;i++)
    {
        ch2_d.s[i]=ch1.s[i];
        ch1_d.s[i]=ch2.s[i];
    }
    ch1_d.s[len]='\0';
    ch2_d.s[len]='\0';
    pair p;
    p.first=ch1_d;
    p.second=ch2_d;
    return p;
}

pair two_pt(chrom ch1,chrom ch2)
{
    int len=strlen(ch1.s);
    int random1=_rand()%len+1;
    //int random2=_rand()%(len+1-random1);
    int random2=_rand()%len+1;
    while(random1>random2)
    {
        random1=_rand()%len+1;
        random2=_rand()%len+1;
    }
    //printf("%d\n",len);
    //random1=3;
    //random2=5;
    //random2=random1+random2;
    //printf("%d %d\n",random1,random2);
    //printf("%s %s\n",ch1.s,ch2.s);
    int i=0;
    chrom ch1_d,ch2_d; // duplicate chromes of ch1 & ch2
    for(i=0;i<random1;i++)
    {
        ch1_d.s[i]=ch1.s[i];
        ch2_d.s[i]=ch2.s[i];
    }
    for(i=random1;i<random2;i++)
    {
        ch2_d.s[i]=ch1.s[i];
        ch1_d.s[i]=ch2.s[i];
    }
    for(i=random2;i<len;i++)
    {
        ch1_d.s[i]=ch1.s[i];
        ch2_d.s[i]=ch2.s[i];
    }
    ch1_d.s[len]='\0';
    ch2_d.s[len]='\0';
    ch1_d.fit=0;
    ch2_d.fit=0;
    pair p;
    p.first=ch1_d;
    p.second=ch2_d;
    return p;
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


int precon(char a[])
{
    int c = 0 , i=0;
    for(i=0;a[i]!='\0';i++)
    {
        if(a[i]=='\0')
            break;
        else if(a[i+1]!='\0' && a[i]!='0') c++;
    }
    return c;
}

float fittness(char *s,chrom train[],int size_train)    //train_de
{
    char st[60];
    char *b=st;
    int i=0,pos=0,neg=0,clas=0,k=0,j=0;
    float f1,f2,f3;
    strcpy(st,s);
    for(i=0;i<size_train;i++)
    {
        for(j=0;train[i].s[j]!='\0';j++)
        {

            if(st[j+1]=='\0' && train[i].s[j]==st[j])
            {
                pos++;
                continue;
            }

            else if(st[j+1]=='\0' && train[i].s[j]!=st[j])
            {
                neg++;
                continue;
            }

            else if(st[j]=='0')
                continue;

            else if(train[i].s[j]=='0')
                continue;

            else if(st[j]!='0' && train[i].s[j]!='0' && train[i].s[j]==st[j])
                continue;

            else if(st[j]!='0' && train[i].s[j]!='0' && train[i].s[j]!=st[j])
                break;
        }

        for(j=0;train[i].s[j]!='\0';j++)
        {
            if(st[j+1]=='\0' && train[i].s[j]==st[j])
            {
                clas++;
                continue;
            }
        }

    }

    k=precon(st);
    f1=(float)pos-(float)neg;
    f2=f1/(float)clas;
    f3=f2+1/(float)k;

    return f3;
}

Accuracy_t acc(chrom pop_de[],chrom test_de[],int pop_size,int test_size)
{
    int i=0,j=0,k=0,hits=0,miss=0;
    for(i=0;i<test_size;i++)
    {
        int flag=0;
        for(j=0;j<pop_size;j++)
        {
            for(k=0;k<test_de[i].s[k]!='\0';k++)
            {
                if(pop_de[j].s[k+1]=='\0' && test_de[i].s[k]==pop_de[j].s[k])
                {
                    hits++;
                    flag=1;
                    break;
                }
                else if(pop_de[j].s[k]=='0')
                    continue;

                else if(pop_de[j].s[k]!='0' && test_de[i].s[k]!='0' && test_de[i].s[k]==pop_de[j].s[k])
                    continue;

                else if(pop_de[j].s[k]!='0' && test_de[i].s[k]!='0' && test_de[i].s[k]!=pop_de[j].s[k])
                    break;
            }
            if(flag==1)
                break;
        }
        flag=0;
    }
    float accur=(float)(hits)/(float)(test_size);
    Accuracy_t accu;
    accu.hits=hits;
    accu.misses=test_size-hits;
    accu.id=_rand();
    return accu;
}

int match(char a[], char b[])
{
    int c = 0,i=0;
    for(i=0;a[i+1]!='\0';i++)
    {
        if(a[i]==b[i])c++;
    }
    return c;
}
bool is_redundant(char a[],int size_rule_set,chrom ruleset[])
{
    int flag = 0,i=0,j=0;
    for(i=0;i<size_rule_set;i++)
    {
        char b[14];
        int size_rule=0;
        for(j=0;ruleset[i].s[j]!='\0';i++)
        {
            b[j] = ruleset[i].s[j];
            size_rule++;
        }
        b[j]='\0';
        if(min(precon(a),precon(b))==match(a,b))
        {
            if(b[size_rule]==a[size_rule])
            {
                flag = 1;
                break;
            }
        }
    }
    if(flag==1)return true;
    return false;
}

bool is_conflicting(char a[],int size_rule_set,chrom ruleset[])
{
    int flag = 0,i=0,j=0;
    for(i=0;i<size_rule_set;i++)
    {
        char b[14];
        int size_rule=0;
        for(j=0;ruleset[i].s[j]!='\0';i++)
        {
            b[j] = ruleset[i].s[j];
            size_rule++;
        }
        b[j]='\0';
        if(min(precon(a),precon(b))==match(a,b))
        {
            if(b[size_rule]!=a[size_rule])
            {
                flag = 1;
                break;
            }
        }
    }
    if(flag==1)return true;
    return false;
}

void quick_sort(chrom a[],int l,int u)
{
    int j;
    if(l<u)
    {
        j=partition(a,l,u);
        quick_sort(a,l,j-1);
        quick_sort(a,j+1,u);
    }
}

int partition(chrom a[],int l,int u)
{
    float v,temp;
    int i,j;
    v=a[l].fit;
    i=l;
    j=u+1;

    do
    {
        do
            i++;

        while(a[i].fit<v&&i<=u);

        do
            j--;
        while(v<a[j].fit);

        if(i<j)
        {
            temp=a[i].fit;
            a[i].fit=a[j].fit;
            a[j].fit=temp;
        }
    }while(i<j);

    a[l].fit=a[j].fit;
    a[j].fit=v;

    return(j);
}


int dataRead(chrom data[] , chrom inp[], char path[]){ //data=encoded data, inp=decoded data

   // printf("Inside Data Read\n");
	char ch ;
	FILE *myfile = fopen( path, "r");
	if(myfile == NULL){
        printf("%s : ",path);
        printf("Unable to open the text file !!!\n");
	}
	int cur = 0, i = 0,siz=0;
	do{
		ch = fgetc(myfile);
		if(ch == '\n'){
            siz++;
			data[i].s[cur] = '\0';
			inp[i].s[cur] = '\0';
			data[i].fit = 0;
			inp[i].fit = 0;
			strcpy(data[i].s,encoding(data[i].s));
			//printf("%s\n",data[i].s);
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

	return siz;
}

pair generate_offspring(chrom train_en[] , chrom train_de[] , chrom pop_en[] , chrom pop_de[] ,int size_train , int size_pop)
{
    int i=0;
    for(i=0;i<size_pop;i++)
    {
        pop_de[i].fit=fittness(pop_de[i].s,train_de,size_train);
        pop_en[i].fit=pop_de[i].fit;
    }
    pair pl_s,pl_c,pl_m,pl_f;
    pl_s=selectionRW(pop_en,size_pop);
    pl_c=two_pt(pl_s.first,pl_s.second);
    mutate(&pl_c.first);
    mutate(&pl_c.second);

    pl_m.first=pl_c.first;
    pl_m.second=pl_c.second;

    bool redundant1,redundant2,conflicting1,conflicting2;
    redundant1=is_redundant(decoding(pl_m.first.s),size_pop,pop_de);
    conflicting1=is_conflicting(decoding(pl_m.first.s),size_pop,pop_de);
    redundant2=is_redundant(decoding(pl_m.second.s),size_pop,pop_de);
    conflicting2=is_conflicting(decoding(pl_m.second.s),size_pop,pop_de);

    pair ini,pl_f_de;
    ini.first.s[0]='\0';
    ini.first.fit=0;
    ini.second.s[0]='\0';
    ini.second.fit=0;

    if(redundant1==false && conflicting1==false)
    {
        pl_f.first=pl_m.first;
        pl_f.first.fit=fittness(decoding(pl_f.first.s),train_de,size_train);
//        pl_f_de.first.s=decoding(pl_f.first.s);

        char *temp = decoding(pl_f.first.s);
        int j;
        for(j=0;temp[j] != '\0';j++)
            pl_f_de.first.s[j] = temp[j];

        pl_f_de.first.fit=pl_f.first.fit;
    }
    else if(redundant1==true || conflicting1==true)
        pl_f_de.first=ini.first;

    if(redundant2==false && conflicting2==false)
    {
        pl_f.second=pl_m.second;
        pl_f.second.fit=fittness(decoding(pl_f.second.s),train_de,size_train);
 //       pl_f_de.second.s=decoding(pl_f.second.s);

        char *temp = decoding(pl_f.second.s);
        int j;
        for(j=0;temp[j] != '\0';j++)
            pl_f_de.second.s[j] = temp[j];
        pl_f_de.second.fit=pl_f.second.fit;
    }
    else if(redundant2==true || conflicting2==true)
        pl_f_de.second=ini.second;

    return pl_f_de;
}

void begin(){

    chrom train_en[1000] , test_en[1000] , train_de[1000] , test_de[1000] ;
    chrom rule_set_en[1000] , rule_set_de[1000] , pop_en[1000] , pop_de[1000] , offspring_de[1000] , offspring_en[1000];
    //printf("test1\n");
    int size_train = dataRead(train_en , train_de , "heart_train.txt");
    int size_test = dataRead(test_en , test_de , "heart_test.txt");
    int size_rule_set = dataRead(rule_set_en , rule_set_de , "rule_set.txt");

    //printf("Done with reading \n");
    int size_pop=size_rule_set;
    int i , off_cntr = 0 , j;


    for(i=0;i<size_rule_set;i++)
    {
        pop_en[i] = rule_set_en[i];
        pop_de[i] = rule_set_de[i];
    }

    for(j=0;j<10;j++){// generations

        //printf("Genertion %d : \n" , j);
        for(i=0;i<30;i++){ // offspring generation

          //  printf("Offspring %d : \n" , i);
            pair ofsp = generate_offspring(train_en , train_de , pop_en , pop_de ,size_train , size_pop);
            //printf("yay !!\n");
            if(ofsp.first.s[0] != '\0')
                offspring_de[off_cntr++] = ofsp.first  ;
            if(ofsp.second.s[0] != '\0')
                offspring_de[off_cntr++] = ofsp.second ;

        }

        for(i=0;i<off_cntr;i++)
        {
            char *temp = encoding(offspring_de[i].s);
            int j;
            for(j=0;temp[j] != '\0';j++)
                offspring_en[i].s[j] = temp[j];
            //offspring_en[i].s = encoding(offspring_de[i].s);
            offspring_en[i].fit = offspring_de[i].fit;
        }

        for(i=0;i<off_cntr;i++)
            pop_de[size_pop+i] = offspring_de[i];

        quick_sort(pop_de,0,size_pop+off_cntr-1);


        for(i=0;i<off_cntr+size_pop;i++)
        {
            char *temp = encoding(pop_de[i].s);
            int j;
            for(j=0;temp[j] != '\0';j++)
                pop_en[i].s[j] = temp[j];
            //pop_en[i].s = encoding(pop_de[i].s);
            pop_en[i].fit = pop_de[i].fit;
        }

        off_cntr = 0;

    }

    //printf("Done with Genertion\n");

    //printf("Accuracy : %.2f\n",acc(pop_de,test_de,size_pop,size_test));



    FILE *f = fopen("Final_rule_set.txt","w");

    if(f == NULL)
        printf("Unable to Open File 111");

    printf("---------Rule Set After Applying Genetic Algorithm :----------- \n\n");

    printf("----------------------Encoded Rule-Set :-----------------------\n");
    for(i=0;i<size_pop+off_cntr;i++){
        printf("\t");
        for(j=0;pop_en[i].s[j] != '\0';j++)
            printf("%c",pop_en[i].s[j]);
        printf("\n");
    }

    printf("\n---------------------Decoded Rule-Set :----------------------- \n\n");

    for(i=0;i<size_pop+off_cntr;i++){
        printf("\t");
        if(pop_de[i].s[0] == '0')
            printf("*") , fputc('*',f);
        else
            printf("%c" , pop_de[i].s[0]) , fputc(pop_de[i].s[0],f);
        for(j=1;pop_de[i].s[j]!='\0';j++){
                printf(" , ");
            fputc(',',f);
            if(pop_de[i].s[j] == '0' && pop_de[i].s[j+1] != '\0')
                printf("*") , fputc('*',f);
            else
                printf("%c" , pop_de[i].s[j]) , fputc(pop_de[i].s[j],f);
        }
        fputc('\n',f);
        printf("\n");
    }

    fclose(f);

    printAccuracy(acc(pop_de,test_de,size_pop,size_test));
}

Accuracy_t getAccuracy(char *rulesFileName, char *testFileName, int tokens){
    FILE *rulesFile = fopen(rulesFileName, "r");
    Accuracy_t temp;
    temp.hits = 100;
    temp.misses = 0;
    if (rulesFile == NULL)
    {
        printf("Accuracy => Rules File not Found %s\n", rulesFileName);
        return temp;
    }

    FILE *testFile = fopen(testFileName, "r");
    if (testFile == NULL)
    {
        printf("Accuracy => Test File not Found %s\n", testFileName);
        return temp;
    }
    Accuracy_t accuracy;
    accuracy.hits = accuracy.misses = 0;

    char rulesLine[MAX_CHARS],testLine[MAX_CHARS];
    while (fgets (testLine, MAX_CHARS, testFile) != NULL)
    {
        int missed = 1;
        rewind(rulesFile);
        while(fgets (rulesLine, MAX_CHARS, rulesFile) != NULL)
        {
            if(matches(rulesLine, testLine, tokens))
            {
                accuracy.hits++;
                missed = 0;
                break;
            }
        }
        if(missed)
        {
            accuracy.misses++;
        }
    }

    fclose(rulesFile);
    fclose(testFile);

    return accuracy;
}

int matches(char rulesLine[], char testLine[], int tokens){

    printf("\n");
    char rulesTokens[tokens][MAX_ATTRIBUTE_LEN];
    tokenizei(rulesLine, rulesTokens);

    char testTokens[tokens][MAX_ATTRIBUTE_LEN];
    tokenizei(testLine, testTokens);
    int i;
    int tokenIndex;
    for(tokenIndex = 0; tokenIndex < tokens; tokenIndex++)
    {
        if(strcmp(rulesTokens[tokenIndex], "*") == 0)
            continue;
        else if(strcmp(rulesTokens[tokenIndex], testTokens[tokenIndex]) != 0)
            return 0;
    }
    return 1;

}

void tokenizei(char line[], char tokenList[][MAX_ATTRIBUTE_LEN]){

    char *cpy = strdup(line);
    char *token;
    int tokenIndex;
    for(token = strtok(cpy, ","), tokenIndex = 0;
        token && *token; token = strtok(NULL, ",\n"), tokenIndex++)
    {
        strcpy(tokenList[tokenIndex], token);
    }

}

void printAccuracy(Accuracy_t accuracy)
{
    printf("\n\nACCURACY\n\n");
    printf("Hits : %d\n", accuracy.hits);
    printf("Misses : %d\n",accuracy.misses);
    printf("ID : %d\n", accuracy.id);
    printf("Hit Percentage: %lf%\n", (100.0*accuracy.hits)/(accuracy.hits + accuracy.misses));
}


int main()
{
    begin();
    printf("done");
    /*
    char s[60];
    scanf("%s",&s);
    printf("%s\n",s);
    char ans[60];
    char t[60];
    *t='\0';
    printf("\n");
    strcpy(ans,decoding(s));
    printf("%s\n",ans);
    printf("\n");
    strcat(t,ans);
    printf("%s\n",t);
    /*
    printf("%d\n",_rand());
    chrom chromes[2];
    scanf("%s",&chromes[0].s);
    fflush(stdin);
    scanf("%s",&chromes[1].s);
    fflush(stdin);
    pair p=two_pt(chromes[0],chromes[1]);
    printf("%s %s\n",p.first.s,p.second.s);
    */
    return 0;
}
