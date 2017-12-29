#include<stdio.h>
#include<conio.h>
#include<stdbool.h>

 #define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })


typedef struct Chrom
{
    char s[60];
    float fit;
}chrom;


void quick_sort(chrom a[],int,int);
int partition(chrom a[],int,int);
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
int precon(char a[])
{
    int c = 0,i=0;
    for(i=0;a[i]!='\0';i++)
    {
        if(a[i+1]!='\0' && a[i]!='0') c++;
    }
    return c;
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
bool is_redundant(char a[],int size_rule_set,int ruleset[][14])
{
    int flag = 0,i=0,j=0;
    for(i=0;i<size_rule_set;i++)
    {
        char b[14];
        int size_rule=0;
        for(j=0;ruleset[i][j]!='\0';i++)
        {
            b[j] = ruleset[i][j];
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

bool is_conflicting(char a[],int size_rule_set,int ruleset[][14])
{
    int flag = 0,i=0,j=0;
    for(i=0;i<size_rule_set;i++)
    {
        char b[14];
        int size_rule=0;
        for(j=0;ruleset[i][j]!='\0';i++)
        {
            b[j] = ruleset[i][j];
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


int main()
{
    return 0;
}
