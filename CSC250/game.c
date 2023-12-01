#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct {
	char attack[20];
	int chance;
	int damage;
	char damtype[10];
}attack;
typedef struct {
	int numChar;
	char teamName[30];
} teamDef;
typedef struct {
	char name[30];
	int character;
	int health;
	int attackOrder;
	int team;
	int turnsStunned;
} character;


void newGame(teamDef teams[], int arSize, character characters[]);
character* loadGame(FILE* fp, character characters[], teamDef teams[], int* arSize, int* turn);
void saveGame(character characters[], teamDef teams[], int arSize, int turn);
character* charDeath(character characters[], int* arSize, int charId, teamDef teams[]);
character* roundExecute(character characters[], int* roundNumber, int* arSize, attack attacks[], int atord[], teamDef teams[]);
int* recalcAtord(character characters[], int arSize, int atord[]);
void printCharStruct(character characters[], int arSize);
void printAtordArray(character characters[], int atord[], int charSize);
character* lightAttack(character characters[], int* arSize, attack attacks[], int Id, teamDef teams[], int atord[]);
character* heavyAttack(character characters[], int* arSize, attack attacks[], int Id, teamDef teams[], int atord[]);
void heal(character characters[], int arSize, attack attacks[], int Id);
void stun(character characters[], int arSize, attack attacks[], int Id);
int chooseAttack(attack attacks[]);
attack* initAttacks();

int main () {
	srand(time(NULL));
	FILE* fbin = fopen("GameSave.bin", "rb");
	teamDef teams[2];
	int charSize = 1, option, round = 0;
	strcpy(teams[1].teamName, "Computer");
	character* characters = malloc(sizeof(character)*2);
	attack* attacks = initAttacks();
	if (fbin == NULL){
		printf("No save on file. Creating new SaveGame\n");
		printf("How many total Characters will there be among the 2 Teams? ");
		scanf("%d", &charSize);
		characters = realloc(characters, sizeof(character)*charSize);
		newGame(teams, charSize, characters);
		printCharStruct(characters, charSize);
	} else {
		int loadOption = 3;
		do{
			printf("Game Menu: \n"
				   "1: Start New Game\n"
				   "2: Load Game\n");
			scanf("%d", &loadOption);
			if(loadOption == 1){
				fclose(fbin);
				printf("How many total Characters will there be among the 2 Teams? ");
				scanf("%d", &charSize);
				characters = realloc(characters, sizeof(character)*charSize);
				newGame(teams, charSize, characters);
				printCharStruct(characters, charSize);
			} else if(loadOption == 2){
				characters = loadGame(fbin, characters, teams, &charSize, &round);
				printCharStruct(characters, charSize);
			} else {
				printf("Invalid Option\n");
			}
		} while(loadOption != 1 && loadOption != 2);
	}
	int* atord = malloc(sizeof(int) * charSize);
	atord = recalcAtord(characters, charSize, atord);
	printAtordArray(characters, atord, charSize);

	// Game Initialization ends here

	while (1){
		if (teams[0].numChar == 0 || teams[1].numChar == 0){
			if(teams[1].numChar == 0){
				printf("The %s team has won the Game\n", teams[0].teamName);
			} else {
				printf("The %s team has won the Game\n", teams[1].teamName);
			}
			return 0;
		}
		printf("What would you like to do?\n"
			   "1: Execute next round\n"
			   "2: Save Game\n"
			   "3: Exit\n");
		scanf("%d", &option);
		switch(option){
			case 1:
				characters = roundExecute(characters, &round, &charSize, attacks, atord, teams);
				break;
			case 2:
				saveGame(characters, teams, charSize, round);
				break;
			case 3:
				return 0;
				break;
			default:
				printf("Invalid Option\n");
				break;
		}
	}
	
	return 0;
}

void newGame(teamDef teams[], int arSize, character characters[]) {
	do {
	printf("Make sure the number of total entities adds up to %d\n", arSize);
	printf("How many Players? ");
	scanf("%d", &teams[0].numChar);
	printf("How many Computers? ");
	scanf("%d", &teams[1].numChar);
	} while(teams[0].numChar + teams[1].numChar != arSize);
	printf("Number of Players: %d\n", teams[0].numChar);
	printf("Number of Computers: %d\n", teams[1].numChar);
	printf("Enter a team name for the Player (1 - 30 Characters): ");
	getchar();
	fgets(teams[0].teamName, sizeof(teams[0].teamName), stdin);
	teams[0].teamName[strlen(teams[0].teamName)-1] = '\0';
	int attackReservations[arSize];
	for(int i = 0; i < teams[0].numChar; i++){
		printf("Enter the name for Player %d (1 - 30 Characters): ", i);
		fgets(characters[i].name, sizeof(characters[i].name), stdin);
		characters[i].name[strlen(characters[i].name) - 1] = '\0';
		characters[i].character = i;
		characters[i].health = 100;
		characters[i].team = 1;
		characters[i].turnsStunned = 0;
/*		printf("Character Id: %d\n"
			   "Character Health: %d\n"
			   "Character Team: %d\n"
			   "Attack Order: \n\n", characters[i].character, characters[i].health, characters[i].team);
*/	}
	for(int i = teams[0].numChar; i < arSize; i++){
		strcpy(characters[i].name, "Computer");
		characters[i].character = i;
		characters[i].health = 100;
		characters[i].team = 2;
		characters[i].turnsStunned = 0;
/*		printf("Character Id: %d\n"
			   "Character Health: %d\n"
			   "Character Team: %d\n"
			   "Attack Order: \n\n", characters[i].character, characters[i].health, characters[i].team);
*/	}
	int i = 0;
	while(i < arSize){
		int t = rand() % arSize;
		if(attackReservations[t] != 1){
			attackReservations[t] = 1;
			characters[i].attackOrder = t;
			i++;
		}
	}
	return;
}

