//
//  main.cpp
//  CSCI 3120 Assignment 3
//
//  Created by Burak Can ÖZTER on 2018-07-20.
//  Copyright © 2018 Burak Can ÖZTER. All rights reserved.
//


#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <thread>
#include <mutex>
#include <semaphore.h>
#include <dispatch/dispatch.h>
#include <time.h>
#include <iomanip>


#define seats 3         //Number of seats available in the waiting room


int num_of_patients_waiting=0;    //Int variable to keep track of patients waiting
int waiting_room_seats[seats];    //Waiting room has 3 seats
int next_seat =0;                //Seats for patients to sit
int next_patient=0;             //Patients for doctor to treat next.





dispatch_semaphore_t sem_patients;
dispatch_semaphore_t sem_doctor;



pthread_mutex_t mutex_lock; //Lock, unlock for critical sections



void *patient(void * patient_ID){
    
    /*
     This function defines patients' behaviors.
     Patient_ID is to keep track of which patient is currently waiting.
     num_of_patients_waiting is int variable that updates everytime a patient enters the waiting room.
     If there are seats available meaning there is a seat available in the
     waiting room.
     Patient can enter and wait for the doctor.
     Else,
     Patient can either go drink a coffee and try visiting the doctor later.
     
     */
    int id_patient = *(int*)patient_ID;
    
    while(1){
        pthread_mutex_lock(&mutex_lock);
        
        while(num_of_patients_waiting < seats){
            waiting_room_seats[next_seat] = id_patient;
            num_of_patients_waiting++;
            
            std::cout << "Patient " << id_patient << " waiting." << " ";
            std::cout << "Seats occupied = " << num_of_patients_waiting << ".\n" <<std::endl;
            next_seat = (next_seat +1) % seats;
            pthread_mutex_unlock(&mutex_lock);
            
            dispatch_semaphore_signal(sem_patients);
            int random_time = rand() %3 +1; //Get a random time between 1 and 3
            sleep(random_time);
            dispatch_semaphore_wait(sem_doctor,DISPATCH_TIME_FOREVER);
        }
        
        if(num_of_patients_waiting >= seats){
            /*
             No seats available for the patient to wait inside the room.
             Do something else.
             */
            pthread_mutex_unlock(&mutex_lock);
            int random_time= rand() % 3 + 1; //Get a random time between 1 and 3
            std::cout << "Patient " << id_patient << " will try later." << "\n" << std::endl;
            sleep(random_time);
            random_time= rand() % 3 + 1; //Get a random time between 1 and 3
            std::cout << "Patient " << id_patient << " drinking coffee for " << random_time << " second(s).\n" << std::endl;
            sleep(random_time);
        }
        pthread_exit(NULL);
    }
    
}


void *doctor(void *){
    
    /*
     This function defines doctors behaviors.
     Doctor wait for a patient to treat, otherwise he/she will take a nap.
     
     If, doctor is treating a patients, num_of_patients_waiting is being decreased so
     New patients can come in and wait to be treated.
     
     */
    
    while(1){
        
        if(num_of_patients_waiting > 0){
            dispatch_semaphore_wait(sem_patients,DISPATCH_TIME_FOREVER);
            
            pthread_mutex_lock(&mutex_lock);
            num_of_patients_waiting--;
            
            int random_time= rand() % 3 + 1; //Get a random time between 1 and 3
            std::cout << "Doctor treating patient for " << random_time << " seconds.";
            std::cout << "Seats occupied = " << num_of_patients_waiting << "\n" << std::endl;
            std::cout << "Patient " << waiting_room_seats[next_patient] << " getting treatment." << "\n" << std::endl;
            sleep(random_time);
            waiting_room_seats[next_patient]= 0;
            next_patient = (next_patient+1) % seats;
            pthread_mutex_unlock(&mutex_lock);
            
            dispatch_semaphore_signal(sem_doctor);
        }
        else{
            /*
             No patients available.
             */
            std::cout << "No patients.. taking a nap." << "\n" << std::endl;
            int random_time = rand() %3 +1;
            sleep(random_time); //Get a random time between 1 and 3
        }
    }
}


int main(int argc, const char * argv[]) {
    int patients;       //Int type variable for number of patients
    srand(time(NULL));  //Random number for threads to sleep
    
    /*
     Information available for user
     */
    std::cout << "Please enter number of patients: ";
    
    std::cin >> patients;       //Get a number chosen by the user
    int patient_ID[patients];   //Patients to be assigned with IDs
    
    /*
     Initialize threads.
     */
    pthread_t doctor_thread;
    pthread_t thread_id[patients];
    

    sem_patients = dispatch_semaphore_create(0);
    sem_doctor =  dispatch_semaphore_create(1);
    //sem_init(&s_patients,0,0); //Semaphore initialize
    //sem_init(&s_doctor,0,1);   //Semaphore initialize
    
    pthread_mutex_init(&mutex_lock, NULL); //Mutex initialize
    
    
    pthread_create(&doctor_thread,NULL,doctor,NULL); //Create doctor
    
    for(int i =0; i<patients; i++){
        /*
         The number of threads for patients are based on user input.
         patients is being determined by the user and created here.
         */
        patient_ID[i]=i;
        pthread_create(&thread_id[i],NULL,patient,(void*)&patient_ID[i]);
        
    }
    
    /*
     This is where threads join
     */
    
    pthread_join(doctor_thread,NULL);
    for(int i=0; i<patients; i++){
        pthread_join(thread_id[i],NULL);
    }
    
    
    
    return 0; //End of program
}




