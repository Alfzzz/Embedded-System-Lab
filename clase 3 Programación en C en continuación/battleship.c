/*Battleship*/
/*Alfredo Zhu Chen A01651980*/
//Mi compañero Juan Pablo y yo compartimos ideas para hacer esta práctica, entonces puede que compartan cierta lógica en la programación, saludos
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
void printBoard(int size,char board[size][size]); //function to print the board
void placeShips(int size,int ships[17][3]);	//function to place the ships manually
void random_placeShips(int size,int ships[17][3]);//function to place the ships randomly
void boardInit(int size,char board[size][size]);	//function to initialize the characters of the board
int play(int size,char board_p1[size][size],char board_p2[size][size],int ships1[17][3],int ships2[17][3]);//function of the game 
int main(){
	srand(time(0));//generate random seeds
	int size, option, winner;;//size of the 
	printf("Enter the size of the board(nxn),use size of n>7 for random ships placing. The recommended size is 9\n");
	scanf("%i",&size); //ask for the size of the board with size nxn
	
	//initialize the board of both players and print them out
	char board1[size+1][size+1];//board of the player 1
	char board2[size+1][size+1];//board of the player 2
	boardInit(size+1,board1);//initialize board of player 1
	printBoard(size+1,board1);//print the board of player 1
	boardInit(size+1,board2);//initialize board of player 2
	printBoard(size+1,board2);//print the board of player 2

	//Set ships coordinates either manually or randomly
	int ships1[17][3]; //array with the ships coordinates for player 1, there are 5 ships with a total of 17 pairs of coordinates, the 3rd column represents if the coordinate is hit
	int ships2[17][3];//array with the ships coordinates for player 2,ships[17][3]------->|row|column|hit x17
	
	printf("-----------------    Player 1   -----------------\n");
	printf("Player1:Enter '1' to place ships manually and '2' to place ships randomly(enter the number without quotes)\n");
	scanf("%i",&option); //ask if the user wants to place the ships manually or randomly
	if(option==1){
		placeShips(size+1,ships1);//Place the ships manually for player 1 with option 1	
	}else{
		random_placeShips(size+1,ships1);//Place the ships randomly for player 1 with option 2	
	}
	printf("-----------------    Player 2   -----------------\n");
	printf("Player2:Enter '1' to place ships manually and '2' to place ships randomly(enter the number without quotes)\n");
	scanf("%i",&option);
	if(option==1){
		placeShips(size+1,ships2);//Place the ships randomly for player 2 with option 1
	}else if(option==2){
		random_placeShips(size+1,ships2);//Place the ships randomly for player 2 with option 2
	}
	//Call the play function and wait for it to return the winner
	winner=play(size+1,board1,board2,ships1,ships2);
	if(winner==1)
		printf("Play 1 wins");
	else
		printf("Play 2 wins");
	
	
	
	return 0;	
}

void printBoard(int size,char board[size][size]){
	/*arguments:
		int size:size of the the board(including the the board indices!)
		char board[size][size]: the board of the player*/
	int i,j;
	for(i=0;i<size;i++){
		for(j=0;j<size;j++){
			if(j==0 & i>9)	
				printf("%i ",i);//horizontal indices>9
			else if(i==0 & j>9)
				printf("%i ",j);//vertical indices>9
			else
				printf(" %c ",board[i][j]);//coordinates and indices<10
		}
		printf("\n");
	}
}

void boardInit(int size,char board[size][size]){
	/*arguments:
		int size:size of the the board(including the the board indices!)
		char board[size][size]: the board of the player*/
	int i,j;
	for(i=0;i<size;i++){
		for(j=0;j<size;j++){
			if(i==0)
				board[0][j]=j+'0';//Set horiziontal indices to the character of the number
			else if(j==0)
				board[i][0]=i+'0';//Set vertical indices to the character of the number
			else
				board[i][j]='#';//coordinates character set to '#'
		}
	}
	
}

