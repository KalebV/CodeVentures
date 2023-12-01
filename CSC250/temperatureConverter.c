#include <stdio.h>

double CtoF(double conv);
double FtoC(double conv);

int main(){
	char unit;
	double temp;
	scanf("%lf%c", &temp, &unit);
	if(unit == 'C'){
		printf("%lfF",CtoF(temp));
	} else if(unit == 'F'){
		printf("%lfC",FtoC(temp));
	} else {
		return 0;
	}
}

double FtoC(double conv){
	return ((conv-32)*5/9);
}
double CtoF(double conv){
	return ((conv*9/5)+32);
}
