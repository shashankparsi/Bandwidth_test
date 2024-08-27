#include<stdio.h>
#include<hip/hip_runtime.h>
#define ITERATIONS 10000
void checkbandwidth(hipStream_t& stream,size_t datasize)
{
	int devicecnt;
	hipGetDeviceCount(&devicecnt);
	printf("Number of GPU's in system are : %d\n",devicecnt);
	float *hdata = new float[datasize];
	if(hdata == NULL)
	{
		printf("memory allocation failed on HOST\n");
		return;
	}
	float *hdata1=new float[datasize];
	if(hdata1==NULL)
	{
		printf("memory allocation failed on HOST\n");
		return;
	}
	for(size_t i=0;i<datasize;i++)
	{
		hdata[i]=static_cast<float>(i);
	}
	float *ddata=NULL;
	float *ddata1=NULL;
	hipMalloc((void**)&ddata,datasize*sizeof(float));
	hipMalloc((void**)&ddata1,datasize*sizeof(float));
	hipStreamSynchronize(stream);
	hipEvent_t start,stop;
	hipEventCreate(&start);
	hipEventCreate(&stop);
	hipEventRecord(start,stream);
	for(int i=0;i<ITERATIONS;i++)
	{
		hipMemcpyAsync(ddata,hdata,datasize*sizeof(float),hipMemcpyHostToDevice,stream);
	}
	hipEventRecord(stop,stream);
	hipStreamSynchronize(stream);
	float milli_sec = 0;
	hipEventElapsedTime(&milli_sec,start,stop);
	const float bandwidth = ((float(datasize)*sizeof(float)*float(ITERATIONS))/(milli_sec * 1e6));
	printf("CPU to GPU Bandwidth is :%0.3f GB/s\n",bandwidth);
	
	hipEventCreate(&start);
        hipEventCreate(&stop);
        hipEventRecord(start,stream);
        for(int i=0;i<ITERATIONS;i++)
        {
                hipMemcpyAsync(hdata,ddata,datasize*sizeof(float),hipMemcpyDeviceToHost,stream);
        }
        hipEventRecord(stop,stream);
        hipStreamSynchronize(stream);
        float milli_sec1 = 0;
        hipEventElapsedTime(&milli_sec1,start,stop);
        const float bandwidth_one = ((float(datasize)*sizeof(float)*float(ITERATIONS))/(milli_sec1 * 1e6));
        printf("GPU to CPU Bandwidth is :%0.3f GB/s\n",bandwidth_one);

	hipEventCreate(&start);
        hipEventCreate(&stop);
        hipEventRecord(start,stream);
        for(int i=0;i<ITERATIONS;i++)
        {
                hipMemcpyAsync(ddata1,ddata,datasize*sizeof(float),hipMemcpyDeviceToDevice,stream);
        }
        hipEventRecord(stop,stream);
        hipStreamSynchronize(stream);
        float milli_sec2 = 0;
        hipEventElapsedTime(&milli_sec2,start,stop);
        const float bandwidth_two = ((float(datasize)*sizeof(float)*float(ITERATIONS))/(milli_sec2 * 1e6));
        printf("GPU to GPU Bandwidth is :%0.3f GB/s\n",bandwidth_two);

	hipEventCreate(&start);
        hipEventCreate(&stop);
        hipEventRecord(start,stream);
        for(int i=0;i<ITERATIONS;i++)
        {
                hipMemcpyAsync(hdata1,hdata,datasize*sizeof(float),hipMemcpyHostToHost,stream);
        }
        hipEventRecord(stop,stream);
        hipStreamSynchronize(stream);
        float milli_sec3 = 0;
        hipEventElapsedTime(&milli_sec3,start,stop);
        const float bandwidth_three = ((float(datasize)*sizeof(float)*float(ITERATIONS))/(milli_sec3 * 1e6));
        printf("CPU to CPU Bandwidth is :%0.3f GB/s\n",bandwidth_three);

	hipFree(ddata);
	delete[] hdata;
	hipEventDestroy(start);
	hipEventDestroy(stop);
}

int main()
{
	hipStream_t stream;
	hipStreamCreate(&stream);
	const size_t datasize= 1<<20;
	checkbandwidth(stream,datasize);
	hipStreamDestroy(stream);
	return 0;
}


