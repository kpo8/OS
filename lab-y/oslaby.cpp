// ACADEMIC INTEGRITY PLEDGE
//
// - I have not used source code obtained from another student nor
//   any other unauthorized source, either modified or unmodified.
//
// - All source code and documentation used in my program is either
//   my original work or was derived by me from the source code
//   published in the textbook for this course or presented in
//   class.
//
// - I have not discussed coding details about this project with
//   anyone other than my instructor. I understand that I may discuss
//   the concepts of this program with other students and that another
//   student may help me debug my program so long as neither of us
//   writes anything during the discussion or modifies any computer
//   file during the discussion.
//
// - I have violated neither the spirit nor letter of these restrictions.
//
//
//
// Signed: Garrett McDonnell, Aleksandar Drobnjak, Kevin O'Neil	Date: 4/23/2019
// 3460:426 Lab Y - POSIX Threads and Semaphores
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

using std::cout;

void *agentTobaccoPaper(void *ptr);
void *agentTobaccoMatches(void *ptr);
void *agentPaperMatches(void *ptr);
void *pusherTobacco(void *ptr);
void *pusherPaper(void *ptr);
void *pusherMatches(void *ptr);
void *smokerTobacco(void *ptr);
void *smokerPaper(void *ptr);
void *smokerMatches(void *ptr);

// Global declaration of semaphores and bools
// used by all threads
bool isTobacco = false, isPaper = false, isMatch = false;
sem_t agentSem, tobacco, paper, match;
sem_t tobaccoSem, paperSem, matchSem, mutex;

int main()
{
    srand (time(NULL));

    // creates all the necessary semaphores and threads
    // 3 agent, 3 pushers, 2 smokers w/tobacco,
    // 2 smokers w/ tobacco, and 2 smokers w/ matches threads
    pthread_t agentTP, agentTM, agentPM,
              pusherT, pusherP, pusherM,
              smokerT1, smokerT2, smokerP1,
              smokerP2, smokerM1, smokerM2;

    // initializes all semaphores
    sem_init(&agentSem, 0, 1);
    sem_init(&tobacco, 0, 0);
    sem_init(&paper, 0, 0);
    sem_init(&match, 0, 0);

    sem_init(&tobaccoSem, 0, 0);
    sem_init(&paperSem, 0, 0);
    sem_init(&matchSem, 0, 0);
    sem_init(&mutex, 0, 1);

    // starts all the threads
    // 3 agents (for each combination of two items)
    // 3 pushers (to push each of the produced items by the agents)
    // 6 smokers (2 of each who have one of the items, but need 2 other items)
    pthread_create(&smokerT1, NULL, smokerTobacco, NULL);
    pthread_create(&smokerT2, NULL, smokerTobacco, NULL);
    pthread_create(&smokerP1, NULL, smokerPaper, NULL);
    pthread_create(&smokerP2, NULL, smokerPaper, NULL);
    pthread_create(&smokerM1, NULL, smokerMatches, NULL);
    pthread_create(&smokerM2, NULL, smokerMatches, NULL);

    pthread_create(&agentTP, NULL, agentTobaccoPaper, NULL);
    pthread_create(&agentTM, NULL, agentTobaccoMatches, NULL);
    pthread_create(&agentPM, NULL, agentPaperMatches, NULL);

    pthread_create(&pusherT, NULL, pusherTobacco, NULL);
    pthread_create(&pusherP, NULL, pusherPaper, NULL);
    pthread_create(&pusherM, NULL, pusherMatches, NULL);

    // waits for all threads to finish execution
    pthread_join(smokerT1, nullptr);
    pthread_join(smokerT2, nullptr);
    pthread_join(smokerP1, nullptr);
    pthread_join(smokerP2, nullptr);
    pthread_join(smokerM1, nullptr);
    pthread_join(smokerM2, nullptr);
    
    pthread_join(agentTP, nullptr);
    pthread_join(agentTM, nullptr);
    pthread_join(agentPM, nullptr);

    pthread_join(pusherT, nullptr);
    pthread_join(pusherP, nullptr);
    pthread_join(pusherM, nullptr);

    cout << "All 6 smokers finished smoking 3 cigarettes each, now they can go out for lunch!\n";
    return 0;
}

