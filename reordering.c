#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

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
bool hasNodeBeenAdded(int addedNodes,int *finalMatrix,int nodeToCheck)
{
  int i;
// iretate throught the nodes that have been added
  for(i=0;i<addedNodes;i++)
  {
// if the node  that is to be checked is equal with a node already added return
// true
    if (nodeToCheck==finalMatrix[i])
    return true;
  }
  return false;
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
// function that finds the minimun index that has not yet been added
int findMinIndex(int *degrees, bool *addedRows, int grid)
{
// counter
  int i;
// value to hold the minimun value
  int min=1000000;
// counter to hold the index of the row
  int index=-1;
// loop thourght all rows
  for (i=0;i<grid;i++)
  {
// check if the row has been added and if it  is less than min
    if (degrees[i] < min && addedRows[i])
{
// set index to i
    index=i;
// update min
    min=*(degrees+i);
}

  }
// update as false the index that is to be returned
  *(addedRows+index)=false;
return  index;
}
// calculate the degrees of each row
void calDegrees(int *matrix, int grid, int *degrees)
{
// counters to loop throught all matrix values
int i,j,counter;
counter=0;
for (i=0;i<grid;i++)
{
  for (j=0;j<grid;j++)
  {
// just add value since it's 0 or 1
    counter+=matrix[i*grid+j];
  }
// update degrees
  degrees[i]=counter;
// set counter to 0
  counter=0;
}
}
