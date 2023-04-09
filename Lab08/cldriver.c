// #include <fcntl.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <math.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <OpenCL/opencl.h>




// int main() {
//     int md = 4;
//     int kd = 2;
//     int nd = 3;
//     int k, n, m;


//     printf("MATRIX A\n");
//     float matrix_a[md*kd];
//     for (m=0; m < md; m++) {
//         for (k=0; k < kd; k++) {
//             matrix_a[m*kd+k] = m*k+1;
//             printf("%f\t", matrix_a[m*kd+k]);
//         }
//         printf("\n");
//     }

//     printf("MATRIX B\n");
//     float matrix_b[kd*nd];
//     for (k=0; k < kd; k++) {
//         for (n=0; n < nd; n++) {
//             matrix_b[k*nd+n] = k*n+1;
//             printf("%f\t", matrix_b[k*nd+n]);
//         }
//         printf("\n");
//     }


//     printf("MATRIX AB (RESULT)\n");
//     float matrix_result[md*nd];




//     // Define the size of the work-items in the kernel
//     const size_t local_size[2] = {16, 16};

//     // Create variables for the OpenCL platform, device, context, and command queue
//     cl_platform_id platform_id;
//     cl_device_id device_id;
//     cl_context context;
//     cl_command_queue queue;

//     // Create variables for the input and output buffers on the device
//     cl_mem buffer_a;
//     cl_mem buffer_b;
//     cl_mem buffer_result;

//     // Create a variable for the kernel program
//     cl_program program;

//     // Create a variable for the kernel function
//     cl_kernel kernel;

//     // Create a variable for the error code
//     cl_int err;

//     // Load the kernel source code from a file
//     // char* kernel_source = NULL;
//     // size_t kernel_size;
//     // FILE* kernel_file = fopen("matrix_mult_kernel.cl", "r");
//     // if (kernel_file) {
//     //     fseek(kernel_file, 0, SEEK_END);
//     //     kernel_size = ftell(kernel_file);
//     //     rewind(kernel_file);
//     //     kernel_source = (char*)malloc(kernel_size + 1);
//     //     fread(kernel_source, sizeof(char), kernel_size, kernel_file);
//     //     kernel_source[kernel_size] = '\0';
//     //     fclose(kernel_file);
//     // }


//     // Get the ID of the first available platform
//     err = clGetPlatformIDs(1, &platform_id, NULL);
//     if (err != CL_SUCCESS) {
//         printf("Error getting platform IDs\n");
//         return EXIT_FAILURE;
//     }

//     // Get the ID of the first available GPU device
//     err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
//     if (err != CL_SUCCESS) {
//         printf("Error getting device IDs\n");
//         return EXIT_FAILURE;
//     }

//     // Create a context for the device
//     context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);
//     if (err != CL_SUCCESS) {
//         printf("Error creating context\n");
//         return EXIT_FAILURE;
//     }

//     // Create a command queue for the context and device
//     queue = clCreateCommandQueue(context, device_id, 0, &err);
//     if (err != CL_SUCCESS) {
//         printf("Error creating command queue\n");
//         return EXIT_FAILURE;
//     }


//     // Create buffers for the input and output matrices on the device
//     buffer_a = clCreateBuffer(context, CL_MEM_READ_ONLY, md * kd * sizeof(float), NULL, &err);
//     if (err != CL_SUCCESS) {
//         printf("Error creating buffer A\n");
//         return EXIT_FAILURE;
//     }

//     buffer_b = clCreateBuffer(context, CL_MEM_READ_ONLY, kd * nd * sizeof(float), NULL, &err);
//     if (err != CL_SUCCESS) {
//         printf("Error creating buffer B\n");
//         return EXIT_FAILURE;
//     }

//     buffer_result = clCreateBuffer(context, CL_MEM_WRITE_ONLY, md * nd * sizeof(float), NULL, &err);
//     if (err != CL_SUCCESS) {
//         printf("Error creating buffer result\n");
//         return EXIT_FAILURE;
//     }


//     cl_mem matrix_a_buf = clCreateBuffer(context, CL_MEM_READ_ONLY,
//                                     sizeof(float) * md * kd, NULL, &err);
//     cl_mem matrix_b_buf = clCreateBuffer(context, CL_MEM_READ_ONLY,
//                                         sizeof(float) * kd * nd, NULL, &err);
//     cl_mem matrix_result_buf = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
//                                             sizeof(float) * md * nd, NULL, &err);

//     err = clEnqueueWriteBuffer(queue, matrix_a_buf, CL_TRUE, 0,
//                                 sizeof(float) * md * kd, matrix_a, 0, NULL, NULL);
//     err = clEnqueueWriteBuffer(queue, matrix_b_buf, CL_TRUE, 0,
//                                 sizeof(float) * kd * nd, matrix_b, 0, NULL, NULL);



//     for (m=0; m < md; m++) {
//         for (n=0; n < nd; n++) {
//             printf("%f\t", matrix_result[m*nd+n]);
//         }
//         printf("\n");
//     }


// }




#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <OpenCL/opencl.h>

#define MD 4
#define KD 2
#define ND 3

const char* kernel_source = "__kernel void matrix_mult(__global float* a, __global float* b, __global float* result) {\n"
"    int i = get_global_id(0);\n"
"    int j = get_global_id(1);\n"
"    float sum = 0.0f;\n"
"    for (int k = 0; k < KD; k++) {\n"
"        sum += a[i*KD + k] * b[k*ND + j];\n"
"    }\n"
"    result[i*ND + j] = sum;\n"
"}\n";


