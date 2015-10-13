#include <iostream>
#include <cstdlib>
#include <ctime>


int select(int* array, int size, int k);

int main(int argc, char *args[]) {

    int array[6] = {15,3,2,7,1,18};
    int k = 4;
    
    std::cout << select(array, 6, k) << std::endl;


    return 0;
}


        
int select(int* array, int size, int k) {

    int kth = -1;

    srand( time(NULL) );

    int pivot = rand() % size;

    int pivot_val = array[pivot];

    int *larray = (int*) malloc (sizeof(int) * size);
    int *rarray = (int*) malloc (sizeof(int) * size);
    int lnum = 0;
    int rnum = 0;

    std::cout << pivot << " index = " << pivot_val << std::endl;
    for (int i = 0; i < size; ++i) {
        if (array[i] < pivot_val) {
            std::cout << array[i] << " is going into larray" << std::endl;
            larray[lnum] = array[i];
            ++lnum;
        } else if (array[i] > pivot_val){
            std::cout << array[i] << " is going into rarray" << std::endl;
            rarray[rnum] = array[i];
            ++rnum;
        }
    }


    if (lnum < (k - 1)) {
        std::cout << " sorting rarray of size " << rnum << std::endl;
        kth = select(rarray, rnum, k-lnum-1); // we've removed lnum+1 numbers that are less than the kth smallest, so we are not looking for the k-(lnum+1)th smallest number in rarray
    } else if (lnum > (k - 1)) {
        std::cout << " sorting larray of size " << lnum << std::endl;
        kth = select(larray, lnum, k);
    } else {
        kth = pivot_val;
    }

    free(larray);
    free(rarray);

    return kth;
}    
