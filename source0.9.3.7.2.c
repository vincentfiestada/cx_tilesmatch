/*********************************************
ASCII Match Version 0.9.3
Developed by Vincent Paul Fiestada
CS11 WFWX2 MP1
*********************************************/

#define X_DIM 6
#define Y_DIM 6
#define CHARMIN 35
#define CHARMAX 38
#define MINMATCHES 2				//Must be the 1 less than the minimum number of matches needed
//#define WINDOWS						//Define when compiling on Windows
//#define DEBUG						//Define when Debugging

#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<time.h>
#ifdef WINDOWS
#include<windows.h>
#endif

typedef struct{						//A type to store coordinates of tiles
	int x;
	int y;
}gridcoord;

void populate(char grid[Y_DIM][X_DIM]);
int randomTile(int lowerLim, int upperLim);
void clearscreen();
int game();
void info();
void drawGrid(char grid[Y_DIM][X_DIM],unsigned long int score);
void colorize(char printme,int key);
int swapTile(char grid[Y_DIM][X_DIM],gridcoord tile,char direction);
int isValid(gridcoord move,char direction);
void assasinate(char grid[Y_DIM][X_DIM],gridcoord targets[Y_DIM*X_DIM],int numtargs);
void fillGaps(char grid[Y_DIM][X_DIM]);
void copymatrix2d(char from[Y_DIM][X_DIM], char to[Y_DIM][X_DIM]);
int checkmatches_lengthwise(char grid[Y_DIM][X_DIM],gridcoord lastmoved,gridcoord list[Y_DIM*X_DIM],int *insertat);
int checkmatches_crosswise(char grid[Y_DIM][X_DIM],gridcoord lastmoved,gridcoord list[Y_DIM*X_DIM],int *insertat);
int popmatches_universal(char grid[Y_DIM][X_DIM]);
int trymatches(char grid[Y_DIM][X_DIM]);
int areThereMatches(char grid[Y_DIM][X_DIM],gridcoord tile);
void primitSwapTiles(char* from, char* to);

int main(){
	char prompt_resp;
	unsigned long int finalscore = 0;

	//Display the Welcome Screen
	clearscreen();
	printf("\n\n\t");printf(" ");
	colorize('A',1);printf(" ");
	colorize('S',2);printf(" ");
	colorize('C',3);printf(" ");
	colorize('I',4);printf(" ");
	colorize('I',1);printf("    ");
	colorize('M',2);printf(" ");
	colorize('A',3);printf(" ");
	colorize('T',4);printf(" ");
	colorize('C',1);printf(" ");
	colorize('H',2);printf(" ");
	printf("\n\n");
#ifdef DEBUG
	printf("\tRUNNING IN DEBUGGING MODE\n");
#endif
	printf("\n\tPress 1 to PLAY");
	printf("\n\tPress 2 for HELP");
	printf("\n\tPress any other key to exit");
	printf("\n");
	scanf("%c",&prompt_resp);
	switch(prompt_resp){
	case '1':
		finalscore = game();
		clearscreen();
		printf("\n\n\tGAME OVER!");
		printf("\n\tFINAL SCORE: %ld",finalscore);
		printf("\n\n\t");
		//Show the rank based on the score obtained
		if(finalscore>=11000){
			printf("DUDE! Ran out of things to say here!");
			printf("\n\tRANK: Addicted Nerd");
		}
		else if(finalscore>=10000){
			printf("OH MY GLOB! You are Simply Trigonometric!");
			printf("\n\tRANK: Grandmaster Matchmaker");
		}
		else if(finalscore>=9000){
			printf("WOW! That was Analytic!");
			printf("\n\tRANK: El TILEbre");
		}
		else if(finalscore>=8000){
			printf("BOOM! Mathed that one!");
			printf("\n\tRANK: ASCII Enemy # 1");			
		}
		else if(finalscore>=7000){
			printf("AMAZING! You pwned those tiles!");
			printf("\n\tRANK: MasterChief Tilebusting Officer");			
		}
		else if(finalscore>=6000){
			printf("AWESOME! Like Vincent!");
			printf("\n\tRANK: ASCII Mage");			
		}
		else if(finalscore>=5000){
			printf("ASTONISHING! That's you!");
			printf("\n\tRANK: Terminal Hotshot");			
		}
		else if(finalscore>=4000){
			printf("TERRIFIC! Cartesian!");
			printf("\n\tRANK: Professional Hunter of Tiles");			
		}
		else if(finalscore>=3000){
			printf("GREAT! That's Algebraic!");
			printf("\n\tRANK: Triple Threat Tile Destroyer");			
		}
		else if(finalscore>=2000){
			printf("KEEP IT UP! I am speechless!");
			printf("\n\tRANK: Isko the Magnificent");			
		}
		else if(finalscore>=1000){
			printf("YEY! Good job, buddy!");
			printf("\n\tRANK: Novice");			
		}
		else if(finalscore>=800){
			printf("YOU CAN DO BETTER! Keep working on those ASCII muscles!");
			printf("\n\tRANK: Amateur");			
		}
		else if(finalscore>=400){
			printf("AWE COME ON! Even Justin Beiber sings better than that!");
			printf("\n\tRANK: Guy-Who's-Not-Really-Into-It");			
		}
		else if(finalscore>=200){
			printf("YOU'll GET THERE! With a little work, you can be Batman.");
			printf("\n\tRANK: Cinnamon Bun");			
		}
		else{
			printf("TOO BAD! You have potential, probably. I think. Most likely not.");
			printf("\n\tRANK: The guy who directed the Percy Jackson movies");			
		}
		printf("\n\n\tThank you for playing.\n\n");
		break;
	case '2':
		info();
		printf("\n\n\tThis program will now exit.\n\n");
		break;
	default:
		return 0;
	}
}

