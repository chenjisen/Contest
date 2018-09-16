#pragma once

#include "CNC.h"
#include "RGV.h"
#include<vector>

extern int OddCNCLoadTime;
extern int EvenCNCLoadTime;

using namespace std;

bool waitLoadList_empty(vector<CNC>& vcnc);
bool in_first_type(int RGV_Pos, list<int> first_type);

CNC * getDest1(vector<CNC>& vcnc, const RGV& rgv)
{
	const int RGV_Pos = rgv.pos;
	const list<CNC*>& waitLoadList = *rgv.waitLoadList;
	const vector<int>& RGVMoveTime = rgv.RGVMoveTime;
	const int currentTime = rgv.currentTime;
	const int CleanTime = rgv.CleanTime;

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
			act_dis = abs(((*iter)->Pos - 1) / 2 - (RGV_Pos - 1) / 2);
			if (act_dis == min_dis)
			{
				same_disCNC[0] = choose_CNC;
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
			if (same_disCNC[0]->Pos % 2 == 1)  // The position of the  two CNC is both even or odd.
			{
				choose_CNC = same_disCNC[0];  // random choose, can be improved!!!
			}
			else if (same_disCNC[0]->Pos % 2 == 0)
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
			act_dis = abs((vcnc[i].Pos - 1) / 2 - (RGV_Pos - 1) / 2);
			begin_time = currentTime + max(RGVMoveTime[act_dis], vcnc[i].workRemainTime);
			;  // the time that RGV begin to load and unload
			end_time = begin_time + OddCNCLoadTime + CleanTime;
			for (int j = 1; j < 9; ++j)
			{
				if (j == i)
					continue;
				if (currentTime + vcnc[j].workRemainTime < end_time)
				{
					all_waitingtime[i] += (end_time - currentTime - vcnc[j].workRemainTime);
				}
			}
		}
		for (int i : {2, 4, 6, 8})
		{
			act_dis = abs((vcnc[i].Pos - 1) / 2 - (RGV_Pos - 1) / 2);
			begin_time = currentTime + max(RGVMoveTime[act_dis], vcnc[i].workRemainTime);
			;  // the time that RGV begin to load and unload
			end_time = begin_time + EvenCNCLoadTime + CleanTime;
			for (int j = 1; j < 9; ++j)
			{
				if (j == i)
					continue;
				if (currentTime + vcnc[j].workRemainTime < end_time)
				{
					all_waitingtime[i] += (end_time - currentTime - vcnc[j].workRemainTime);
				}
			}
		}
		min_all_waitingtime = all_waitingtime[1];
		ret_CNC = &vcnc[1];
		for (int i = 2; i < 9; ++i)
		{
			if (all_waitingtime[i] < min_all_waitingtime)
			{
				min_all_waitingtime = all_waitingtime[i];
				ret_CNC = &vcnc[i];
			}
		}

	}
	return ret_CNC;

}

CNC * getDest2(vector<CNC>& vcnc, const RGV& rgv)
{
	const int RGV_Pos = rgv.pos;
	//const list<CNC*>& waitLoadList = *rgv.waitLoadList;
	const vector<int>& RGVMoveTime = rgv.RGVMoveTime;
	const int currentTime = rgv.currentTime;
	const int CleanTime = rgv.CleanTime;

	//initial value
	int min_dis = 4, act_dis, begin_time, end_time, all_waitingtime[9], min_all_waitingtime;
	bool same_dis = false;
	CNC *ret_CNC = nullptr, *choose_CNC = nullptr;
	CNC *same_disCNC[2];

	list<int> first_type, second_type;

	for (int i = 1; i < 9; ++i) {
		if (vcnc[i].processType == 1)
		{
			first_type.push_back(i);
		}
		else
			second_type.push_back(i);
	}

	if (!waitLoadList_empty(vcnc))
	{  // There exists waiting CNC.
	   //max_waitCNC = (*waitLoadList.begin())->workRemainTime;
	   //max_Pos = (*waitLoadList.begin())->Pos;
		if (in_first_type(RGV_Pos, first_type))
			ret_CNC = &vcnc[*first_type.begin()];
		else
			ret_CNC = &vcnc[*second_type.begin()];

		if (in_first_type(RGV_Pos, first_type))
		{
			for (int i = 1; i < 9; ++i)
			{
				if (vcnc[i].state == Waitload && !in_first_type(vcnc[i].Pos, first_type))
				{
					act_dis = abs((vcnc[i].Pos - 1) / 2 - (RGV_Pos - 1) / 2);
					if (act_dis == min_dis)
					{
						same_disCNC[0] = choose_CNC;
						same_disCNC[1] = &vcnc[i];
						same_dis = true;
					}
					else if (act_dis < min_dis)
					{
						choose_CNC = &vcnc[i];
						same_dis = false;
						min_dis = same_dis;
					}
				}
			}
		}
		else  // RGV is at the second-type CNC, so it can only move to either the first-type CNC.
		{
			
				for (int i = 1; i < 9; ++i)
				{
					if (in_first_type(vcnc[i].Pos, first_type))
					{
						act_dis = abs(vcnc[i].Pos - 1) / 2 - (RGV_Pos - 1) / 2;
						if (act_dis == min_dis)
						{
							same_disCNC[0] = choose_CNC;
							same_disCNC[1] = &vcnc[i];
							same_dis = true;
						}
						else if (act_dis < min_dis)
						{
							choose_CNC = &vcnc[i];
							same_dis = false;
							min_dis = same_dis;
						}
					}
				}
		}
		if (same_dis)
		{
			if (same_disCNC[0]->Pos % 2 == 1)  // The position of the  two CNC is both even or odd.
				choose_CNC = same_disCNC[0];  // random choose, can be improved!!!
			else if (same_disCNC[0]->Pos % 2 == 0)
			{
				choose_CNC = same_disCNC[1];
			}
			else
				choose_CNC = same_disCNC[0];

		}
		ret_CNC = choose_CNC;
	}
	else  // waitLoadList is empty, all CNC is working
	{
		for (int i = 1; i < 9; ++i)
		{
			act_dis = abs((vcnc[i].Pos - 1) / 2 - (RGV_Pos - 1) / 2);
			begin_time = currentTime + max(RGVMoveTime[act_dis], vcnc[i].workRemainTime);
			// the time that RGV begin to load and unload
			end_time = begin_time + OddCNCLoadTime + CleanTime;
			for (int j = 1; j < 9; ++j)
			{
				if (j == i)
					continue;
				if (currentTime + vcnc[j].workRemainTime < end_time)
				{
					all_waitingtime[i] += (end_time - currentTime - vcnc[j].workRemainTime);
				}
			}
		}

		min_all_waitingtime = all_waitingtime[1];
		ret_CNC = &vcnc[1];

		for (int i = 1; i < 9; i++)
		{
			if (all_waitingtime[i] < min_all_waitingtime)
			{
				min_all_waitingtime = all_waitingtime[i];
				ret_CNC = &vcnc[i];
			}
		}
	}
	return ret_CNC;

}

CNC * getDest_old(const list<CNC*>& waitLoadList, const list<CNC*>& processList)
{
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
	return ret_CNC;
}

bool in_first_type(int Pos, list<int> first_type)
{

	for (auto iter = first_type.begin(); iter != first_type.end(); iter++)
	{
		if (Pos == *iter)
		{
			return true;
		}
	}
	return false;

}

bool waitLoadList_empty(vector<CNC>& vcnc)
{
	for (int i = 1; i < 9; i++)
	{
		if (vcnc[i].state == Waitload)
			return false;
	}
	return true;
}