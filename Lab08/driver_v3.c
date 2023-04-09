// gcc -framework OpenCL driver_v3.c -o ocl
// ./ocl

// Result:
// (base) wenbo@WenbodeMacBook-Pro Lab08 % gcc -framework OpenCL driver_v3.c -o ocl
// (base) wenbo@WenbodeMacBook-Pro Lab08 % ./ocl                                   
// 1 platform(s) found
// MATRIX A
// 1.000000        1.000000
// 1.000000        2.000000
// 1.000000        3.000000
// 1.000000        4.000000
// MATRIX B
// 1.000000        1.000000        1.000000
// 1.000000        2.000000        3.000000
// MATRIX AB (RESULT)

// 2.000000        3.000000        4.000000
// 3.000000        5.000000        7.000000
// 4.000000        7.000000        10.000000
// 5.000000        9.000000        13.000000

// Benchmark:

// (base) wenbo@WenbodeMacBook-Pro Lab08 % hyperfine "./ocl"
// Benchmark 1: ./ocl
//   Time (mean ± σ):      23.0 ms ±   1.8 ms    [User: 6.4 ms, System: 7.7 ms]
//   Range (min … max):    19.4 ms …  28.4 ms    107 runs

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <OpenCL/opencl.h>

////////////////////////////////////////////////////////////////////////////////

// Use a static data size for simplicity
//

////////////////////////////////////////////////////////////////////////////////

const char* KernelSource2 = "__kernel void matrix_mult("
    "__global float* a, __global float* b, __global float* result,"
    "const int md, const int nd, const int kd) {\n"
"    int m = get_global_id(0);\n"
"    int n = get_global_id(1);\n"
"    float temp = 0.0f;\n"
"    for (int k = 0; k < kd; k++) {\n"
"        temp += a[m*kd+k] * b[k*nd+n];\n"
"    }\n"
"    result[m*nd+n] = temp;\n"
"}\n";
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    int err;
    int md = 4;
    int kd = 2;
    int nd = 3;

    cl_uint numPlatforms = 0;
    err = clGetPlatformIDs(0, NULL, &numPlatforms);
    if (err == CL_SUCCESS)
        printf("%u platform(s) found\n", numPlatforms);
    else
        printf("clGetPlatformIDs(%i)\n", err);


    printf("MATRIX A\n");
    float matrix_a[md*kd];
    for (int m=0; m < md; m++) {
        for (int k=0; k < kd; k++) {
            matrix_a[m*kd+k] = m*k+1;
            printf("%f\t", matrix_a[m*kd+k]);
        }
        printf("\n");
    }

    printf("MATRIX B\n");
    float matrix_b[kd*nd];
    for (int k=0; k < kd; k++) {
        for (int n=0; n < nd; n++) {
            matrix_b[k*nd+n] = k*n+1;
            printf("%f\t", matrix_b[k*nd+n]);
        }
        printf("\n");
    }
    printf("MATRIX AB (RESULT)\n");
    float matrix_result[md*nd];

    cl_device_id device_id;
    cl_context context;
    cl_command_queue commands;
    cl_program program;
    cl_kernel kernel;

    cl_mem input1;
    cl_mem input2;
    cl_mem output;


    // Connect to a compute device
    int gpu = 1;
    err = clGetDeviceIDs(NULL, gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to create a device group!\n");
        return EXIT_FAILURE;
    }

    // Create a compute context 
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
    if (!context)
    {
        printf("Error: Failed to create a compute context!\n");
        return EXIT_FAILURE;
    }

    // Create a command commands
    commands = clCreateCommandQueue(context, device_id, 0, &err);
    if (!commands)
    {
        printf("Error: Failed to create a command commands!\n");
        return EXIT_FAILURE;
    }

    // Create the compute program from the source buffer
    program = clCreateProgramWithSource(context, 1, (const char **)&KernelSource2, NULL, &err);
    if (!program)
    {
        printf("Error: Failed to create program");
    }

    // Build the program executable
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to build program executable\n");
        exit(1);
    }

    // Create the compute kernel in the program we wish to run
    kernel = clCreateKernel(program, "matrix_mult", &err);
    if (!kernel || err != CL_SUCCESS)
    {
        printf("Error: Failed to create compute kernel!\n");
        exit(1);
    }

    // Create the input and output arrays in device memory for our calculation
    input1 = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(float) * md*kd, NULL, NULL);
    input2 = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(float) * kd*nd, NULL, NULL);
    output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * md*nd, NULL, NULL);
    if (!input1 || !input2 || !output)
    {
        printf("Error: Failed to allocate device memory!\n");
        exit(1);
    }    

    // Write our data set into the input array in device memory 
    err = clEnqueueWriteBuffer(commands, input1, CL_TRUE, 0, sizeof(float) * md*kd, matrix_a, 0, NULL, NULL);
    err |= clEnqueueWriteBuffer(commands, input2, CL_TRUE, 0, sizeof(float) * kd*nd, matrix_b, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to write to source array!\n");
        exit(1);
    }

    // Set the arguments to our compute kernel
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input1);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &input2);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &output);
    err |= clSetKernelArg(kernel, 3, sizeof(int), &md);
    err |= clSetKernelArg(kernel, 4, sizeof(int), &nd);
    err |= clSetKernelArg(kernel, 5, sizeof(int), &kd);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to set kernel arguments! %d\n", err);
        exit(1);
    }

    // Execute the kernel over the entire range of our 1d input data set
    size_t global[2] = { md, nd };
    err = clEnqueueNDRangeKernel(commands, kernel, 2, NULL, global, NULL, 0, NULL, NULL);
    if (err)
    {
        printf("Error: Failed to execute kernel!\n");
        return EXIT_FAILURE;
    }

    // Wait for the command commands to get serviced before reading back results
    clFinish(commands);

    // Read back the results from the device to verify the result matrix
    err = clEnqueueReadBuffer(commands, output, CL_TRUE, 0, sizeof(float) * md*nd, matrix_result, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to read output array! %d\n", err);
        exit(1);
    }

    printf("\n");

    for (int m=0; m < md; m++) {
        for (int n=0; n < nd; n++) {
            printf("%f\t", matrix_result[m*nd+n]);
        }
        printf("\n");
    }

    clReleaseMemObject(input1);
    clReleaseMemObject(input2);
    clReleaseMemObject(output);

    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);

    return 0;


    
}
