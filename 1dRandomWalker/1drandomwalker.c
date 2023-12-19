#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define pi 3.141592653589793


struct Data{
    double *Path;
    double TotTimePerWalker;
    double MFPT;
    double Error;
    

};
struct Data drunkard(double xf, double x0, double stepsize);
struct Data MFPTandError(double xf, double x0, double stepsize, double D, long int N);
double analiticsolution(double xf, double x0, double D);


int main(void){
    double xf=100;
    double x0=50;
    double stepsize=1;
    double D=stepsize*stepsize/2;
    long int N=10001;
    srand48(time(NULL)); 
    struct Data meantime=MFPTandError(xf, x0, stepsize, D, N);
    struct Data drunk=drunkard(xf,x0,stepsize);
    printf("\nMFPT: %.2lf (+-) %2lf \n", meantime.MFPT, meantime.Error);
    printf("Analitic solution: %.2f ", analiticsolution(xf,x0,D));
    free(drunk.Path);
    return 0;
}


struct Data drunkard(double xf, double x0, double stepsize){
    //simulate the drunkard walk
    struct Data result;
    double l = stepsize;
    double location = x0;
    long long int tau=0;
    int antistuck = 1;
    result.Path=malloc(1000000*sizeof(double));
    //if the drunkard fall off the sinkhole, it stops.
        while(location<xf){

            //randomic number with [0.0 , 1.0] limits
            float luckynumber=drand48();
            if(location==0.00000){
                location=location+l;
                antistuck=0;
                //reflexive wall, if drunkard hit it, step forward
            }
            
            if(luckynumber>0.5){
                location=location+l;
                //if randomic higher than 0.5 step forward

            }
            else{
            
                location=location-l;
                //if randomic smaller than 0.5 step backwards
            }
            if(antistuck+1>=10000){
                printf("Drunkard stuck in an infinite loop. Exiting.\n");
                break;
            }

            
            tau++;
            result.Path[tau-1]=location;
            //collect path made by walker
            
        
            }
            result.TotTimePerWalker=tau;
            
return result;
            
}

struct Data MFPTandError(double xf, double x0, double stepsize, double D, long int N){
    //calculate Mean First Passage Time
    struct Data result;
    double analiticres=analiticsolution(xf,x0,D);
    double variance=0;
    double variance2=0;
    double *t = malloc(N * sizeof(double)); 
    double sum=0;
     double MFPT=0;
     //N is the number of walkers.
    for(long long int j=0; j<N;j++){
        struct Data drunk=drunkard(xf,x0,stepsize);
        t[j]=drunk.TotTimePerWalker;
        sum=sum+t[j];
        variance=pow((t[j]-analiticres),2);
        variance2=variance/N;
        result.MFPT=sum/N;
        printf("N= %.lld\n", j);
        free(drunk.Path);
    

    }
    //standard deviation
    result.Error=sqrt(variance2);
    free(t);
    

    return result;


}


double analiticsolution(double xf, double x0, double D){
    //raw analitic MFPT solution.
    double result=0, a=0;
    int i=0;
    double difconst=(2*pow(xf,2))/(D*pow(pi,3));
    for(i=0;i<1000;i++){
        result=result+difconst*cos((i+0.5)*pi*x0/xf)*pow(-1,i)/pow((i+0.5),3);
    }
return result;

}