#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define MAX_VERTEX 10000
#define MAX_EDGE 50000000

void makeset (int i);
void initial (int n);


int vertex[MAX_VERTEX];
int connect[MAX_VERTEX];

typedef struct EDGE {  //    간선과 연결된 정점 데이터와 가중치를 저장하기 위한 구조체

    int v1;
    int v2;
    int weight;

}edge;
int heapnum =0;
int set_find(int curr);
edge ea[MAX_EDGE];
edge delete_min_heap(int n);
typedef struct HEAP {  //    가장 적은 비용을 가진 간선부터 차례로 그래프 포함 여부를 판별하기 위해서사용된다.

    int n;
    edge* heap[MAX_VERTEX * MAX_VERTEX];

}Heap;
Heap* h ;

void set_union(int a, int b);

int num_ver = 0;
int num_edge = 0;

int main(int argc, char **argv) {
    clock_t start = clock();

    FILE *writefp = fopen("hw3_result.txt", "w");

    printf("argument count is %d\n", argc);

    char *inputfile;
    if(argc == 2) { //ㅡ !=2로 바꾸기
        fprintf(stderr, "please enter input file name\n");
        exit(0);
    } else {
        inputfile = argv[1];
       // FILE* readfp = fopen(inputfile, "r");
        FILE* readfp = fopen("/Users/ihaeeun/CLionProjects/hw3/input", "r");

        if(readfp == NULL){
            printf("The input file does not exist.\n");
            exit(1);
        }

        int v1tmp;
        int v2tmp;
        int weighttmp;
        fscanf(readfp,"%d",&num_ver);
        fscanf(readfp,"%d",&num_edge);

        for( int num = 0 ; num < num_edge ; num ++)
        {
            edge e;

            fscanf(readfp,"%d %d %d", &v1tmp, &v2tmp, &weighttmp);
            e.v1  = v1tmp;
            e.v2  = v2tmp;
            e.weight = weighttmp;

            connect[e.v1] = 1;
            connect[e.v2] = 1;
            int l ;
            l=++heapnum;
            while(l != 1 && e.weight < ea[l/2].weight) // 부모노드와 비교
            {
                ea[l]=ea[l/2];
                l/=2;
            }

            ea[l]=e;


        }

        int k ;
        for(k=0; k<num_ver; k++) {
            if(connect[k]!=1)
                break;
        }

        if(k==num_ver) {
            int MST[num_ver-1][3];
            initial (num_ver);
            for (int i = 0; i < num_ver-1 ; i++)
                for (int j = 0; j < 3; j++)
                    MST[i][j] = -1;
            int next_index = 0;

            int i;
            int j;
            int index =0;
            while ( index < num_ver - 1)
            {
                edge temp;
                temp = delete_min_heap(heapnum);
                i = set_find(temp.v1);
                j = set_find(temp.v2);

                if (i != j) {         // 서로 속한 집합이 다르면
                    set_union(i, j);   // 두개의 집합을 합친다.
                    MST[index][0] = temp.v1;
                    MST[index][1] = temp.v2;
                    MST[index][2] = temp.weight;
                    index++;

                }

                next_index++;

            }
            int weight = 0;

            for (i = 0; i < num_ver - 1; i++)
            {  fprintf(writefp,"%d  %d  %d\n", MST[i][0], MST[i][1], MST[i][2]);
                if (i < num_ver - 1)
                    weight = weight + MST[i][2];
            }
            fprintf(writefp,"%d", weight);
            fprintf(writefp,"\nconnected");
//59657304
        }
        else{
            int weight = 0;
            for (int p = 1; p < num_ver - 1; p++)
            {  fprintf(writefp,"%d  %d  %d", ea[p].v1, ea[p].v2, ea[p].weight);
                weight = weight + ea[p].weight;
            }
            fprintf(writefp,"\n%d" ,weight);
            fprintf(writefp,"\ndisconnected");

        }
        //   Print_Heap(h->n,h );

    }

    fclose(writefp);
    clock_t end = clock();

    printf("\noutput written to hw3_result.txt.\nTime: %lf\n", (double)(end - start)/CLOCKS_PER_SEC);


}

void initial (int n)
{
    int i;
    for (i = 0; i < n; i++)
        makeset(i);
}

void makeset (int i)
{
    vertex[i] = i;
}
int set_find(int i)
{
    int j;
    j = i;
    while (vertex[j] != j)
        j = vertex[j];
    return j;
}

void set_union(int a, int b)
{

    if(a<b) {
        vertex[b] =a;
    }
    else
        vertex[a]=b;
}


edge delete_min_heap(int n)            //최소 비용 간선 리턴
{
    int parent, child;
    edge item, temp;
    if(!(n))
    {
        exit(1);
    }
    item = ea[1];
    temp = ea[(n)--];
    parent = 1;
    child = 2;
    while(child <= n)
    {
        if((child < n) && (ea[child].weight) > ea[child+1].weight)        //child중 작은값을 가리키도록
            child++;
        if(temp.weight <= ea[child].weight)
            break;
        ea[parent] = ea[child];
        parent = child;
        child *= 2;
    }
    ea[parent] = temp;
    return item;
}

