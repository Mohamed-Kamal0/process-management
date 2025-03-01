#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
int main(int argc, char **argv)
{

    char *filename = argv[1];
    int N = atoi(argv[2]);
    int P = atoi(argv[3]);

    FILE *file = fopen(filename, "r");

    int S;
    fscanf(file, "%d", &S);

    int grades[S][2];
    for (int i = 0; i < S; i++)
    {
        fscanf(file, "%d %d", &grades[i][0], &grades[i][1]);
    }

    int spTA = S / N;
    int remain = S % N;

    fclose(file);
    int fd[N][2];
    pid_t pid[N];

    for (int i = 0; i < N; i++)
    {
        pipe(fd[i]);
        pid[i] = fork();

        if (pid[i] == 0)
        {
            close(fd[i][0]);
            int start_index = i * spTA;
            int end_index = start_index + spTA;

            if (i == N - 1)
                end_index += remain;

            int pass_count = 0;
            for (int j = start_index; j < end_index; j++)
            {
                int total = grades[j][0] + grades[j][1];
                if (total >= P)
                    pass_count++;
            }
            write(fd[i][1], &pass_count, sizeof(pass_count));
            close(fd[i][1]);
            exit(0);
        }
    }
    int results[N];
    for (int i = 0; i < N; i++)
    {
        close(fd[i][1]);
        read(fd[i][0], &results[i], sizeof(results[i]));
        close(fd[i][0]);
    }
    for (int i = 0; i < N; i++)
    {
        waitpid(pid[i], NULL, 0);
    }
    for (int i = 0; i < N; i++)
    {
        printf("%d", results[i]);
        if (i < N - 1)
            printf(" ");
    }
    printf("\n");
}