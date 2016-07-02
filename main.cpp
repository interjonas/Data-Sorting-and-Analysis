#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <cstdio>
#include <time.h>
#include <omp.h>

std::string s1(std::string input);
std::string s2(std::string input);
std::string append(std::string input);
void splitter(std::string input1, std::string input2, int N, int arr[],std::string fname);
void insertion_sort(int arr[], int length);
void average(int x[],int N);
void select_data(std::string input2, int arr[], int N);
void initiate_data(int data[],std::string fname,int N);
void mergeArray(int a[], int arrayBase, int arraySizeN, int mid);
void mergeSort(int a[], int arrayBase, int arraySizeN);
void mergeSortParallel(int a[], int arrayBase, int arraySizeN);
void averageParrallel(int x[], int N);
int swaps=0;
int comps=0;
int stoi(const char *s)

{
    int i;
    i = 0;
    while(*s >= '0' && *s <= '9')
    {
        i = i * 10 + (*s - '0');
        s++;
    }
    return i;
}

using namespace std;


int main()
{
	char input[20];
	string sub1="";
	string sub2="";
	do {

		cin.getline(input,sizeof(input));
		string input1=input;
		sub1 = s1(input);
		sub2 = s2(input);
		std::transform(sub1.begin(), sub1.end(), sub1.begin(), ::tolower);
		std::transform(sub2.begin(), sub2.end(), sub2.begin(), ::tolower);
		if (sub1 =="start")
		{
			std::string name = append(sub2);
			ifstream inFile(name.c_str(), ios::in);
			if (! inFile)
			   {
			      cerr << "unable to open input file: "
				   << name << endl;
			   }
			if (inFile)
			{
				cout << "start processing fitness data of: " << name << endl;
				bool status = false;
					ifstream myfile;
					std::string line ="";
					myfile.open(name.c_str());
					getline(myfile, line);
					const char *line1 = line.c_str();
					int N=stoi(line1);
					//N=N-1;
					int* data = NULL;   // Pointer to int, initialize to nothing.
					data = new int[N];
					for (int i=0;i<N;i++)
					{
						getline(myfile, line);
						const char *line2 = line.c_str();
						data[i]=stoi(line2);
					}
					do
					{
						cin.getline(input,sizeof(input));
						input1=input;
						sub1 = s1(input);
						sub2 = s2(input);
						std::transform(sub1.begin(), sub1.end(), sub1.begin(), ::tolower);
						std::transform(sub2.begin(), sub2.end(), sub2.begin(), ::tolower);
						splitter(sub1,sub2,N,data,name);
					}
					while (sub1 != "end");
					delete [] data;
					data=NULL;
					myfile.close();
					cout << "end of processing fitness data for: " << name << endl;
			}
		}
		else if (sub1 != "exit")
		{
			cout << "command invalid without inital array" << endl;
		}
	} while (sub1 != "exit");
	cout << "Program Terminating" << endl;

}
void initiate_data(int data[],std::string fname, int N)
{
	swaps=0;
	comps=0;
	ifstream inFile(fname.c_str(), ios::in);
	if (! inFile)
   {
		cerr << "unable to open input file: " << fname << endl;
	}
	if (inFile)
	{
		bool status = false;
		ifstream myfile;
		std::string line ="";
		myfile.open(fname.c_str());
		getline(myfile, line);
		for (int i=0;i<N;i++)
		{
			getline(myfile, line);
			const char *line2 = line.c_str();
            data[i]=stoi(line2);
		}
	}
}
std::string s1(std::string input)
{
	int x = input.find(" ");
	std::string str2 = input.substr (0,x);
	return str2;
}
std::string s2(std::string input)
{
	int x = input.find(" ");
	int y= input.length();
	std::string str2 = input.substr (x+1,y);
	return str2;
}
std::string append(std::string input)
{
	int x = input.find(".txt");
	string app = ".txt";
	if (x>=0)
	{
		return input;
	}
	else
	{
		input = input.append(app);
		return input;
	}
}
void splitter(std::string input1, std::string input2, int N, int arr[], std::string fname)
{
	if (input1=="insertionsort")
	{
	    uint64_t elapse;
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
		initiate_data(arr,fname,N);
		insertion_sort(arr,N);
		elapse = 1000000000 * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
        cout << "Time to run Insertion Sort(ms): " <<(long double) diff/1000000  << endl;
	}
	else if(input1=="mergesort")
    {
        uint64_t elapse;
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        initiate_data(arr,fname,N);
		mergeSort(arr,0,N-1);
        cout << "Number of Comparisons:\t" << comps << endl;
        cout << "Number of Swaps:\t" << swaps << endl;
        elapse = 1000000000 * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
        cout << "Time to run Merge Sort(ms): " <<(long double) diff/1000000  << endl;
    }
    else if(input1=="parallelmergesort")
    {
        uint64_t elapse;
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        initiate_data(arr,fname,N);
		mergeSortParallel(arr,0,N-1);
		cout << "Number of Comparisons (Parallel):\t" << comps << endl;
        cout << "Number of Swaps (Parallel):\t" << swaps << endl;
        elapse = 1000000000 * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
        cout << "Time to run Parallel Merge Sort(ms): " <<(long double) diff/1000000  << endl;
    }
    else if(input1=="parallelaverage")
    {
        uint64_t elapse;
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        averageParrallel(arr,N);
        elapse = 1000000000 * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
        cout << "Time to run Parallel Average(ms): " <<(long double) diff/1000000  << endl;
    }
	else if(input1=="average")
	{
	    uint64_t elapse;
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
		average(arr,N);
		elapse = 1000000000 * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
        cout << "Time to run Average(ms): " <<(long double) diff/1000000  << endl;
	}
	else if(input1=="select")
	{
	    uint64_t elapse;
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
		select_data(input2,arr,N);
		elapse = 1000000000 * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
        cout << "Time to run Select(ms): " <<(long double) diff/1000000  << endl;
	}
	else if(input1=="start")
	{
		cout << "Already initiated a data file" << endl;
	}
	else if (input1=="end")
	{

	}
	else
	{
		cout << "Incorrect Command" << endl;
	}
}
void mergeSort(int a[], int arrayBase, int arraySizeN)
{
    if (arrayBase >= arraySizeN)
        return;

    int mid;
    if (arrayBase < arraySizeN)
    {
        mid=(arrayBase+arraySizeN)/2;
        mergeSort(a,arrayBase,mid);
        mergeSort(a,mid+1,arraySizeN);
        mergeArray(a,arrayBase,arraySizeN,mid);
    }
    return;
}
void mergeSortParallel(int a[], int arrayBase, int arraySizeN)
{
    int mid;
    int threads= omp_get_num_threads();
    if (threads<=1)
    {
        mergeSort(a,arrayBase,arraySizeN);
    }
    else if (arrayBase < arraySizeN && threads >1)
    {
        #pragma omp parallel sections
        {
            //#pragma omp section
            mid=(arrayBase+arraySizeN)/2;
            #pragma omp section
            mergeSortParallel(a,arrayBase,mid);
            #pragma omp section
            mergeSortParallel(a,mid+1,arraySizeN);
        }
        mergeArray(a,arrayBase,arraySizeN,mid);
    }
}
void mergeArray(int a[], int arrayBase, int arraySizeN, int mid)
{
    int i, j, k, c[arraySizeN];
    i = arrayBase;
    k = arrayBase;
    j = mid + 1;

    while (i <= mid && j <= arraySizeN)
    {
        if (a[i] < a[j])
        {
            c[k] = a[i];
            k++;
            i++;
			swaps++;
			comps++;
        }
        else
        {
            c[k] = a[j];
            k++;
            j++;
            swaps++;
			comps++;
        }
    }
    while (i <= mid)
    {
        c[k] = a[i];
        k++;
        i++;
    }
    while (j <= arraySizeN)
    {
        c[k] = a[j];
        k++;
        j++;
    }
    for (i = arrayBase; i < k; i++)
    {
        a[i] = c[i];
    }
}
void insertion_sort(int arr[], int length)
 {
 	int i, j ,tmp;
	int c=0;
	int d=0;
 	for (i = 1; i < length; i++)
	{
		j = i;
 		while (j > 0 && arr[j - 1] > arr[j])
		{
 			tmp = arr[j];
 			arr[j] = arr[j - 1];
 			arr[j - 1] = tmp;
 			j--;
			c++;
			//d++;
 		}//end of while loop
 	d++;
	}//end of for loop
	//test(arr);
	cout << "Number of Comparisons:		" << d+c << endl;
	cout << "Number of Swaps:		" << c << endl;
 }//end of insertion_sort.

