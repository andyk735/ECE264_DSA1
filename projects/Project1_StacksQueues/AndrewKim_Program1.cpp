/*  
Andrew Kim - Programming Assignment #1: Stacks and Queues
ECE264: Data Structures and Algorithms I, Fall 2020
Professor Carl Sable

This program serves to accurately process and execute commands given by any valid 
input text document, using an implemented singly linked list comprised of stacks and queues. 
All processes, errors, and values popped are outputted into a separate text document.
Both text documents are defined by the user.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <sstream>

using namespace std;

ifstream ifs;
ofstream ofs;

// SimpleList - abstract base class definition that functions as a simply linked list
// Uses template T for int, double, and string functionality
template <typename T>
class SimpleList
{
    private:
        // Node - elements of SimpleList that are not adjacent in memory but linked
        // Contains the data within the node and a pointer to the next node
        struct Node
        {
            T data;
            Node *next;

            // Node Constructor, default pointer is NULL
            Node(T value, Node *ptr = NULL)
            {
                data = value;
                next = ptr;
            }
        };

        // private data members
        int listSize;
        Node *head;
        Node *tail;
        string listName;

    public:                           
        // SimpleList Constructor, it begins empty with head and tail pointing to NULL
        SimpleList(string listName) : listName(listName)        // here listName is initialized in order to avoid issues with searchSimpleList
        {
            listSize = 0;
            head = NULL;
            tail = NULL;
        }

        string getListName() const
        {
            return listName;
        }

        int getListSize() const
        {
            return listSize;
        }

        // push and pop will be overridden by Stack and Queue for their respective functionalities
        virtual void push(T value) = 0;
        virtual T pop() = 0;

    protected:
        // insert a Node at the start of a SimpleList
        void insertNodeAtStart(T value)
        {
            Node *newNode = new Node(value, head);
            head = newNode;                     // create a newNode w/ next ptr to the current head, set current head to point to newNode
            listSize++;
        }

        // insert a Node at the end of a SimpleList
        void insertNodeAtEnd(T value)
        {
            Node *newNode = new Node(value);

            if (tail == NULL)                   // if there were no nodes, set head and tail to newNode, now that there is one in the SimpleList
            {
                head = newNode; 
                tail = newNode;
            }

            else                                // if there are nodes, set tail to newNode
            {
                tail->next = newNode;
            }
            
            tail = newNode;                     // reassign tail pointer to new end Node
            listSize++;
        }

        // remove Node from the start of a SimpleList
        T removeNodefromStart()
        {
            Node *temp = head;
            T tempValue = temp->data;

            head = head->next;                  // assign the head to the next Node, which is the new start Node
            delete temp;
            listSize--;

            return tempValue;                   // return the removed item to print what value was popped in exectuteCommand()
        }
};

// Stack - class definition for Stack, inherits SimpleList, acts like a bucket
template <typename T>
class Stack : public SimpleList<T>
{
    public:
        // Stack Constructor, initializes name argument to listName
        Stack(string name): SimpleList<T>(name) {}

        // pushing onto Stack means adding to the top/head
        void push(T value)
        {
            this->insertNodeAtStart(value);
        }

        // popping from Stack means removing from top/head
        T pop()
        {
            return this->removeNodefromStart();
        }
};

// Queue - class definition for Queue, inherits SimpleList, acts like a line
template <typename T>
class Queue : public SimpleList<T>
{
    public:
        // Queue Constructor, initializes name argument to listName
        Queue(string name): SimpleList<T>(name) {}

        // pushing onto Queue means adding to the back/tail
        void push(T value)                      
        {
            this->insertNodeAtEnd(value);
        }

        // popping from Queue means removing from front/head
        T pop()                                
        {
            return this->removeNodefromStart();
        }
};

// Declarations for lists containing pointers to all stacks and queues of integers, doubles, and strings
// Defined here because it requires SimpleList, but will also be used in upcoming functions
list<SimpleList<int> *> listSLi;
list<SimpleList<double> *> listSLd;
list<SimpleList<string> *> listSLs; 

// searchSimpleList - search through a C++ list of SimpleLists to find & return a SimpleList w/ a specific name
template <typename T>
SimpleList<T>* searchSimpleList(list<SimpleList<T> *> mainList, string name)
{
    // iterator is used to go through the list and return a reference to a SimpleList
    typename list<SimpleList<T> *>::iterator itr;

    for(itr = mainList.begin(); itr != mainList.end(); ++itr)
    {
        if(name == (*itr)->getListName())
        {
            return *itr;                                        // itr enables the function to output the SimpleList at that pointer
        }
    }
    return NULL;
}

// executeCommand - executes each line of the input text document by using arguments provided by processCommand()
// inputs[] is an array containing each word of a single command/line in the text document (ex. ["create", "i1", "queue"])
template <typename T>
void executeCommand(list<SimpleList<T> *> &mainList, string inputs[], T value)
{
    // 1st word is "create"
    if(inputs[0] == "create")
    {
        // error check SimpleList existence using 2nd word
        if(searchSimpleList(mainList, inputs[1]) != NULL)     
        {
            ofs << "ERROR: This name already exists!\n";
        }

        else
        {
            // 3rd word is "stack", so create a Stack
            if(inputs[2] == "stack")                            
            {
                Stack<T> *ptrS = new Stack<T>(inputs[1]);
                mainList.push_front(ptrS);                      // push_front() is a C++ list function, it pushes ptrS to a list of pointers
            }

            // 3rd word is "queue", so create a Queue
            if(inputs[2] == "queue")                            
            {
                Queue<T> *ptrQ = new Queue<T>(inputs[1]);
                mainList.push_front(ptrQ);
            }
        }
    }

    // 1st word is "push"
    else if(inputs[0] == "push")
    {
        // error check SimpleList existence using 2nd word
        SimpleList<T> *ptrCheck = searchSimpleList(mainList, inputs[1]);
        if(ptrCheck == NULL)                                    
        {
            ofs << "ERROR: This name does not exist!\n";
        }

        else
        {
            ptrCheck->push(value);                              // value is of type T because it could be an int, double, or string
        }
    }

    // 1st word is "pop"
    else if(inputs[0] == "pop")
    {
        // error check SimpleList existence using 2nd word
        SimpleList<T> *ptrCheck = searchSimpleList(mainList, inputs[1]);
        if(ptrCheck == NULL)
        {
            ofs << "ERROR: This name does not exist!\n";
        }

        // error check SimpleList size
        else if(ptrCheck->getListSize() == 0)
        {
            ofs << "ERROR: This list is empty!\n";
        }

        // output the value that was popped
        else
        {
            ofs << "Value popped: " << ptrCheck->pop() << endl;
        }
    }
}

// processCommand - reads each command line, interprets how to execute them, then calls executeCommand
void processCommand(string command)
{
    // outputs which command is currently being processed and executed
    ofs << "PROCESSING COMMAND: " << command << endl;

    string steps[3];                                            // steps is an array of each word in a single command/line
    stringstream ssin(command);                                 // stringstream is used to separate a string into an array of strings/words
    int i = 0;
    while(ssin.good() && i < sizeof(steps))
    {
        ssin >> steps[i++];                                     // steps is now an array of a command line's words (ex. ["create", "i1", "queue"])
    }

    // process commands w/ lists of type int
    if(steps[1].substr(0, 1) == "i")            
    {
        int num = 0;

        // if there is a 3rd word, it means this value must be pushed onto the SimpleList
        if(!steps[2].empty())
        {
            num = atoi(steps[2].c_str());                       // convert string to int
        }
        executeCommand(listSLi, steps, num);                    // execute the command using listSLi, steps stores the actual command
    }

    // process commands w/ lists of type double 
    else if(steps[1].substr(0, 1) == "d")
    {
        double num = 0.0;

        // if there is a 3rd word, it means this value must be pushed onto the SimpleList
        if(!steps[2].empty())
        {
            num = atof(steps[2].c_str());                       // convert string to double
        }
        executeCommand(listSLd, steps, num);                    // execute the command using listSLd, steps stores the actual command
    }

    // process commands w/ lists of type string 
    else if(steps[1].substr(0, 1) == "s")
    {
        executeCommand(listSLs, steps, steps[2]);               // execute the command using listSLs, steps stores the actual command
    }
}

// main - opens input and output text files and parses/processes through each input line
int main()
{
    // user input for input and output text files
    string inFile;
    string outFile;
    
    cout << "Enter name of input file: ";
    cin >> inFile;
    cout << "Enter name of output file: ";
    cin >> outFile;

    // open input and output files
    ifs.open(inFile.c_str());
    ofs.open(outFile.c_str());

    // go through entire input file and process each command
    string line;
    while(getline(ifs, line))
    {
        processCommand(line);
    }

    ifs.close();
    ofs.close();

    return 0;
}
