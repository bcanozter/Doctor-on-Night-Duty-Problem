## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Compiling](#compiling)
* [Preview](#preview)
* [Author](#author)

## General info

 Create N patients using PThreads, with each patient running as a separate thread. The doctor runs
 as a separate thread as well. Patient threads will alternate between having coffee in the cafeteria
 and being treated by the doctor. If the doctor is available, they will be treated immediately.
 Otherwise, they will either sit in a chair in the waiting room or, if no chairs are available, will go
 back to the cafeteria for a random amount of time and will return to see the doctor at a later time. If
 a patient arrives and notices that the doctor is taking a nap, the patient must notify the doctor using
 a semaphore. When the doctor finishes treating a patient, she must check to see if there are patients
 waiting for help in the waiting room. If so, the doctor must help each of these patients in turn. If no
 patients are present, the doctor may return to napping.
	
## Technologies
Project is created with:
* XCode version: 10.2.1

## Compiling

To compile this software you must use a gcc terminal.
1. Open the bash terminal
2. Change the directory to where the make file is.
3. use g++ --std=c++11 main.cpp
4. run the outfile.


## Preview

<img src="https://i.imgur.com/K9yHxOC.png">


## Author

* Burak Can Ozter
* burak.ozter@dal.ca
	
