
#include "pthreadsReordering.c"
#include <time.h>
int *generateMatrix(int grid);
int main()
{
// clock to measure time
  clock_t start, end;
     double cpu_time_used;
srand(time(0));
  int i,j;
// size of matrix
int grid=1000;
// generate random values
int * matrix=generateMatrix(grid);
// allocate memory
int *finaMatrix=malloc(grid*sizeof(int));
//star clock
start=clock();
//reorder
finaMatrix=reordering(matrix,grid);
// end clock
end=clock();
// calculate time
cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

//calculate bandwidth
int min, max, bandwidth;
bool minInserted=false;
min=0;
max=0;
bandwidth=0;
for(i=0;i<grid;i++)
{
  for (j=0;j<grid;j++){

    if (matrix[finaMatrix[i]*grid+j]==1 )
    {
      if(minInserted==false)
      {
        min=j;
        minInserted=true;
}
    max=j;

    }

  }
if(max-min>bandwidth)
{
  bandwidth=max-min;
}
minInserted=false;
min=0;
max=0;

}
// print time
printf("%f\n",cpu_time_used );
// print bandwidth
printf("%d\n",bandwidth );
free(finaMatrix);
free (matrix);
  return 0;
}
int *generateMatrix(int grid){
// generate random values with small propability to produce 1
int * matrix=malloc(grid*grid*sizeof(int));
 int i,j;
 for(i=0;i<grid;i++){
 for (j=0;j<i;j++)
 {
if ((rand()%1000)==0)
   {
     matrix[i*grid+j]=1;
     matrix[j*grid+i]=1;
}
else
{
  matrix[i*grid+j]=0;
  matrix[j*grid+i]=0;
}
 }

 }
for(i=0;i<grid;i++){
  matrix[i*grid+i]=0;
}
return matrix;
}
