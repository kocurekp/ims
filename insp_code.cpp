
#include "simlib.h"
#include <iostream>

#define KOLIK 10
Facility Fac[KOLIK];
Queue Q;
using namespace std;



class Trans : public Process {

	void Behavior() {
		int kt = -1;
		
		zpet:
		for (int a=0; a<KOLIK; a++) 
			if (!Fac[a].Busy()) { kt=a; break; }
			
		if (kt == -1) {
			Into(Q);
			Passivate();
			goto zpet;
		}

		Seize(Fac[kt]);
		Wait(Exponential(30));
		Release(Fac[kt]);
		
		if (Q.Length()>0) {
			(Q.GetFirst())->Activate();
		}
	}
	
};

class	Gen : public Event {
	void Behavior() {
		(new Trans)->Activate();
		Activate(Time + Exponential(1));
	}
};