void populate(char grid[Y_DIM][X_DIM]){
	int i=0;
	int k=0;

	srand(time(NULL));
	for(k=0;k<Y_DIM;k++){
		for(i=0;i<X_DIM;i++){
			grid[k][i] = randomTile(CHARMIN,CHARMAX);
		}
	}
}

int randomTile(int lowerLim, int upperLim){
	return rand() % (upperLim - lowerLim + 1) + lowerLim;
}

void clearscreen(){
	if (system("cls")){
		system("clear");
	}
}

void drawGrid(char grid[Y_DIM][X_DIM],unsigned long int score){
	int i=0;
	int k=0;
	int key=1;
	
#ifndef DEBUG
	clearscreen();
#endif
	printf("\n\n\t      ASCII MATCH");
	printf("\n\n\tSCORE: %ld",score);
	printf("\n\n\t   ");
	for(i=0;i<X_DIM;i++){
		printf("%2d ",i+1);								//Print the column names
	}
	printf("\n\t   ");
	for(i=0;i<X_DIM;i++){
		printf("---");
	}
	printf("\n");
	for(k=0;k<Y_DIM;k++){
		printf("\t%2d|",k+1);							//Print the row names
		for(i=0;i<X_DIM;i++){
			printf(" ");
			switch((int)grid[k][i]){					//Choose the color key
			case CHARMIN:
				key=1;
				break;
			case CHARMIN+1:
					key=2;
					break;
			case CHARMIN+2:
					key=3;
					break;
			case CHARMIN+3:
					key=4;
					break;
			}
			colorize(grid[k][i],key);
			printf(" ");
		}
		printf("\n\n");
	}
}

void colorize(char printme,int key){
	#ifdef WINDOWS												//Prints a char in color if WINDOWS is defined
	HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
	WORD color;
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	GetConsoleScreenBufferInfo(h, &csbiInfo);
	color = csbiInfo.wAttributes;
	switch(key){
		case 1:
		SetConsoleTextAttribute ( h, FOREGROUND_BLUE | FOREGROUND_INTENSITY );
		break;
		case 2:
		SetConsoleTextAttribute ( h, FOREGROUND_GREEN | 10 );
		break;
		case 3:
		SetConsoleTextAttribute ( h, FOREGROUND_RED | 12 );
		break;
		default:
		SetConsoleTextAttribute ( h, FOREGROUND_BLUE | 11 );
	}
	#endif
	printf("%c",printme);
	#ifdef WINDOWS
	SetConsoleTextAttribute ( h, color);
	#endif
}

void copymatrix2d(char from[Y_DIM][X_DIM], char to[Y_DIM][X_DIM]){
	int i=0;
	int k=0;
	for(k=0;k<Y_DIM;k++){
		for(i=0;i<X_DIM;i++){
			to[k][i] = from[k][i];
		}
	}
}

