#include "stdafx.h"
#include "CNC.h"

// get random integer in [min_value, max_value]
int getRand(int min_value, int max_value)
{
	return rand() % (max_value - min_value + 1) + min_value;
}
