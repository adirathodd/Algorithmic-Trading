#include "dataframe.h"
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <pthread.h>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

typedef struct{
    int id;
    vector<string> filepaths;
} thread_args;

int nthreads;
size_t nfiles;
DataFrame *df_array;

void *read_df(void *arg){
    thread_args args = *(thread_args *)arg;
    int id = args.id;

    int chunk = nfiles / nthreads;
    int start = id * chunk, end = (id + 1) * chunk;

    if(nthreads == args.id + 1) end = nfiles;

    for (int i = start; i < end; i++) {
        DataFrame df = DataFrame();
        df.readCSV(args.filepaths[i]);
        df_array[i] = df;
    }

    return NULL;
}

int main(int argc, char *argv[]){
    if(argc != 3){
        printf("Usage - %s <csv folder path> <no. of threads>\n", argv[0]);
        return -1;
    }

    string folderpath = argv[1];
    if (folderpath.back() != '/') {
        folderpath += '/';
    }

    nthreads = atoi(argv[2]);

    // get all the filepaths from the given folder
    vector<string> filepaths;
    DIR *dirp = opendir(argv[1]);
    struct dirent* dp;
    while ((dp = readdir(dirp)) != NULL) {
        string filename = dp->d_name;
        fs::path filePath = filename;
        if (filename == "." || filename == ".." || filePath.extension() != ".txt") {
            continue;
        }
        filepaths.push_back(folderpath + filename);
    }
    closedir(dirp);

    nfiles = filepaths.size();
    df_array = (DataFrame *)malloc(sizeof(DataFrame) * nfiles);

    pthread_t threads[nthreads];
    thread_args args1[nthreads];

    for(int i = 0; i < nthreads; i++){
        args1[i].id = i;
        args1[i].filepaths = filepaths;
    }

    for(int i = 0; i < nthreads; i++) pthread_create(&threads[i], NULL, read_df, &args1[i]);
    for(int i = 0; i < nthreads; i++) pthread_join(threads[i], NULL);

    free(df_array);

    return 0;

}