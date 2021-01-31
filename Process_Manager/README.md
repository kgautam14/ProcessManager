
******************Process Manager*****************

University of Victoria
CSC 360 Assignment 1
Kushank Gautam
V00843403

*************************************************
How to run pman.c

1. In order to compile the program simply run the command 'make' from the terminal, this will compile pman.c.
2. Execute './pman' to run the executable file and you should be provided with the Process Manager.

*************************************************

How to operate pman.c

The process manager can execute 5 commands:

1. bg <cmd>: cmd should be an executable file starting with './' in order to be executed in background.
2. bglist: will display all the current processes been run by pman, displays stopped processes with a tag <stopped>.
3. bgkill <pid>: will send a TERM signal to the process with id = pid.
4. bgstpop <pid>: will send a STOP signal to the process with id = pid.
5. bgstart <pid>: will send a CONT signal to a previously STOPPED process with id = pid.

***************************************************

Things to Know:
1. pstat <pid> has not been implemented, running on MAC OS it kept showing me errors for the process file and I was unable to figure it out.
2. If you run bg <cmd> and then immediately run bgstop <pid>, the program stalls for some reason that I haven't been able to identify yet. If you run bgkill <pid> and then run bgstop <pid> for different processes ofcourse, that will work fine.
3. If you would like to use inf.c as a backgroun process, do compile it first using "gcc inf.c -o inf".
5. Also bgstop <pid> hangs the pman if using with bg processes that print! To check if my processes were running, terminated or stopped, I was checking by running 'ps' on my terminal. 
4. On compilation, linux proivdes a Warning since I am using gets(), hoping thats all right.

****************************************************



