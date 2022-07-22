#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <array>
#include <iostream>

using namespace std;

sem_t sSmokers[3]; 
sem_t sAgents; 
int global=-1;

int sWait[3]; 

bool doneSmoked=false; 
sem_t readyAgent; 
sem_t readySmoker; 

int s=1;

string drawAgent[]={"paper & matches","paper & tobbacco ","tobacco and matchess"};

void *smoke(void *pVoid)
{
    int *sNumber= (int*)(pVoid);

    
    for (int i = 0; i <7; ++i)
    {
        usleep( rand() % 30000 );
        cout<<"Smoker "<<*sNumber<<" waiting for "<<drawAgent[*sNumber-1]<<" "<<endl<<endl;
        s++;
        if ( s>3 )
        sem_post(&readySmoker);

        sem_wait(&sSmokers[(*sNumber-1)]);
        if ( global == 0 )
        {

        usleep( rand() % 30000 );
        cout<<"Smoker 1 is making a cigarette"<<endl;
	    sleep(1);
	    cout<<"Smoker 1 is now smoking"<<endl;
	
            sWait[0]--;
            sem_post(&sAgents);
        

        }

        else  if ( global == 1 )
        {

        usleep(rand()%30000);
        cout<<"Smoker 2 is making a cigarette"<<endl;
		sleep(1);
	    cout<<"Smoker 2 is now smoking"<<endl;
	    sWait[1]--;
            sem_post(&sAgents);
        }
		
        else if ( global == 2 )
        {

        usleep(rand()%30000);
        cout<<"Smoker 3 is making a cigarette"<<endl;
		sleep(1);
	    cout<<"Smoker 3 is now smoking"<<endl;
	    sWait[2]--;
            sem_post(&sAgents);
        }
		
        
        if (i==6){
            cout<<endl;
        }
    }
		
    pthread_exit(NULL);
}

void *agent(void *pVoid)
{

    srand(time(NULL));
    int *sNumber= (int*)(pVoid);
    sem_wait(&readyAgent);
    for (int i = 0; i < 21; ++i)
    {
	sem_wait(&sAgents);

        while(true)
        {

            int randInt=(rand()%3);
            usleep(rand()%4000);
	    if( sWait[0]==0 && sWait[1]==0 && sWait[2]==0 )
            {
                doneSmoked = true;
                break;
            }
            else if(sWait[randInt]!=0)
            {	sleep(1);
		cout<<"Agent throw "<<drawAgent[randInt]<<" on the table"<<endl;
                global=randInt;
                sem_post(&sSmokers[randInt]);
                break;

            }	
	}
    }

}
void * function(void *pVoid)
{

    sem_wait( &readySmoker );

    sem_post( &readyAgent );

}

int main(void)
{
    for (int i = 0; i < 3; ++i)
    {
        sWait[i]=7;
    }
	
	
    sem_init(&sSmokers[0],0,0);
    sem_init(&sSmokers[1],0,0);
    sem_init(&sSmokers[2],0,0);
	
	
    sem_init(&sAgents,0,1);
	
    
    sem_init(&readyAgent,0,0);
    sem_init(&readySmoker,0,0);
   
	
	pthread_t smoker1;
	pthread_t smoker2;
	pthread_t smoker3;
	pthread_t agent1;
	pthread_t pusher;
	
    int * athread = new int[1];
    athread[0]=1;

    int * athread1 = new int[1];
    athread1[0]=2;

    int * athread2 = new int[1];
    athread2[0]=3;

    pthread_create(&smoker1, NULL, smoke,athread );
	
    pthread_create(&smoker2,NULL,smoke,athread1);
    	
    pthread_create(&smoker3,NULL,smoke,athread2);

    pthread_create(&agent1,NULL,agent,athread);

    pthread_create(&pusher,NULL,function,NULL);

    pthread_join(smoker1,NULL);
    pthread_join(smoker2,NULL);
    pthread_join(smoker3,NULL);
	
    return 0;
}
