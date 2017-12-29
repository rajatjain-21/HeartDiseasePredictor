#include<stdio.h>
#include<string.h>

int size_train=10;
char train[11][60];

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

int dataRead(chrom data[] , chrom inp[], char path[]){ //data=encoded data, inp=decoded data

	char ch ;
	FILE *myfile = fopen( path, "r");
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

	return siz;
}

float fittness(char *s,int size_train)
{
    char st[60];
    char *b=st;
    int i=0,pos=0,neg=0,clas=0,k=0,j=0;
    float f1,f2,f3;
    strcpy(st,s);
    for(i=0;i<size_train;i++)
    {
        for(j=0;train[i][j]!='\0';j++)
        {

            if(st[j+1]=='\0' && train[i][j]==st[j])
            {
                pos++;
                continue;
            }

            else if(st[j+1]=='\0' && train[i][j]!=st[j])
            {
                neg++;
                continue;
            }

            else if(st[j]=='0')
                continue;

            else if(train[i][j]=='0')
                continue;

            else if(st[j]!='0' && train[i][j]!='0' && train[i][j]==st[j])
                continue;

            else if(st[j]!='0' && train[i][j]!='0' && train[i][j]!=st[j])
                break;
        }

        for(j=0;train[i][j]!='\0';j++)
        {
            if(st[j+1]=='\0' && train[i][j]==st[j])
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


int main()
{
    char s[60];
    scanf("%s",&s);
    printf("%s\n",s);
    int k=0;
    k=precon(s);
    printf("%d",k);
    return 0;
}
