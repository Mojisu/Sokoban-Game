
#include "sokoban.h"

int main(){
	int command, n;

	createRankList();
	while(1){
	printf("|--------------------------------------------------|\n");
	printf("|                                                  |\n");
	printf("|--------------1. G A M E S T A R T----------------|\n");
	printf("|                                                  |\n");
	printf("|--------------2. R A N K I N G -------------------|\n");
	printf("|                                                  |\n");
	printf("|--------------3. D E M O P L A Y -----------------|\n");
	printf("|                                                  |\n");
	printf("|--------------4. H E L P -------------------------|\n");
	printf("|                                                  |\n");
	printf("|--------------0. E X I T -------------------------|\n");
	printf("|                                                  |\n");
	printf("|--------------------------------------------------|\n");
	
	scanf("%d",&n);

	if(n==1 || n==3){	
		printf("\nInput Stage(1 to 12) : ");
		scanf("%d",&stage);
		stage--;
	}

	// setting Demo Play
	if(n==3){
		n=1;
		demoPflag=1;
	}

	if(n==4){
		showHelp();
		getch();
		continue;
	}
	break;
}

	initscr();
	noecho();
	keypad(stdscr, TRUE);
	

	switch(n){
		case 1:
			//createRankList();
			srand((unsigned int)time(NULL));
				
				InitGame();
				nodelay(stdscr, TRUE);
				do{
					command = GetCommand();

						// Get Time Difference
						nowTime = time(NULL);
						deltaTime = nowTime - initTime;
							
						if (ProcessCommand(command) == QUIT){
							DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
								move(HEIGHT / 2, WIDTH / 2 - 4);
								printw("Good-bye!!");
								
								refresh();
								getch();
								endwin();
								
								return 0;
						}
					
						// Timeover condition
						if (!(stageLimitTime - deltaTime)){
							timeOver = 1;
							break;
							
						}
					
						CheckGameOver();
				} while (!gameOver);

		

			nodelay(stdscr, FALSE);
			

			DrawBox(HEIGHT / 2 - 1, WIDTH - 5, 1, 10);
			move(HEIGHT / 2, WIDTH - 4);
			if (timeOver)
				printw("Time Over!");
			else
				printw("Game Clear!");

			getch();
			if(!demoPflag){
			score = ((1000/score) + (stageLimitTime - deltaTime)) * (stage+1);
			newRank(score);
			writeRankFile();
			}
			freeall();

			refresh();
			endwin();
			system("clear");
			return 0;
				
		case 2:
				/* RANKING */
				rank();
				

		case 4:
				/* HELP */
				//showHelp();
		
		case 0:
				endwin();
				return 0;
	}
	
	
	endwin();
	
	return 0;

}

/*
 * Initialize Global Variables
 */
void InitGame(){
	int i,j;
	
	initTime = time(NULL);
	nowTime = 0;
	deltaTime = 0;
	stageLimitTime = mapDatas[stage].LimitTime;

	score=0;	
	gameOver=0;
	leftBoxes=0;
	top=-1;

	// Initialize Fields
	/* TODO
	 *
	 * You may change this part to construct multi-stage game
	 * Field's Type
	 * 0 : ROAD
	 * 1 : WALL
	 * 2 : BOX
	 * 3 : DESTINATION
	 */
	

	for(i=0;i<WIDTH;i++)
		for(j=0;j<HEIGHT;j++)
			field[j][i]=0;
	
	for(i=0;i<WIDTH;i++)
		for(j=0;j<HEIGHT;j++){
			field[j][i] = mapDatas[stage].field[j][i]-48;
			if(field[j][i]==9){
				cursor.x = i;
				cursor.y = j;
				field[j][i]=0;
			}
		}

	totalBoxes = leftBoxes = mapDatas[stage].box;

	//init stack
	push(cursor);

	DrawOutline();
	DrawField();
	PrintScore(score);
	
	return;
}


/*
 * Draw Outline
 */
void DrawOutline(){	
	int i,j;
	DrawBox(0,0,HEIGHT,WIDTH*2);

	move(0,WIDTH+19);
	printw(" LEFT");
	DrawBox(1,WIDTH+17,1,8);

	move(4,WIDTH+19);
	printw("MOVED");
	DrawBox(5,WIDTH+17,1,8);
	
	move(8, WIDTH+19);
	printw("TIME");
	DrawBox(9, WIDTH+17, 1, 8);


	move(HEIGHT+3, 0);
	printw("2013 C Programming Project 2");
	
	move(HEIGHT+4, 0);
	printw("Sokoban");

	move(HEIGHT+5, 0);
	printw("<Hotkey> R: Restart, Q: Quit");

	move(HEIGHT+6, 0);
	printw("         N: NextStage B: Back");

	move(HEIGHT+7, 0);
	printw("         Z: Recover Movement");
		
	
	


}

