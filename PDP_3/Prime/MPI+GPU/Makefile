MPI_CUDA_PRIME: MPI+GPU.o CUDA.o
	mpiicc MPI+GPU.o CUDA.o -L /util/cuda/4.2.9/cuda/lib64/ -I /util/cuda/4.2.9/cuda/include/ -lcudart -o MPI_CUDA_PRIME
MPI+GPU.o: MPI+GPU.c
	mpiicc -c MPI+GPU.c -o MPI+GPU.o -w
CUDA.o: CUDA.cu
	nvcc -c CUDA.cu -o CUDA.o -w

.phony: clean
clean:
	rm -rvf MPI_CUDA_VecAdd MPI+GPU.o CUDA.o;
