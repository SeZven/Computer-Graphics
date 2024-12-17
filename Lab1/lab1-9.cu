# help of AI
#include <stdlib.h>
#include <stdio.h>

#define N 10000000

__global__ void vector_add(float *out, float *a, float *b, int n) {
	int index = blockIdx.x * blockDim.x + threadIdx.x;
	if(i < n){
		out[i] = a[i] + b[i];
	}
}

int main(){
	float *a, *b, *out; 
	float *a_device, *b_device, *out_device; //GPU

	// Allocate memory
	a   = (float*)malloc(sizeof(float) * N);
	b   = (float*)malloc(sizeof(float) * N);
	out = (float*)malloc(sizeof(float) * N);

	// Allocate memory GPU
    cudaMalloc((void**)&a_device, sizeof(float) * N);
    cudaMalloc((void**)&b_device, sizeof(float) * N);
    cudaMalloc((void**)&out_device, sizeof(float) * N);

	// Initialize array
	for(int i = 0; i < N; i++){
		a[i] = rand()%10; b[i] = rand()%10;
	}

	 // Copy data from CPU to GPU
    cudaMemcpy(a_device, a, sizeof(float) * N, cudaMemcpyHostToDevice);
    cudaMemcpy(b_device, b, sizeof(float) * N, cudaMemcpyHostToDevice);

	// #Threads and #Blocks
	int threadsPerBlock = 256;
    int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;

	// Add the vectors
	vector_add<<<blocksPerGrid, threadsPerBlock>>>(out_device, a_device, b_device, N);

	// Result from GPU to CPU
	cudaMemcpy(out, out_device, sizeof(float) * N, cudaMemcpyDeviceToHost);

	// Check the result
	printf( "result[0], %.0f + %.0f = %.0f\n", a[0],b[0],out[0] );
	printf( "result[N-1], %.0f + %.0f = %.0f\n", a[N-1],b[N-1],out[N-1] );
}

