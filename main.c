
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
typedef enum {false,true}bool;

typedef struct Dl_Node_Tag
{
    int row,col;
    struct Dl_Node_Tag *next,*prev;
}DL_Node;

typedef struct Linked_list
{
    int row,col;
}Arr_of_ll;

typedef struct Priority_queue
{
    Arr_of_ll Arr[4];
}Pri_que;

DL_Node* DL_InsertAtStart(int i,int j,DL_Node *dlptr)
{
    DL_Node *ptr;
    ptr = (DL_Node*)malloc(sizeof(DL_Node));
    ptr->row = i;
    ptr->col = j;
    ptr->next = NULL;
    ptr->prev = NULL;
    if(dlptr==NULL)
    {
        dlptr = ptr;
    }
    else
    {
        ptr->next = dlptr;
        dlptr->prev = ptr;
        dlptr = ptr;
    }
    return dlptr;
}

bool isValidCell(int x, int y, int M, int N)
{
    if (x < 0 || y < 0 || x >= M || y >= N)
    {
        return false;
    }
    return true;
}

int min = INT_MAX;

void Check(int M, int N, int visited[][N], DL_Node *dlptr, int Answer[][N])
{
    DL_Node *dl;
    int cost = 0,found = 0;
    for(int i = 0;i<M;i++)
    {
        for(int j=0;j<N;j++)
        {
            dl = dlptr;
            found = 0;
            if(visited[i][j] == 1)
            {
                while(dl!=NULL && found == 0)
                {
                    if(dl->row == i && dl->col == j)
                    {
                        found = 1;
                        cost++;
                    }
                    dl = dl->next;
                }
            }
        }
    }
    if(cost < min)
    {
        min = cost;
        for(int i=0;i<M;i++)
        {
            for(int j=0;j<N;j++)
            {
                Answer[i][j] = visited[i][j];
            }
        }
    }
}

void countPaths(int N, int M, int maze[][N], int x, int y, int D1, int D2, int visited[][N], int *count,DL_Node *dlptr, int Answer[][N])
{
    Pri_que p;

    visited[x][y] = 1;
    if (x == D1 && y == D2)
    {
        //visited[D1][D2]==1;
        Check(M,N,visited,dlptr,Answer);
        (*count)++;
        visited[D1][D2]=0;
        return;
    }
    // mark current cell as visited
    // if current cell is a valid and open cell,
    if (isValidCell(x, y, M, N) && maze[x][y]!=0)
    {
        int dx=0;
        // go down (x, y) --> (x + 1, y)
        if (x + 1 < M && !visited[x + 1][y])
        {
            p.Arr[dx].row = x+1;
            p.Arr[dx].col = y;
            dx++;
        }
        else
        {
            p.Arr[dx].row = -1;
            p.Arr[dx].col = -1;
            dx++;
        }
        // go up (x, y) --> (x - 1, y)
        if (x - 1 >= 0 && !visited[x - 1][y])
        {
            p.Arr[dx].row = x-1;
            p.Arr[dx].col = y;
            dx++;
        }
        else
        {
            p.Arr[dx].row = -1;
            p.Arr[dx].col = -1;
            dx++;
        }
        // go right (x, y) --> (x, y + 1)
        if (y + 1 < N && !visited[x][y + 1])
        {
            p.Arr[dx].row = x;
            p.Arr[dx].col = y+1;
            dx++;
        }
        else
        {
            p.Arr[dx].row = -1;
            p.Arr[dx].col = -1;
            dx++;
        }
        // go left (x, y) --> (x, y - 1)
        if (y - 1 >= 0 && !visited[x][y - 1])
        {
            p.Arr[dx].row = x;
            p.Arr[dx].col = y-1;
            dx++;
        }
        else
        {
            p.Arr[dx].row = -1;
            p.Arr[dx].col = -1;
            dx++;
        }
        for(int i=0;i<4;i++)
        {
            if(p.Arr[i].row != -1)
            {
                countPaths(N,M,maze, p.Arr[i].row, p.Arr[i].col, D1, D2 ,visited, count,dlptr,Answer);
            }
        }
    }
    // backtrack from current cell and remove it from current path
    visited[x][y] = 0;
}

int main()
{
    int i, j;
    int count = 0;
    FILE *fptr;
    fptr = fopen("input.txt","r");
    // scan the source and Destination
    int Sx,Sy,Dx,Dy,M,N;
    fscanf(fptr,"%d %d %d %d",&Sx,&Sy,&Dx,&Dy);
    fscanf(fptr,"%d %d",&M,&N);
    int maze[M][N];
    int Answer[M][N];
    int visited[M][N];
    int temp;
    DL_Node *dlptr = NULL;
    for(i=0;i<M;i++)
    {
        for(j =0;j<N;j++)
        {
            fscanf(fptr, "%d", &temp);
            if(temp == 2) //if cell is costly
            {
                dlptr = DL_InsertAtStart(i,j,dlptr);
            }
            maze[i][j] = temp;
            visited[i][j]=0;
        }
    }
    countPaths(N , M, maze, Sx, Sy, Dx, Dy, visited, &count, dlptr, Answer);
    printf("Total number of unique paths are %d \n",count);
    FILE *f;
    f = fopen("output.txt","a");
    for(int i=0;i<M;i++)
    {
        for (int j = 0; j < N; j++)
        {
            if(j!=N-1)
            {
                fprintf(f, "%d ", Answer[i][j]);
            }
            else
            {
                fprintf(f, "%d", Answer[i][j]);
            }
        }
        fprintf(f, "\n");
    }
    fprintf(f,"Total cost : %d",min);
    fclose(f);
    return 0;
}
