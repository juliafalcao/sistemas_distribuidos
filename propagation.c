#include <stdio.h>
#include <string.h>
#include <mpi.h>

int np = 6;
int neighbors[6][6] = {{0, 1, 1, 0, 0, 0}, {1, 0, 1, 1, 1, 0}, {1, 1, 0, 0, 0, 1}, {0, 1, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0}};

// function that counts the neighbors of a given process
int count_neighbors(int rank) {
	int i, count = 0;
	
	for (i = 0; i < np; i++) {
		if (neighbors[rank][i])
			count++;
		
		return count;
	}
}


int main(int argc, char** argv) {
    int my_rank, n_neighbors, i, tag = 50;
    int source = 0;
    char data[100] = "wow so much data\n";
    MPI_Status status;
   

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    n_neighbors = count_neighbors(my_rank);
    
    if (my_rank == source) {
    	for (i = 0; i < np; i++) {
    		if (neighbors[my_rank][i]) {
    			printf("Enviando mensagem para %d\n.", i);
    			MPI_Send(data, strlen(data)+1, MPI_CHAR, i, tag, MPI_COMM_WORLD);
    		}
    	}
    	
    	for (i = 0; i < n_neighbors; i++) {
    		MPI_Recv(data, 100, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
    		printf("Recebendo mensagem de %d\n.", status.MPI_SOURCE);
    	}
    }
    
    else {
    	MPI_Recv(data, 100, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
    	
    	for (i = 0; i < np; i++)
    		if (neighbors[my_rank][i])
    			MPI_Send(data, strlen(data)+1, MPI_CHAR, i, tag, MPI_COMM_WORLD);


    	for (i = 0; i < n_neighbors-1; i++)
    		MPI_Recv(data, 100, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
    }
    
    
    
    

    MPI_Finalize();
}
