#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <ctime>
#include <conio.h>
#include <iomanip>
#include <pthread.h>
#include <omp.h>

using namespace std;

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

void mult(int **M1, int **M2, int **M3, int size, int num_t){         //Умножение матриц
	omp_set_num_threads(num_t);
	#pragma omp parallel for shared(M1,M2,M3)
	for (int i=0; i<size; i++)
            for (int j=0; j<size; j++)
            {
                int res=0;
                for (int k=0; k<size; k++)
                {
                    res+=M1[i][k]*M2[k][j];
                }
                M3[i][j]=res;
            }
}


int main(int argc, char** argv){
	int size=1000;

	int **A, **B, **C;
	int threadsNum=4;
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
	
	cout<<"\nMultiplying..."<<endl;
	
	int time1[50];
	for (int i=0; i<1; i++){
		unsigned int timeStart=clock();
		mult(A,B,C,size,threadsNum);
		unsigned int timeEnd=clock();
		time1[i]=timeEnd-timeStart;
	}
	int avg=0;
	for (int i=0; i<1; i++){
		cout<<time1[i]<<endl;
		avg=avg+time1[i];
	}
	cout<<"avg= "<<avg/1<<endl;
	free_mem(A,size);
	free_mem(B,size);
	free_mem(C,size);
	return 0;
}