//Author Rishab DAS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#define SIZE 128

typedef struct {
    size_t size;
    size_t used;
    char *data;
} bufferstr;

int AppendBs(bufferstr *bs, char letter)
{
    if (bs->used == bs->size) {
    size_t size = bs->size * 2;
    char *p = realloc(bs->data, sizeof(char) * size);
    if (!p) return 1;

    bs->data = p;
    bs->size = size;
    }

    bs->data[bs->used-1] = letter;
    bs->data[bs->used] = '\0';
    ++bs->used;

    return 0;
}
void BsDestroy(bufferstr *bs)
{
    free(bs->data);
}
int Initialis(bufferstr *bs, size_t size)
{
    bs->data = malloc(sizeof(char) * size);
    if (!bs->data) return 1;

    bs->size = size;
    bs->used = 1;
    bs->data[0] = '\0';
    return 0;
}

int wrap(int width, int inputReader, int WriterOutput){
    char *ReadBuff;
    
    ReadBuff = (char*) calloc(SIZE, sizeof(char)); 

    bufferstr WordsBs;
    WordsBs.used = 1;
    char prev[2] = " ";
	int Word1 = 1;
    int LenCurrent = -1; 
    int newWord = 1; 
    
    int error = 0;

    
    while(read(inputReader,ReadBuff,SIZE) != 0){
        for(int i = 0; i < SIZE; i++){ 
            char current = ReadBuff[i];

            if(i > 0 && (ReadBuff[i-1] == 0 && ReadBuff[i] == 0)){
                if(LenCurrent > 0)
                    write(WriterOutput,"\n",1);
                break;
            } 

            if(!isspace(current) && current != '\0'){
                if(newWord) 
                    Initialis(&WordsBs,32);

                AppendBs(&WordsBs, current);
                newWord = 0;
            }
            else{
                    if(WordsBs.used - 1 > width){
                        if(LenCurrent > 0)
                            write(WriterOutput,"\n",1);
                        write(WriterOutput,WordsBs.data,WordsBs.used-1);
                        BsDestroy(&WordsBs); 
                        newWord = 1;
                        LenCurrent += WordsBs.used-1; 
                        WordsBs.used = 1; 
                        error = 1;
                    }
                    else{
                        LenCurrent += WordsBs.used; 
                        if(LenCurrent > width && WordsBs.used > 1){ 
                                write(WriterOutput,"\n",1); 
                                write(WriterOutput,WordsBs.data,WordsBs.used-1);
                                BsDestroy(&WordsBs);
                                newWord = 1;
                                LenCurrent = WordsBs.used-1;
                                WordsBs.used = 1;
                                Word1 = 0;
                            
                        }
                        else if((prev[0] == '\n' && current == '\n') && LenCurrent > 0){ 
                            write(WriterOutput,"\n",1); 
                            write(WriterOutput,"\n",1); 
                            Word1 = 1;
                            LenCurrent = -1; 
                        }
                        else if(WordsBs.used > 1){
                            if(!Word1){
                                write(WriterOutput," ",1);
                            }
                            write(WriterOutput,WordsBs.data,WordsBs.used-1);
                            BsDestroy(&WordsBs);
                            WordsBs.used = 1;
                            newWord = 1;
                            Word1 = 0;
                        }
                        else{
                            LenCurrent--;
                        }
                    }

            }
            prev[0] = current;
        }
        free(ReadBuff);
        ReadBuff = (char*) calloc(SIZE, sizeof(char));
    }
    free(ReadBuff);
    if(error)
        return EXIT_FAILURE; 
    else
        return EXIT_SUCCESS; 
}

int concatenate(bufferstr* bslist, char* str){
    int index = 0;
    while(str[index] != '\0'){
        if(AppendBs(bslist, str[index])){
            return 1;
        }
        index++;
    }
    return 0;
}

int isdir(char *name) {
    struct stat data;
    int err = stat(name, &data);
    if (err == -1) {
        perror(name);  
        return -1;
    }
    if (S_ISDIR(data.st_mode)) {
        return 1;
    } 
    else if (S_ISREG(data.st_mode)) {
        return 0;
    } 
    return -1;
}

int main(int argc, char* argv[]) {

   if(argc < 2) return EXIT_FAILURE; 
    int width = atoi(argv[1]);
    if(width < 1) return EXIT_FAILURE; 
    int error = 0;
    if(argc == 2) {
         if(wrap(width, 0, 1))  
            return EXIT_FAILURE;
         return EXIT_SUCCESS;
    }    
     
    int check = isdir(argv[2]);
    if(check == 0) {
        int fd = open(argv[2], O_RDONLY);
        if(wrap(width, fd, 1)){ 
            close(fd);
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
        close(fd);
    } 
    else if(check == 1) { 
        DIR *parentDir = opendir(argv[2]);  
        struct dirent *folder;
        while ((folder = readdir(parentDir))) {
            bufferstr source;
            Initialis(&source, 32);
            concatenate(&source,argv[2]);
            AppendBs(&source,'/');
            concatenate(&source,folder->d_name);
            int check2 = isdir(source.data);
            if(!check2){

                if(folder->d_name[0] == 'w' && folder->d_name[1] == 'r' && folder->d_name[2] == 'a' && folder->d_name[3] == 'p' && folder->d_name[4] == '.') {
                    BsDestroy(&source);
                    continue;
                }
                
                if(folder->d_name[0] == '.') {
                    BsDestroy(&source);
                    continue;
                }
                int fd2 = open(source.data, O_RDONLY);
               
                bufferstr out_source;
                Initialis(&out_source, 32);
                concatenate(&out_source,argv[2]);
                AppendBs(&out_source,'/');
                concatenate(&out_source,"wrap.");
                concatenate(&out_source, folder->d_name);
                
                int fd3 = open(out_source.data, O_WRONLY|O_CREAT|O_TRUNC, 0600);
                if(wrap(width, fd2, fd3))
                    error = 1;
                close(fd3);
                close(fd2);
                BsDestroy(&out_source);
                BsDestroy(&source);
            }
            else if(check2 == 1){ 
                BsDestroy(&source);
                continue;
            }
            else{
                error = 1;
                BsDestroy(&source);
                continue;
            }
            
        }
        closedir(parentDir); 
    }
    else{
        return EXIT_FAILURE;
    }
    if(error)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}