character* loadGame(FILE* fp, character characters[], teamDef teams[], int* arSize, int* turn){
	fread(arSize, sizeof(int), 1, fp);
	characters = realloc(characters, sizeof(character)*(*arSize));
	for (int i = 0; i < *arSize; i++){
		fread(&characters[i], sizeof(character), 1, fp);
	}
	for (int i = 0; i < 2; i++){
		fread(&teams[i], sizeof(teamDef), 1, fp);
	}
	fread(turn, sizeof(int), 1, fp);
	fclose(fp);
	return characters;
}

void saveGame(character characters[], teamDef teams[], int arSize, int turn){
	FILE* fbin = fopen("GameSave.bin", "wb");
	fseek(fbin, 0, SEEK_SET);
	fwrite(&arSize, sizeof(int), 1, fbin);
	for (int i = 0; i < arSize; i++){
		fwrite(&characters[i], sizeof(character), 1, fbin);
	}
	for (int i = 0; i < 2; i++){
		fwrite(&teams[i], sizeof(teamDef), 1, fbin);
	}
	fwrite(&turn, sizeof(int), 1, fbin);
	fclose(fbin);

	return;
}

character* charDeath(character characters[], int* arSize, int charId, teamDef teams[]){
	for(int i = 0; i < *arSize; i++){
		if (characters[i].character == charId){
			if (characters[i].team == 1){
				printf("%s has died\n\n", characters[i].name);
			} else {
				printf("Computer Id %d has died\n\n", charId);
			}
			teams[characters[i].team - 1].numChar -= 1;
			for(int j = 0; j < *arSize; j++){
				if(characters[j].attackOrder > characters[i].attackOrder){
					characters[j].attackOrder -= 1;
				}
			}
			if (*arSize - 1 == i){
				characters = realloc(characters, sizeof(character)*(*arSize - 1));
			} else {
				for(int j = i; j <= (*arSize - 1); j++){
					strcpy(characters[i].name, characters[i + 1].name);
					characters[i].character = characters[i + 1].character;
					characters[i].health = characters[i + 1].health;
					characters[i].team = characters[i + 1].team;
					characters[i].attackOrder = characters[i + 1].attackOrder;
					characters[i].turnsStunned = characters[i + 1].turnsStunned;
				}
				characters = realloc(characters, sizeof(character)*(*arSize - 1));
			}
			*arSize -= 1;
		}
	}
	return characters;
}

character* roundExecute(character characters[], int* roundNumber, int* arSize, attack attacks[], int atord[], teamDef teams[]){
	int attackChoice, Id;
	if(characters[atord[*roundNumber]].turnsStunned > 0){
		printf("Character is Stunned, moving to next turn\n");
		characters[atord[*roundNumber]].turnsStunned -= 1;
		if(*roundNumber == *arSize - 1){
			*roundNumber = 0;
		} else {
			*roundNumber += 1;
		}
		return characters;
	}
	if(characters[atord[*roundNumber]].team == 1){
		attackChoice = chooseAttack(attacks);
		printf("Choose a target (Type in a character Id): ");
		scanf("%d", &Id);
	} else {
		attackChoice = rand() % 4;
		if (attackChoice == 2) {
			Id = rand() % teams[1].numChar + teams[0].numChar;
		} else {
			Id = rand() % teams[0].numChar;
		}
	}
	switch(attackChoice){
		case 0:
			lightAttack(characters, arSize, attacks, Id, teams, atord);
			break;
		case 1:
			heavyAttack(characters, arSize, attacks, Id, teams, atord);
			break;
		case 2:
			heal(characters, *arSize, attacks, Id);
			break;
		case 3:
			stun(characters, *arSize, attacks, Id);
			break;
	}
	if(*roundNumber == *arSize - 1){
		*roundNumber = 0;
	} else {
		*roundNumber += 1;
	}
	printf("----------------------------------------------\n");
	printCharStruct(characters, *arSize);
	return characters;
}

