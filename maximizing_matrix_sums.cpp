//==================================================================================================================
//COT4400 Project #3
//Christopher Reiner
//See PDF in project folder for in-depth description.
//==================================================================================================================

//NOTE: Pre-increment used throughout to reduce compile time instruction count
//Post-Increment psuedocode
//int j = i;
//i = i + 1;
//return j;
//Pre-increment psuedocode
//i = i + 1;
//return i;

#include<stdlib.h>
#include<iostream>
#include<vector>
#include<fstream>

//penalty modifier value set by assignment parameters
#define PENALTY 2

using namespace std;

//function declaration for calculating row penalty
int penaltyCalc (int row, int rNew, int rOld);

//class for holding vector Element data
class Element {
public:
    int total;
    int currentRow;
    void printTotal() { cout << total << endl; };
    void printCurrentRow() { cout << currentRow << endl; };
private:
    //int tempTotal         <---------USED FOR DEBUGGING
    //int tempCurrentRow    <---------USED FOR DEBUGGING
};

//declare variables
Element DPMax (int col, int rNew, int rOld);

int rows = 0;
int cols = 0;
vector<vector<vector<Element>>> memoLUT;
vector<vector<int>> input;

//recursive function to determine maximum value
Element DP () {
    memoLUT.resize (rows);
    for (int i = 0; i < rows; ++i) {
        memoLUT[i].resize (rows);
        for (int j = 0; j < rows; ++j) {
            memoLUT[i][j].resize (cols);
            for (int k = 0; k < cols; ++k) {
                memoLUT[i][j][k].total = '\0';
            }
        }
    }
    return DPMax(0, 0, 0);
}

//stores totals that have already been calculated for recall
Element DPMax (int col, int rNew, int rOld) {
    if (memoLUT[rNew][rOld][col].total == '\0') {
        int maxRow = 0;
        int tempTotal = 0;
        int maxVal = 0;
        if (col == cols - 1) {
            maxVal = input[0][col] - penaltyCalc (0, rNew, rOld);
            for (int i = 1; i < rows; ++i) {
                tempTotal = input[i][col] - penaltyCalc (i, rNew, rOld);
                if (tempTotal > maxVal) {
                    maxVal = tempTotal;
                    maxRow = i;
                }
            }
        } else {
            maxVal = DPMax (col + 1, 0, rNew).total + (input[0][col] - penaltyCalc (0, rNew, rOld));
            for (int i = 0; i < rows; ++i) {
                tempTotal = DPMax (col + 1, i, rNew).total + (input[i][col] - penaltyCalc (i, rNew, rOld));
                if (tempTotal > maxVal) {
                    maxRow = i;
                    maxVal = tempTotal;
                }
            }
        }
        memoLUT[rNew][rOld][col].currentRow = maxRow;
        memoLUT[rNew][rOld][col].total = maxVal;
    }
    return memoLUT[rNew][rOld][col];
}

//function to determine penalty definition
int penaltyCalc (int row, int rNew, int rOld) {
    if ((row >= rNew && row <= rOld) || (row <= rNew && row >= rOld)) return 0;
    else return (PENALTY * min(abs(row - rNew), abs(row - rOld)));
}

//main function
int main () {

    //input string variable
    string userInput;

    //prompt for user input
    cout << "Enter the name of the file you wish to analyze: " << endl;

    //record user input
    cin >> userInput;

    //open input stream
    ifstream inFile;
    inFile.open(userInput);

    //input error checking
    if (!inFile) {
        cout << "Unable to open file";
        exit(1);
    }

    //read in row size and column size
    inFile >> rows;
    inFile >> cols;

    //resize vector as required
    input.resize(rows);
    for (int i = 0; i < rows; ++i) {
        input[i].resize(cols);
        for (int j = 0; j < cols; ++j) {
            inFile >> input[i][j];
        }
    }

    //close input stream
    inFile.close();

    //output calculations to a text file
    ofstream outputFile;
    outputFile.open("output.txt");

    //console out for debugging
    /*Element maxSumConsole = DP();
    cout << "\nSee output.txt file for formatted output...\n" << endl;
    cout << "The solution is: " << maxSumConsole.total << "\n";
    cout << "This solution may be reached by selecting the numbers found in these locations...\nthe number "
         << input[maxSumConsole.currentRow][0] << " in (";
    int rOldConsole = 0;
    cout << maxSumConsole.currentRow << ", 0)";
    for (int i = 1; i < cols; ++i) {
        int currentRow = memoLUT[maxSumConsole.currentRow][rOldConsole][i].currentRow;
        cout << "\nthe number " << input[currentRow][i] << " in (" << currentRow << ", " << i << ")";
        rOldConsole = maxSumConsole.currentRow;
        maxSumConsole.currentRow = currentRow;
    }*/

    //maxSum calculated by dynamic programming
    Element maxSumOutput = DP();

    //write max total to output file
    outputFile << maxSumOutput.total << "\n";
    outputFile << maxSumOutput.currentRow << " ";
    int rOldOutput = 0;
    for (int i = 1; i < cols; ++i) {
        int currentRow = memoLUT[maxSumOutput.currentRow][rOldOutput][i].currentRow;
        outputFile << currentRow << " ";
        rOldOutput = maxSumOutput.currentRow;
        maxSumOutput.currentRow = currentRow;
    }
    outputFile.close();
}
