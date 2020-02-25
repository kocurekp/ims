#include "simlib.h"
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

using namespace std;

//total area of 
//current area of palm oil plants 20 000 000 ha 
//total possible area of palm oil suitable for farming  234 000 000 ha
//sum of areas for palm oil 254 000 000ha  
//ha/143 trees
//


#define CO2_FROM_REPLACING_FOREST 400
#define CO2_FROM_REPLACING_PEATLAND 1550
#define MIN_CO2_FIRE_REPLACE 207
#define MAX_CO2_FIRE_REPLACE 650
#define MIN_TIME_BEFORE_PRODUCTION_START 3
#define MAX_TIME_BEFORE_PRODUCTION_START 4
#define OIL_PALM_CO2_STORE 91
#define CO2_FROM_PRODUCTION_FERTILIZERS 1.3825
#define TONS_OF_CO2_PER_KM_ON_SEA 	0.0000125
#define TONS_OF_CO2_PER_KM_ON_LAND 	0.000091
#define YEARS_TO_REPLACE_PLANTATION 26
#define CO2_FROM_HARVEST_HECTARE_YEAR 2.27
#define AMNT_TON_FFB_PRODUCED_YEAR 32
#define AMNT_OF_OIL_PER_TON_OF_FFB 0.255
#define START_TIME 0


double CO2_CNT;//tonnes
float ffb; //amnt of tonnes of ffb (1300ffb/yr x 25kg per fruit / 1000(kg->ton))
float oil;
float others;
int harvestctr;
int exportctr;
bool fireflag;
				int ctr;


/*
class Land : public Process {
	void Behavior() {
		(new Cultivation)->Activate();
		(new Planting)->Activate();
	}
} ;
*/
// Store area("Area", 254000000);
//Sklad může být chápán jako několik zařízení. Rozdíl je v organizaci fronty

Facility hectare("1 hectare");
//N zařízení má N samostatných front

// Store area("Area", 234000000);

class Cultivation : public Process {

	void Behavior() {

		//cout<<"hello world"<<endl;
		Seize(hectare);
		int prev=1;
		float co2;

		switch ( prev )
      {
         case 1:
			co2=CO2_FROM_REPLACING_FOREST;//forest
            break;
         case 2:
			co2=CO2_FROM_REPLACING_PEATLAND;//peat soil 
            break;
         default:
            break;
      }
      if (fireflag)//?????????????????????????????????????????????????????????????
      {
		cout<<"fire"<<endl;
      		// CO2_CNT += (co2 + (MIN_CO2_FIRE_REPLACE+MAX_CO2_FIRE_REPLACE)/2);
      		CO2_CNT = (Uniform(MIN_CO2_FIRE_REPLACE,MAX_CO2_FIRE_REPLACE));
      }else{
      	CO2_CNT += co2;
      }

		// ctr++;
		Release(hectare);
		
	  }
};


class Planting : public Process {

	void Behavior() {
		Seize(hectare);
		CO2_CNT-=OIL_PALM_CO2_STORE;
		//(new FertilizeGen)->Activate();
		Wait(Uniform(MIN_TIME_BEFORE_PRODUCTION_START,MAX_TIME_BEFORE_PRODUCTION_START));//waiting for 3 and a half years for treees to grow
		Release(hectare);
		
	}
	
};

class Fertilizer : public Process {
public:
	void Behavior() {
		//cout<<"hello world"<<endl;
		double time=Time;
		CO2_CNT+=CO2_FROM_PRODUCTION_FERTILIZERS;//acounted for production of fertilisers
		
	}
};

class FertilizeGen : public Event {
public:
	void Behavior() {
		Fertilizer *f = new Fertilizer;
		f->Activate();
		//Wait(1);
		Activate(Time+Uniform(1,2));//once a year
		
	}
	
}; 


class Cut : public Process {
public:
	void Behavior() {
		double time=Time;
		//cout<<time<<endl;
		// cout<<"hello world"<<endl;
		if (!hectare.Busy())
		{
			/* code */
		Seize(hectare);
		Wait(Uniform(MIN_TIME_BEFORE_PRODUCTION_START,MAX_TIME_BEFORE_PRODUCTION_START));
		CO2_CNT+=OIL_PALM_CO2_STORE;//TODO value
		Release(hectare);
		}
	}
	
}; 
class Export : public Process {
public:
	void Behavior() {
		double time=Time;
		if (oil != 0)
		{
			int distance = Uniform(10,500);
			float oil_land_CO2 = distance * oil * TONS_OF_CO2_PER_KM_ON_LAND; //co2 from traveling to ports
			float t_km_sea= TONS_OF_CO2_PER_KM_ON_SEA * oil;

			// float rnd = Random();
			//main_asia=india+pakistan+bangladesh+south korea+ japan
			float main_asia = 4169*0.305*t_km_sea; //4169 distance to port 0.19% of import
			float china = 4985 * 0.092*t_km_sea;
			float middle_east = 8221 * 0.143*t_km_sea;
			float europe = 13688 * 0.24*t_km_sea;
			float africa = 10224 * 0.15*t_km_sea;
			float america = 21560 * 0.065*t_km_sea;
			float oceania = 8080 * 0.005*t_km_sea;

			CO2_CNT += oil_land_CO2;
			CO2_CNT += (main_asia+china+middle_east+europe+africa+america+oceania);
			oil=0;
			exportctr++;
		}
	}
	
}; 


