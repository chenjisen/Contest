// Contest.cpp: 定义控制台应用程序的入口点。
//


#include "stdafx.h"

#include "RGV.h"
#include "scheduling.h"

using namespace std;

int currentTime;
const int TotalTime = 28800;	// 8h
const int ShortTotalTime = 3600;	// 1h


extern list<CNC*> waitLoadList, processList, waitUnloadList;


// get random integer in [min_value, max_value]
int getRand(int min_value, int max_value)
{
	return rand() % (max_value - min_value + 1) + min_value;
}





int main()
{
	vector<int> RGVMoveTime(4);

	int CNCProcessTime_1;
	int CNCProcessTime_2_part1;
	int CNCProcessTime_2_part2;
	int OddCNCLoadTime;
	int EvenCNCLoadTime;
	int CleanTime;
	vector<CNC> vcnc(9);	// vcnc[0] will not be not used
	RGV rgv;


	srand((unsigned int)time(NULL));
	RGVMoveTime[0] = 0;

	RGVMoveTime[1] = 20;
	RGVMoveTime[2] = 33;
	RGVMoveTime[3] = 46;
	CNCProcessTime_1 = 560;
	CNCProcessTime_2_part1 = 400;
	CNCProcessTime_2_part2 = 378;
	OddCNCLoadTime = 28;
	EvenCNCLoadTime = 31;
	CleanTime = 25;

	// Case 1
	// 1#, 3#, 5#, 7#
	for (int i : { 1, 3, 5, 7 }) {
		vcnc[i].init(i, OddCNCLoadTime, CNCProcessTime_1);
	}
	// 2#, 4#, 6#, 8#
	for (int i : {2, 4, 6, 8}) {
		vcnc[i].init(i, EvenCNCLoadTime, CNCProcessTime_1);
	}

	rgv.init(RGVMoveTime, CleanTime);

	cout << endl;

	for (currentTime = 0; currentTime < ShortTotalTime; ++currentTime) {

		cout << '[' << currentTime << ']';

		if (rgv.state == Stop) {

			// do job
			rgv.dest = getDest(waitLoadList, processList, waitUnloadList);
			rgv.startRun();

		}

		if (rgv.state == Waitclean) {
			// after unload, start clean immediately

			rgv.startClean();
		}


		if (rgv.state == Wait) {
			switch (rgv.dest->state)
			{
			case Waitload:
				rgv.startLoad();
				break;

			case Waitunload:
				rgv.startUnload();
				break;

			case Process: case CNCLoad: case CNCUnload: default:
				break;
			}
		}



		// update state and go to next second

		rgv.updateState();

		for (CNC& cnc : vcnc)
			cnc.updateState();

		cout << endl;

	}


	system("pause");
	return 0;
}

