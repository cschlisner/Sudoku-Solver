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
int topC = 0, topR = 0, topB = 0;
bool logEnable = false;
ofstream logFile;

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
	if (logEnable) logFile<<"--logging--"<<endl;
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

void writeBoard(int x, int y){
	logBoard();
	for (int j=0; j<9; ++j){
		if (j%3==0)
			 logFile<<endl<<"-------------------"<<endl;
		else logFile<<endl;
		for (int i=0; i<9; ++i){
			if (i%3==0)
				logFile<<"|";	
			else logFile<<" ";		
			logFile<<entries[i][j].value;	
		}
		logFile<<"|";
		if (j==y)
			logFile<<"<";
	}
	logFile<<endl<<"-------------------"<<endl;
	logFile<<" ";
	for (int i=0; i<9; ++i){
		if (i==x)
			logFile<<"^";
		logFile<<"  ";				
	}
	logFile<<endl<<endl;
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
	}
	cout<<endl<<"-------------------"<<endl<<endl;
}

bool checkPuzzle(){
	if (logEnable) logFile<<"--checking puzzle--"<<endl;
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
		if (competitors[i] <= competitors[mostLikely] && competitors[i] > 0){
			mostLikely = i;
			if (logEnable){
				logFile<<"# of possible "<<Entry.solution[i]<<"'s: "<<competitors[i]<<endl;
				logFile<<"most likely element: "<<mostLikely+1<<endl;
			}
		}

	}
	return mostLikely;
}

void solveObvious(){
	for (int i=0; i<9; ++i){
		for (int j=0; j<9; ++j){
			entry ent = entries[i][j];
			if (ent.value == 0 && ent.totalPos == 1){
				entries[i][j].value = ent.solution[0];
				if (logEnable) logFile<<"solved obvious"<<"("<<i<<","<<j<<"): "<<ent.solution[0]<<endl;
				logBoard();
				writeBoard(i,j);
			}
		}
	}
}
		
void solveEntry(int x, int y){
	if (logEnable) logFile<<endl<<"for ("<<x<<","<<y<<"):"<<endl;
	logBoard();
	int ans;
	entry Entry = entries[x][y];
	if (Entry.value == 0){
		if (Entry.totalPos <= 0){
			cout<<endl<<"Crashed at ("<<x<<","<<y<<"):"<<endl;
			dispBoard(x,y);
			exit(0);
	   	}
		else if (Entry.totalPos > 1){
			ans = determinePossibility(Entry);
			entries[x][y].value = Entry.solution[ans];
		}
		else if (Entry.totalPos == 1)
			entries[x][y].value = Entry.solution[0];

		if (logEnable){
			logFile<<"tried "<<entries[x][y].value<<" of [ ";
			for (int i=0; i<9; ++i)
				logFile<<Entry.solution[i];
			logFile<<" ]  possible: "<<Entry.totalPos<<" chose element: "<<ans+1<<endl;
			writeBoard(x,y);
		}
	}
}

void bubbleSort(int valsArr[], int queArr[]){
	int i, j, flag = 1;    
    int temp, tamp;            
    int numLength = 9; 
    for(i = 1; (i <= numLength) && flag; i++)
	{
	  flag = 0;
	  for (j=0; j < (numLength -1); j++)
	 {
		   if (valsArr[j+1] > valsArr[j])     
		  { 
				temp = valsArr[j];           
				valsArr[j] = valsArr[j+1];
				valsArr[j+1] = temp;
				tamp = queArr[j];           
				queArr[j] = queArr[j+1];
				queArr[j+1] = tamp;
				flag = 1;           
		   }
	  }
	}
}

void prioritize(){
	if (logEnable) logFile<<"--sorting--"<<endl;	
	int rowQueue[9], columnQueue[9], boxQueue[9], rowVals[9], colVals[9], boxVals[9];
	int i;
	for (int i=0; i<9; ++i){
		rowVals[i] = rowLog[i].filled;
		colVals[i] = columnLog[i].filled;
		boxVals[i] = boxLog[i].filled;
		rowQueue[i] = i;
		columnQueue[i] = i;
		boxQueue[i] = i;
	}
	
	bubbleSort(colVals, columnQueue);
	bubbleSort(rowVals, rowQueue);
	bubbleSort(boxVals, boxQueue);

	bool stop = false;
	if (boxLog[boxQueue[topB]].filled > rowLog[rowQueue[topR]].filled && boxLog[boxQueue[topB]].filled > columnLog[columnQueue[topC]].filled){
		if (logEnable) logFile<<"solving for box "<<boxQueue[topB]<<endl;
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
		if (logEnable) logFile<<"solving for row "<<rowQueue[topR]<<", column ";
		for (i=0; i<9; ++i){
			if (logEnable) logFile<<columnQueue[i];
			if (entries[columnQueue[i]][rowQueue[topR]].value == 0){
				if (logEnable) logFile<<"<--"<<endl;
				solveEntry(columnQueue[i], rowQueue[topR]);
				break;
			}
		}
	}
	else {
		if (logEnable) logFile<<"solving for column "<<columnQueue[topC]<<", row ";
		for (i=0; i<9; ++i){
			if (logEnable) logFile<<rowQueue[i];
			if (entries[columnQueue[topC]][rowQueue[i]].value == 0){
				if (logEnable) logFile<<"<--"<<endl;
				solveEntry(columnQueue[topC], rowQueue[i]);
				break;
			}
		}
	}
	if (logEnable) logFile<<endl;
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
		logBoard();
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
	if (argc > 1 && argc < 4){
		cout<<endl;
		if (argc == 3 && string(argv[2])=="-l"){
			cout<<"Logging Enabled"<<endl;
			logEnable = true;
			string name = argv[1];
			logFile.open((name+" - solve log.txt").c_str());
		}
		cout<<"Loading \""<<argv[1]<<"\"..."<<endl;
		readBoard(argv[1]);
		setUpBoxes();
		logBoard();
		dispBoard(0,0);
		cout<<"Solving";
		solveLoop();
		if (logEnable) logFile.close();
	}
	else cout<<endl<<"Usage: "<<argv[0]<<" [Path/To/Board] [-l for logging]"<<endl<<endl;
	return 0;
}