class CuttingGen : public Event {
public:
	void Behavior() {
		Cut *c = new Cut;
		c->Activate();
		//Wait(1);
		
		Activate(Time+YEARS_TO_REPLACE_PLANTATION);
		
	}
	
}; 


class Harvest : public Process {
public:
	void Behavior() {
		double time=Time;

		// cout<<hectare.Busy()<<endl;
		if (!hectare.Busy())
		{
				/* code */
			Seize(hectare);
			// CO2_CNT+=CO2_FROM_HARVEST_HECTARE_YEAR;//t/ha/yr
			//(new FertilizeGen)->Activate();
			// Wait(Uniform(3,4));//waiting for 3 and a half years for treees to grow
			// Wait(1);
			// crude = 0;
			oil = 0;
			others = 0;
			for (int i = 0; i < AMNT_TON_FFB_PRODUCED_YEAR; i++)
			{
				////////////////////////////////////???????????????????????????????????????????????????????????????????
				others += (0.745*1.9); //tonnes of trash to burn *co2 produces while burning wood
				////////////////////////////////////////////////////////////////////////////////////////////////////
				//// CO2_CNT=others*1; //TODO replace 1 with konstant of co2 from burning ton 
				// crude += 0.23;
				// kernel += 0.025;
				oil += AMNT_OF_OIL_PER_TON_OF_FFB;		
			}
			harvestctr++;
			CO2_CNT+=others+CO2_FROM_HARVEST_HECTARE_YEAR;
			Release(hectare);
		}

		/**
		 * crude a kernel transport a produkce
		 */
	}
	
};

// class Manufacturing : public Process {
// public:
// 	void Behavior() {

// 	cout<<"asd"<<endl;

		
// 	}
	
// };

class HarvestGen : public Event {
public:
	void Behavior() {
		Harvest *h = new Harvest;
		h->Activate();
		//Wait(1);
	// cout<<Time<<endl;
		Activate(Time+1);
		
	}
	
}; 
class ExportGen : public Event {
public:
	void Behavior() {
		Export *e = new Export;
		e->Activate();
		//Wait(1);
	// cout<<CO2_CNT<<endl;
		Activate(Time+1);
		
	}
	
}; 

void print_help(){
	cout<<"help"<<endl;//todo
	exit(1);
}

int main(int argc, char *argv[])
{
	int input_area = 1;
	int end_time = 1;
	fireflag = 0;

	if (argc>1) {
		for (int i=1; i<argc; i++)
		{
			if (strcmp(argv[i], "-a")==0)
			{
				cout<<"area set"<<endl;

				input_area = stoi(argv[i+1]);
			}else if (strcmp(argv[i], "-t")==0)
			{
				cout<<"time set"<<endl;

				end_time = stoi(argv[i+1]);
			}else if (strcmp(argv[i], "-f")==0)
			{
				fireflag = 1;
			}else if (strcmp(argv[i], "-h")==0)
			{
				print_help();
			}
		}
	}
				cout<<input_area<<endl;
				cout<<end_time<<endl;




	// input_area=stoi(argv[1]); //check input
	// end_time=stoi(argv[2]);




	ctr = 0;


	CO2_CNT=0;
	others=0;
	oil =0;
	ffb =0;
	// int input_area=1;
	// ctr =0;
	harvestctr =0;
	exportctr =0;

	Init(START_TIME, end_time);
	/* code */
	(new Cultivation)->Activate();
	(new FertilizeGen)->Activate();
	(new Planting)->Activate();
	(new CuttingGen)->Activate();
	(new HarvestGen)->Activate();
	(new ExportGen)->Activate();
	Run();
	
	double result = CO2_CNT*input_area; 

	cout<<result<<endl;
	printf("%f\n", result);

	hectare.Output();

	cout<<"harvest"<<endl;
	cout<<harvestctr<<endl;
	cout<<"export"<<endl;
	cout<<exportctr<<endl;
	cout<<ctr<<endl;
}