void average(int x[], int N)
{
	int sum=0;
	for (int i=0;i<N;i++)
	{
		sum=sum+x[i];
	}
	double avg = sum/N;
	cout << "The Average is:		" << avg << endl;
}
void averageParrallel(int x[], int N)
{
	int sum=0;
	//int mid =N/2;
	int threads = omp_get_num_threads();

	#pragma omp parallel for shared(N,x) reduction (+:sum) num_threads(threads)
        for (int i=0;i<N;i++)
        {
            sum=sum+x[i];
        }

	double avg = sum/N;
	cout << "The Average (Parallel) is:	" << avg << endl;
}
void select_data(std::string input2, int arr[], int N)
{
	bool status = true;
	int z=0;
	for (int i=0;i<N-1;i++)
	{
		if(arr[i]>arr[i+1])
		{
			cout << "Data not sorted" << endl;
			status=false;
			break;
		}

	}
	if(status)
	{
	    const char *line1 = input2.c_str();
        int num=stoi(line1);
		 if(input2=="min")
			{
				cout << "Minimum:	" << arr[0] << endl;
			}
		else if(input2=="max")
			{
				cout << "Maximum:	" << arr[N-1] << endl;
			}
		else if(input2=="median" || input2=="med")
			{
				if(N%2==0)
				{
					int med = N/2;
					cout << "Median:		" << arr[med-1] << endl;
				}
				else if (N%2==1)
				{
					int med = N/2;
					int med2 = med+1;
					int sum= arr[med-1]+arr[med2-1];
					med=sum/2;
					cout << "Median:		" << med <<endl;
				}
			}
			else if (num>0 && num<N)
			{
				cout << "The " << z << " element is:	 		";
				z=z-1;
				cout << arr[z] << endl;
			}
			else if (num<0 || num>N)
			{
				cout << "Not acceptable bounds" << endl;
			}
	}
}
