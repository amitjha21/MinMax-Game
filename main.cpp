#include "main.h"
#define INF std::numeric_limits<int>::max()
#define MAX(x,y) (x>y)?x:y
#define MIN(x,y) (x<y)?x:y
using namespace std;
bool checkArrBound(int, int);
cellSt** copyArr(cellSt **src, cellSt **dest);


int MinValueAB(vecArray t,int alpha,int beta);
int MaxValueAB(vecArray t,int alpha,int beta);


void freeArray(vecArray ptr)
{
//cout<<"\nfreeing "<<ptr;
//for(int i = 0; i < sizeN; i++)
//	{
//		free(ptr[i]);
//	}
//	free(ptr);
}

void outputFile(vecArray outputArr, string moveMade, IdxValues idv)
{
//    int ret=0;
    ofstream outfile;
    outfile.open("output.txt");

    if(outfile.is_open())
    {
        outfile << idv.col<<idv.row<<' '<<moveMade;
        outfile<<endl;
        for(int i =0; i<sizeN; ++i)
        {
            for(int j=0; j<sizeN; ++j)
            {
                outfile <<outputArr[i][j].occupiedBy;

            }
            outfile << endl;
        }
        outfile.close();
  //      ret = 1;
    }

    else
    {
    //    ret = 0;
        cout << "Unable to open file";
    }
    //freeArray(outputArr);
//    return ret;
//cout <<idv.col<<idv.row<<' '<<moveMade<<endl<<DEPTH;
}
double timeElaspsed()
{
    endTime =  chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_seconds = endTime-startTime;
    return elapsed_seconds.count();
}

double allowedTime(double timeRemaining, int cellRemaining)
{
    double divFact = 0.0, logVal,k;

    if(timeRemaining<0.0 || cellRemaining<1)
    return 0.001;
    if(cellRemaining == 1){
    k = timeRemaining>0.0?timeRemaining:0.001;
    return k;
    }
    divFact = timeRemaining / cellRemaining;
    logVal = log2(cellRemaining);
    return (divFact*logVal);
}

//new Check Raid
vecArray checkRaidForBlank(vecArray currArr, vecArray tempRaid,int i,int j,char my,char op)
{
    int k1,k2,l=1;
//    ValueSt maxRaid;

    vecArray currArrNew = currArr;
    vecArray tempRaidNew = tempRaid;
    vecArray tempRaidToRet = tempRaidNew;
    //copyArr(currArr,currArrNew);
    //copyArr(tempRaid,tempRaidNew);
    //copyArr(tempRaidNew,tempRaidToRet);

    raidPossible = 'N';
    while(l<5)
    {
        tempRaidNew = currArrNew; //copyArr(currArrNew,tempRaidNew);
        switch (l)
        {
        case 1:
            k1 = i;
            k2 = j-1;
            break;
        case 2:
            k1 = i;
            k2 = j+1;
            break;
        case 3:
            k1 = i-1;
            k2 = j;
            break;
        case 4:
            k1 = i+1;
            k2 = j;
            break;
        }
        ++l;
        if(checkArrBound(k1,k2) && currArrNew[k1][k2].occupiedBy == my)
        {
            if(checkArrBound(i,j-1) && (currArrNew[i][j-1].occupiedBy == op))
            {
                tempRaidNew[i][j].occupiedBy = my;
                tempRaidNew[i][j-1].occupiedBy = my;
                raidPossible = 'Y';
            }

            if(checkArrBound(i,j+1) && (currArrNew[i][j+1].occupiedBy == op))
            {
                tempRaidNew[i][j].occupiedBy = my;
                tempRaidNew[i][j+1].occupiedBy = my;
                raidPossible = 'Y';
            }
            if(checkArrBound(i-1,j) && (currArrNew[i-1][j].occupiedBy == op))
            {
                tempRaidNew[i][j].occupiedBy = my;
                tempRaidNew[i-1][j].occupiedBy = my;
                raidPossible = 'Y';
            }
            if(checkArrBound(i+1,j) && (currArrNew[i+1][j].occupiedBy == op))
            {
                tempRaidNew[i][j].occupiedBy = my;
                tempRaidNew[i+1][j].occupiedBy = my;
                raidPossible = 'Y';
            }

            if(raidPossible == 'Y')
            {
                //copyArr(tempRaidNew,tempRaidToRet);
                tempRaidToRet = tempRaidNew;
                freeArray(tempRaidNew);
                freeArray(currArrNew);
                return tempRaidToRet;
            }
        }
    }
     freeArray(tempRaidNew);
     freeArray(currArrNew);
    return tempRaidToRet;
}


