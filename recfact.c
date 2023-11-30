#include <stdio.h>


int factr(int a);
int factl(int a);

int main(){

	int n = 5;
	for(int i=1;i<=10;i++){
		printf("%d! (rec): %d\n",i,factr(i));
	}
	for(int i=1;i<=10;i++){
		printf("%d! (loop): %d\n",i,factl(i));
	}
	return 0;
}

int factl(int a){
	int ans = 1;
	for (int i = 1; i<=a; i++){
		ans*=i;
	}
	return ans;
}

int factr(int a){
	int ans = 1;
	if (a != 1){
		ans = a * factr(a-1);
	}
	return ans;
}