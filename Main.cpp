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
};

struct box {
	int y, x;
};

log boxLog[9], rowLog[9], columnLog[9];
entry entries[9][9];
box boxes[9];
int topC = 0, topR = 0, topB = 0;;

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
	ifstream BoardFile ((BoardName).c_str());
	if (BoardFile){
		while (!BoardFile.eof()){
			BoardFile.get(ent);
			if (ent-'0' != -38){
				entries[i][j].value = ent - '0';
				if (i==8){
					i=0;
					++j;
				}
				else ++i;
			}
		}
	}
	else {
		cout<<"Error: "<<BoardName<<" does not exist!"<<endl<<endl;
		exit(0);
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
			if (entries[i][j].value == 0) {
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


void dispBoard(int x, int y){
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

bool checkPuzzle(){
	logBoard();
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
	for (int p=0; p<9; ++p){
		for (int i=0; i<9; ++i){
			if (entries[i][y].value == 0 || entries[x][i].value == 0){
				for (int j=0; j<9; ++j){
					if (entries[i][y].solution[j]==Entry.solution[p] || entries[x][i].solution[j]==Entry.solution[p] && entries[i][y].value == 0)
						++competitors[p];
				}
			}
		}
	}
	for(int i=0; i<Entry.totalPos; ++i){
		if (competitors[i] <= competitors[mostLikely] && competitors[i] > 0)
			mostLikely = i;
	}
	return mostLikely;
}

void solveObvious(){
	logBoard();
	for (int i=0; i<9; ++i){
		for (int j=0; j<9; ++j){
			entry ent = entries[i][j];
			if (ent.value == 0 && ent.totalPos == 1){
				entries[i][j].value = ent.solution[0];
				logBoard();
			}
		}
	}
}
		
void solveEntry(int x, int y){
	logBoard();
	int ans;
	entry Entry = entries[x][y];
	if (Entry.value == 0){
		if (Entry.totalPos <= 0){
			cout<<endl<<"Stuck: resetting board..."<<endl;
			dispBoard(x,y);
			exit(0);
	   	}
		else if (Entry.totalPos > 1){
			ans = determinePossibility(Entry);
			entries[x][y].value = Entry.solution[ans];
		}
		else if (Entry.totalPos == 1)
			entries[x][y].value = Entry.solution[0];
	}
}

void prioritize(){
	logBoard();
	int rowQueue[9], columnQueue[9], boxQueue[9], rowVals[9], colVals[9], boxVals[9];
	for (int i=0; i<9; ++i){
		rowVals[i] = rowLog[i].filled;
		colVals[i] = columnLog[i].filled;
		boxVals[i] = boxLog[i].filled;
		rowQueue[i] = i;
		columnQueue[i] = i;
		boxQueue[i] = i;
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
	flag = 1;
	for(i = 1; (i <= numLength) && flag; i++)
	{
	  flag = 0;
	  for (j=0; j < (numLength -1); j++)
	 {
		   if (boxVals[j+1] > boxVals[j])     
		  { 
				temp = boxVals[j];           
				boxVals[j] = boxVals[j+1];
				boxVals[j+1] = temp;
				tamp = boxQueue[j];           
				boxQueue[j] = boxQueue[j+1];
				boxQueue[j+1] = tamp;
				flag = 1;           
		   }
	  }
	}

	bool stop = false;
	if (boxLog[boxQueue[topB]].filled > rowLog[rowQueue[topR]].filled && boxLog[boxQueue[topB]].filled > columnLog[columnQueue[topC]].filled){
		for (int i=boxes[boxQueue[topB]].x; i<(boxes[boxQueue[topB]].x+3); ++i){
				for (int j=boxes[boxQueue[topB]].y; j<(boxes[boxQueue[topB]].y+3); ++j){
					if (entries[i][j].value == 0){
						solveEntry(i,j);
						stop = true;
					}
					if (stop)
						break;
				}
				if (stop)
					break;
		}
	}

	if (rowLog[rowQueue[topR]].filled > columnLog[columnQueue[topC]].filled){
		for (i=0; i<9; ++i){
			if (entries[columnQueue[i]][rowQueue[topR]].value == 0){
				solveEntry(columnQueue[i], rowQueue[topR]);
				break;
			}
		}
	}
	else {
		for (i=0; i<9; ++i){
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
	if (boxLog[boxQueue[topB]].filled == 9)
		++topB;
}

void solveLoop(){
	if (!checkPuzzle()){
		cout<<".";
		solveObvious();
		prioritize();		
		solveLoop();
	}
	else {
		cout<<endl<<"All Done!"<<endl;
		dispBoard(8,8);
	}
}

int main(int argc, char** argv){
	if (argc == 2){
		cout<<"Loading \""<<argv[1]<<"\"..."<<endl;
		readBoard(argv[1]);
		setUpBoxes();
		logBoard();
		dispBoard(0,0);
		cout<<"Solving";
		solveLoop();
	}
	else cout<<"usage: "<<argv[0]<<" [Path/To/Board]"<<endl<<endl;
	return 0;
}
