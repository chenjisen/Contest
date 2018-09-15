#include "stdafx.h"
#include "RGV_2.h"
#include "scheduling.h"

void RGV_2::startWork()
{
	if (state == Stop) {
		cout << "[" << currentTime << "]" << "[RGV]";
		// do job
		
		//temp
		//dest = getDest(*this);
		startRun();

	}

	if (state == Waitclean) {
		cout << "[" << currentTime << "]" << "[RGV]";
		// after unload, start clean immediately

		startClean();
	}


	if (state == Wait) {
		switch (dest->state)
		{
		case Waitload:
			cout << "[" << currentTime << "]" << "[RGV]";

			startLoad();
			break;


		case Process: case CNCLoad: default:
			break;
		}
	}
}
