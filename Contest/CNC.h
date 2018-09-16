#pragma once

#include <list>

using namespace std;


// get random integer in [min_value, max_value]
int getRand(int min_value, int max_value);


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
	int troubleRemainTime;


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
		troubleRemainTime = getRand(10, 20);
	}

	void endTrouble()
	{
		cout << "[" << currentTime << "][CNC" << Pos << "][END TROUBLE]";
		state = Waitload;
	}
};


