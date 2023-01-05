// THIS IS THE PROVIDED CODE FOR PROGRAM #2, DSA 1, FALL 2020

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

// A simple class; each object holds four public fields
class Data {
public:
  string lastName;
  string firstName;
  string ssn;
};

// Load the data from a specified input file
void loadDataList(list<Data *> &l, const string &filename) {

  ifstream input(filename);
  if (!input) {
    cerr << "Error: could not open " << filename << "\n";
    exit(1);
  }

  // The first line indicates the size
  string line;
  getline(input, line);
  stringstream ss(line);
  int size;
  ss >> size;

  // Load the data
  for (int i = 0; i < size; i++) {
    getline(input, line);
    stringstream ss2(line);
    Data *pData = new Data();
    ss2 >> pData->lastName >> pData->firstName >> pData->ssn;
    l.push_back(pData);
  }

  input.close();
}

// Output the data to a specified output file
void writeDataList(const list<Data *> &l, const string &filename) {

  ofstream output(filename);
  if (!output) {
    cerr << "Error: could not open " << filename << "\n";
    exit(1);
  }

  // Write the size first
  int size = l.size();
  output << size << "\n";

  // Write the data
  for (auto pData:l) {
    output << pData->lastName << " " 
	   << pData->firstName << " " 
	   << pData->ssn << "\n";
  }

  output.close();
}

// Sort the data according to a specified field
// (Implementation of this function will be later in this file)
void sortDataList(list<Data *> &);

// The main function calls routines to get the data, sort the data,
// and output the data. The sort is timed according to CPU time.
int main() {
  string filename;
  cout << "Enter name of input file: ";
  cin >> filename;
  list<Data *> theList;
  loadDataList(theList, filename);

  cout << "Data loaded.\n";

  cout << "Executing sort...\n";
  clock_t t1 = clock();
  sortDataList(theList);
  clock_t t2 = clock();
  double timeDiff = ((double) (t2 - t1)) / CLOCKS_PER_SEC;

  cout << "Sort finished. CPU time was " << timeDiff << " seconds.\n";

  cout << "Enter name of output file: ";
  cin >> filename;
  writeDataList(theList, filename);

  return 0;

}

// -------------------------------------------------
// YOU MAY NOT CHANGE OR ADD ANY CODE ABOVE HERE !!!
// -------------------------------------------------

// You may add global variables, functions, and/or
// class defintions here if you wish.

/*  
Andrew Kim - Programming Assignment #2: Sorting
ECE264: Data Structures and Algorithms I, Fall 2020
Professor Carl Sable

This program sorts a user-defined text document containing 100,000s to 1,000,000s
of randomly generated first names, last names, and social security numbers. 
*/

// global variables
int listSize;
int data, fourSSN, fiveSSN = 0;
list<Data*>::iterator i, it;
string temp, fn;
Data* ptrList[1100000];                 // list of Data pointers, used for faster radixSort()

int ssn2int(string s, int length);      // declaration of ssn2int, radixSort runs faster if it's declared first, then defined after

// [T1 & T2]
// T1 and T2 are not sorted at all, so I used the built in list::sort functionality, which is merge sort, to sort each Data member by character
bool comparison(const Data* data1, const Data* data2)
{
    const char *d1 = data1->lastName.c_str();
    const char *d2 = data2->lastName.c_str();

    for ( ; *d1 == *d2; d1++, d2++)     // returns false if Data members are not in descending order
    {
        if (*d1 == 0) 
        {
            for (d1 = data1->firstName.c_str(), d2 = data2->firstName.c_str(); *d1 == *d2; d1++, d2++) 
            {
                if (*d1 == 0) 
                {
                    for (d1 = data1->ssn.c_str(), d2 = data2->ssn.c_str(); *d1 == *d2; d1++, d2++) 
                    {
                        if (*d1 == 0) 
                        {
                            return *d1 < *d2;       
                        }
                    }
                    return *d1 < *d2;
                }
            }
            return *d1 < *d2;
        }
    }
    return *d1 < *d2;
}

// [T3] 
// Since T3 is already close-to-sorted, insertionSort is very fast at sorting the SSNs
void insertionSort(list<Data*>::iterator begin, list<Data*>::iterator end)
{
    for (it = next(begin); it != end; it++) 
    {
        auto startSSN = it;
        auto prevSSN = prev(startSSN);
        temp = (*startSSN)->ssn;
        while(startSSN != begin && strcmp(((*prevSSN))->ssn.data(), temp.data()) > 0)        // .data() worked faster than .c_str()
        {   
            (*startSSN)->ssn = (*(prevSSN))->ssn;
            startSSN--;
            prevSSN--;
        }
        (*startSSN)->ssn = temp;
    }
}

