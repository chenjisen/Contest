#pragma once

#include "CNC.h"

using namespace std;

CNC * getDest(const list<CNC*>& waitLoadList, const list<CNC*>& processList, const int RGV_Pos))
{
	/*
	//initial value
	int max_waitCNC, max_Pos;  //actually the minimum value
	bool flag = false;
	CNC *ret_CNC = nullptr;

	if (!waitLoadList.empty()) {
		max_waitCNC = (*waitLoadList.begin())->workRemainTime;
		max_Pos = (*waitLoadList.begin())->Pos;
		ret_CNC = *waitLoadList.begin();
		flag = true;
		for (auto iter = waitLoadList.begin(); iter != waitLoadList.end(); iter++)
		{
			if ((*iter)->workRemainTime < max_waitCNC)
			{
				max_waitCNC = (*iter)->workRemainTime;
				//max_Pos = *iter->Pos;
				ret_CNC = *iter;
			}
		}
	}
	if (!processList.empty()) {
		if (!flag) {
			max_waitCNC = (*processList.begin())->workRemainTime;
			max_Pos = (*processList.begin())->Pos;
			ret_CNC = *processList.begin();
			flag = true;
		}
		for (auto iter = processList.begin(); iter != processList.end(); iter++)
		{
			if ((*iter)->workRemainTime < max_waitCNC)
			{
				max_waitCNC = (*iter)->workRemainTime;
				//max_Pos = *iter->Pos;
				ret_CNC = *iter;
			}
		}
	}
	if (!flag) {
		cerr << "All list is empty!" << endl;
	}
	*/
	//initial value
	int min_dis = 4, act_dis, begin_time, end_time, all_waitingtime[9], min_all_waitingtime;
	bool same_dis = false;
	CNC *ret_CNC = nullptr, *choose_CNC = nullptr;
	CNC *same_disCNC[2];

	if (!waitLoadList.empty()) {  // There exist waiting CNC.
		//max_waitCNC = (*waitLoadList.begin())->workRemainTime;
		//max_Pos = (*waitLoadList.begin())->Pos;
		ret_CNC = *waitLoadList.begin();

		for (auto iter = waitLoadList.begin(); iter != waitLoadList.end(); iter++)
		{
			act_dis = abs( ((*iter)->Pos - 1)/2 - RGV_Pos );
			if ( act_dis = min_dis )
			{
				same_disCNC[0] = choose_CNC_pos;
				same_disCNC[1] = *iter;
				same_dis = true;
			}
			else if (act_dis < min_dis)
			{
				choose_CNC = *iter;
				same_dis = false;
			}
		}
		if (same_dis)
		{
			if(same_disCNC[0]%2 == same_disCNC[0]%2)  // The position of the  two CNC is both even or odd.
			{
				choose_CNC = same_disCNC[0];  // random choose, can be improved!!!
			}
			else if(same_disCNC[0]%2 == 0)
			{
				choose_CNC = same_disCNC[1];
			}
			else
				choose_CNC = same_disCNC[0];
				
		}
	}
	else  // waitLoadList is empty, all CNC is working
	{
		for (int i : { 1, 3, 5, 7 })
		{
			act_dis = abs( (CNC[i].Pos - 1)/2 - RGV_Pos );
			begin_time = currentTime + max(RGVMoveTime[act_dis], CNC[i].workRemainTime);
			;  // the time that RGV begin to load and unload
			end_time = begin_time + OddCNCLoadTime + CleanTime;
			for (int j = 1; j < 9; ++j)
			{
				if (j == i)
					continue;
				if (currentTime + CNC[j].workRemainTime < end_time)
				{
					all_waitingtime[i] += (end_time - currentTime - CNC[j].workRemainTime);
				}
			}
		}
		for (int i : {2, 4, 6, 8})
		{
			act_dis = abs( (CNC[i].Pos - 1)/2 - RGV_Pos );
			begin_time = currentTime + max(RGVMoveTime[act_dis], CNC[i].workRemainTime);
			;  // the time that RGV begin to load and unload
			end_time = begin_time + EvenCNCLoadTime + CleanTime;
			for (int j = 1; j < 9; ++j)
			{
				if (j == i)
					continue;
				if (currentTime + CNC[j].workRemainTime < end_time)
				{
					all_waitingtime[i] += (end_time - currentTime - CNC[j].workRemainTime);
				}
			}
		}
		min_all_waitingtime = all_waitingtime[1];
		ret_CNC = *CNC[1];
		for (int i = 2; i < 9; ++i)
		{
			if(all_waitingtime[i] < min_all_waitingtime)
			{
				min_all_waitingtime = all_waitingtime[i];
				ret_CNC = *CNC[i]
			}
		}
		
	}
    

}
	return ret_CNC;
}
