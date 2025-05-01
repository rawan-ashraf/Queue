//task 1 printer
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

class print {
    public:
    string pc; // PC name
    int duration;// Duration of the job
    int hours;//  time in hours
    char colom;// ':'
    int minutes;// time in minutes
    // Constructor
    print(string PC, int dur, int h, int m,char c) {
        pc = PC;
        duration = dur;
        hours = h;
        colom = c;
        minutes = m;
    }

// Default constructor

    print() {
        pc=" ";
        duration = 0;
        hours = 0;
        colom = 0;
        minutes = 0;
    };
// Overload << operator to print the job details
    friend ostream&operator<<(ostream&o,const print&p){
        o<<p.pc<<" "<<setw(2)<<setfill('0')<<p.hours<<":"<<setw(2)<<setfill('0')<<p.minutes<<" "<<p.duration;
    return o;}
// Overload >> operator to read job details from file
    friend istream&operator>>(istream& in, print&p){
        in>>p.pc>>p.hours>>p.colom>>p.minutes>>p.duration;
        return in;}


// Compare print jobs based on time and duration for sorting
    bool operator<(const print& other) const {
        if (hours < (other.hours)){
            return true;
        }
        else if (other.hours == hours && minutes < other.minutes){
            return true;
        }
        else if(other.hours == hours && other.minutes == minutes && duration < other.duration){
            return true;
        }
        else {
            return false;
        }
    }

};
class Node{
public:
    print data;// Print job data
    Node*next;
    Node(print d){
        data = d;
        next = nullptr;
    }
};

class Queue {
private:
    Node *first,*last;
public:
    Queue(){
    first=last=nullptr;}
    Node *temp=first;

// Add a new print job to the queue
    void enqueue(Node*n){
        if(isEmpty()){
          first=last=n;
         }
         else{
             last->next = n; // Add new job to the end
             last = n;
            }
    }

     // Remove and return the first print job from the queue
   print dequeue(){

        Node *temp=first;
        print d=temp->data;

            if(first==last){
                delete last;
                first=last=nullptr;
                return d;

            }
            else{
                first=first->next; // Move to next job
                delete temp;
                return d;
            }
    }
    // Check if the queue is empty
    bool isEmpty(){
       return first==nullptr;

    }
     // Get the first job in the queue
   print front(){
    if(!isEmpty()){
        return first->data;
    }
        else {
            cout<<"Empty Queue";
        }
    }
    // Clear the queue by removing all jobs
    void clear(){
        while(!isEmpty()){
            dequeue();
        }
    }
// Sort the jobs based on time and duration using insertion sort
    void sort(print arr[],int size){

        for (int i = 1, j; i < size; i++) {
             print key = arr[i];
            for (j = i; j > 0 && key < arr[j-1]  ; j--) {
                arr [j] = arr [j-1];
            }
            arr [j] = key;
        }


    }

    void job(){
        Node*n=first;
        ifstream infile("printer.txt");// Open file
        if(!infile){
            cout<<"failed to open"<<endl;
        }
        int size;
        infile>>size;// Read number of jobs

        int waiting_time=0;
        int current_time=0;
        double total_waiting_time=0;
        int total_duration=0;
        while(n!=nullptr){
            // If the current time is before or at the arrival time of the print job,
            if(current_time<=(n->data.hours*60 + n->data.minutes)){
                //set current time with the arrival time of the printer
                current_time=(n->data.hours*60 + n->data.minutes);
                waiting_time=0;
            }
            else{
                waiting_time=current_time-(n->data.hours*60 + n->data.minutes);
            }
            total_duration+=n->data.duration;
            total_waiting_time+=waiting_time;

            // Output job details
            cout <<dequeue();
            cout<< " executed at " <<setw(2)<<setfill('0')<< current_time / 60 << ":"
            <<setw(2)<<setfill('0')<< current_time % 60
                << "    waiting time: " << waiting_time << endl;

            current_time += n->data.duration;
            n=n->next;

        }
        cout<<endl;
        cout<<"The printer was occupied for "<<total_duration<<" minuites"<<endl;
        cout << fixed << setprecision(1); //display average time rounded to 1 decimal place
        cout<<"Average waiting time for all jobs is "<<total_waiting_time/size<<" minuites"<<endl;
    }
       // Read jobs from file and add to the queue
    void ReadFile() {
        ifstream infile;
        infile.open("printer.txt");
        int size;
        infile>>size;
        infile.ignore();
        print arr[size];

        for (int i=0; i<size; i++) {
            infile>>arr[i];// Read job details
        }
        sort(arr, size); // Sort jobs
        for(int i=0;i<size;i++) {
            Node*n=new Node(arr[i]);
            enqueue(n);// Add sorted jobs to the queue
        }

        infile.close();// Close file

    }


};


int main()
{

    Queue q;
    q.ReadFile(); //function to sort and enqeue the data
    q.job(); //function to calculate current and waiting time

return 0;
}