// this file is for you for testing purposes, it won't be included in evaluation.

#include <iostream>
#include <random>
#include <ctime>
#include <utility>      // for pair
#include "the8.h"


using namespace std;


int main()
{
    srandom(time(0));
    int INF = 100000;
    int n1 = 4;
    int** graph = new int*[n1]{
        new int[n1] { 0, 5 ,INF, 10},
        new int[n1] { INF, 0, 3, INF },
        new int[n1] { INF, INF, 0, 1 },
        new int[n1] { INF, INF, INF, 0 }
    };

    int res1=minCost(graph,n1,0,1,2,3,5,6);
    cout<<res1<<endl;
    int** graph2 = new int*[n1]{
        new int[n1] { 0, 1 ,2, INF},
        new int[n1] { INF, 0, 1, 2 },
        new int[n1] { INF, INF, 0, 1 },
        new int[n1] { INF, INF, INF, 0 }
    };
    int res2=minCost(graph2,n1,0,1,2,3,3,5);
    cout << res2<<endl;



    /*int arr[] = {0,1,2,3,4};

#ifdef DEBUG
    DEBUG_ARRAY(arr, 5);
    DEBUG_STDERR(arr[3]);
#endif*/

    return 0;
}
