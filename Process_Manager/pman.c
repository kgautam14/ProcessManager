#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

int flag =1;
int count = 0;
int MAX = 100, MAX_INPUT_SIZE = 100;

typedef struct bgTask {
    
    int stat;   //1: Running & 0: Stopped
    pid_t pid;
    char task[64];
    struct bgTask *next;

}bgTask;
bgTask *listhead = NULL;


int processRunning(pid_t pid){

    bgTask* track = listhead;
    while(track != NULL){
        if(track-> pid == pid){
            return 1;
        }
        track = track->next;
    }
    return 0;
}

void removeNode(pid_t pid){
    if(processRunning(pid) == 0){
        return;
    }

    // Removing node from list
    bgTask* track1 = listhead;
    bgTask* track2 = NULL;

    while(track1!=NULL){
        if(track1->pid == pid){
            if(track1 == listhead){
            listhead = listhead->next;
        }
        else{
            track2->next = track1->next;
        }
        free(track1);
        return;
    }
    track2 = track1;
    track1 = track1->next;
    }
}

void add_bg_list(char* comms, pid_t pid){

    bgTask* newTask = (bgTask*)malloc(sizeof(bgTask));
    newTask->pid = pid;
    newTask->stat = 1;
    strcpy(newTask->task, comms);
    newTask->next = NULL;

    if(listhead == NULL){
        listhead = newTask;
    }
    else{
        bgTask* track = listhead;
        while(track->next != NULL){
            track = track->next;
        }
        track->next = newTask;
    }
}

bgTask* getNode(pid_t pid){

    bgTask* track = listhead;
    while(track!=NULL){
        if(listhead->pid == pid){
            return track;
        }
    }
    return NULL;
}

void show_list(){
    
    int counter = 0; 
   	char cwd[100];
    bgTask* tracker = listhead;

    while (tracker != NULL && getcwd(cwd, sizeof(cwd)) != NULL)
        {
            count++;
            if(tracker->stat == 1){
                printf("%d:\t %s/%s\n", tracker->pid, cwd, tracker->task);
            }
            else{
                printf("%d:\t %s/%s\t <stopped> \n", tracker->pid, cwd, tracker->task);
            }
            tracker=tracker->next;
            counter++;
        }
    printf("Total background jobs: %d\n", counter);

}

void check_zomb(){

    int status;
    pid_t retVal;
    int ops = WUNTRACED | WCONTINUED | WNOHANG;

    while(1){
        if(listhead == NULL){
            return;
        }
        else
        {
            retVal = waitpid(-1, &status, ops);
            if(retVal > 0 )
            {
                if(WIFSTOPPED(status)){
                    printf("Process: %d has been stopped.\n", retVal);
                    bgTask* curr = getNode(retVal);
                    curr->stat = 0;
                }
                if (WIFCONTINUED(status)){
                    printf("Process: %d has been restarted.\n", retVal);
                    bgTask* curr = getNode(retVal);
                    curr->stat = 1;
                }
                if(WIFSIGNALED(status)){
                    printf("Process: %d has been terminated.\n", retVal);
                    removeNode(retVal);
                }
            }
            else
            {
                break;
            }
        }
    }
}

void bg_entry(char **argv){
    
    
        pid_t pid;
        pid = fork();
        
        if(pid ==0)
        {
            if( execvp(argv[1], argv)< 0)
            {
                perror("Error on execvp!"); 
                exit(EXIT_FAILURE);
            }
            sleep(1);
            exit(EXIT_SUCCESS);
        }
        else if(pid > 0)
        {
            count++;
            add_bg_list(argv[1], pid);
            sleep(1);
        }
        else
        {
            perror("failed to fork\n");
            exit(EXIT_SUCCESS);
        }

}

void run()
{
    check_zomb();

// ***** Tokenizing
    char cmd[MAX];
    pid_t pid;
    int status;
    printf("PMAN: > ");
    gets(cmd);
    if(strcmp(cmd, "") == 0){
        printf("Please provide a Valid Input\n");
        exit(EXIT_FAILURE);
        }
    char *sendtok[MAX];
    char *tokens[MAX];

    for(int i =0; i<MAX; i++){
    tokens[i] = (char*)malloc(100*sizeof(char*));
    sendtok[i] = (char*)malloc(100*sizeof(char*));
    }
    int i =0, x = 0;

    tokens[0] = strtok(cmd, " ");
    while(tokens[i] != NULL){
        tokens[++i] = strtok(NULL, " ");
        sendtok[++x] = tokens[i];
    }
    
// ******Tokenizing ends, Case handling begins
    
    if(strcmp(tokens[0], "") == 0) {
        printf("Invalid command, try again or enter exit to quit!\n");
    }
    else if(strcmp(tokens[0], "bg")==0){
        bg_entry(sendtok);
    }
    else if(strcmp(tokens[0], "bglist")==0){
        show_list();
    }
    else if(strcmp(tokens[0], "bgkill") == 0){

        pid = atoi(tokens[1]);
        if(processRunning(pid) == 1){
            if(!kill(pid, SIGTERM)){
                sleep(1);
            }
            else{
                printf("Failed to execute commandn\n");
            }
        }
        else if(processRunning(pid) == 0){
            printf("Error: Process %d does not exist\n", pid);
        }

    }
    else if(strcmp(tokens[0], "bgstop") == 0){
        pid = atoi(tokens[1]);
        if(processRunning(pid) == 1){
            if(!kill(pid, SIGSTOP)){
                sleep(1);
            }
            else{
                printf("Failed to execute commandn\n");
            }
        }
        else if(processRunning(pid) == 0){
            printf("Error: Process %d does not exist\n", pid);
        }
    }
    else if(strcmp(tokens[0], "bgstart") == 0){
        
         pid = atoi(tokens[1]);
        if(processRunning(pid) == 1){
            if(!kill(pid, SIGCONT)){
                sleep(1);
            }
            else{
                printf("Failed to execute commandn\n");
            }
        }
        else if(processRunning(pid) == 0){
            printf("Error: Process %d does not exist\n", pid);
            }
    }
    else if(strcmp(tokens[0], "exit") == 0){
        flag = 1;
        exit(EXIT_SUCCESS);
    }
    else if(strcmp(tokens[0], "pstat") == 0){
        printf("PSTAT function has not been implemented, Sorry!");
    }
    else{
        printf("%s:\t command not found\n", cmd);
    }
}


int main(){
    while(flag){
        run();
    }
    return 0;
}