int swapTile(char grid[Y_DIM][X_DIM],gridcoord tile,char direction){
	gridcoord partnertile;
	char temp;
	partnertile.x = tile.x;
	partnertile.y = tile.y;
	unsigned long int score = 0;
	gridcoord lmatches[Y_DIM*X_DIM];
	gridcoord cmatches[Y_DIM*X_DIM];
	int linsertat = 0;
	int cinsertat = 0;
	int lengtmt = 0;
	int crossmt = 0;
	char prompt_resp;

	switch(direction){
	case 'w':
	case 'W':
		partnertile.y = tile.y-1;
		break;
	case 's':
	case 'S':
		partnertile.y = tile.y+1;
		break;
	case 'a':
	case 'A':
		partnertile.x = tile.x-1;
		break;
	case 'd':
	case 'D':
		partnertile.x = tile.x+1;
		break;
	default:
		printf("\n:( ERROR. Unknown command.");
	}
	//Swap tiles (performed on copy of grid)
	temp = grid[tile.y][tile.x];
	grid[tile.y][tile.x] = grid[partnertile.y][partnertile.x];
	grid[partnertile.y][partnertile.x] = temp;
	//Check for lengthwise and crosswise matches
	lengtmt = checkmatches_lengthwise(grid,partnertile,lmatches,&linsertat);
	crossmt = checkmatches_crosswise(grid,partnertile,cmatches,&cinsertat);
	//If number of adjacent tiles that are matches meet the MINMATCHES requirement, update the score
	//and update the original grid
	if(lengtmt>=MINMATCHES||crossmt>=MINMATCHES){
		if(lengtmt>=MINMATCHES){
			score+=lengtmt;
			assasinate(grid,lmatches,lengtmt);			//remove the matching tiles that were found
		}
		if(crossmt>=MINMATCHES){
			score+=crossmt;
			assasinate(grid,cmatches,crossmt);
		}
		grid[partnertile.y][partnertile.x] = '\0';		//remove the tile that was moved itself
		score+=1;
	}
	else{
	//Else, check the other tile that was swapped.
		linsertat = 0;
		cinsertat = 0;
		lengtmt = checkmatches_lengthwise(grid,tile,lmatches,&linsertat);
		crossmt = checkmatches_crosswise(grid,tile,cmatches,&cinsertat);
		if(lengtmt>=MINMATCHES||crossmt>=MINMATCHES){
			if(lengtmt>=MINMATCHES){
				score+=lengtmt;
				assasinate(grid,lmatches,lengtmt);			
			}
			if(crossmt>=MINMATCHES){
				score+=crossmt;
				assasinate(grid,cmatches,crossmt);
			}
			grid[tile.y][tile.x] = '\0';	
			score+=1;
		}
		else{
	//Return tiles to previous positions
			temp = grid[partnertile.y][partnertile.x];
			grid[partnertile.y][partnertile.x] = grid[tile.y][tile.x];
			grid[tile.y][tile.x] = temp;
		}
	}
	
	return score;
}

int popmatches_universal(char grid[Y_DIM][X_DIM]){
	int i=0;
	int k=0;
	int lengtmt = 0;
	int crossmt = 0;
	char gridcopy[Y_DIM][X_DIM];
	gridcoord lmatches[Y_DIM*X_DIM];
	gridcoord cmatches[Y_DIM*X_DIM];
	gridcoord tile;
	int didchangel = 0;
	int didchangec = 0;
	int tsambapoints = 0;
	int linsertat = 0;
	int cinsertat = 0;
	
	copymatrix2d(grid,gridcopy);
	for(k=0;k<Y_DIM;k++){
		for(i=0;i<X_DIM;i++){
			tile.x = i;
			tile.y = k;
			lengtmt = checkmatches_lengthwise(gridcopy,tile,lmatches,&linsertat);
			crossmt = checkmatches_crosswise(gridcopy,tile,cmatches,&cinsertat);
			if(lengtmt>=MINMATCHES||crossmt>=MINMATCHES){
				if(lengtmt>=MINMATCHES){
					didchangel = 1;
					tsambapoints+=lengtmt;
				}
				if(crossmt>=MINMATCHES){
					didchangec = 1;
					tsambapoints+=crossmt;
				}
				grid[tile.y][tile.x] = '\0';		//remove the tile that was moved itself
				tsambapoints+=1;
			}
		}
	}
	
	if(didchangel){
		assasinate(gridcopy,lmatches,linsertat);			//remove the matching tiles that were found
		copymatrix2d(gridcopy,grid);
	}
	if(didchangec){
		assasinate(gridcopy,cmatches,cinsertat);			//remove the matching tiles that were found
		copymatrix2d(gridcopy,grid);
	}
	fillGaps(grid);
	if(didchangec==1||didchangel==1){
		return tsambapoints += popmatches_universal(grid);
	}
	return tsambapoints;
}

