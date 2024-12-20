#include <iostream>
#include <string>
#include <cctype>
using namespace std;
int modInverse(int a,int m){
    int t1=0,t2=1,t,r1=m,r2=a; 
    while(r2!=0){ // applying extended euclidean algorithm to get t1 which is the mod inverse we are looking for
        int q,r;
        q=r1/r2;
        r=r1%r2;
        t=t1-t2*q; 
        r1=r2;
        r2=r;
        t1=t2;
        t2=t;
    }
    if(r1==1){
        while(t1<0){ // mod inverse must be bewteen 0 & m-1
            t1=t1+m;
    }
        return t1%m; // returning mod inverse of a and m with gcd of 1
    }
    else return -1; // not coprime so no mod inverse
}
int main(){
    int m,a;
    cin >> a >> m;
    cout << modInverse(a,m);
}