int* recalcAtord(character characters[], int arSize, int atord[]){
	for(int i = 0; i < arSize; i++){
		for(int j = 0; j < arSize; j++){
			if(i == characters[j].attackOrder){
				atord[i] = characters[j].character;
			}
		}
	}
	atord = realloc(atord, sizeof(int) * arSize);
/*	for (int i = 0; i < arSize; i++){
		printf("%d", atord[i]);
	}
	printf("\n");
*/	return atord;
}

void printCharStruct(character characters[], int arSize){
	for(int i = 0; i < arSize; i++) {
		printf("Character Name: %s\n"
			   "Character Id: %d\n"
			   "Character Health: %d\n"
			   "Character Team: %d\n"
			   "Attack Order: %d\n\n", characters[i].name, characters[i].character, characters[i].health, characters[i].team, characters[i].attackOrder);
	}
	return;
}

void printAtordArray(character characters[], int atord[], int charSize){
	printf("New Attack Order\n");
	for (int i = 0; i < charSize; i++){
		for(int j = 0; j < charSize; j++){
			if(characters[j].character == atord[i]){
				printf("%s ", characters[j].name);
			}
		}
	}
	printf("\n\n");
	return;
}

character* lightAttack(character characters[], int *arSize, attack attacks[], int Id, teamDef teams[], int atord[]){
	if(rand() % 100 < attacks[0].chance){
		for(int i = 0; i < *arSize; i++){
			if (characters[i].character == Id){
				characters[i].health -= attacks[0].damage;
				printf("Success! Opponent damaged for %d, their remaining health is %d\n", attacks[0].damage, characters[i].health);
				if(characters[i].health <= 0){
					characters[i].health = 0;
					characters = charDeath(characters, arSize, Id, teams);
					recalcAtord(characters, *arSize, atord);
					printAtordArray(characters, atord, *arSize);
				}
			}
		}
	} else {
		printf("Missed!\n");
	}
	return characters;
}

character* heavyAttack(character characters[], int* arSize, attack attacks[], int Id, teamDef teams[], int atord[]){
	if(rand() % 100 < attacks[1].chance){
		for(int i = 0; i < *arSize; i++){
			if (characters[i].character == Id){
				characters[i].health -= attacks[1].damage;
				printf("Success! Opponent damaged for %d, their remaining health is %d\n", attacks[1].damage, characters[i].health);
				if(characters[i].health <= 0){
					characters[i].health = 0;
					characters = charDeath(characters, arSize, Id, teams);
					recalcAtord(characters, *arSize, atord);
					printAtordArray(characters, atord, *arSize);
				}
			}
		}
	} else {
		printf("Missed!\n");
	}
	return characters;
}

void heal(character characters[], int arSize, attack attacks[], int Id){
	if(rand() % 100 < attacks[2].chance){
		for(int i = 0; i < arSize; i++){
			if (characters[i].character == Id){
				characters[i].health -= attacks[2].damage;
				if(characters[i].health > 100){
					characters[i].health = 100;
				}
				printf("Success! Target healed for %d, their remaining health is %d\n", -attacks[2].damage, characters[i].health);
			}
		}
	} else {
		printf("Missed!\n");
	}
	return;
}

void stun(character characters[], int arSize, attack attacks[], int Id){
	if(rand() % 100 < attacks[3].chance){
		for(int i = 0; i < arSize; i++){
			if (characters[i].character == Id){
				printf("Success! Opponent was hit stunning them for 2 turns\n", attacks[3].damage, characters[Id].health);
				characters[Id].turnsStunned = 2;
			}
		}
	} else {
		printf("Missed!\n");
	}
	return;
}

int chooseAttack(attack attacks[]){
	int attackChoice;	
	do {
		printf("Choose An attack\n");
		for (int i = 0; i < 4; i++){
			printf("Attack %d Name: %s\n"
				"Attack %d Type: %s\n"
				"Attack %d Hit Chance: ", 
				i, attacks[i].attack,
				i, attacks[i].damtype,
				i);
				switch(attacks[i].chance){
					case 30:
						printf("low\n");
						break;
					case 50:
						printf("medium\n");
						break;
					case 80:
						printf("high\n");
						break;
					case 100:
						printf("always\n");
						break;
				}
		}
		printf("Note -- If you enter an enemy's Id when trying to heal, you will heal your enemy\nNote -- If you enter a friendly Id while attacking, you will attack your allies\nEnter your Choice of attack: ");
		scanf("%d", &attackChoice);
	} while (attackChoice > 3 || attackChoice < 0); 
	return attackChoice;
}

attack* initAttacks(){
	attack* attacks = malloc(sizeof(attack)*4);
	attacks[0] = (attack){"Light Attack", 80, 30, "Melee"};
	attacks[1] = (attack){"Heavy Attack", 50, 60, "Melee"};
	attacks[2] = (attack){"Restore Health", 100, -30, "Heal"};
	attacks[3] = (attack){"Shield Bash", 30, 0, "Stun"};
	return attacks;
}