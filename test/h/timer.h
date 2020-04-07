#ifndef timer_h_
#define timer_h_



class Timer
{

public:

	static int context_switch_on_demand;
	static void  interrupt timer(...);

};

void inic();
void restore();



#endif