int checkScore(vecArray array1)
{
    char currOccupiedBy;
    int currCellScore;
    int youScore = 0,total_cell_X =0, total_cell_O=0;
    for(int i=0; i<sizeN; ++i)
    {
        for(int j=0; j<sizeN; ++j)
        {
            currOccupiedBy = array1[i][j].occupiedBy;
            currCellScore = array1[i][j].cellValue;
            if(currOccupiedBy==c_x)
            {
                total_cell_X = total_cell_X + currCellScore;
            }
            else if(currOccupiedBy==c_o)
                total_cell_O = total_cell_O + currCellScore;
            else {}
        }
    }
    if(youPlay == c_x)
        youScore = total_cell_X - total_cell_O;
    else
        youScore = total_cell_O - total_cell_X;

   // delete [] array1;
    return youScore;
}

bool noMoreCell(vecArray arrState)
{
    bool noMoreEmptyCell = true;
    for(int i =0; i<sizeN; ++i)
    {
        for(int j=0; j<sizeN; ++j)
        {
            if(arrState[i][j].occupiedBy == c_bl)
                return false;
        }
    }
    return noMoreEmptyCell;
}

cellSt** copyArr(cellSt **src, cellSt **dest)
{
    for(int i = 0; i<sizeN; ++i)
    {
        for(int j = 0; j<sizeN; ++j)
        {
            dest[i][j].cellValue = src[i][j].cellValue;
            dest[i][j].occupiedBy = src[i][j].occupiedBy;
        }
    }
    return dest;
}

//Alphabeta
int MaxValueAB(vecArray arrState,int alpha, int beta)          //MaxValue
{
    char occBy;
    vecArray oldArray = arrState;
    vecArray tempRaid;
    if(timeAllowed<=timeElaspsed()||noMoreCell(arrState))
    {
        return checkScore(arrState);
    }

    for(int i =0; i<sizeN; ++i)
    {
        for(int j=0; j<sizeN; ++j)
        {
            occBy = arrState[i][j].occupiedBy;
            if(occBy == c_bl)
            {
                arrState[i][j].occupiedBy = youPlay;
                int temp1 = MinValueAB(arrState,alpha,beta);
                alpha = MAX(alpha,temp1);
                if(beta <= alpha)
                    {
                   //  freeArray(tempArrayNew);
                   // freeArray(tempRaid);
                    return beta;
                    }
                     arrState[i][j].occupiedBy = c_bl;
                //tempArrayNew = copyArr(arrState,tempArrayNew);   //After one iterarion we nee to check for more
            }
        }
    }
    //raid
    //tempArrayNew = copyArr(arrState,tempArrayNew);
    arrState = oldArray;

    for(int i =0; i<sizeN; ++i)
    {
        for(int j=0; j<sizeN; ++j)
        {
            occBy = arrState[i][j].occupiedBy;
            if(occBy == c_bl)
            {
                tempRaid = arrState;

                //    ValueSt maxRaid1 = checkRaid(tempRaid,tempRaid,i,j,youPlay,oppPlay,'X');
                arrState =  checkRaidForBlank(tempRaid,tempRaid,i,j,youPlay,oppPlay);
                if(raidPossible =='Y')
                {
                    int temp2 = MinValueAB(arrState,alpha,beta);
                    alpha = MAX(alpha,temp2);
                }
                if(beta <= alpha)
                    {
                    //freeArray(tempArrayNew);
                    //freeArray(tempRaid);
                    return beta;
                    }

                arrState = oldArray;//copyArr(arrState,tempArrayNew);   //After one iterarion we nee to check for more
            }
        }
    }
    freeArray(oldArray);
    freeArray(arrState);
    return alpha;
}

