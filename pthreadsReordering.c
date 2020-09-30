#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREADS 4
void *SimpleAddition(void *threadargs);
void *findMin(void *threadargs);
void *compare( void *threadargs);
// function to calculate the degrres of each node
void calDegrees(int *matrix, int grid, int * degrees);
// function to calculate node with the minimum degree
int findMinIndex(int *degrees, bool *ddedRows, int grid);
// function to check if a node thas been added
bool hasNodeBeenAdded(int addedNodes,int *finalMatrix,int nodeToCheck);
// function to update quee after inserting a node to final vector
void updateQ(int *matrix, int grid,int index, int **nodes, int *size,int *degrees);
// reordering function to calculate the permutation order of nodes
int *reordering(int * initialMatrix,int grid)
{
  // variables to use for loops and temporary for shifting
int i,j,temporary;
// variable to hold the index to enter to permutation vector
int index;
// variable to hold the extracted node from quee
int extract;
int counter=0; // this counter will show how many objects have been added to
// the finalMatrix
// pointer to hold the quee
int *nodes=NULL;
// variable to hold the size of the quee
int size=0;
// pointer to hold the permutation vector
int *finalMatrix=malloc(grid*sizeof(int));
// pointer to hold which nodes have been added to permutation vector
bool *addedRow=malloc(grid*sizeof(bool));
// pointer to hold the degrees of each node
int *degrees=malloc(grid*sizeof(int));
// calculate the degrees
calDegrees(initialMatrix,grid,degrees);
// initialize as true the added row
for (i=0;i<grid;i++)
{
  addedRow[i]=true;
}
// while not all nodes have been added
while (counter<grid)
{
// find the index of the minimum degree
index=findMinIndex(degrees,addedRow,grid);
// append to permutation vector the index-th vector
finalMatrix[counter]=index;
// increace counter
counter++;
// update the quee
updateQ(initialMatrix,grid,index,&nodes,&size,degrees);
// while the quee size is greater than zero
while (size>0)
{

  // check if nodes has allocated successfully memory
if(nodes==NULL)
{
  printf("%d\n",size );
  printf("wtf\n" );
}
// take the first value of quee
  extract=nodes[0];
// shift all the values to the left
  for(i=0;i<size-1;i++)
  {
    nodes[i]=nodes[i+1];
  }
  // reduce the size of the quee
size--;
// reallocate size for quee according to new size
  nodes=realloc(nodes,(size)*sizeof(int));
// check if the node has not been added
  if (hasNodeBeenAdded(counter,finalMatrix,extract)==false)
  {
    // if not add it to the permutation vector
*(finalMatrix+counter)=extract;
// incre counter
    counter++;
// update quee
    updateQ(initialMatrix,grid,extract,&nodes,&size,degrees);

  }

}

}
// reverse the results
if (grid%2==0)
{
  for(i=0;i<grid/2;i++)
  {
    temporary=*(finalMatrix+i);
    *(finalMatrix+i)=*(finalMatrix+grid-i-1);
    *(finalMatrix+grid-i-1)=temporary;
  }
}
else
{
for (i=0;i<(grid-1)/2;i++)
{
  temporary=*(finalMatrix+i);
  *(finalMatrix+i)=*(finalMatrix+grid-i-1);
  *(finalMatrix+grid-i-1)=temporary;
}
}
// free memory
free(addedRow);
free(degrees);
  return finalMatrix;
}
// struct to pass data in pthread that parallelize the process of finding
struct thread_data_added {
// thread id
  int thread_id;
// value that shows how many nodes have already been added to final result
  int addedNodes;
// pointer to vector with the values of nodes that have been added already
  int *finalMatrix;
// value that shows which node is to be checked of whether has already been added
  int nodeToCheck;
// value to hold the result of each thread
  bool hasNodeBeenAdded;
  pthread_t *threads;
};
// parallel function to check of whether a node has been added
bool hasNodeBeenAdded(int addedNodes,int *finalMatrix,int nodeToCheck)
{

int rc,i;
// allocate space for struct objects equal to the number of threas
struct thread_data_added *thread_data_array=(struct thread_data_added*)malloc(NUM_THREADS*sizeof(struct thread_data_added));
// create pthread pointer
pthread_t threads[NUM_THREADS];
// set proper values to each thread basically the only difference is thread id
for (i=0;i<NUM_THREADS;i++)
{

  thread_data_array[i].thread_id=i;
  thread_data_array[i].finalMatrix=finalMatrix;
  thread_data_array[i].nodeToCheck=nodeToCheck;
  thread_data_array[i].addedNodes=addedNodes;
// create each thread in order to execute compare function
  rc=pthread_create(&threads[i],NULL,compare,(void *)&thread_data_array[i]);
}
// join threads
for(i=0;i<NUM_THREADS;i++){
  pthread_join(threads[i],NULL);
}
// check the result of each thread
for(i=0;i<NUM_THREADS;i++){
// if any thread has true value that means that the nodeToCheck has already benn
// added to the finalMatrix so the function has to reutrn true
  if (thread_data_array[i].hasNodeBeenAdded)
  {

    free(thread_data_array);
return true;
  }
}
// else free point and return false
free(thread_data_array);
return false;
}
// function that is to be executed by each thread
// the main idea is to seperate the values of final matrix to each thread so
// as to speed up the check. It is important because this function will be called
// many times
void *compare( void *threadargs)
{
// declare variables to take values of the function arguments
  int i,task_id,addedNodes,nodeToCheck;
  bool hasNodeBeenAdded;
  int cr;
  int *finaMatrix;
  pthread_t *threads;
  struct thread_data_added *task_data;
  // initialize the values with the values of the struct
  task_data=(struct thread_data_added *) threadargs;
  task_id=task_data->thread_id;
  addedNodes=task_data->addedNodes;
  nodeToCheck=task_data->nodeToCheck;
  finaMatrix=task_data->finalMatrix;
  // by default the hasNodeBeenAdded value is false
  task_data->hasNodeBeenAdded=false;
// in case addedNodes/NUM_THREADS is not a perfect devision we give the extra
// load to the last thread
  if (task_id==NUM_THREADS-1)
  {
// check if value has been added
    for(i=(NUM_THREADS-1)*(addedNodes/NUM_THREADS);i<addedNodes;i++)
    {
      if(finaMatrix[i]==nodeToCheck){
// set hasNodeBeenAdded to ture if nodeToCheck is found
        task_data->hasNodeBeenAdded=true;

      }
    }
  }
  else
  {
// case for the other threads
    for(i=(task_id)*(addedNodes/NUM_THREADS);i<(task_id+1)*(addedNodes/NUM_THREADS);i++)
    {
      if(finaMatrix[i]==nodeToCheck){
        task_data->hasNodeBeenAdded=true;
        }
      }
    }
  }

