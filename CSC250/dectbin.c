#include <stdio.h>
int dtb(int dec);
int main () {
	int decimal = 0;
	scanf("%d",&decimal);
	dtb(decimal);
	return 0;
}
int dtb(int dec) {
	if (dec == 0){
		return 0;
	}else {
		dtb(dec/2);
		printf("%d", dec%2);
	}
}