int checkmatches_lengthwise(char grid[Y_DIM][X_DIM],gridcoord lastmoved,gridcoord list[Y_DIM*X_DIM],int *insertat){
	int i=0;
	int matches=0;
	
	//Check for matches lengthwise
	i=lastmoved.y-1;
	while(i>=0){
		if(grid[lastmoved.y][lastmoved.x]==grid[i][lastmoved.x]){
			matches+=1;
			list[*insertat].x=lastmoved.x;	//Add coordinates into list of matches to (maybe) remove
			list[*insertat].y=i;
			*insertat+=1;
		}
		else break; 						//Break so that non-touching matches are not counted
		i--;
	}
	
	i=lastmoved.y+1;
	while(i<Y_DIM){
		if(grid[lastmoved.y][lastmoved.x]==grid[i][lastmoved.x]){
			matches+=1;
			list[*insertat].x=lastmoved.x;
			list[*insertat].y=i;
			*insertat+=1;
		}
		else break;
		i++;
	}

	return matches;
}

int checkmatches_crosswise(char grid[Y_DIM][X_DIM],gridcoord lastmoved,gridcoord list[Y_DIM*X_DIM],int *insertat){
	int i=0;
	int matches=0;
	
	//Check for matches crosswise; process is similar to checkmatches_lengthwise(...)
	i = lastmoved.x-1;
	while(i>=0){									
		if(grid[lastmoved.y][lastmoved.x]==grid[lastmoved.y][i]){
			matches+=1;
			list[*insertat].x=i;
			list[*insertat].y=lastmoved.y;
			*insertat+=1;
		}
		else break;
		i--;
	}
	
	i=lastmoved.x+1;
	while(i<X_DIM){
		if(grid[lastmoved.y][lastmoved.x]==grid[lastmoved.y][i]){
			matches+=1;
			list[*insertat].x=i;
			list[*insertat].y=lastmoved.y;
			*insertat+=1;
		}
		else break;
		i++;
	}

	return matches;
}

int isValid(gridcoord move,char direction){
	//0 0 0 is the command to quit.
	if(move.x==-1&&move.y==-1&&direction=='0'){
		return 1;
	}
	//Leftmost tiles cannot be moved to the left
	if(move.x==0&&(direction=='a'||direction=='A')){
		printf("\tYou can't move that to the left.\n");
		return 0;
	}
	//Rightmost tiles cannot be moved to the right
	if(move.x==X_DIM-1&&(direction=='d'||direction=='D')){
		printf("\tYou can't move that to the right.\n");
		return 0;
	}
	//Topmost tiles cannot be moved up
	if(move.y==0&&(direction=='w'||direction=='W')){
		printf("\tYou can't move that up.\n");
		return 0;
	}
	//Bottom-most grid cannot be moved down
	if(move.y==Y_DIM-1&&(direction=='s'||direction=='S')){
		printf("\tYou can't move that down.\n");
		return 0;
	}
	//Coordinates must be valid
	if(move.y<0||move.x<0||move.y>=Y_DIM||move.x>=X_DIM){
		printf("\tI can't find those coordinates. Terminals are dumb, huh?\n");
		return 0;
	}
	//Direction must be valid
	if(direction!='a'&&direction!='A'&&direction!='s'&&direction!='S'&&direction!='d'&&direction!='D'&&direction!='w'&&direction!='W'){
		printf("\tWrong Directions. Enter W for UP, A for LEFT, S for DOWN, and D for RIGHT.\n");
		return 0;
	}
	return 1;
}

void assasinate(char grid[Y_DIM][X_DIM],gridcoord targets[Y_DIM*X_DIM],int numtargs){
	int i=0;
	
	//Change all grid members in the gridcoord list to NULL
	for(i=0;i<numtargs;i++){
		grid[targets[i].y][targets[i].x] = '\0';
	}
}

void fillGaps(char grid[Y_DIM][X_DIM]){
	int i=0;
	int k=0;
	int didchange=0;
	
	for(k=Y_DIM-1;k>=0;k--){									//Start at the bottom row and work up to row 0
		for(i=0;i<X_DIM;i++){
			if(grid[k][i] == '\0'){
				if(k==0){
					grid[k][i] = randomTile(CHARMIN,CHARMAX); 	//If at topmost row, assign a random element
				}
				else{
					didchange = 1;
					grid[k][i] = grid[k-1][i];					//Otherwise, make the element above it "fall down" by swapping
					grid[k-1][i] = '\0';						
				}
			}
		}
	}
	if(didchange){
		fillGaps(grid);
	}
}

