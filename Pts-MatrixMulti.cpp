#include <iostream>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <pthread.h>
#include <stdio.h>
#include <bits/stdc++.h>
using namespace std::chrono;
using namespace std;
const int size = 10;
//External Code Refernece: Modified to matrices
//Author:Ali Hassani
#define NUM_THREADS 4

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 int row_i = 0;

 int ** a;
 int ** b;
 int ** c;


struct work
{
    int **a,**b,**c;
    int start;
    int end;

  
};

struct randomwork
{
 int seed_factor;
    int **matrix;
    int start;
    int end;


};


void *populateMatrix(void *args)
{
    
    randomwork *task = ((struct randomwork *)args);
    
    //Seed the srand using time and random seed factor
    srand(time(NULL) * task->seed_factor);
 
    for (int i = task->start; i < task->end; i++)
    {
      for  (int j = 0; j < size; j++)
        {

            task->matrix[i][j] = rand() % 100;
        }
        
    }

    return NULL;
}

//Add Vector Thread method
void *multiplyMatrix(void *args)
{

    //cast the void* args to AddTask*
    work *wtask = ((struct work *)args);

    
    for (int i = wtask->start; i < wtask->end; i++)
    {
       
      for (int j = 0; j <size; j++)
      {
        for (int k =0; k < size; k++)
            {     
              wtask->c[i][j] = wtask->a[i][k] * wtask->b[k][j];
            }
      }

    }
    return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{

auto start = high_resolution_clock::now();
    srand(time(0));

pthread_t threads[NUM_THREADS]; 

//create an array of threads for random data generation
    pthread_t randomThreads[NUM_THREADS];
 
 //We use this seed factor to make sure the srand get a unique seed inside randomVector method
    int seed_factor = 16;

    //calculates the partition size for random data generation
    int partition_size = size / (NUM_THREADS / 2);
 

 a = new int*[size];
 for (int j = 0; j < size; ++j){
    a[j] = new int[size];
}

 b = new int*[size];
for (int j = 0; j < size; ++j){
    b[j] = new int[size];
}

 c = new int*[size];
for (int j = 0; j < size; ++j){
    c[j] = new int[size];
}
 
 


 //use half of the available threads for randomly populating first matrix
    for (size_t i = 0; i < NUM_THREADS / 2; i++)
    {
        //create a pointer to Random Task
        struct randomwork *rwork = (struct randomwork *)malloc(sizeof(struct randomwork));

        //set the random_task values

        //unique number for seed generation
        rwork->seed_factor = seed_factor++;

        
        //rwork->matrix = a;
         rwork->matrix=a;
        //a = rwork->matrix;

        //set the start index of for loop
       rwork->start = i * partition_size;

        //cacluates the end index of for loop, if it's the last thread, the end index should be same as matrix size
        rwork->end = (i + 1) == (NUM_THREADS / 2) ? size : ((i + 1) * partition_size);

        //start random data generation thread
        pthread_create(&randomThreads[i], NULL, populateMatrix, (void *)rwork);
    }


cout << " I did not here" << endl;
for (size_t i = 0; i < NUM_THREADS / 2; i++)
    {
        struct randomwork *rwork2 = (struct randomwork *)malloc(sizeof(struct randomwork));
        rwork2->seed_factor = seed_factor++;
        rwork2->matrix = b;
        //b= rwork2->matrix;
        rwork2->start = i * partition_size;
        rwork2->end = (i + 1) == (NUM_THREADS / 2) ? size : ((i + 1) * partition_size);
        pthread_create(&randomThreads[i + NUM_THREADS / 2], NULL, populateMatrix, (void *)rwork2);
    }
  //wait until all the random data generation threads joined the main thread
    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(randomThreads[i], NULL);
    }

    //calcuates the partion size for the matrix multiplication
    partition_size = size / NUM_THREADS;

    //creatte threads for matrix additions
    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        //create a pointer to AddTask construct and allocate memory for in the heap
        struct work *worker = (struct work *)malloc(sizeof(struct work));

        //initialise the Add Task
        worker->a = a;
        worker->b = b;
        worker->c = c;

        //calculates the start and end of each thread (based on partition size)
        worker->start = i * partition_size;  
        worker->end = (i + 1) == NUM_THREADS ? size : ((i + 1) * partition_size); 

        //start work multiplication thread
        pthread_create(&threads[i], NULL, multiplyMatrix, (void *)worker);
    }
for (size_t i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }



 cout << endl
         << "Multiplication of A and B" << endl;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
            cout << a[i][j] << " ";       
        cout << endl;
    }

cout << endl
         << "Multiplication of A and B" << endl;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
            cout << b[i][j] << " ";       
        cout << endl;
    }

   auto stop = high_resolution_clock::now();
auto duration = duration_cast<milliseconds>(stop - start);
    



    cout << "Time taken by function: "
         << duration.count() << " milliseconds" << endl;
    
 
// Displaying the result matrix
   cout << endl
         << "Multiplication of A and B" << endl;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
            cout << c[i][j] << " ";       
        cout << endl;
    }




    return 0;





}