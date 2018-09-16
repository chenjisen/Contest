#pragma once


#include "CNC.h"


#include <fstream>



using namespace std;

// the state of RGV: (init), stop, run, load, unload, clean
// when loading, unloading and cleaning, RGV cannot move
// wait is similar to stop, but the task has been distributed
// Stop_2 is used in QUESTION 2

enum RGVStateT { Stop, Wait, Waitclean, Run, Load, Clean };


class RGV
{

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

	vector<CNC>* vcnc;

	vector<Material> vecMaterial;
	vector<Material>::iterator currentMaterial;

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
		nextProcess = 1;
	}

	void startLoad()
	{
		cout << "start load" << endl;
		cout << "[" << currentTime << "]";
		dest->startLoad();
		workRemainTime = dest->workRemainTime;
		state = Load;

		/* QUESTION 1 */
		if (processNumber == 1) {
			updateFile_startLoad_1();
		}
		/* QUESTION 2 */
		else if (processNumber == 2) {
			updateFile_startLoad_2();
		}
	}

	void updateFile_startLoad_1()
	{
		
		// previous material
		if ((*vcnc)[pos].isFirstMaterial == false) {
			currentMaterial = (*vcnc)[pos].currentMaterial;
			currentMaterial->startUnloadTime_1 = currentTime;
		}

		// new material
		vecMaterial.push_back(Material());
		currentMaterial = vecMaterial.end() - 1;
		currentMaterial->pos_1 = pos;
		currentMaterial->startLoadTime_1 = currentTime;
		(*vcnc)[pos].currentMaterial = currentMaterial;

	}

	void updateFile_startLoad_2()
	{
		if (currentProcess() == 1) {
			if ((*vcnc)[pos].isFirstMaterial == false) {
				// previous material
				currentMaterial = (*vcnc)[pos].currentMaterial;
				currentMaterial->startUnloadTime_2_part1 = currentTime;
			}

			// new material
			vecMaterial.push_back(Material());
			currentMaterial = vecMaterial.end() - 1;
			currentMaterial->pos_2_part1 = pos;
			currentMaterial->startLoadTime_2_part1 = currentTime;
			(*vcnc)[pos].currentMaterial = currentMaterial;

		}

		if (currentProcess() == 2) {

			auto temp_new_material = currentMaterial;

			if ((*vcnc)[pos].isFirstMaterial == false) {
				// previous material
				currentMaterial = (*vcnc)[pos].currentMaterial;
				currentMaterial->startUnloadTime_2_part2 = currentTime;
			}

			// new material
			currentMaterial = temp_new_material;
			currentMaterial->pos_2_part2 = pos;
			currentMaterial->startLoadTime_2_part2 = currentTime;
			(*vcnc)[pos].currentMaterial = currentMaterial;

		}
	}




	void endLoad()
	{
		cout << "end load" << endl;


		/* QUESTION 1 */
		if (processNumber == 1) {

			state = Waitclean;

			if ((*vcnc)[pos].isFirstMaterial) {
				state = Stop;   // the first time
				(*vcnc)[pos].isFirstMaterial = false;
			}


		}

		/* QUESTION 2 */
		if (processNumber == 2) {

			// process 1 finishes
			if (currentProcess() == 1) {

				state = Stop;
				nextProcess = 2;

				// the first time
				if ((*vcnc)[pos].isFirstMaterial) {
					(*vcnc)[pos].isFirstMaterial = false;
					nextProcess = 1;
				}

			}


			// process 2 finishes
			if (currentProcess() == 2) {

				state = Waitclean;
				nextProcess = 1;

				// the first time
				if ((*vcnc)[pos].isFirstMaterial) {
					(*vcnc)[pos].isFirstMaterial = false;
					state = Stop;
				}
			}
		}

	}


	void updateRemainTime()
	{
		--workRemainTime;
	}

	int currentProcess()
	{
		return (*vcnc)[pos].processType;
	}

	void writeFile()
	{
		ofstream file;
		ofstream scrapped_file; // for QURSTION 3
		file.open("materials.csv", ios::out);
		scrapped_file.open("scrapped_materials.csv", ios::out);
		//file << currentMaterials[pos].toString_1();

	}

};