int game(){
	char grid[Y_DIM][X_DIM];
	gridcoord target;
	unsigned long int gamescore = 0;
	char direction;																		//'w' - up, 's' - down, 'a' - left, 'd' - right

	do{
		populate(grid);																	//If the grid has no POSSIBLE matches in the first place,
		popmatches_universal(grid);													//repopulate it until there are POSSIBLE matches
	}while(!trymatches(grid));
	do{
		drawGrid(grid,gamescore);
		do{
			printf("\n\tMOVE: ");
			scanf("%d %d %c",&target.x, &target.y, &direction);
			target.x-=1;																//Decrement to get the real tile index
			target.y-=1;
			if(target.x==-1&&target.y==-1&&direction=='0'){
				return gamescore;
			}
		}while(!isValid(target,direction));												//Keep asking until input is valid
		gamescore+=swapTile(grid,target,direction);
		gamescore += popmatches_universal(grid);
	}while((target.x!=-1&&target.y!=-1&&direction!='0')&&(trymatches(grid)==1));
	return gamescore;
}

void info(){
	clearscreen();
	printf("\n\tCX ASCII MATCH");
	printf("\n\tVersion 0.9.3.7.2 Alpha (Ubuntu)");
	printf("\n\tBuild Date: 8/26/2013");
	printf("\n\tby Vincent Paul Fiestada");
	printf("\n\n\tFollow me on Tumblr: vincentofearth.tumblr.com");
	printf("\n\tEmail me: vincentfiestada@hotmail.com");
	printf("\n\n\tINSTRUCTIONS:"
			"\n\tYour goal is to destroy as many tiles as possible "
			"\n\tby swapping them with adjacent tiles to create rows"
			"\n\tor columns of %d or more identical tiles. Enter your"
			"\n\tmove in this format:\n\t"
			"X Y DIRECTION\n\tW for Up, S for Down, D for Right, A for Left."
			"\n\n\tEnter 0 0 0 to quit a game in progress.",MINMATCHES+1);
}

int trymatches(char grid[Y_DIM][X_DIM]){
	//Check if there are still valid swaps
	//by trying all valid moves on each tile
	int i=0;
	int k=0;
	gridcoord tile;
	char prompt_resp;

	for(k=0;k<Y_DIM;k++){
		for(i=0;i<X_DIM;i++){
			tile.x = i;
			tile.y = k;
			if(isValid(tile,'w')){
				primitSwapTiles(&grid[tile.y][tile.x],&grid[tile.y-1][tile.x]);
				if(areThereMatches(grid,tile)){
					primitSwapTiles(&grid[tile.y-1][tile.x],&grid[tile.y][tile.x]);	//Swap temporarily
					return 1;
				}
				primitSwapTiles(&grid[tile.y-1][tile.x],&grid[tile.y][tile.x]);	//Return to location
			}
			if(isValid(tile,'s')){
				primitSwapTiles(&grid[tile.y][tile.x],&grid[tile.y+1][tile.x]);
				if(areThereMatches(grid,tile)){
					primitSwapTiles(&grid[tile.y+1][tile.x],&grid[tile.y][tile.x]);
					return 1;
				}
				primitSwapTiles(&grid[tile.y+1][tile.x],&grid[tile.y][tile.x]);
			}
			if(isValid(tile,'a')){
				primitSwapTiles(&grid[tile.y][tile.x],&grid[tile.y][tile.x-1]);
				if(areThereMatches(grid,tile)){
					primitSwapTiles(&grid[tile.y][tile.x-1],&grid[tile.y][tile.x]);
					return 1;
				}
				primitSwapTiles(&grid[tile.y][tile.x-1],&grid[tile.y][tile.x]);
			}
			if(isValid(tile,'d')){
				primitSwapTiles(&grid[tile.y][tile.x],&grid[tile.y][tile.x+1]);
				if(areThereMatches(grid,tile)){
					primitSwapTiles(&grid[tile.y][tile.x+1],&grid[tile.y][tile.x]);
					return 1;
				}
				primitSwapTiles(&grid[tile.y][tile.x+1],&grid[tile.y][tile.x]);
			}
		}
	}
	return 0;
}

int areThereMatches(char grid[Y_DIM][X_DIM],gridcoord tile){
	gridcoord dummy[Y_DIM*X_DIM];						//This isn't really needed since we just want to know if there /are/ matches
														//But is a required function argument
	int dummyinsertat = 0;
	
	if(checkmatches_lengthwise(grid,tile,dummy,&dummyinsertat)>=MINMATCHES){
		return 1;
	}
	if(checkmatches_crosswise(grid,tile,dummy,&dummyinsertat)>=MINMATCHES){
		return 1;
	}
	return 0;
}

void primitSwapTiles(char* from, char* to){
	char temp;
	temp = *to;
	*to = *from;
	*from = temp;
}