Data* counter[100000][500];             // count "bucket" for Data pointers
int bucket[100000] = {0};               // initialize count bucket for SSN values

// [T4]
// Radix Sort was used because no names had to be dealt with, so this is fastest to just sort numbers
// I read online that it is fastest to split a 9 digit number into one radix of 4 base-10 digits and one radix of 5 base-10 digits
void radixSort(list<Data *> &l)
{
    int fiveSSN = 0;
    for (int i = 0; i < listSize; i++)                      // goes through the ptrList and puts each 5 digit number into the correct bucket
    {
        fiveSSN = ssn2int((ptrList[i]->ssn), 5);
        counter[fiveSSN] [bucket[fiveSSN]] = ptrList[i];
        bucket[fiveSSN]++;
    }
    
    int ssnInt = 0, ssnCount = 0;
    for(int i = 0; i < listSize; i++)                       // go through every 5-digit number and check if there is an occurence in each bucket
    {
        if(ssnCount == bucket[ssnInt])                      // no occurences of that 5-digit number, move onto the next 5-digit number
        {
            ssnCount = 0;
            ssnInt++;
            i--;
        }
        else                                                // put the Data that corresponds to that 5-digit number into the correct position in ptrList
        {
            ptrList[i] = counter[ssnInt][ssnCount];
            ssnCount++;
        }
    }
    
    for(int i = 0; i < 100000; i++)
    {
        bucket[i]=0;
    }

    int fourSSN = 0;
    for(int i = 0; i < listSize; i++)                       // goes through the ptrList and puts each 5 digit number into the correct bucket
    {
        fourSSN = ssn2int((ptrList[i]->ssn), 4);
        counter[fourSSN] [bucket[fourSSN]] = ptrList[i];
        bucket[fourSSN]++;
    }
    
    ssnInt = 0; ssnCount = 0;
    for(int i = 0; i < listSize; i++)                       // go through every 4-digit number and check if there is an occurence in each bucket
    {
        if(ssnCount == bucket[ssnInt])                      // no occurences of that 4-digit number, move onto the next 4-digit number
        {
            ssnCount = 0;
            ssnInt++;
            i--;
        }
        else                                                // put the Data that corresponds to that 4-digit number into the correct position in ptrList
        {
            ptrList[i] = counter[ssnInt][ssnCount];
            ssnCount++;
        }
    }

    auto itr = l.begin();               
    for(int j = 0; j < listSize; j++)                       // now that ptrList is correctly sorted, assign it to the actual main list
    {
        (*itr++) = ptrList[j];
    }
}

// ssn2int - converts the SSN into an integer of either the first 4 digits or the last 5 digits
int ssn2int(string ssn, int length)
{
    if(length == 4)             // first four numbers of SSN
    {
        return (1000*(ssn[0]-'0'))+(100*(ssn[1]-'0'))+(10*(ssn[2]-'0'))+(1*(ssn[4]-'0'));
    }
    else                        // last five numbers of SSN
    {
        return (10000*(ssn[5]-'0'))+(1000*(ssn[7]-'0'))+(100*(ssn[8]-'0'))+(10*(ssn[9]-'0'))+(1*(ssn[10]- '0'));
    }
}

// sortDataList - identifies the list and applies the correct sorting algorithm
void sortDataList(list<Data *> &l) 
{
    listSize = l.size();                
    for (auto a: l)                             // fills in ptrList array with each Data pointer
    {
        ptrList[data] = a;
        data++;
    }

    // T4
    if((ptrList[0]->firstName == ptrList[listSize - 1]->firstName) && (ptrList[0]->lastName == ptrList[listSize - 1]->lastName)) 
    {
        radixSort(l);                           // Radix Sort
    }

    // T3
    else if(strcmp(((*next(l.begin(), 5))->lastName).data(), ((*next(l.begin(), 350))->lastName).data() ) <=0 && ((*next(l.begin(), 10))->lastName).compare((*next(l.begin(), 320))->lastName)<=0)
    {
        auto start = l.begin();
        fn = (*start)->firstName;
        
        for(i = l.begin(); i != l.end(); i++)
        {
          if((*i)->firstName != fn){            // iterates through the list until a different firstName is found
            insertionSort(start, i);            // Insertion Sort is run for those firstNames only
            start = i;
            fn = (*i)->firstName;
          }
        }
        insertionSort(start, l.end());
    }

    // T1 & T2
    else
    {
        l.sort(comparison);                     // Merge Sort
    }
}