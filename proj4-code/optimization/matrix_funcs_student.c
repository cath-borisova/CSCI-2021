#include <stdio.h>
#include "matrix.h"

void get_every_fifth_student(const matrix_t *matrix, long results[RESULTS_LEN]) {
    // TODO: Write a more efficient implementation of get_every_fifth
	long cols = matrix -> cols;		//make number of columns a constant
	long max = matrix -> rows * cols;		//make total number of elements a constant
	long i = 0;		//1D array index
	if(cols%5==0){			//if the matrix is even
		//while loop iterates through in intervals of 10, summing 2 elements at a time for each index of results
		while(i+9<max){		
			results[0] += matrix->data[i] + matrix->data[i+5];
			results[1] += matrix->data[i+1] + matrix->data[i+6];
			results[2] += matrix->data[i+2] + matrix->data[i+7];
			results[3] += matrix->data[i+3] + matrix->data[i+8];
			results[4] += matrix->data[i+4] + matrix->data[i+9];
			i += 10;
		}
		//if there are 5 elements left over, sum each element individually
		if(i+4<max){
			results[0] += matrix->data[i];
			results[1] += matrix->data[i+1];
			results[2] += matrix->data[i+2];
			results[3] += matrix->data[i+3];
			results[4] += matrix->data[i+4];
			i += 5;
		}
	} else {		//if the matrix is odd
		//the outer while loop iterates through each row
		while(i<max){
			long coli = 0;		//columns index
			//this while loop iterates through in intervals of 20, summing 3 elements at a time  for each index of results and making sure you haven't exceeded into the next row
			while(coli+19<cols){
				results[0] += matrix->data[i] + matrix->data[i+5] + matrix->data[i+10] + matrix->data[i+15];
				results[1] += matrix->data[i+1] + matrix->data[i+6] + matrix->data[i+11] + matrix->data[i+16];
				results[2] += matrix->data[i+2] + matrix->data[i+7] + matrix->data[i+12] + matrix->data[i+17];
				results[3] += matrix->data[i+3] + matrix->data[i+8] + matrix->data[i+13] + matrix->data[i+18];
				results[4] += matrix->data[i+4] + matrix->data[i+9] + matrix->data[i+14] + matrix->data[i+19];
				i += 20;
				coli += 20;
			}
			//if there are at least 10 elements left over in the row, sum 2 elements at a time for each index of results
			if(coli+9<cols){
				results[0] += matrix->data[i] + matrix->data[i+5];
				results[1] += matrix->data[i+1] + matrix->data[i+6];
				results[2] += matrix->data[i+2] + matrix->data[i+7];
				results[3] += matrix->data[i+3] + matrix->data[i+8];
				results[4] += matrix->data[i+4] + matrix->data[i+9];
				i += 10;
				coli += 10;
			}
			//if there are at least 5 elements left over in the row, sum each element individually
			if(coli+4<cols){
				results[0] += matrix->data[i];
				results[1] += matrix->data[i+1];
				results[2] += matrix->data[i+2];
				results[3] += matrix->data[i+3];
				results[4] += matrix->data[i+4];
				i += 5;
				coli += 5;
			}
			//for any additional elements left over in the row, sum each element individually
			while(coli<cols){
				results[coli%5] += matrix->data[i];
				i++;
				coli++;
			}
		}
	}
	
}
	
long get_every_student(const matrix_t *matrix) {
	long i = 0;		//index
	long max = matrix -> rows * matrix -> cols;		//make total number of elements in the matrix a constant
	long sum = 0;						//sum of elements
	//while loop iterates through in intervals of 10, summing 10 elements at a time
	while(i+9<max){
    		sum += matrix->data[i] + matrix->data[i+1] + matrix->data[i+2] + matrix->data[i+3] + matrix->data[i+4] + matrix->data[i+5] + matrix->data[i+6] + matrix->data[i+7] + matrix->data[i+8] + matrix->data[i+9];
		i += 10;
	}
	//if there are at least 5 elements left over, sum 5 elements at a time
	if(i+5 <=max){
		sum += matrix->data[i] + matrix->data[i+1] + matrix->data[i+2] + matrix->data[i+3] + matrix->data[i+4] + matrix->data[i+5];
		i += 5;
	}
	//sum any remaining elements individually
	while(i < max){
		sum += matrix->data[i];
		i++;
	}
	return sum;
 
}