// function to update Q
void updateQ(int *matrix, int grid,int index,int **nodes,int *size ,int *degrees)
{
// variables to hold
  int memory,writter;
  bool flag=true;
// store the initial quee size
  int initialQSize=*size;
// usefull countes
  int i,k;
  int j=0;
// loop through all nodes
  for (i=0;i<grid;i++)
  {
    // index + i*grid value is 1 and i not equal to index then add node
    if(matrix[index+i*grid]==1 && i!=index)
    {
      // increase size wich indicates the number of nodes in quee
    (*size)++;
// reallocate space for the new node
      *nodes=(int*)realloc(*nodes,*size*sizeof(int));
// check if reallocation was successfull
if (nodes==NULL)
{
exit(1);
printf("fucked up\n" );
}
// this is the first time quee is updated so no need for sorting
if(*size-1==initialQSize)
{
    (*nodes)[initialQSize]=i;
}
// if this is the second or more node we add we need to sort according to the
// degree of each node
else
 {
// start the sorting only for the new inserted nodes
      for(j=initialQSize;j<(*size)-1;j++)
      {
// if the degree of the newly inserted node is found to be less than some node
//  that has already be inserted it will have to be less than all the other
// nodes(those that have now be inserted) to the right so the node i has to take
// the place of node j and all the other nodes will be shifted to the right
        if (degrees[(*nodes)[j]]>degrees[i])
        {
// hold the value of the node that will be written
          writter=(*nodes)[j];
// shift nodes
          for (k=j+1;k<*size;k++)
          {
            // hold the node that the written node will take its place but
            // we dont want to lose its value
            memory=(*nodes)[k];
            // shift
            (*nodes)[k]=writter;
            // update writter with the next node that is to be shifted
            writter=memory;
          }
// set node in j place to i and set flag to be known that we have inserted at
// least 2 nodes
          (*nodes)[j]=i;
          flag=false;
// break
          break;
        }
      }
if(flag)
{
// this is a check that it's not the first element we insert and it this the one
// with the greatest degree thus it has to be placed in the end of the q
(*nodes)[*size-1]=i;

}
flag=true;
    }
  }
}

}
// struct to use to pass data to threads
struct thread_data_min {
// thread id
  int thread_id;
// the min value per thread
  int minPerThread;
  int indexPerThread;
// pointer with the degrees of each node
  int *degrees;
// bool pointer that shows whether a node has been added or not
  bool *addedRows;
  int grid;
};
// function that finds the minimun index that has not yet been added
int findMinIndex(int *degrees, bool *addedRows, int grid)
{
int i,rc,index,min;
struct thread_data_min *thread_data_array = (struct thread_data_min*)malloc(NUM_THREADS*sizeof(struct thread_data_min));
pthread_t threads[NUM_THREADS];
for (i=0;i<NUM_THREADS;i++)
{
  thread_data_array[i].thread_id=i;
  thread_data_array[i].degrees=degrees;
  thread_data_array[i].grid=grid;
  thread_data_array[i].minPerThread=10000000;
  thread_data_array[i].indexPerThread=-1;
  thread_data_array[i].addedRows=addedRows;
  rc=pthread_create(&threads[i],NULL,findMin,(void *)&thread_data_array[i]);
}

for(i=0;i<NUM_THREADS;i++){
  pthread_join(threads[i],NULL);
}
min=50000;
index=-1;
for(i=0;i<NUM_THREADS;i++){
  if (min>thread_data_array[i].minPerThread)
  {

    min=thread_data_array[i].minPerThread;
    index=thread_data_array[i].indexPerThread;
  }
}

// update as false the index that is to be returned
  *(addedRows+index)=false;
free(thread_data_array);
return  index;
}
  void *findMin(void *threadargs){
    int i,task_id,grid,minPerThread,indexPerThread;
    int *degrees;
    bool *addedRows;
    struct thread_data_min *task_data;
    task_data=(struct thread_data_min *) threadargs;
    task_id=task_data->thread_id;
    grid=task_data->grid;
    addedRows=task_data->addedRows;
    degrees=task_data->degrees;
    minPerThread=task_data->minPerThread;
    indexPerThread=task_data->indexPerThread;
    if (task_id==NUM_THREADS-1)
    {
      for(i=(NUM_THREADS-1)*(grid/NUM_THREADS);i<grid;i++)
      {

      if (minPerThread>degrees[i] && addedRows[i])
      {
        minPerThread=degrees[i];
        indexPerThread=i;
      }
      }
    }
    else
    {
      for(i=(task_id)*(grid/NUM_THREADS);i<(task_id+1)*(grid/NUM_THREADS);i++)
      {

      if(minPerThread>degrees[i] && addedRows[i]){
        minPerThread=degrees[i];
        indexPerThread=i;
      }
      }
    }
    task_data->minPerThread=minPerThread;
    task_data->indexPerThread=indexPerThread;
  }

