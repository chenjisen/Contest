#pragma once

#include <list>
#include <string>

using namespace std;


// get random integer in [min_value, max_value]
int getRand(int min_value, int max_value);

struct Material
{
	int no;

	int pos_1;
	int startLoadTime_1;
	int startUnloadTime_1;

	/* QUESTION 2 */
	int pos_2_part1;
	int pos_2_part2;
	int startLoadTime_2_part1;
	int startLoadTime_2_part2;
	int startUnloadTime_2_part1;
	int startUnloadTime_2_part2;

	/* QUESTION 3 */
	bool isScrapped;

	Material()
	{
		no = pos_1
			= startLoadTime_1 = startUnloadTime_1
			= pos_2_part1 = pos_2_part2
			= startLoadTime_2_part1 = startLoadTime_2_part2
			= startUnloadTime_2_part1 = startUnloadTime_2_part2
			= -1;

		isScrapped = false;
	}


	string toString_1()
	{
		// 加工物料序号	加工CNC编号	上料开始时间	下料开始时间
		return to_string(no) + "," + to_string(pos_1) + ","
			+ to_string(startLoadTime_1) + "," + to_string(startUnloadTime_1) + "\n";
	}

	string toString_2()
	{
		// 加工物料序号	工序1的CNC编号	上料开始时间	下料开始时间	工序2的CNC编号	上料开始时间	下料开始时间
		return to_string(no) + "," + to_string(pos_2_part1) + ","
			+ to_string(startLoadTime_2_part1) + "," + to_string(startUnloadTime_2_part1)
			+ to_string(pos_2_part2) + ","
			+ to_string(startLoadTime_2_part2) + "," + to_string(startUnloadTime_2_part2)
			+ "\n";
	}

};


// the state of CNC: waiting for loading, loading, processing,
// (QUESTION 3) trouble

enum CNCStateT { Waitload, CNCLoad, Waitprocess, Process, Trouble};

class CNC
{

public:

	int Pos; // position
	int LoadTime; // load and unload time
	int ProcessTime;
	int workRemainTime;
	int processType; // 0: none, 1: first process, 2: second process

	CNCStateT state;
	list<CNC*>* waitLoadList;
	list<CNC*>* processList;
	int currentTime;

	/* QUESTION 3 */
	bool hasTrouble;
	int troubleArrivalTime;

	vector<Material>::iterator currentMaterial;
	bool isFirstMaterial;

	void init(int pos, int loadtime, int processtime, int _type,
		list<CNC*>* _waitLoadList, list<CNC*>* _processList)
	{
		Pos = pos;
		LoadTime = loadtime;
		ProcessTime = processtime;
		workRemainTime = 0;
		waitLoadList = _waitLoadList;
		processList = _processList;
		processType = _type;
		isFirstMaterial = true;

		// init
		state = Waitload;  
		waitLoadList->push_back(this);

	}


	void updateRemainTime()
	{
		--workRemainTime;

		/* QUESTION 3 */
		if (hasTrouble) {
			--troubleArrivalTime;
			if (troubleArrivalTime <= 0) {
				// get into trouble
				state = Trouble;

				// trouble remain time, 10 min ~ 20 min
				workRemainTime = getRand(600, 1200);
			}
		}

	}

	void endWork()
	{
		if (workRemainTime == 0) {


			switch (state)
			{
			case Waitload:
				break;

			case CNCLoad:
				cout << "[" << currentTime << "]" << "[CNC" << Pos << "]";
				endLoad(); break;

			case Process:
				cout << "[" << currentTime << "]" << "[CNC" << Pos << "]";
				endProcess(); break;


			default:
				break;
			}
		}
	}


	void startLoad()
	{
		cout << "[CNC" << Pos << "]start load; " << endl;


		workRemainTime = LoadTime;
		state = CNCLoad;
		waitLoadList->remove(this);
	}

	void endLoad()
	{
		cout << "end load" << endl;
		// after loading, start processing immediately
		cout << "[" << currentTime << "]";
		startProcess();

	}

	void startProcess()
	{
		cout << "[CNC" << Pos << "]start process; " << endl;
		workRemainTime = ProcessTime;
		state = Process;
		processList->push_back(this);

		hasTrouble = false;

		/* QUESTION 3 */

		/*// CNC may have trouble
		int r = getRand(1, 100);
		if (r == 1) {
			// trouble "will" happen
			hasTrouble = true;
			troubleArrivalTime = workRemainTime * getRand(1, 100) / 100;
		}*/


	}

	void endProcess()
	{
		cout << "end process" << endl;
		state = Waitload;
		processList->remove(this);
		waitLoadList->push_back(this);
	}


	/* QUESTION 3 */
	void startTrouble()
	{
		cout << "[" << currentTime << "][CNC" << Pos << "][TROUBLE!!!]" << endl;

	}

	void endTrouble()
	{
		cout << "[" << currentTime << "][CNC" << Pos << "][END TROUBLE]";
		state = Waitload;
	}
};


