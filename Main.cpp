#include <iostream>
#include <string.h>
#include <fstream>
#include <cstdlib>
using namespace std; 

struct log {
	int filled;
	bool present[10];
	bool used[10];
};

struct entry {
	int value, tried, totalPos;
	int solution[9];
	int x, y;
	bool original;
};

struct box {
	int y, x;
	entry Entry[9];
};

log boxLog[9], rowLog[9], columnLog[9];
entry entries[9][9];
box boxes[9];
bool feedBack;
int topC = 0, topR = 0;


void setUpBoxes(){
	 // starting from 1st Box (top left of board)
	boxes[0].y = 0;
	boxes[1].y = 0;	
	boxes[2].y = 0;		
	boxes[3].y = 3;	
	boxes[4].y = 3;	
	boxes[5].y = 3;	
	boxes[6].y = 6;	
	boxes[7].y = 6;	
	boxes[8].y = 6;	
	boxes[0].x = 0;
	boxes[1].x = 3;	
	boxes[2].x = 6;
	boxes[3].x = 0;
	boxes[4].x = 3;
	boxes[5].x = 6;
	boxes[6].x = 0;
	boxes[7].x = 3;
	boxes[8].x = 6;
}


void readBoard(string BoardName){
	char ent;
	int i=0, j=0;
	ifstream BoardFile (("Sudoku Boards/"+BoardName).c_str());
	while (!BoardFile.eof()){
		BoardFile.get(ent);
		if (ent-'0' != -38){
			entries[i][j].value = ent - '0';
			if ((ent - '0')>0) entries[i][j].original = true;
			if (i==8){
				i=0;
				++j;
			}
			else ++i;
		}
	}
	
}

int findBox(int X, int Y){
	int correctBox;
	for (int i=0; i<9; ++i){
		for (int j=boxes[i].x; j<(boxes[i].x+3); ++j){
			if (j==X){
				for (int k=boxes[i].y; k<(boxes[i].y+3); ++k){
					if (k==Y)
						correctBox = i;
				}
			}
		}
	}
	return correctBox;
}

void logBoard(){
	cout<<"--logging--"<<endl;
	bool possible[10] = {true, true, true, true, true, true, true, true, true, true};
	int i, j, p;
	for (int c=0; c<9; ++c){
		boxLog[c].filled = 0;
		rowLog[c].filled = 0;
		columnLog[c].filled = 0;
		for (int k=1; k<11; ++k){
			rowLog[c].present[k] = false;
			columnLog[c].present[k] = false;
			boxLog[c].present[k] = false;
		}
					
	}
	for (j=0; j<9; ++j){
		for (i=0; i<9; ++i){		
			entries[i][j].x = i;
			entries[i][j].y = j;
			int b = findBox(i, j);	
			if (entries[i][j].value > 0){
				++boxLog[b].filled;
				++rowLog[j].filled;
				++columnLog[i].filled;
				for (int k=1; k<11; ++k){
					 if (entries[i][j].value == k){
						rowLog[j].present[k] = true;
						columnLog[i].present[k] = true;
						boxLog[b].present[k] = true;
					}
				}
			}
		}
	}
	for (j=0; j<9; ++j){
		for (i=0; i<9; ++i){
			if (entries[j][i].value == 0) {
				int b = findBox(i,j);
				entries[i][j].totalPos = 0;
				memset(possible, true, sizeof(possible));
				memset(entries[i][j].solution, 0, sizeof(entries[i][j].solution));
				for (p=1; p<10; ++p){
					if (rowLog[j].present[p] || columnLog[i].present[p] || boxLog[b].present[p])
						possible[p] = false;
				 }
				 for (p=1; p<10; ++p){
				 	if (possible[p]){
						 entries[i][j].solution[entries[i][j].totalPos] = p;
						 ++entries[i][j].totalPos;
					 }
				 }
			}
		}
	}
}


void letsSeeIt(int x, int y){
	logBoard();
	for (int j=0; j<9; ++j){
		if (j%3==0)
			 cout<<endl<<"-------------------"<<endl;
		else cout<<endl;
		for (int i=0; i<9; ++i){
			if (i%3==0)
				cout<<"|";	
			else cout<<" ";		
			cout<<entries[i][j].value;	
		}
		cout<<"|";
		if (j==y)
			cout<<"<";
	}
	cout<<endl<<"-------------------"<<endl;
	cout<<" ";
	for (int i=0; i<9; ++i){
		if (i==x)
			cout<<"^";
		cout<<"  ";				
	}
	cout<<endl<<endl;
}

/*void removeDuplicates(){ 
	for (int i=0; i<9; ++i){
		for (int k=1; k<11; ++k){
			rowLog[i].used[k] = false;
			columnLog[i].used[k] = false;
			boxLog[i].used[k] = false;
		}									<-- Make this into a function to revise non-original entries
	}
	for (int j=0; j<9; ++j){
		for (int i=0; i<9; ++i){			
			int b = findBox(i, j);
			for (int k=1; k<11; ++k){
				 if (entries[i][j].value == k){

					if (!rowLog[j].used[k]) rowLog[j].used[k] = true;
					else entries[i][j].value = 0;
					if (!columnLog[i].used[k]) columnLog[i].used[k] = true;
					else entries[i][j].value = 0;
					if (!boxLog[b].used[k]) boxLog[b].used[k] = true;
					else entries[i][j].value = 0;
				}
			}
		}
	}
}*/

