#include<stdio.h>
#include<stdlib.h>

int main()
{
    float **A,**I,temp;
    int i,j,k,matsize;

    printf("Enter the size of the matrix(i.e. value of 'n' as size is nXn):");
    scanf("%d",&matsize);

    A=(float **)malloc(matsize*sizeof(float *));            //allocate memory dynamically for matrix A(matsize X matsize)
    for(i=0;i<matsize;i++)
        A[i]=(float *)malloc(matsize*sizeof(float));

    I=(float **)malloc(matsize*sizeof(float *));            //memory allocation for indentity matrix I(matsize X matsize)
    for(i=0;i<matsize;i++)
        I[i]=(float *)malloc(matsize*sizeof(float));

    printf("Enter the matrix: ");                           // ask the user for matrix A
    for(i=0;i<matsize;i++)
        for(j=0;j<matsize;j++)
            scanf("%f",&A[i][j]);

    for(i=0;i<matsize;i++)                                  //automatically initialize the unit matrix, e.g.
        for(j=0;j<matsize;j++)                              //  -       -
            if(i==j)                                        // | 1  0  0 |
                I[i][j]=1;                                  // | 0  1  0 |
            else                                            // | 0  0  1 |
                I[i][j]=0;                                  //  -       -
/*---------------LoGiC starts here------------------*/      //procedure // to make the matrix A to unit matrix

    for(k=0;k<matsize;k++)                                  //by some row operations,and the same row operations of
    {                                                       //Unit mat. I gives the inverse of matrix A
        temp=A[k][k];                   //'temp'  
        // stores the A[k][k] value so that A[k][k]  will not change
        for(j=0;j<matsize;j++)      //during the operation //A[i] //[j]/=A[k][k]  when i=j=k
        {
            A[k][j]/=temp;                                  //it performs // the following row operations to make A to unit matrix
            I[k][j]/=temp;                                  //R0=R0/A[0][0],similarly for I also R0=R0/A[0][0]
        }                                                   //R1=R1-R0*A[1][0] similarly for I
        for(i=0;i<matsize;i++)                              //R2=R2-R0*A[2][0]      ,,
        {
            temp=A[i][k];                       //R1=R1/A[1][1]
            for(j=0;j<matsize;j++)             //R0=R0-R1*A[0][1]
            {                                   //R2=R2-R1*A[2][1]
                if(i==k)
                    break;                      //R2=R2/A[2][2]
                A[i][j]-=A[k][j]*temp;          //R0=R0-R2*A[0][2]
                I[i][j]-=I[k][j]*temp;          //R1=R1-R2*A[1][2]
            }
        }
    }
/*---------------LoGiC ends here--------------------*/
    printf("The inverse of the matrix is: ");               //Print the //matrix I that now contains the inverse of mat. A
    for(i=0;i<matsize;i++)
    {
        for(j=0;j<matsize;j++)
            printf("%f  ",I[i][j]);
        printf(" ");
    }
    return 0;
}