void placeShips(int size,int ships[17][3]){
	/*arguments:
		int size:size of the the board(including the the board indices!)
		int ships[17][3]: the array storing the coordinate and status of the ships*/
	int row1,column1,row2,column2,i,j,coordinatesCounter;
	printf("No.|Class of ship|size|Quantity \n");
	printf("1  |Carrier      |5   |1\n");
	printf("2  |Battleship	 |4   |1\n");
	printf("3  |Crusier      |3   |1\n");
	printf("4  |Submarine	 |3   |1\n");
	printf("5  |Destroyer	 |2   |1\n");
	coordinatesCounter=0;
	printf("The two pairs of coordinates must be in the same row or column and within the size of each ship\n");
	for(j=1;j<5+1;j++){
		printf("Select row of first coordinate for ship %i\n",j);
		scanf("%i",&row1);
		printf("Select column of first coordinate for ship %i\n",j);
		scanf("%i",&column1);
		printf("Select row of second coordinate for ship %i\n",j);
		scanf("%i",&row2);
		printf("Select column of second coordinate for ship %i\n",j);
		scanf("%i",&column2);
		
		int dif_row=abs(row1-row2);
		int dif_col=abs(column1-column2);
		
		if(dif_row>dif_col){
			for(i=0;i<dif_row+1;i++){
				ships[coordinatesCounter][0]=((row1<row2) ? row1:row2)+i;//increase rows for the minimum row value
				ships[coordinatesCounter][1]=column1;//same column value
				coordinatesCounter++;		
			}
		}
		else{
			for(i=0;i<dif_col+1;i++){
				ships[coordinatesCounter][0]=row1;//same row value
				ships[coordinatesCounter][1]=((column1<column2) ?column1:column2)+i;//increase columns for the minimum column value
				coordinatesCounter++;
			}
		
		}		
	}
	coordinatesCounter=0;
	for(i=0;i<17;i++){//Set hit status to 0
		ships[i][2]=0;
	}	
}

void random_placeShips(int size,int ships[17][3]){
	/*arguments:
		int size:size of the the board(including the the board indices!)
		int ships[17][3]: the array storing the coordinate and status of the ships*/
	printf("Random placing in progress\n");
	int  not_correctPlacing, direction,not_validate_coordinates,row,column,i,j,coordinatesCounter;
	int bad_coordinates=17;//Set a value for bad_coordinates to enter the following while cycle
	//Set coordinates randomly until there are no repeated coordinates
	while(bad_coordinates!=0){
		//initialize the coordinates and the status of the ship to 0
		for(i=0;i<17;i++){
			ships[i][0]=0;
			ships[i][1]=0;
			ships[i][2]=0;
		}
		
		coordinatesCounter=0;//set the number of the coordinates set to 0
		int ships_sizes[5]={5,4,3,3,2};
		//Iterate over all the ships
		for(i=0;i<5;i++){
			not_validate_coordinates=1;//initialize this value to enter the following while cycle
			//Set random pair of coordinate until there is no repeated
			while(not_validate_coordinates==1){
				not_validate_coordinates=0;//exit while cycle condition
				row=(rand()%(size-2))+1;//random row
				column=(rand()%(size-1))+1;//random column
				for(j=0;j<17;j++){
					if(row==ships[j][0] & column==ships[j][1])//check if there are the pair of coordinates is already set
						not_validate_coordinates=1;	//keep the the cycle if there are any repeated coordinates	
				}
			}
			
			not_correctPlacing=0;//Set this value to enter the folloing while cycle
			//place the ships in random directions until it is posible 
			while(not_correctPlacing==0){
				direction=(rand()%(3));//obtain a random number from 0 to 3, up=0,right=1,down=2,left=3
				if(direction==0 & row-ships_sizes[i]>=0){//If it is going up it does not go out of range
					for(j=0;j<ships_sizes[i];j++){
						ships[coordinatesCounter][0]=row-j;//decrease row if extending to
						ships[coordinatesCounter][1]=column;//same column if extending up
						coordinatesCounter++;
					}
					not_correctPlacing=1;
				}else if(direction==1 & column+ships_sizes[i]<=size-1){//If it is going right it does not go out of range
					for(j=0;j<ships_sizes[i];j++){
						ships[coordinatesCounter][0]=row;//same row if extending right
						ships[coordinatesCounter][1]=column+j;//increase column if extending right
						coordinatesCounter++;
					}
					not_correctPlacing=2;
				}else if(direction==2 & (row+ships_sizes[i]<=size-1)){//If it is going down it does not go out of range
					for(j=0;j<ships_sizes[i];j++){
						int qqqqq=row+j;
						ships[coordinatesCounter][0]=row+j;//increase column if extending down
						ships[coordinatesCounter][1]=column;//same column if extending down
						coordinatesCounter++;
					}
					not_correctPlacing=3;
				}else if(direction==3 &  column-ships_sizes[i]>=0){//If it is going left it does not go out of range
					for(j=0;j<ships_sizes[i];j++){
						ships[coordinatesCounter][0]=row;//same row if extending to the left
						ships[coordinatesCounter][1]=column-j;//decrease column if extending left
						coordinatesCounter++;
					}
					not_correctPlacing=4;
				}
				printf("Random placing in progress\n");
			}
		}

		bad_coordinates=0;//Reset bad coordinates counter
		for(i=0;i<17;i++){
			for(j=0;j<17;j++){
				if(ships[i][0]==ships[j][0] & ships[i][1]==ships[j][1] & i!=j)
					bad_coordinates++;//increase if there are any repeated coordinates
			}
		}
	}	
}

