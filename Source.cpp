#include "std_lib_facilities.h"
#include <time.h>

void printTime(clock_t t1, clock_t t2)
{
	double secs = double(t2 - t1) / CLOCKS_PER_SEC;
	int mins = floor(secs / 60);
	secs = secs - mins * 60;
	cout << "Time: ";
	if (mins < 10) cout << "0";
	cout << mins << ":";
	if (secs < 10) cout << "0";
	cout << secs << endl;
}

int fact(int n)
{
	for (int iii = n - 1; iii > 1; --iii)
	{
		n *= iii;
	}
	return n;
}

static int squareCount = 0;
static int* conversionSet; //Position conversion set for printing squares
static int sideLength;
static int setSize;
static int originalSum;

void printSquare(int set[])
{
	static ofstream ofs{ "../../Magic Squares.txt" };
	for (int iii = 0; iii < setSize; ++iii)
	{
		ofs << set[conversionSet[iii]] << "\t";
		if (((iii + 1) % sideLength) == 0) ofs << endl;
	}
	ofs << "***" << endl;
	++squareCount;
}

void createConversionSet(const int& c)
{
	int n = pow(c, 2);
	conversionSet = new int[n];
	int curr = 0;
	for (int iii = 0; iii < c; ++iii)
	{
		for (int jjj = 0; jjj < c - iii; ++jjj)
		{
			conversionSet[(c + 1) * iii + jjj] = curr;
			++curr;
		}
		for (int jjj = 1; jjj < c - iii; ++jjj)
		{
			conversionSet[(c + 1) * iii + c * jjj] = curr;
			++curr;
		}
	}
}

void comb(int set[], const int& segmentStart, const int& segmentLength, int depth, int exemptPos, int sum, const int& level, const bool& horizontal);

void perm(int set[], const int& segmentStart, const int& segmentLength, int length, const int& level, const bool& horizontal)
{
	if (length == 1)
	{
		//Moves onto the next segment
		int newSegmentStart = segmentStart + segmentLength;
		int newSegmentLength = (horizontal) ? segmentLength - 1 : segmentLength;
		int newDepth = newSegmentStart;
		int newExemptPos = setSize;
		int newLevel = (horizontal) ? level : level + 1;
		bool newHorizontal = !horizontal;
		int newSum = originalSum;
		if (newHorizontal)
		{
			for (int iii = 0; iii < newLevel; ++iii)
			{
				newSum -= set[conversionSet[sideLength * newLevel + iii]];
			}
		}
		else
		{
			for (int iii = 0; iii <= newLevel; ++iii)
			{
				newSum -= set[conversionSet[sideLength * iii + newLevel]];
			}
		}
		comb(set, newSegmentStart, newSegmentLength, newDepth, newExemptPos, newSum, newLevel, newHorizontal);
	}
	else
	{
		perm(set, segmentStart, segmentLength, length - 1, level, horizontal);
		for (int iii = 0; iii < length - 1; iii++)
		{
			if (length % 2 == 1) swap(set[segmentStart], set[segmentStart + length - 1]);
			else swap(set[segmentStart + iii], set[segmentStart + length - 1]);
			perm(set, segmentStart, segmentLength, length - 1, level, horizontal);
		}
	}
}

void comb(int set[], const int& segmentStart, const int& segmentLength, int depth, int exemptPos, int sum, const int& level, const bool& horizontal)
{
	if (level == sideLength - 1) printSquare(set); //Completed square 
	else if (depth == segmentStart + segmentLength - 1)//Last position in segment
	{
		//Finds the element to complete the sum, then perms
		for (int iii = depth; iii < exemptPos; ++iii)
		{
			if (set[iii] == sum)
			{
				if (iii != depth) swap(set[iii], set[depth]);
				int* tempSet = new int[setSize];
				copy(set, set + setSize, tempSet);
				perm(tempSet, segmentStart, segmentLength, segmentLength, level, horizontal);
				break;
			}
		}
	}
	else
	{
		if (set[depth] < sum) comb(set, segmentStart, segmentLength, depth + 1, exemptPos, sum - set[depth], level, horizontal);
		//Iterates backwards from the exempt pos to find an element that works
		while (exemptPos > segmentStart + segmentLength)
		{
			--exemptPos;
			if (set[exemptPos] < sum)
			{
				swap(set[exemptPos], set[depth]);
				comb(set, segmentStart, segmentLength, depth + 1, exemptPos, sum - set[depth], level, horizontal);
			}
		}
	}
}

void makeSquares(int c)
{
	//Static variable intialisation
	sideLength = c;
	createConversionSet(c); //Position conversion set for printing squares
	originalSum = (pow(c, 3) + c) / 2; //
	setSize = pow(c, 2);
	int* set = new int[setSize];
	for (int iii = 0; iii < setSize; ++iii)
	{
		set[iii] = iii + 1;
	}
	int segmentStart = 0; //The index of the start of the current segment
	int segmentLength = sideLength; //Length of the current segment
	int exemptPos = setSize; //Denotes the exempt boundary for element swapping in comb()
	int depth = 0; //Depth within the entire set
	int level = 0; //The segment level/diagonal depth
	bool horizontal = true; //Whether the segment is vertical or horizontal
	clock_t t1, t2;
	t1 = clock();
	comb(set, segmentStart, segmentLength, depth, exemptPos, originalSum, level, horizontal);
	t2 = clock();
	cout << "Total: " << squareCount << endl;
	//Four possible rotations
	cout << "Rotationally unique: " << squareCount / 4 << endl;
	//Swapping the order of the rows and/or columns preserves the magic sqaure, and so
	//there are (c!)^2 possible row and column permutations
	cout << "Compositionally unique: " << squareCount / pow(fact(c), 2) << endl;
	//Permuting the rows and columns elminates half the possible rotations, hence the
	//count is only decreased by half
	cout << "Rotationally and Compositionally unique: " << squareCount / (pow(fact(c), 2) * 2) << endl;
	printTime(t1, t2);
	delete[] set;
	delete[] conversionSet;
}

int main()
{
	int c;
	cin >> c;
	makeSquares(c);
	return 0;
}