#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

// 结构体用于传递线程参数
struct thread_data 
{
    int thread_id;
    double *matrix_a;
    double *matrix_b;
    double *result;
    int rows;
    int cols;
    int thread_count;
};

// 矩阵乘法线程函数
void *matrix_multiply(void *arg) 
{
    struct thread_data *data = (struct thread_data *)arg;
    int start_row = data->thread_id * (data->rows / data->thread_count);
    int end_row = (data->thread_id + 1) * (data->rows / data->thread_count);
    
    for (int i = start_row; i < end_row; i++) 
    {
        for (int j = 0; j < data->cols; j++) 
        {
            double sum = 0.0;
            for (int k = 0; k < data->cols; k++) 
            {
                sum += data->matrix_a[i * data->cols + k] * data->matrix_b[k * data->cols + j];
            }
            data->result[i * data->cols + j] = sum;
        }
    }
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 7) {
        printf("Usage: mymatrixmultiply -a <matrix_a_file> -b <matrix_b_file> -t <thread_count>\n");
        return 1;
    }

    char *matrix_a_file = NULL;
    char *matrix_b_file = NULL;
    int thread_count = 0;

    // 解析命令行参数
    for (int i = 1; i < argc; i += 2) 
    {
        if (strcmp(argv[i], "-a") == 0) 

            matrix_a_file = argv[i + 1];
        } else if (strcmp(argv[i], "-b") == 0) 
        {
            matrix_b_file = argv[i + 1];
        } else if (strcmp(argv[i], "-t") == 0) 
        {
            thread_count = atoi(argv[i + 1]);
        }
    }

    clock_t start, end;

    start = clock();

    // 打开输入文件并读取矩阵大小
    FILE *file_a = fopen(matrix_a_file, "r");
    FILE *file_b = fopen(matrix_b_file, "r");
    int rows, cols;

    if (file_a == NULL || file_b == NULL) 
    {
        printf("Error opening matrix file.\n");
        return 1;
    }

    fscanf(file_a, "%d %d", &rows, &cols);
    fscanf(file_b, "%d %d", &rows, &cols);

    // 分配内存以存储矩阵数据
    double *matrix_a = malloc(rows * cols * sizeof(double));
    double *matrix_b = malloc(rows * cols * sizeof(double));
    double *result = malloc(rows * cols * sizeof(double));

    // 从文件中读取矩阵数据
    int current_row = 0;
    double value;
    char line[100];

    while (fgets(line, sizeof(line), file_a) != NULL) 
    {
        if (line[0] == '#') 
        {
            continue; // 忽略注释行
        }
        sscanf(line, "%lf", &value);
        matrix_a[current_row] = value;
        current_row++;
    }

    current_row = 0;

    while (fgets(line, sizeof(line), file_b) != NULL) 
    {
        if (line[0] == '#') 
        {
            continue; // 忽略注释行
        }
        sscanf(line, "%lf", &value);
        matrix_b[current_row] = value;
        current_row++;
    }

    // 关闭文件
    fclose(file_a);
    fclose(file_b);

    // 创建线程并进行矩阵乘法计算
    pthread_t *threads = malloc(thread_count * sizeof(pthread_t));
    struct thread_data *thread_data_array = malloc(thread_count * sizeof(struct thread_data));

    for (int i = 0; i < thread_count; i++) 
    {
        thread_data_array[i].thread_id = i;
        thread_data_array[i].matrix_a = matrix_a;
        thread_data_array[i].matrix_b = matrix_b;
        thread_data_array[i].result = result;
        thread_data_array[i].rows = rows;
        thread_data_array[i].cols = cols;
        thread_data_array[i].thread_count = thread_count;

        pthread_create(&threads[i], NULL, matrix_multiply, (void *)&thread_data_array[i]);
    }

    // 等待线程完成
    for (int i = 0; i < thread_count; i++)
    {
        pthread_join(threads[i], NULL);
    }

    end = clock();
    printf("time: %f\n", (double)(end - start) / CLOCKS_PER_SEC);

    FILE *file = fopen("result.txt", "w");
    fprintf(file, "%d %d\n", rows, cols);
    for (int i = 0; i < rows; i++) 
    {
        fprintf(file, "# Line%d", i);
        for (int j = 0; j < cols; j++) 
        {
            fprintf(file, "%.6f\n ", result[i * cols + j]);
        }
    }
    fclose(file);

    // 释放内存
    free(matrix_a);
    free(matrix_b);
    free(result);
    free(threads);
    free(thread_data_array);

    return 0;
}