bool checkPuzzle(){
	logBoard();
	cout<<"--checking--"<<endl;
	bool allThere = false, noRepeats=true;
	for (int i=0; i<9; ++i){
		if (boxLog[i].filled==9 && rowLog[i].filled==9 && columnLog[i].filled==9)
			allThere = true;
		for (int j=1; j<10; ++j){
				if (!boxLog[i].present[j] || !rowLog[i].present[j] || !columnLog[i].present[j])
					noRepeats = false;
		}
	}
	if (allThere && noRepeats) 
		return true;
	else return false;
}
		
int determinePossibility(entry Entry){
	int competitors[9], mostLikely=0, x=Entry.x, y=Entry.y;
	memset(competitors, 0, sizeof(competitors));
	for (int p=0; p<9; ++p){ // every possibility in entry Entry
		for (int i=0; i<9; ++i){ // row/column incrementer
			if (entries[i][y].value == 0 || entries[x][i].value == 0){
				for (int j=0; j<9; ++j){ // entries[x][y] possibilities
					if (entries[i][y].solution[j]==Entry.solution[p] && entries[i][y].value == 0) //rows
						++competitors[p];
					if (entries[x][i].solution[j]==Entry.solution[p] && entries[x][i].value == 0) //columns
						++competitors[p];
				}
			}
		}
	}
	for(int i=0; i<Entry.totalPos; ++i){
		if (competitors[i] <= competitors[mostLikely] && competitors[i] > 0)
			mostLikely = i;
		cout<<"# of possible "<<Entry.solution[i]<<"'s: "<<competitors[i]<<endl;
		cout<<"most likely element: "<<mostLikely+1<<endl;
	}
	return mostLikely;
}
		
void solveEntry(int x, int y){
	cout<<endl<<"for ("<<x<<","<<y<<"):"<<endl;
	logBoard();
	int ans;
	entry Entry = entries[x][y];
	if (Entry.value == 0){
		if (Entry.totalPos-1<0){
			cout<<endl<<"Possibilities: "<<"[ ";
			for (int i=0; i<9; ++i)
				cout<<Entry.solution[i];
			cout<<" ] totalPos: "<<Entry.totalPos<<endl;
			letsSeeIt(x,y);
		 	exit(0);
	   	}
		else if (Entry.totalPos-1>0){
			ans = determinePossibility(Entry);
			entries[x][y].value = Entry.solution[ans];
		}
		else if (Entry.totalPos-1==0){
			entries[x][y].value = Entry.solution[0];
			entries[x][y].original = true;
			cout<<"set original"<<endl;
		}
		if (feedBack){
			cout<<"tried "<<entries[x][y].value<<" of [ ";
			for (int i=0; i<9; ++i)
				cout<<Entry.solution[i];
			cout<<" ]  possible: "<<Entry.totalPos<<" chose element: "<<ans+1<<endl;
			letsSeeIt(x,y);
		}
	}
	else {
		cout<<"Filled. Value is "<<Entry.value<<". ";
		if (Entry.original) cout<<"Entry is original.";
		else cout<<"Entry is not original.";
		cout<<endl;
	}
}

void prioritize(){
	logBoard();
	cout<<"--sorting--"<<endl;
	int rowQueue[9], columnQueue[9], rowVals[9], colVals[9];
	for (int i=0; i<9; ++i){
		rowVals[i] = rowLog[i].filled;
		colVals[i] = columnLog[i].filled;
		rowQueue[i] = i;
		columnQueue[i] = i;
	}
	int i, j, flag = 1;    
    int temp, tamp;            
    int numLength = 9; 
    for(i = 1; (i <= numLength) && flag; i++)
	{
	  flag = 0;
	  for (j=0; j < (numLength -1); j++)
	 {
		   if (colVals[j+1] > colVals[j])     
		  { 
				temp = colVals[j];           
				colVals[j] = colVals[j+1];
				colVals[j+1] = temp;
				tamp = columnQueue[j];           
				columnQueue[j] = columnQueue[j+1];
				columnQueue[j+1] = tamp;
				flag = 1;           
		   }
	  }
	}
	flag = 1;
	for(i = 1; (i <= numLength) && flag; i++)
	{
	  flag = 0;
	  for (j=0; j < (numLength -1); j++)
	 {
		   if (rowVals[j+1] > rowVals[j])     
		  { 
				temp = rowVals[j];           
				rowVals[j] = rowVals[j+1];
				rowVals[j+1] = temp;
				tamp = rowQueue[j];           
				rowQueue[j] = rowQueue[j+1];
				rowQueue[j+1] = tamp;
				flag = 1;           
		   }
	  }
	}
	if (columnLog[columnQueue[topC]].filled < rowLog[rowQueue[topR]].filled){
		cout<<"solving for row "<<rowQueue[topR]<<", column ";
		for (i=0; i<9; ++i){
			cout<<columnQueue[i];
			if (entries[columnQueue[i]][rowQueue[topR]].value == 0){
				solveEntry(columnQueue[i], rowQueue[topR]);
				break;
			}
		}
	}
	else {
		cout<<"solving for column "<<columnQueue[topC]<<", row ";
		for (i=0; i<9; ++i){
			cout<<rowQueue[i];
			if (entries[columnQueue[topC]][rowQueue[i]].value == 0){
				solveEntry(columnQueue[topC], rowQueue[i]);
				break;
			}
		}
	}
	if (columnLog[columnQueue[topC]].filled == 9)
		++topC;
	if (rowLog[rowQueue[topR]].filled == 9)
		++topR;
}

void solveLoop(){
	if (!checkPuzzle()){
		prioritize();		
		solveLoop();
	}
	else {
		letsSeeIt(0,0);
	}
}

int main(int argc, char** argv){
	feedBack = true;
	readBoard("Board1.txt");
	setUpBoxes();
	logBoard();
	letsSeeIt(0,0);
	solveLoop();	
	return 0;
}