int main() {
    int k, n, m;

    printf("MATRIX A\n");
    float matrix_a[MD*KD];
    for (m = 0; m < MD; m++) {
        for (k = 0; k < KD; k++) {
            matrix_a[m*KD + k] = m*k + 1;
            printf("%f\t", matrix_a[m*KD + k]);
        }
        printf("\n");
    }

    printf("MATRIX B\n");
    float matrix_b[KD*ND];
    for (k = 0; k < KD; k++) {
        for (n = 0; n < ND; n++) {
            matrix_b[k*ND + n] = k*n + 1;
            printf("%f\t", matrix_b[k*ND + n]);
        }
        printf("\n");
    }

    printf("MATRIX AB (RESULT)\n");
    float matrix_result[MD*ND];

    // Define the size of the work-items in the kernel
    const size_t local_size[2] = { 16, 16 };

    // Create variables for the OpenCL platform, device, context, and command queue
    cl_platform_id platform_id;
    cl_device_id device_id;
    cl_context context;
    cl_command_queue queue;

    // Create variables for the input and output buffers on the device
    cl_mem buffer_a;
    cl_mem buffer_b;
    cl_mem buffer_result;

    // Create a variable for the kernel program
    cl_program program;

    // Create a variable for the kernel function
    cl_kernel kernel;

    // Create a variable for the error code
    cl_int err;

    // Get the ID of the first available platform
    err = clGetPlatformIDs(1, &platform_id, NULL);
    if (err != CL_SUCCESS) {
        printf("Error getting platform IDs\n");
        return EXIT_FAILURE;
    }

    // Get the ID of the first available GPU device
    err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
    if (err != CL_SUCCESS) {
        printf("Error getting device IDs\n");
        return EXIT_FAILURE;
    }

    // Create a context for the device
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);
    if (err != CL_SUCCESS) {
        printf("Error creating context\n");
        return EXIT_FAILURE;
    }

    // Create a command queue for the context and device
    queue = clCreateCommandQueue(context, device_id, 0, &err);
    if (err != CL_SUCCESS) {
        printf("Error creating command queue\n");
        return EXIT_FAILURE;
    }
    // Create the input and output buffers on the device
    buffer_a = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float)*MD*KD, NULL, &err);
    if (err != CL_SUCCESS) {
        printf("Error creating buffer A\n");
        return EXIT_FAILURE;
    }
    buffer_b = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float)*KD*ND, NULL, &err);
    if (err != CL_SUCCESS) {
        printf("Error creating buffer B\n");
        return EXIT_FAILURE;
    }
    buffer_result = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float)*MD*ND, NULL, &err);
    if (err != CL_SUCCESS) {
        printf("Error creating buffer result\n");
        return EXIT_FAILURE;
    }

    // Write the input matrices to the device buffers
    err = clEnqueueWriteBuffer(queue, buffer_a, CL_TRUE, 0, sizeof(float)*MD*KD, matrix_a, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Error writing buffer A\n");
        return EXIT_FAILURE;
    }
    err = clEnqueueWriteBuffer(queue, buffer_b, CL_TRUE, 0, sizeof(float)*KD*ND, matrix_b, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Error writing buffer B\n");
        return EXIT_FAILURE;
    }

    // Create a program object from the kernel source code
    program = clCreateProgramWithSource(context, 1, (const char **) & kernel_source, NULL, &err);
    if (err != CL_SUCCESS) {
        printf("Error creating program\n");
        return EXIT_FAILURE;
    }

    // Build the program for the specified device
    // err = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Error building program\n");
        return EXIT_FAILURE;
    }

    // Create the kernel function from the program
    kernel = clCreateKernel(program, "matrix_mult", &err);
    if (err != CL_SUCCESS) {
        printf("Error creating kernel\n");
        return EXIT_FAILURE;
    }

    // Set the arguments of the kernel function
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer_a);
    if (err != CL_SUCCESS) {
        printf("Error setting kernel argument 0\n");
        return EXIT_FAILURE;
    }
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &buffer_b);
    if (err != CL_SUCCESS) {
        printf("Error setting kernel argument 1\n");
        return EXIT_FAILURE;
    }
    err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &buffer_result);
    if (err != CL_SUCCESS) {
        printf("Error setting kernel argument 2\n");
        return EXIT_FAILURE;
    }

    // Define the global work size as the size of the output matrix
    const size_t global_size[2] = { MD, ND };

    // Enqueue the kernel function for execution on the device
    err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_size, local_size, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Error enqueuing kernel\n");
        return EXIT_FAILURE;
    }


    // Wait for the kernel to finish executing
    clFinish(queue);

    // Read the results from the output buffer
    err = clEnqueueReadBuffer(queue, buffer_result, CL_TRUE, 0, sizeof(float)*MD*ND, matrix_result, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Error reading result buffer\n");
        return EXIT_FAILURE;
    }

    // Print the result matrix
    for (m = 0; m < MD; m++) {
        for (n = 0; n < ND; n++) {
            printf("%f\t", matrix_result[m*ND + n]);
        }
        printf("\n");
    }

    // Release OpenCL resources
    clReleaseMemObject(buffer_a);
    clReleaseMemObject(buffer_b);
    clReleaseMemObject(buffer_result);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return EXIT_SUCCESS;
}