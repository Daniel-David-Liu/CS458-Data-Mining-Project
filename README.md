# CS458-Data-Mining-Project

Implementation of Task 1 of the final group project for CS 458 at UNR, Fall 2018.

## Getting Started

Details of the project task can be found in "Project Description.pdf". Essentially a training data set was provided to use in Weka, where the trained model would then be used against the testing data set. The program converts the data in csv files into a Weka readable sparse .arff format. Labels with less than 5 occurrences were trimmed. The program must be compiled with C++11, eg:
```
$ g++ -std=c++11 -o preprocess Task1_Preprocessor.cpp
$ ./preprocess
```
To do the same for the training data set, simply swap all the training files with testing and vice versa in the program (lines 18, 20, 22, 24).
