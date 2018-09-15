#include "stdafx.h"
#include "RGV.h"
#include "scheduling.h"

 void RGV::startWork()
{
	if (state == Stop) {
		cout << "[" << currentTime << "]" << "[RGV]";
		// do job
		dest = getDest(*vcnc, *this);
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
