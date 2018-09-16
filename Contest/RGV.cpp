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
			dest = getDest2(*vcnc, *this);
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

	/* QUESTION 3 */
	for (auto &cnc : *vcnc) {
		if (cnc.state == Trouble) {

		}
	}

}
