
#include <stdio.h>
#define MAX 10

int n,m,eps[MAX][MAX],trans[MAX][MAX][MAX];
int closure[MAX][MAX],newTrans[MAX][MAX][MAX];
int final[MAX],newFinal[MAX];
char sym[MAX];

void dfs(int s,int v[]){
    v[s]=1;
    for(int i=0;i<n;i++)
        if(eps[s][i] && !v[i])
            dfs(i,v);
}

int main(){
    int i,j,k,q,p,t,u,s;

    printf("Enter states and symbols count: ");
    scanf("%d%d",&n,&m);

    printf("Enter symbols: ");
    for(i=0;i<m;i++) scanf(" %c",&sym[i]);

    printf("Enter epsilon transition matrix:\n");
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            scanf("%d",&eps[i][j]);

    for(k=0;k<m;k++){
        printf("Enter matrix for %c:\n",sym[k]);
        for(i=0;i<n;i++)
            for(j=0;j<n;j++)
                scanf("%d",&trans[i][j][k]);
    }

    printf("Enter final states (-1 to stop): ");
    while(scanf("%d",&s)==1 && s!=-1)
        if(s>=0 && s<n) final[s]=1;

    for(i=0;i<n;i++){
        int v[MAX]={0};
        dfs(i,v);
        for(j=0;j<n;j++)
            closure[i][j]=v[j];
    }

    for(q=0;q<n;q++)
        for(k=0;k<m;k++)
            for(p=0;p<n;p++)
                if(closure[q][p])
                    for(t=0;t<n;t++)
                        if(trans[p][t][k])
                            for(u=0;u<n;u++)
                                if(closure[t][u])
                                    newTrans[q][u][k]=1;

    for(q=0;q<n;q++)
        for(p=0;p<n;p++)
            if(closure[q][p] && final[p])
                newFinal[q]=1;

    printf("\nNFA without epsilon transitions:\n");
    for(q=0;q<n;q++)
        for(k=0;k<m;k++){
            printf("d(q%d,%c) = { ",q,sym[k]);
            for(t=0;t<n;t++)
                if(newTrans[q][t][k])
                    printf("q%d ",t);
            printf("}\n");
        }

    printf("Final states: ");
    for(i=0;i<n;i++)
        if(newFinal[i]) printf("q%d ",i);

    return 0;
}
    