/*
 *
 * Get key from keyboard and return input key
 * 
 */

int GetCommand(){
	int command;
	int i,j;
	
	if(!demoPflag)
		command = wgetch(stdscr);
	else{
		getch();
		command = (int)demo[stage][demoCount]-48;
	
		demoCount++;
		//delay;
		for(i=0;i<50000000;)
			i++;
	}

	switch(command){
	case KEY_UP:
		break;
	case UP:
		break;
	case KEY_DOWN:
		break;
	case DOWN:
		break;
	case KEY_LEFT:
		break;
	case LEFT:
		break;
	case KEY_RIGHT:
		break;
	case RIGHT:
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	case 'r':
		InitGame();
		break;
	case 'n':
		command = NEXT;
		break;
	case 'b':
		command = BACK;
		break;
	case 'z':
		command = PREV;
		break;

	default:
		command = NOTHING;
		break;
	}
	return command;
}

/*
 *
 * Do something with respect to input key
 * return QUIT if input is q, Otherwise, 1
 *
 * TODO : 
 *   Do something for some keys, 'r' or 'n'.. etc
 *   If command is 'n', then go to next stage, and
 *   If command is 'r', then restart this stage, ..
 *   ...
 */
int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	int dr=0;

	if (command == QUIT)
		ret = QUIT;
	else if (command == KEY_UP || command == UP){
		CheckToMove(cursor.x,cursor.y, UP);
	}else if (command == KEY_DOWN || command == DOWN){
		CheckToMove(cursor.x,cursor.y, DOWN);
	}else if (command == KEY_LEFT || command == LEFT){
		CheckToMove(cursor.x,cursor.y, LEFT);
	}else if (command == KEY_RIGHT || command == RIGHT){
		CheckToMove(cursor.x,cursor.y, RIGHT);
	}else if (command == PREV){
		if(top!=0){
			pop();
			cursor.x=Stack[top].c.x;
			cursor.y=Stack[top].c.y;
			score--;
		}	
	}else if (command == NEXT){
		if(stage<11){
			stage++;
			InitGame();
		}
	}else if (command == BACK){
		if(stage>0){
			stage--;
			InitGame();
		}
	}



	DrawField();
	PrintScore(score);

	return ret;
}


/*
 *
 * Check whether the user can move to some direction or not.
 * For example, if the user wants to move to up, then check
 * if (cursor.x, cursor.y - 1) is a WALL or something else
 * that blocks the user's movement.
 *
 * TODO :
 *   Move the cursor if possible
 *   Consider these cases, when user wants to move to ->,
 *
 *   1. (User)->(Box)(Box)
 *   2. (User)->(Wall)
 *   3. (User)->(Box)(Wall)
 *   4. Something else...
 *
 *   For example, if current state is like this,
 *
 *       [User](Box)(Road)(Wall)
 *
 *   Then the next state should be the following,
 *   If the current mode is RIGHT,
 *
 *       (Road)[User](Box)(Wall)
 *
 *   You must check the boundary case to prevent the
 *   segmentation fault.
 */

