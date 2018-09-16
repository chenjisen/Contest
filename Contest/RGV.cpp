#include "stdafx.h"
#include "RGV.h"
#include "scheduling.h"

void RGV::startWork()
{
	if (state == Stop) {
		cout << "[" << currentTime << "]" << "[RGV]";
		// do job
		if (processNumber == 1)
			dest = getDest1(*vcnc, *this);
		else if (processNumber == 2)
			dest = getDest2(*vcnc, *this, nextProcess);
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

			
		case Trouble:	/* QUESTION 3 */
			cout << "[" << currentTime << "]" << "[RGV] dest get into trouble, choose a new dest..." << endl;

			// do job
			if (processNumber == 1)
				dest = getDest1(*vcnc, *this);
			else if (processNumber == 2)
				dest = getDest2(*vcnc, *this, nextProcess);
			startRun();

			break;

		case Process: case CNCLoad: default:
			break;
		}
	}



}
