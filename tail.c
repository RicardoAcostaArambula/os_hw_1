#include <stdio.h>

int main(int argc, char **argv){

    if (argc == 1){
        /*Case when tail is call without -n optional nor file*/

    } else if (argc == 2){
        /*Case when tail is call with a file*/

    } else if (argc == 3){
        /*case when tail is call with an optional -n and a number of lines*/
        /*Ex. tail -n 10*/
    } else if (argc ==4){
        /*case when tail is call with a file and optional in reasonable order*/

    }
    return 0;
}