void CheckToMove(int x, int y, int mode){

	

	switch(mode){
	case UP:
		if(field[y-1][x]==WALL || ((field[y-1][x]==BOX || field[y-1][x]==DSTNBOX) && (field[y-2][x]==WALL || field[y-2][x]==BOX || field[y-2][x]==DSTNBOX)))	//conditions that the cursor can't move
			break;
		//conditions that the cursor would move(below)
		if(field[y-1][x]==BOX){
			field[y-1][x]=WAY;
			if(field[y-2][x]==WAY)
				field[y-2][x]=BOX;
			else if(field[y-2][x]==DESTINATION)
				field[y-2][x]=DSTNBOX;
		}
		else if(field[y-1][x]==DSTNBOX){
			field[y-1][x]=DESTINATION;	
			if(field[y-2][x]==WAY)
                field[y-2][x]=BOX;
            else if(field[y-2][x]==DESTINATION)
				field[y-2][x]=DSTNBOX;
		}
		cursor.y--;
		score++;
		push(cursor);
		break;

	case DOWN: 
		if(field[y+1][x]==WALL || ((field[y+1][x]==BOX || field[y+1][x]==DSTNBOX) && (field[y+2][x]==WALL || field[y+2][x]==BOX || field[y+2][x]==DSTNBOX)))
			break;
		//conditions that the cursor would move(below)
		if(field[y+1][x]==BOX){
			field[y+1][x]=WAY;
	        if(field[y+2][x]==WAY)
				field[y+2][x]=BOX;
		    else if(field[y+2][x]==DESTINATION)
		        field[y+2][x]=DSTNBOX;
		}
		else if(field[y+1][x]==DSTNBOX){
			field[y+1][x]=DESTINATION;
		    if(field[y+2][x]==WAY)
			    field[y+2][x]=BOX;
			else if(field[y+2][x]==DESTINATION)
			    field[y+2][x]=DSTNBOX;
		}
		cursor.y++;
		score++;
		push(cursor);
		break;

	case LEFT: 
		if(field[y][x-1]==WALL || ((field[y][x-1]==BOX || field[y][x-1]==DSTNBOX) && (field[y][x-2]==WALL || field[y][x-2]==BOX || field[y][x-2]==DSTNBOX)))
			break;
        //conditions that the cursor would move(below)
		if(field[y][x-1]==BOX){
	            field[y][x-1]=WAY;
		        if(field[y][x-2]==WAY)
                	field[y][x-2]=BOX;
				else if(field[y][x-2]==DESTINATION)
					field[y][x-2]=DSTNBOX;
		}
		else if(field[y][x-1]==DSTNBOX){
			field[y][x-1]=DESTINATION;
			if(field[y][x-2]==WAY)
				field[y][x-2]=BOX;
			else if(field[y][x-2]==DESTINATION)
				field[y][x-2]=DSTNBOX;
		}
		cursor.x--;
		score++;
		push(cursor);
		break;
	case RIGHT:
		if(field[y][x+1]==WALL || ((field[y][x+1]==BOX || field[y][x+1]==DSTNBOX) && (field[y][x+2]==WALL || field[y][x+2]==BOX || field[y][x+2]==DSTNBOX)))
			break;
		//conditions that the cursor would move(below)
		if(field[y][x+1]==BOX){
			field[y][x+1]=WAY;
			if(field[y][x+2]==WAY)
				field[y][x+2]=BOX;
			else if(field[y][x+2]==DESTINATION)
				field[y][x+2]=DSTNBOX;
		}
		else if(field[y][x+1]==DSTNBOX){
			field[y][x+1]=DESTINATION;
			if(field[y][x+2]==WAY)
				field[y][x+2]=BOX;
			else if(field[y][x+2]==DESTINATION)
				field[y][x+2]=DSTNBOX;
		}
		cursor.x++;
		score++;
		push(cursor);
		break;
	}
}

void Color(int a){
	start_color();

	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_GREEN);
	init_pair(3, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(4, COLOR_RED, COLOR_BLACK);
	init_pair(5, COLOR_RED, COLOR_YELLOW);
	init_pair(6, COLOR_YELLOW, COLOR_RED);

	switch(a){
	case 0:
		attron(COLOR_PAIR(1));
		printw("  ");
		attroff(COLOR_PAIR(1));
		break;
	case 1:
		attron(COLOR_PAIR(2));
		printw("  ");
		attroff(COLOR_PAIR(2));
		break;
	case 2:
		attron(COLOR_PAIR(3));
		printw("  ");
		attroff(COLOR_PAIR(3));
		break;
	case 3:
		attron(COLOR_PAIR(4));
		printw(" .");
		attroff(COLOR_PAIR(4));
		break;
	case 4:
		attron(COLOR_PAIR(5));
		printw(" .");
		attroff(COLOR_PAIR(5));
		break;
	case 5:
		attron(COLOR_PAIR(6));
		printw("  ");
		attroff(COLOR_PAIR(6));
		break;



	}

	return;
}

void DrawField(){
	int i,j;

	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			Color(field[j][i]);
		}
	}
	move(cursor.y+1, (cursor.x*2)+1);
	Color(5);

}


/*
 *
 * Check whether the game is over or not.
 * The game is over if user has moved all
 * boxes to destination, or something else
 * such as the time is limited.
 *
 */

void CheckGameOver(){
	int i,j;
	int count = 0;

	for(j=0;j<HEIGHT;j++){
		for(i=0;i<WIDTH;i++){
			if(field[j][i] == 4){
				count++;
			}
		}
	}
	if(count== totalBoxes)
		gameOver = 1;
	leftBoxes = count;

	/*
	 * TODO :
	 *
	 * If Time is over,
	 * Then do something for finishing the game
	 * as timeover.
	 */
	return;
}