// calculate the degrees of each row
struct thread_data{
  int thread_id;
  int *degrees;
  int grid;
  int *matrix;
};
void calDegrees(int *matrix, int grid, int *degrees)
{
  int i,rc;
pthread_t threads[NUM_THREADS];
struct thread_data *thread_data_array = (struct thread_data*)malloc(NUM_THREADS*sizeof(struct thread_data));
for (i=0;i<NUM_THREADS;i++)
{
  thread_data_array[i].thread_id=i;
  thread_data_array[i].degrees=degrees;
  thread_data_array[i].grid=grid;
  thread_data_array[i].matrix=matrix;
  rc=pthread_create(&threads[i],NULL,SimpleAddition,(void *)&thread_data_array[i]);
}
for(i=0;i<NUM_THREADS;i++){
  pthread_join(threads[i],NULL);
}
free(thread_data_array);
}
void *SimpleAddition(void *threadargs){
int i,j ,task_id, grid;
int *matrix, *degrees;
struct thread_data *task_data;
task_data=(struct thread_data *) threadargs;
task_id=task_data->thread_id;
grid=task_data->grid;
matrix=task_data->matrix;
degrees=task_data->degrees;
  if (task_id==NUM_THREADS-1)
  {
    for(i=(NUM_THREADS-1)*(grid/NUM_THREADS);i<grid;i++)
    {
      for (j=0;j<grid;j++)
      {
        degrees[i]=degrees[i]+matrix[i*grid+j];
      }
    }
  }
  else
  {
    for(i=(task_id)*(grid/NUM_THREADS);i<(task_id+1)*(grid/NUM_THREADS);i++)
    {
      for (j=0;j<grid;j++)
      {
        degrees[i]=degrees[i]+matrix[i*grid+j];
      }
    }
  }


}