int MinValueAB(vecArray arrState, int alpha, int beta)            //MinValue
{
    char occBy;
    vecArray oldArray = arrState;
    vecArray tempRaid;

    if(timeAllowed<=timeElaspsed()||noMoreCell(arrState))
    {
        return checkScore(arrState);
    }
    for(int i =0; i<sizeN; ++i)
    {
        for(int j=0; j<sizeN; ++j)
        {
            occBy = arrState[i][j].occupiedBy;
            if(occBy == c_bl)
            {
                arrState[i][j].occupiedBy = oppPlay;
                int m = MaxValueAB(arrState,alpha,beta);
                beta = MIN(beta,m);
                 if(beta <= alpha)
                {
                    return alpha;
                }
                 arrState[i][j].occupiedBy = c_bl;
                //arrState = copyArr(arrState,tempArrayNew);   //After one iterarion we nee to check for more
            }
        }
    }

//raid
    arrState = oldArray;
    for(int i =0; i<sizeN; ++i)
    {
        for(int j=0; j<sizeN; ++j)
        {
            occBy = arrState[i][j].occupiedBy;
            //if(occBy == oppPlay)       //Own piece
            if(occBy == c_bl)
            {
                tempRaid = arrState;
                arrState = checkRaidForBlank(tempRaid,tempRaid,i,j,oppPlay,youPlay);
                if(raidPossible == 'Y')
                {
                    int m = MaxValueAB(arrState,alpha,beta);
                    beta = MIN(beta,m);
                }
                if(beta <= alpha)
                {
                     freeArray(arrState);
                    freeArray(tempRaid);
                    return alpha;
                }
               arrState = oldArray;
            }
        }
    }

    freeArray(arrState);
    freeArray(tempRaid);

    return beta;
}


bool checkArrBound(int i, int j)
{
    if(i>-1 && j>-1 && i<sizeN && j<sizeN)
        return true;

    return false;
}

//index convertoer

