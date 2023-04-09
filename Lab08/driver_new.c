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

// Simple compute kernel which computes the square of an input array 
//
const char *KernelSource = "\n" \
"__kernel void square(                                                       \n" \
"   __global float* input,                                              \n" \
"   __global float* output,                                             \n" \
"   const unsigned int count)                                           \n" \
"{                                                                      \n" \
"   int i = get_global_id(0);                                           \n" \
"   if(i < count)                                                       \n" \
"       output[i] = input[i] * input[i];                                \n" \
"}                                                                      \n" \
"\n";


// const char* KernelSource2 = "__kernel void matrix_mult(__global float* a, __global float* b, __global float* result) {\n"
// "    int m = get_global_id(0);\n"
// "    int n = get_global_id(1);\n"
// "    float temp = 0.0f;\n"
// "    for (int k = 0; k < 2; k++) {\n"
// "        temp += a[m*k_dim+k] * b[k*n_dim+n];\n"
// "    }\n"
// "    result[m*n_dim+n] = temp;\n"
// "}\n";
const char* KernelSource2 = "__kernel void matrix_mult(__global float* a, __global float* b, __global float* result) {\n"
"    int m = get_global_id(0);\n"
"    int n = get_global_id(1);\n"
"    float temp = 0.0f;\n"
"    for (int k = 0; k < 2; k++) {\n"
"        temp += a[m*2+k] * b[k*3+n];\n"
"    }\n"
"    result[m*3+n] = temp;\n"
"}\n";

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    int err;                            // error code returned from api calls
    int md = 4;
    int kd = 2;
    int nd = 3;

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
    // float matrix_result[md*nd];
    float matrix_result[md*nd];

    // size_t global;                      // global domain size for our calculation
    // size_t local;                       // local domain size for our calculation

    cl_device_id device_id;             // compute device id 
    cl_context context;                 // compute context
    cl_command_queue commands;          // compute command queue
    cl_program program;                 // compute program
    cl_kernel kernel;                   // compute kernel
    
    cl_mem input1;                       // device memory used for the input array
    cl_mem input2;                       // device memory used for the input array
    cl_mem output;                      // device memory used for the output array



    cl_uint numPlatforms = 0;
    // cl_int CL_err = CL_SUCCESS;
    err = clGetPlatformIDs( 0, NULL, &numPlatforms );

    if (err == CL_SUCCESS)
        printf("%u platform(s) found\n", numPlatforms);
    else
        printf("clGetPlatformIDs(%i)\n", err);



    
    // // Fill our data set with random float values
    // //
    // int i = 0;
    // unsigned int count = DATA_SIZE;
    // for(i = 0; i < count; i++)
    //     data[i] = rand() / (float)RAND_MAX;
    
    // Connect to a compute device
    //
    int gpu = 1;
    err = clGetDeviceIDs(NULL, gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to create a device group!\n");
        return EXIT_FAILURE;
    }
  
    // Create a compute context 
    //
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
    if (!context)
    {
        printf("Error: Failed to create a compute context!\n");
        return EXIT_FAILURE;
    }

    // Create a command commands
    //
    commands = clCreateCommandQueue(context, device_id, 0, &err);
    if (!commands)
    {
        printf("Error: Failed to create a command commands!\n");
        return EXIT_FAILURE;
    }

    // Create the compute program from the source buffer
    //
    program = clCreateProgramWithSource(context, 1, (const char **) & KernelSource2, NULL, &err);
    if (!program)
    {
        printf("Error: Failed to create compute program!\n");
        return EXIT_FAILURE;
    }

    // Build the program executable
    //
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        // size_t len;
        // char buffer[2048];

        // printf("Error: Failed to build program executable!\n");
        // clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        // printf("%s\n", buffer);
        // exit(1);
        printf("Error: Failed to build program executable!\n");
        return EXIT_FAILURE;
    }

    printf("Create Build Program Success!!!");

    // Create the compute kernel in the program we wish to run
    //
    kernel = clCreateKernel(program, "matrix_mult", &err);
    // kernel = clCreateKernel(program, "square", &err);
    if (!kernel || err != CL_SUCCESS)
    {
        printf("Error: Failed to create compute kernel!\n");
        exit(1);
    }

    printf("Create Compute Kernel Success!!!");

    // Create the input and output arrays in device memory for our calculation
    //
    input1 = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(float) * (md*kd), NULL, NULL);
    input2 = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(float) * (kd*nd), NULL, NULL);
    output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * (md*nd), NULL, NULL);
    if (!input1 || !output || !input2)
    {
        printf("Error: Failed to allocate device memory!\n");
        exit(1);
    }    

    printf("Create buffer Success!!!");
    
    // Write our data set into the input array in device memory 
    //
    err = clEnqueueWriteBuffer(commands, input1, CL_TRUE, 0, sizeof(float) * (md*kd), matrix_a, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to write to source array1!\n");
        exit(1);
    }
    err = clEnqueueWriteBuffer(commands, input2, CL_TRUE, 0, sizeof(float) * (kd*nd), matrix_b, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to write to source array2!\n");
        exit(1);
    }

    // Set the arguments to our compute kernel
    //
    err = 0;
    err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input1);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &input2);
    // err |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &count);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), output);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to set kernel arguments! %d\n", err);
        exit(1);
    }

    // Get the maximum work group size for executing the kernel on the device
    //
    const size_t local[2] = { 16, 16 };
    err = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), local, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to retrieve kernel work group info! %d\n", err);
        exit(1);
    }

    // Execute the kernel over the entire range of our 1d input data set
    // using the maximum number of work group items for this device
    //
    // global = count;
    // global = md*nd;
    const size_t global[2] = { md, nd };
    err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, global, local, 0, NULL, NULL);
    if (err)
    {
        printf("Error: Failed to execute kernel!\n");
        return EXIT_FAILURE;
    }

    // Wait for the command commands to get serviced before reading back results
    //
    clFinish(commands);

    // Read back the results from the device to verify the output
    //
    err = clEnqueueReadBuffer( commands, output, CL_TRUE, 0, sizeof(float) * (md*nd), matrix_result, 0, NULL, NULL );  
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to read output array! %d\n", err);
        exit(1);
    }
    
    // Validate our results
    //
    // correct = 0;
    // for(i = 0; i < count; i++)
    // {
    //     if(results[i] == data[i] * data[i])
    //         correct++;
    // }
    
    // Print a brief summary detailing the results
    //
    // printf("Computed '%d/%d' correct values!\n", correct, count);

    for (int m=0; m < md; m++) {
        for (int n=0; n < nd; n++) {
            printf("%f\t", matrix_result[m*nd+n]);
        }
        printf("\n");
    }

    // Shutdown and cleanup
    //
    clReleaseMemObject(input1);
    clReleaseMemObject(input2);
    clReleaseMemObject(output);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);

    return 0;
}
