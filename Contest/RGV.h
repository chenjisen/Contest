#pragma once


#include "CNC.h"


#include <fstream>
#include <string>


using namespace std;

// the state of RGV: (init), stop, run, load, unload, clean
// when loading, unloading and cleaning, RGV cannot move
// wait is similar to stop, but the task has been distributed
// Stop_2 is used in QUESTION 2
enum RGVStateT { Stop, Wait, Waitclean, Run, Load, Clean, Stop_2};

struct Material
{
	int no;
	int pos;
	int startLoadTime;
	int endLoadTime;
	int startProcessTime;
	int endProcessTime;
	int startUnloadTime;
	string toString()
	{
		// 加工物料序号	加工CNC编号	上料开始时间	下料开始时间
		return to_string(no) + "," + to_string(pos) + "," + to_string(startLoadTime) + ","
			+ to_string(startUnloadTime) + "\n";
	}

	void clear()
	{
		no = pos = startLoadTime = endLoadTime = startProcessTime = endProcessTime
			= startProcessTime = endProcessTime = startUnloadTime
			= -1;

	}
};


class RGV
{
private:


public:
	int pos; // position
	RGVStateT state;
	int workRemainTime;
	CNC* dest;
	vector<int> RGVMoveTime;
	int CleanTime;
	int currentTime;
	int processNumber; // 1: QUESTION 1, 2: QUESTION 2
	int nextProcess;  // for QUESTION 2, 

	list<CNC*>* waitLoadList;
	list<CNC*>* processList;
	list<CNC*>* waitUnloadList;



	ofstream file;
	ofstream scrapped_file; // for QURSTION 3


	int materialNumber;
	vector<Material> currentMaterials;
	vector<CNC>* vcnc;
	

	void init(vector<int> RGVmovetime, int cleantime, int processnumber, 
		list<CNC*>* _waitLoadList, list<CNC*>* _processList,
		vector<CNC>* _vcnc)
	{
		RGVMoveTime = RGVmovetime;
		CleanTime = cleantime;
		pos = 2;
		state = Stop;
		workRemainTime = 0;
		dest = nullptr;
		processNumber = processnumber;
		nextProcess = 1;
		waitLoadList = _waitLoadList;
		processList = _processList;
		vcnc = _vcnc;
		file.open("materials.csv", ios::out);
		scrapped_file.open("scrapped_materials.csv", ios::out);
		materialNumber = 0;
		currentMaterials = vector<Material>(9);
		for (auto& m : currentMaterials)
			m.clear();
	}

	void startWork();


	void endWork()
	{
		if (workRemainTime == 0) {

			switch (state)
			{
			case Stop: case Wait:
				break;

			case Run:
				cout << "[" << currentTime << "]" << "[RGV]";
				endRun();
				break;

			case Load:
				cout << "[" << currentTime << "]" << "[RGV]";
				endLoad();
				break;

			case Clean:
				cout << "[" << currentTime << "]" << "[RGV]";
				endClean();
				break;

			default:
				break;
			}
		}
	}

	void startRun()
	{
		cout << "start run, pos:" << pos << ", dest pos:" << dest->Pos << endl;
		int truePos = (pos - 1) / 2;
		int trueDestPos = (dest->Pos - 1) / 2;
		int dist = abs(truePos - trueDestPos);

		workRemainTime = RGVMoveTime[dist];
		state = Run;

		// pos == destpos
		if (workRemainTime == 0) {
			cout << "[" << currentTime << "]";
			endRun();
		}
	}

	void endRun()
	{
		pos = dest->Pos;
		cout << "end run, pos:" << pos << endl;
		state = Wait;
	}

	void startClean()
	{
		cout << "start clean" << endl;
		workRemainTime = CleanTime;
		state = Clean;
	}

	void endClean()
	{
		cout << "end clean" << endl;
		state = Stop;
	}

	void startLoad()
	{
		cout << "start load" << endl;
		cout << "[" << currentTime << "]";
		dest->startLoad();
		workRemainTime = dest->workRemainTime;
		state = Load;
		
		
		// write previous material
		if (currentMaterials[pos].no != -1) {
			// write csv
			currentMaterials[pos].startUnloadTime = currentTime;
			file << currentMaterials[pos].toString();

		}

		// new material
		currentMaterials[pos].pos = pos;
		currentMaterials[pos].startLoadTime = currentTime;

	}

	void endLoad()
	{
		cout << "end load" << endl;

		currentMaterials[pos].endLoadTime = currentTime;

		
			
		state = Waitclean;



		/* QUESTION 2 */
		if (processNumber == 2) {
			// process 1 finishes
			if ((*vcnc)[pos].processType == 1)
				state = Stop;
		}

		if (currentMaterials[pos].no == -1)
			state = Stop;   // the first time

		// new material
		++materialNumber;
		currentMaterials[pos].no = materialNumber;

	}


	void updateRemainTime()
	{

		--workRemainTime;


	}
};