IdxValues idxConv(int row, int col)
{
    IdxValues ret;
    ret.col = (int)col + 65;
    ret.row = row + 1;
    return ret;
}
int cellToFil(vecArray myvector)
{
//float cellFor;
int count1 = std::accumulate(myvector.begin(),
                            myvector.end(),
                            0,
                            [](int i, const vector<cellSt>& bs)
                            { return i + std::count_if(bs.begin(),
                                                   bs.end(),
                                                   [](const cellSt& b)
                                                   { return b.occupiedBy == c_bl;}); });
allEmpty = count1 == (sizeN*sizeN)?'Y':'N';
return ceil(count1/2.0);
}
void maxVec(vecArray myVec)
{
    int maxV = -INF, ro,co;
    for(int i=0;i<sizeN;++i)
    {
        for(int j=0;j<sizeN;++j)
        {
            if(myVec[i][j].cellValue>maxV){
               maxV = myVec[i][j].cellValue;
               ro  = i;
               co = j;
               }
        }
    }
    rowSol = ro;
    colSol = co;
}
int main()
{
    string data = "";
    char *datat,*tempCellVal;

    ifstream infile;


    startTime =  chrono::high_resolution_clock::now();
    infile.open("input.txt");
    moveVal = ' ';
    moveFinal = " ";

    if(infile.is_open())
    {
        // cout << "Reading from the file" << endl;
        if(getline(infile, data))
            sizeN = atoi (data.c_str());
        infile.close();
    }

     vector<vector<cellSt> > mainArray(sizeN,vector<cellSt>(sizeN));
     vector<vector<cellSt> > tempArray(sizeN,vector<cellSt>(sizeN));
   /* cellSt **mainArray = (cellSt **)malloc(sizeN * sizeof(cellSt *));
    for (int i=0; i<sizeN; i++)
        mainArray[i] = (cellSt *)malloc(sizeN * sizeof(cellSt));

    cellSt **tempArray = (cellSt **)malloc(sizeN * sizeof(cellSt *));
    for (int i=0; i<sizeN; i++)
        tempArray[i] = (cellSt *)malloc(sizeN * sizeof(cellSt));
*/

    infile.open("input.txt");
    if(infile.is_open())
    {
        while(getline(infile, data))
        {
            //cout << data << endl;

            ++fileLineCount;

            if(fileLineCount == 1)
            {

            }
            else if(fileLineCount == 2)
            {
                MODE = data;

            }
            else if(fileLineCount == 3)
            {
                youPlay = (char)data[0];
                oppPlay = youPlay == c_x? c_o:c_x;
            }
            else if(fileLineCount == 4){
                totalTime = atof (data.c_str());
                DEPTH = atoi (data.c_str());
                }
            else if(fileLineCount <= 4+sizeN)
            {

                datat = (char*)data.c_str();
                tempCellVal = strtok (datat," ");

                colCount = -1;
                ++rowCount;
                while (tempCellVal != NULL)
                {
                    ++colCount;
                    cellVal = atoi (tempCellVal);
                    //mainMatrix(rowCount,colCount).cellValue = cellVal;
                    mainArray[rowCount][colCount].cellValue = cellVal;
                    tempCellVal = strtok (NULL," ");
                }
            }
            else
            {
                datat = (char*)data.c_str();
                ++rowCountState;
                for(int j = 0; j<sizeN; ++j)
                {
                       if((char)data[j]==c_x || (char)data[j]==c_o || (char)data[j]==c_bl)
                        mainArray[rowCountState][j].occupiedBy = (char)data[j];
                }
            }

            data = "";
        }
        infile.close();
    }


//    tempArray = copyArr(mainArray,tempArray);
//int t = cellToFil(mainArray);
//cout <<"ctf:"<<t<<endl;
      timeAllowed = allowedTime(totalTime,cellToFil(mainArray));
        tempArray = mainArray;

    if(MODE == c_comp)                   //AlphaBeta
    {
        if(allEmpty == 'N'){
        char occBy;
        int outputRet,maxValSoFar=-INF;

        for(int i =0; i<sizeN; ++i)
        {
            for(int j=0; j<sizeN; ++j)
            {
                occBy = mainArray[i][j].occupiedBy;
                if(occBy == c_bl)
                {
                    mainArray[i][j].occupiedBy = youPlay;
                    outputRet = MinValueAB(mainArray,-INF,INF);
  //                  cout<<endl<<"OutRet(s)= "<<outputRet<<"  "<<idxConv(i,j).col<<idxConv(i,j).row;
                    if(maxValSoFar<outputRet)
                    {
                        maxValSoFar = outputRet;
                        rowSol = i;
                        colSol = j;
                        moveSol = "Stake";
                    }
                    mainArray[i][j].occupiedBy = c_bl;
                }
            }
        }

//*----------------
//Raid
       // mainArray = copyArr(tempArray,mainArray);
        mainArray = tempArray;
        for(int i =0; i<sizeN; ++i)
        {
            for(int j=0; j<sizeN; ++j)
            {
//            d = DEPTH;
             mainArray = tempArray;
//                mainArray = copyArr(tempArray,mainArray);
                occBy = mainArray[i][j].occupiedBy;

                if(occBy == c_bl)
                {   mainArray = checkRaidForBlank(mainArray,mainArray,i,j,youPlay,oppPlay);
                    if(raidPossible == 'Y')
                    {
                        mainArray[i][j].occupiedBy = youPlay;
                        outputRet = MinValueAB(mainArray,-INF,INF);
                        if(outputRet>maxValSoFar)
                        {
                            maxValSoFar = outputRet;
                            rowSol = i;
                            colSol = j;
                            moveSol = "Raid";
                        }
                    }
                }
            }
        }
    }
    else{
        maxVec(mainArray);
    }
    }
    else
    {
    cout << "No Valid File/Mode found\n";
    return 1;
    }

    tempArray[rowSol][colSol].occupiedBy = youPlay;
    IdxValues id = idxConv(rowSol,colSol);
    if(moveSol == "Raid")
    {
        if(checkArrBound(rowSol,colSol-1) && tempArray[rowSol][colSol-1].occupiedBy == oppPlay)
            tempArray[rowSol][colSol-1].occupiedBy = youPlay;
        if(checkArrBound(rowSol,colSol+1) && tempArray[rowSol][colSol+1].occupiedBy == oppPlay)
            tempArray[rowSol][colSol+1].occupiedBy = youPlay;
        if(checkArrBound(rowSol-1,colSol) && tempArray[rowSol-1][colSol].occupiedBy == oppPlay)
            tempArray[rowSol-1][colSol].occupiedBy = youPlay;
        if(checkArrBound(rowSol+1,colSol) && tempArray[rowSol+1][colSol].occupiedBy == oppPlay)
            tempArray[rowSol+1][colSol].occupiedBy = youPlay;
    }
    cout<<timeAllowed<<" / "<<timeElaspsed();
    outputFile(tempArray,moveSol,id);
    allEmpty = 'N';
    return 0;
}