// function used to simulate when an agent produces tobacco and paper
void *agentTobaccoPaper(void *ptr){
    int sleepFor = rand() % 200 + 1;

    for(int i = 0; i < 6; ++i){
        usleep(sleepFor * 1000);
        while(sem_wait(&agentSem) != 0) {}

        sem_post(&tobacco);
        sem_post(&paper);
    }
    pthread_exit(0);
}

// function used to simulate when an agent produces tobacco and matches
void *agentTobaccoMatches(void *ptr){
    int sleepFor = rand() % 200 + 1;

    for(int i = 0; i < 6; ++i){
        usleep(sleepFor * 1000);
        while(sem_wait(&agentSem) != 0) {}

        sem_post(&tobacco);
        sem_post(&match);
    }
    pthread_exit(0);
}

// function used to simulate when an agent produces paper and matches
void *agentPaperMatches(void *ptr){
    int sleepFor = rand() % 200 + 1;

    for(int i = 0; i < 6; ++i){
        usleep(sleepFor * 1000);
        while(sem_wait(&agentSem) != 0) {}

        sem_post(&paper);
        sem_post(&match);
    }
    pthread_exit(0);
}

// function used to simulate when tobacco is ready
void *pusherTobacco(void *ptr){
    for(int i = 0; i < 12; ++i){
        while(sem_wait(&tobacco) != 0) {}
        while(sem_wait(&mutex) != 0) {}

        if(isPaper){
            isPaper = false;
            sem_post(&matchSem);
        }
        else if(isMatch){
            isMatch = false;
            sem_post(&paperSem);
        }
        else{
            isTobacco = true;
        }

        sem_post(&mutex);
    }
    pthread_exit(0);
}

// function used to simulate when paper is ready
void *pusherPaper(void *ptr){
    for(int i = 0; i < 12; ++i){
        while(sem_wait(&paper) != 0) {}
        while(sem_wait(&mutex) != 0) {}

        if(isTobacco){
            isTobacco = false;
            sem_post(&matchSem);
        }
        else if(isMatch){
            isMatch = false;
            sem_post(&tobaccoSem);
        }
        else{
            isPaper = true;
        }

        sem_post(&mutex);
    }
    pthread_exit(0);
}

// function used to simulate when matches are ready
void *pusherMatches(void *ptr){
    for(int i = 0; i < 12; ++i){
        while(sem_wait(&match) != 0) {}
        while(sem_wait(&mutex) != 0) {}

        if(isTobacco){
            isTobacco = false;
            sem_post(&paperSem);
        }
        else if(isPaper){
            isPaper = false;
            sem_post(&tobaccoSem);
        }
        else{
            isMatch = true;
        }

        sem_post(&mutex);
    }
    pthread_exit(0);
}

// Smoker who has tobacco, needs paper and matches
void *smokerTobacco(void *ptr){
    int sleepFor = rand() % 50 + 1;

    for(int i = 0; i < 3; ++i){
        while(sem_wait(&tobaccoSem) != 0) {}
        usleep(sleepFor * 1000);
        cout << "Smoker who had tobacco finished smoking # " << i + 1 << "\n";
        usleep(sleepFor * 1000);
        sem_post(&agentSem);
    }
    pthread_exit(0);
}

// Smoker who has paper, needs tobacco and matches
void *smokerPaper(void *ptr){
    int sleepFor = rand() % 50 + 1;

    for(int i = 0; i < 3; ++i){
        while(sem_wait(&paperSem) != 0) {}
        usleep(sleepFor * 1000);
        cout << "Smoker who had paper finished smoking # " << i + 1 << "\n";
        usleep(sleepFor * 1000);
        sem_post(&agentSem);
    }
    pthread_exit(0);
}

// Smoker who has matches, needs paper and tobacco
void *smokerMatches(void *ptr){
    int sleepFor = rand() % 50 + 1;

    for(int i = 0; i < 3; ++i){
        while(sem_wait(&matchSem) != 0) {}
        usleep(sleepFor * 1000);
        cout << "Smoker who had matches finished smoking # " << i + 1<< "\n";
        usleep(sleepFor * 1000);
        sem_post(&agentSem);
    }
    pthread_exit(0);
}