int play(int size,char board_p1[size][size],char board_p2[size][size],int ships1[17][3],int ships2[17][3]){
	/*arguments:
		int size:size of the the board(including the the board indices!)
		char board_p1[size][size]:board of player 1
		char board_p2[size][size]: board of player 2
		int ships1[17][3]  the array storing the coordinate and status of the ships of player 1
		int ships2[17][3]: the array storing the coordinate and status of the ships of player 2*/
	int row,column,ships_sinked1,ships_sinked2,i,not_missed;
	printf("---------------------  Let the game begin!Good luck!    ---------------------\n");
	printf("              !=hit              x=missed              #=not revealed\n");
	while(1){
		printf("-----------------            Player 1                -----------------\n");
		//Player 1 asks for attack coordinates
		printf("Player 1:Insert attack coordinates(row)\n");
		scanf("%i",&row);
		printf("Player 1:Insert attack coordinates(column)\n");
		scanf("%i",&column);
		
		//Turn of player 1
		not_missed=0;
		for(i=0;i<17;i++){
			if(row==ships2[i][0] & column==ships2[i][1]){
				board_p2[row][column]='!';//A coordinate of the ship is found, set character to '!'
				ships2[i][2]=1;//Set the coordinate to hit status=1
				not_missed++;//Increase only if a coordinate is hit
			}		
		}
		if(not_missed==0){
			board_p2[row][column]='x';//Coordinate of the ship not found, set character to 'x'
		}
		printBoard(size,board_p2);//print the board of player 2, whick is attacked by player 1

		ships_sinked1=0;//Reset the counter of hit coordiantes
		for(i=0;i<17;i++){
			ships_sinked1=ships_sinked1+ships2[i][2];//Add all the hit coordinates status 
		}
		printf("Player 1 Hit %i coordinates, %i left to win\n",ships_sinked1,17-ships_sinked1);
		if(ships_sinked1==17){
			return 1;//If all coordinates are hit, the player 1 wins	
		}
		
		
		printf("-----------------            Player 2                -----------------\n");
		//Player 2 asks for attack coordinates
		printf("Player 2:Insert attack coordinates(row)\n");
		scanf("%i",&row);
		printf("Player 2:Insert attack coordinates(column)\n");
		scanf("%i",&column);
		
		not_missed=0;
		for(i=0;i<17;i++){
			if(row==ships1[i][0] & column==ships1[i][1]){
				board_p1[row][column]='!';//A coordinate of the ship is found, set character to '!'
				ships1[i][2]=1;//Set the coordinate to hit status=1
				not_missed++;//Increase only if a coordinate is hit
			}		
		}
		if(not_missed==0){
			board_p1[row][column]='x';//Coordinate of the ship not found, set character to 'x'
		}
		printBoard(size,board_p1);//print the board of player 1, whick is attacked by player 2
		

		ships_sinked2=0;
		for(i=0;i<17;i++){
			ships_sinked2=ships_sinked2+ships1[i][2];
			
		}
		printf("Player 2 Hit %i coordinates, %i left to win\n",ships_sinked2,17-ships_sinked2);
		if(ships_sinked2==17){
			return 2;//If all coordinates are hit, the player 2 wins
		}	
	}
		
}


