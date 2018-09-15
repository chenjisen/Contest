#pragma once

#include <list>

using namespace std;

// the state of CNC: waiting for loading, loading, processing
enum CNCStateT { Waitload, CNCLoad, Waitprocess, Process};

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

	}

	void endProcess()
	{
		cout << "end process" << endl;
		state = Waitload;
		processList->remove(this);
		waitLoadList->push_back(this);
	}


};


