// Helper functions for music

#include <cs50.h>
#include<math.h>

#include "helpers.h"

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction){
    double one = fraction[0] - '0';
    double two = fraction[2] - '0';
    double note = one/two;
    return round(note/0.125);
}

int octave(int oct,double fre){
    return round(fre*pow(2,oct-4));
}

// Calculates frequency (in Hz) of a note
int frequency(string note){
    char Q = note[0];
    char SF = note[1];
    double fre = 440.0;
    switch(Q){
        case 'B': fre = fre*pow(2,(double)2/12);
            break;
        case 'C': fre = fre/pow(2,(double)9/12);
            break;
        case 'D': fre = fre/pow(2,(double)7/12);
            break;
        case 'E': fre = fre/pow(2,(double)5/12);
            break;
        case 'F': fre = fre/pow(2,(double)4/12);
            break;
        case 'G': fre = fre/pow(2,(double)2/12);
            break;
        default: break;
    }
    if(SF=='#' || SF=='b'){
        if(SF=='#'){
            fre = fre*pow(2,(double)1/12);
        }
        else{
            fre = fre/pow(2,(double)1/12);
        }
        int x = note[2]-'0';
        return octave(x,fre);
    }
    else {
        int i = note[1]-'0';
        return octave(i,fre);
    }
}


// Determines whether a string represents a rest
bool is_rest(string s)
{
    if(s[0] =='\0'){
        return true;
    }
    else return false;
}
