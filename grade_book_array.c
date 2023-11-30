#include <stdio.h>

int main() {

	int c = 0, a = 0, s = 0;
//Get array sizing
	scanf("%d %d %d",&c, &a, &s);

//Initialize grade book array
	int gb[c][a][s], sum[s];

//Initialize calculation array (Avgs and Sums)
	double avg[c][a];


//Write to Array
	for (int i = 0; i < c; i++){
		for (int j = 0; j < a; j++){
			for (int k = 0; k < s; k++){
				scanf("%d", &gb[i][j][k]);
			}
		}
	}
//Print array
/*
	for (int i = 0; i < c; i++){
		for (int j = 0; j < a; j++){
			for (int k = 0; k < s; k++){
				printf("%d ", gb[i][j][k]);
			}
		printf("\n");
		}
	printf("\n");
	}
*/


	for (int i = 0; i < c; i++){
		for (int j = 0; j < a; j++){
			avg[i][j] = 0;
			for (int k = 0; k < s; k++){
				avg[i][j] += (double)gb[i][j][k];
			}
			printf("%.2lf ", avg[i][j] / (s));
		}
		printf("\n");
	}


	for (int i = 0; i < s; i++){
		sum[i] = 0;
		for(int j = 0; j < c; j++){
			for(int k = 0; k < a; k++){
				sum[i] += gb[j][k][i];
			}
		}
		printf("%d ", sum[i]);
	}





	return 0;
}