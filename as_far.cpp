#include<mpi.h>
#include<iostream>
#include<vector>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include<unistd.h>
using namespace std;

int main()
{
	int myrank,nprocs,initiator;
 	MPI_Init(NULL,NULL);
 	MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
 	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	//to find the maximum rank
	int max=0;

	// to initiate the random every time

	int *array = new int[nprocs];

	for(int i=0;i<nprocs;++i)
	{
		array[i]=-1;
	}
	int root=0;
	if(myrank==root)
	{
		srand(time(NULL));
		initiator=rand()%nprocs;
		cout<<"The inititaor for all the processses will be "<<initiator<<endl;
		MPI_Bcast(&initiator,1,MPI_INT,myrank,MPI_COMM_WORLD);
		
	}
	else 
	{
		MPI_Bcast(&initiator,1,MPI_INT,root,MPI_COMM_WORLD);
	}
	if(myrank == initiator)
	{

		cout<<"\nThe initiator is(IN THE INITIATOR) "<<initiator<<endl;

		array[myrank] = myrank;

		//send to the process which is 1 bigger means make a ring
		MPI_Send(array,nprocs,MPI_INT,myrank+1,999,MPI_COMM_WORLD);

		cout<<"\n\t\t"<<myrank <<" is waiting for the last process \n"<<endl;
		MPI_Recv(array, nprocs, MPI_INT, myrank-1, 999, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		cout<<"\n\t\tThe initiaor received the message \n"<<endl;
		cout <<"My rank is "<< myrank << endl<<endl;


		cout<<"The array in the rank "<<myrank<<" is"<<endl;
		for(int j=0;j<nprocs;++j)
			{
				cout<<array[j]<<" ";
			}
		cout<<endl; 

		max = 0;
		for(int i=0;i<nprocs;++i)
		{
			if(max<array[i])
				max = array[i];
		}


		cout<<"Maximum value is "<<max<<endl;

	}
	else if(myrank != initiator){
		
		MPI_Recv(array, nprocs, MPI_INT, myrank-1, 999, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		cout<<" My rank is "<<myrank<<" and I have received from "<<myrank-1<<endl;

		cout<<"The array in the rank "<<myrank<<" is"<<endl;
		for(int i=0;i<nprocs;++i)
		{
			cout << array[i] << " ";
		}


		sleep(1);
		srand(time(NULL));
		int random= rand()%2+1;
		//cout << "\nRandom value is : " << random << endl;
		//Means he wants to participitate 2 or yes and 1 for no 
		if(random==2)
			array[myrank] = myrank;
		cout<<"\nSending array to the rank "<<(myrank+1)%nprocs<<endl<<endl;

		MPI_Send(array,nprocs,MPI_INT,(myrank+1)%nprocs,999,MPI_COMM_WORLD);


	}

	MPI_Finalize();
}