void PrintScore(int score){
	move(2,WIDTH+18); 
	printw("%8d",totalBoxes-leftBoxes);

	move(6,WIDTH+18); 
	printw("%8d",score);

	move(10,WIDTH+18);
	printw("%8d", stageLimitTime - deltaTime);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void push(struct cursor_ c){
	int i, j;
	top++;

	for(i=0;i<HEIGHT;i++)
		for(j=0;j<WIDTH;j++)
			Stack[top].field[i][j] = field[i][j];
	Stack[top].c.x = c.x;
	Stack[top].c.y = c.y;

	return;
}

struct cursor_ pop(){
	int i, j;
	struct cursor_ c;
	top--;
	for(i=0;i<HEIGHT;i++)
		for(j=0;j<WIDTH;j++)
			field[i][j]=Stack[top].field[i][j];

	return;
}

void showHelp(){

	endwin();

	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n------------------4. HELP --------------------------\n");
	printf("< SOKOBAN GAME > \n\n");
	printf("-RULES-\n");
	printf("1. Get the boxes to the red dot.\n");
	printf("2. There exists limited time.\n");
	printf("3. There is no limted movement.\n");
	printf("4. Press 'N' to skip stage.\n");
	printf("5. Press 'B' to return to back stage.\n");
	printf("6. Press 'R' to initialize the game.\n");
	printf("7. If you succeed each step the rank goes up.\n\n");
	printf("	PROJECT BY : Mojisu \n");
	printf("12/15/2013\n\n");
	printf("ENJOY THE GAME AND FINISH THE EVERY STAGE'\n");
	printf("-----------------------------------------------------\n\n\n\n\n\n\n\n\n\n\n");

	return;
}

void createRankList(){

	FILE* fp;
	int i;
	int data;
	char temp[1+NAMELEN];
	fp = fopen("rank.txt","r");

	if(fp == NULL )
		printw("file open error !\n");
	else
	{
		fscanf(fp,"%d",&num_per);
	
		if(!num_per)
		{
			head = ptr = t = NULL;
			return ;
		}

		fscanf(fp,"%s",temp);
		fscanf(fp,"%d",&data);
		t = (Node*)malloc(sizeof(Node));
		t->score = data;
		strcpy(t->name,temp);
		t->next = NULL;
		head = t;
		ptr = head;

		for(i=1;i<num_per;i++)
		{
			fscanf(fp,"%s",temp);
			fscanf(fp,"%d",&data);
			t = (Node*)malloc(sizeof(Node));
			t->score = data;
			strcpy(t->name,temp);
			t->next = NULL;
			pre = head;

			for(ptr = head;ptr;pre = ptr,ptr = ptr->next)
			{
				if( (ptr->score < t->score) && ptr == head)
				{
					t->next = ptr;
					head = t;
					break;
				}
				else if(ptr->next == NULL && ptr->score > t->score)
				{
					ptr->next = t;
					break;
				}
				else if(pre->score > t->score && ptr->score < t->score)
				{
					t->next = pre->next;
					pre->next = t;
					break;
				}
			}	
		}
	}
	fclose(fp);
}

void rank(){

	int x=0,y=0,sel,i,j;
	clear();
	ptr = head;

	printw("           name          |    score     \n");
	printw("--------------------------------------- \n");
		
	for(ptr=head;ptr;ptr=ptr->next){
		printw("%s",ptr->name);
		for(i=25;i>strlen(ptr->name);i--)
			printw(" ");
		printw("|");
		printw("   %5d",ptr->score);
		printw("\n");
	}
	ptr=head;
	
	sel=wgetch(stdscr);
}

void writeRankFile(){

	FILE *fp;
	int i,cnt=0;
	
	for(ptr = head;ptr;ptr = ptr->next)
		cnt++;

	num_per = cnt;
	fp = fopen("rank.txt","w");

	fprintf(fp,"%d\n",num_per);
	for(ptr = head;ptr;ptr = ptr->next)
		fprintf(fp,"%s %d\n",ptr->name,ptr->score);
	
	fclose(fp);
}

void newRank(int score){
	char str[1+NAMELEN];
	clear();
	echo();
	printw("your name: ");
	scanw("%s",str);
	noecho();

	t = (Node*)malloc(sizeof(Node));
	strcpy(t->name,str);
	t->score = score;

	if(!head)
	{
		head = t;
		head->next = NULL;
	}
	else
		for(ptr = head;ptr;pre = ptr,ptr = ptr->next)
		{
			if( (ptr->score < t->score) && ptr == head)
			{
				t->next = ptr;
				head = t;
				break;
			}
			else if(ptr->next == NULL && ptr->score > t->score)
			{
				ptr->next = t;
				break;
			}
			else if(pre->score > t->score && ptr->score < t->score)
			{	
				t->next = pre->next;
				pre->next = t;
				break;
			}
		}
		
}

void freeall()
{
	Node* temp;
	ptr = head;

	while(ptr)
	{
		temp=ptr;
		ptr = ptr->next;
		free(temp);
	}

}
