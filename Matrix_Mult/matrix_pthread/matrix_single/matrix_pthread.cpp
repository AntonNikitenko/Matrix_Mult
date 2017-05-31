#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <ctime>
#include <conio.h>
#include <iomanip>
#include <pthread.h>

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS               0

using namespace std;


typedef struct args{
	int start;
	int end;
	int size;
	int t_num;
	int **A, **B, **C;
}args_t;

void get_mem(int ***matrix, int size){        //Выделение памяти для матриц
    (*matrix)=new int* [size];
    for (int i=0; i<size; i++)
    {
		(*matrix)[i]=new int [size];
    }
}

void free_mem(int **&matrix, int size){      //Освобождение памяти
	for(int i=0; i<size; i++)
        delete[] matrix[i];
	delete[] matrix;
	matrix=NULL;
}

void* mult(void *argg){         //Умножение матриц
	args_t *arg=(args_t*) argg;
	int start=arg->start;
	int end=arg->end;
	int size=arg->size;

	
	for (int i=start; i<end+1; i++)
            for (int j=0; j<size; j++)
            {
                int res=0;
                for (int k=0; k<size; k++)
                {
					res+=arg->A[i][k]*arg->B[k][j];
                }
				arg->C[i][j]=res;
            }
	
	return NULL;
}


int main(int argc, char** argv){
	
	int **A;
	int **B;
	int **C;
	int size=1000;
	get_mem(&A,size);
	get_mem(&B,size);
	get_mem(&C,size);
	 
	ifstream in1("M1.txt");
	if (in1.is_open())
	{
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				in1 >> A[i][j];
		in1.close();
	}
	else
	{
		cout << "File not found";
	}
	ifstream in2("M2.txt");
	if (in2.is_open())
	{
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				in2 >> B[i][j];
		in2.close();
	}
	else
	{
		cout << "File not found";
	}
	for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				C[i][j]=0;
	
	

	const int num_threads=2;	
	int step=size/num_threads;
	int st=0;
	int endd=0;
	pthread_t threads[num_threads];
	int status;
	int status_addr;
	args_t arg[num_threads];
	for (int i=0; i<num_threads; i++){
		arg[i].A=A;
		arg[i].B=B;
		arg[i].C=C;
		arg[i].size=size;
		arg[i].t_num=i;
		arg[i].start=st;
		st=st+step;
		arg[i].end=st-1;
	}

	cout<<"\nMultiplying..."<<endl;
	int time1[50];	
	for (int s=0;s<1; s++){
		unsigned int timeStart=clock();
		for (int i = 0; i < num_threads; i++) {
			status = pthread_create(&threads[i], NULL, mult, (void*) &arg[i]);
			if (status != 0) {
				printf("Error: can't create thread, status = %d\n", status);
				exit(ERROR_CREATE_THREAD);
			}
		}

	
		for (int i = 0; i < num_threads; i++) {
			status = pthread_join(threads[i], (void**)&status_addr);
			if (status != SUCCESS) {
			    printf("Error: can't join thread, status = %d\n", status);
			    exit(ERROR_JOIN_THREAD);
			}
		}
		int am=0;
		int num_t=0;
		for (int k=0; k<size; k++){
			am++;
			for (int j=0; j<size; j++){
				C[k][j]=arg[num_t].C[k][j];
			}
			if (am==step){
				am=0;
				num_t++;
			}	
		}
		unsigned int timeEnd=clock();
		time1[s]=timeEnd-timeStart;
	}
	int avg=0;
	for (int k=0;k<1;k++){
		cout<<time1[k]<<endl;
		avg=avg+time1[k];
	}
	cout<<"avg= "<<avg/1<<endl;




	free_mem(A,size);
	free_mem(B,size);
	free_mem(C,size);
	return 0;
}