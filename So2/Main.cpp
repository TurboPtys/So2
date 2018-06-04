#include "curses.h"
#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include <mutex>
#include <thread>
#include <pthread.h>
#include <vector>


using namespace std;

bool fin = false;
char p;
int gold;

int stan[10] = { NULL, NULL, NULL, NULL, NULL };
int wozekS[5] = { NULL, NULL, NULL, NULL, NULL };
int kierW[5] = { NULL, NULL, NULL, NULL, NULL };

//osie
int aixSztygar[2] = { 24, 24 };
int aix[8] = { 60, 60, 60, 60, 60,60,60,60 };
int aiyCorf[4]{5,5,4,5};
int aixCorf[4]{28, 30, 28, 30};

float dones[10];
float speedWorks[10];


//sta³e
int static kilofy = 3;
int static miejsca = 2;
int static wozek[2] = { 2, 2 };



//zmienne pomocnicze
int zKilofy = 0;
int zMiejsca = 0;
int zGold = 0;
int zWozek[2] = { 0, 0 };
int zDrogaKier[4] = { 0, 0, 0, 0 };
int zDroga[4] = { 0, 0, 0, 0 };


mutex mtx_kilof;
mutex mtx_miejsce;
mutex mtx_gold;
mutex mtx_wozek;
mutex mtx_drogaWolna;


class Diger{

public:
	Diger(){
		index = NULL;

	};
	Diger(int in, int sz){

		index = in;
		sztygar = sz;

	};
	int index;
	int sztygar;
	int x = 62;
	float speedWork = 2;


	void operator()(){

		while (true){
			if (fin){
				break;
			}

			
			checkKilof();
			move(4, false, 3);
			getPlace();
			dig();
			move(22, true, 3);
			//sendGold();
			loadCorf();
			move(58, true, 3);
			

		}
	}
	

	bool checkKilof(){
		stan[index] = 3;
		mtx_kilof.lock();
		zKilofy++;
		while (true){
			if (zKilofy <= kilofy){


				mtx_kilof.unlock();
				stan[index] = 4;
				return true;
			}
		}

		return false;

	}

	int move(int dest, bool direction,int speed){

		if (direction){

			stan[index] = 2;
			while (aix[index] <= dest)
			{

				aix[index] = aix[index] + speed;
				Sleep(250);

			}
			return 0;
		}
		else{

			//stan[index] = 1;
			while (aix[index] >= dest)
			{

				aix[index] = aix[index] - speed;
				Sleep(250);

			}
			return 1;
		}


	}
	bool getPlace(){
		stan[index] = 5;

		mtx_miejsce.lock();
		zMiejsca++;
		while (true){
			if (zMiejsca <= miejsca){


				mtx_miejsce.unlock();
				return true;
			}
		}
		return false;
	}

	bool dig(){

			stan[index] = 6;

			mtx_gold.lock();
			//zGold = gold;
			//zGold--;
			
			bool tryDig = true;

			while (tryDig){
				if (gold > 0){
					gold--;
					mtx_gold.unlock();
					tryDig = false;
				}
				//zGold = gold;
			}

			for (int i = 0; i < 3; i++){
				Sleep(250);
			}
		
			stan[index] = 7;



		for (float done=0; done < 100;done=done+speedWork){
			Sleep(500);
			speedWork = ((56 - aixSztygar[sztygar]) / 10) + 2;
			dones[index] = done;
			speedWorks[index] = speedWork;
		}
		

		//gold--;
		zKilofy--;
		zMiejsca--;
		//mtx_miejsce.unlock();

		return true;
	}

	bool sendGold(){

		stan[index] = 8;
		
		while (true){
			
			if (aixSztygar[sztygar] == 24 && zWozek[sztygar] <= wozek[sztygar]){
				
				mtx_wozek.lock();
				stan[index] = 9;
				for (int i = 0; i < 3; i++){
					Sleep(250);
				}

				zWozek[sztygar]++;
				mtx_wozek.unlock();
				
			}
		}


	}

	bool loadCorf(){

		stan[index] = 8;
		//mtx_wozek.lock();
		//zWozek[sztygar]++;
		while (true){
			if (aixSztygar[sztygar] == 24 && zWozek[sztygar] <= wozek[sztygar]){
				stan[index] = 9;
				//mtx_wozek.unlock();

				
				stan[index] = 10;
				return true;
			}
		}

		return false;
	}

	bool wait(){

		while (true){
			Sleep(250);
			if (aixSztygar[0]>48){
			
				return true;
			}
		}

		return true;
	}
};

class Nature{

public:
	Nature(){
		index = NULL;
	};
	Nature(int i, int m){

		index = i;
		maxGold = m;

	};
	int index;
	int maxGold;

	void operator()(){

		gold = 4;

		while (true){
			if (fin){
				break;
			}

			for (int i = 0; i < 160; i++){

				Sleep(250);

			}
			int a = rand() % 3 + 3;
			addGold(a);

		}
	}

	bool addGold(int add){

		
		if (gold < maxGold){

			if (gold + add <= maxGold){
				gold = gold + add;
			}
			else{
				gold = maxGold;
			}
			
			return true;
		}
		else{
			
			return false;
		}
		
	}

};

class Sztygar{
public:
	
	int index;
	Sztygar(){
		index = NULL;
	};
	Sztygar(int i){
		index = i;
	};

	void operator()(){

		while (true){
			if (fin){
				break;
			}


			move(54, true, 2);

			//do roboty 

			move(24, false, 2);

			//kontrola urobku
			wait(2*4);

			move(6, false, 2);

			//kopaæ szybciej

			move(24, true, 2);

			wait(2*4);

			//kontrola urobku
		}
	}


	int move(int dest, bool direction, int speed){

		if (direction){

			//stan[index] = 4;
			while (aixSztygar[index] <= dest)
			{

				aixSztygar[index] = aixSztygar[index] + speed;
				Sleep(250);

			}
			return 0;
		}
		else{

			//stan[index] = 1;
			while (aixSztygar[index] >= dest)
			{

				aixSztygar[index] = aixSztygar[index] - speed;
				Sleep(250);

			}
			return 1;
		}


	}

	bool wait(int k){

		for (int i = 0; i < k; i++){

			Sleep(250);
		}

		return true;
	}
	

};

class Corf{
public:
	int index;
	int korytarz;
	int droga;
	Corf(int i,int k){
	
		index = i;
		korytarz = k;
	
	};
	Corf(){
	
		index = NULL;
		korytarz = NULL;
	};

	void operator()(){

		while(true){

			loadCorf();
			checkWay(1);
			goUp(5, 1);
			Sleep(500);
			checkWay(2);
			goDown(1, 5);
			//Sleep(500);
		}

	}

	bool loadCorf(){

		wozekS[index] = 1;
		while (true)
		{
			for (int i = 0; i < 8; i++){

				if (stan[i] == 10){

					mtx_wozek.lock();
					zWozek[korytarz]++;

					while (true){

						if (zWozek[korytarz] <= wozek[korytarz]){
							mtx_wozek.unlock();
							return true;
						}

					}
					

				}

			}

		}

	}

	bool checkWay(int way){

		wozekS[index] = 2;
		kierW[index] = way;
		mtx_drogaWolna.lock();
	

		while (true){
			if (zDrogaKier[2 * korytarz] == 0 || zDrogaKier[2 * korytarz] == way){

				zDrogaKier[2 * korytarz] = way;
				zDroga[2 * korytarz]++;
				mtx_drogaWolna.unlock();
				droga = 28;
				return true;
			}
			else if (zDrogaKier[2 * korytarz + 1] == 0 || zDrogaKier[2 * korytarz + 1] == way)
			{
				zDrogaKier[2 * korytarz + 1] = way;
				zDroga[2 * korytarz + 1]++;
				mtx_drogaWolna.unlock();
				droga = 30;

				return true;
			}

			
		}
	}

	bool goUp(int start, int dest){
		wozekS[index] = 3;
		aixCorf[index] = droga;
		for (int i = start; i > dest; i--){
			aiyCorf[index]--;
			Sleep(1000);
		}
		
		mtx_drogaWolna.lock();
		if (droga == 28){
			zDroga[2 * korytarz]--;
		}
		else if (droga ==30){
			zDroga[2 * korytarz + 1]--;
		}
		mtx_drogaWolna.unlock();
		
		wozekS[index] = 4;
		if (zDroga[2 * korytarz] == 0){
			zDrogaKier[2 * korytarz] = 0;
		}
		else if (zDroga[2*korytarz+1]==0)
		{
			zDrogaKier[2 * korytarz + 1] = 0;

		}
		kierW[index] = 0;
		return true;
	}

	bool goDown(int start, int dest){
		wozekS[index] = 5;
		aixCorf[index] = droga;
		for (int i = start; i < dest; i++){
			aiyCorf[index]++;
			Sleep(500);
		}

		mtx_drogaWolna.lock();
		/////////////////////////
		wozekS[index] = 6;
		if (droga == 28){
			zDroga[2 * korytarz]--;
		}
		else{
			zDroga[2 * korytarz + 1]--;
		}
		

		if (zDroga[2 * korytarz] == 0){
			zDrogaKier[2 * korytarz] = 0;
		}
		else if (zDroga[2 * korytarz + 1] == 0)
		{
			zDrogaKier[2 * korytarz + 1] = 0;

		}
		/////////////////////////
		wozekS[index] = 7;
		mtx_drogaWolna.unlock();
		mtx_wozek.lock();
		zWozek[korytarz]--;
		mtx_wozek.unlock();
		return true;
	}
};

void* check(void *){
	do{
		p = getch();


	} while (p != 'q');

	fin = true;
	return NULL;
}



int main(){

	bool d = false;
	srand(time(NULL));
	
	//tworzenie obiektów
	Nature n = Nature(0, 10);
	Sztygar Sz[2] = { Sztygar(0), Sztygar(1) };
	Diger K[8];
	Corf C[3];
	for (int i = 0; i < 8; i++){

		K[i] = Diger(i,0);
	}

	for (int i = 0; i < 3; i++){
		C[i] = Corf(i, 0);
	}


	thread kop[8];
	thread corf[3];

	//w¹tki
	for (int i = 0; i < 8; i++){
		kop[i] = thread(&Diger::operator(), K[i]);
	}

	for (int i = 0; i < 3; i++){
		corf[i] = thread(&Corf::operator(), C[i]);
	}

	
	pthread_t cyk;

	pthread_create(&cyk, NULL, check, NULL);
	

	thread sz1(&Sztygar::operator(), Sz[0]);
	thread n1(&Nature::operator(), n);

	int i = 0;
	initscr(); //Start
	if (has_colors() == TRUE) //1
	{

		while (true){

			start_color(); //2
			init_pair(1, COLOR_WHITE, COLOR_BLACK); //3
			init_pair(2, COLOR_BLUE, COLOR_BLACK);
			init_pair(3, COLOR_GREEN, COLOR_BLACK);
			init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
			init_pair(5, COLOR_RED, COLOR_BLACK);
			init_pair(6, COLOR_YELLOW, COLOR_BLACK);
			init_pair(7, COLOR_CYAN, COLOR_BLACK);
			
			attron(COLOR_PAIR(1)); //4
			printw("SO2 PROJEKT - SYMULATOR KOPALNI  \t");
			attron(A_BOLD);

			printw("Czas: %.2f s zmiejsca: %d gold: %d/t%d Postep:%.0f Seed: %.2f   ", (float)i / 4,zMiejsca,gold,aixSztygar[0],dones[0],speedWorks[0]);
			attroff(A_BOLD);
			
			printw("U1  %d %d\t %d %d\t %d%d\t |^|%d %d|x|%d %d |-|%d", wozekS[0],kierW[0], wozekS[1],kierW[1], wozekS[2],kierW[2],zDrogaKier[0],zDrogaKier[1],zDroga[0],zDroga[1],zWozek[0]);
			attron(A_BOLD);

			int rzedy = 0;
			int kolumny = 0;

			//getmaxyx(stdscr, rzedy, kolumny);

			

			
			WINDOW *win = newwin(10, 65, 4, 1);
			WINDOW *win2 = newwin(10, 65, 15, 1);
			mvwprintw(win, 3, 28, "----", 1);
			mvwprintw(win, 4, 27, "|    |", 1);
			mvwprintw(win, 5, 27, "|    |", 1);
			mvwprintw(win, 6, 28, "----", 1);
			mvwprintw(win2, 3, 28, "----", 1);
			mvwprintw(win2, 4, 27, "|    |", 1);
			mvwprintw(win2, 5, 27, "|    |", 1);
			mvwprintw(win2, 6, 28, "----", 1);
			


			/*pthread_t kop[3];

			for (int i = 0; i < 3; i++){

				pthread_create(&kop[i], NULL, &Diger::exe, &K[i]);
			}*/


			for (int i = 0; i < 8; i++){

				if (stan[i] == 0){
					wattron(win, COLOR_PAIR(1));
				}
				else if (stan[i] == 4){
					wattron(win, COLOR_PAIR(2));
				}
				else if (stan[i] == 6 || stan[i] ==5){
					wattron(win, COLOR_PAIR(3));
				}
				else if (stan[i] == 7){
					wattron(win, COLOR_PAIR(4));
					mvwprintw(win, i + 1, aix[i], "K%d %0.f %0.f", stan[i],dones[i],speedWorks[i]);
				}
				else if (stan[i] == 8){
					wattron(win, COLOR_PAIR(5));
				}
				else{
					wattron(win, COLOR_PAIR(1));
				}

				if (stan[i] != 7){

					mvwprintw(win, i + 1, aix[i], "K%d", stan[i]);
				}
				

				

			}
			
			for (int i = 0; i < gold; i++){

				

				wattron(win, COLOR_PAIR(6));
				mvwprintw(win, i + 1, 1, "Z");
				
			}




			wattron(win, COLOR_PAIR(7));
			mvwprintw(win, 5, aixSztygar[0], "Sz%d",1);

			for (int i = 0; i < 3; i++){
				wattron(win, COLOR_PAIR(6));
				mvwprintw(win, aiyCorf[i], aixCorf[i], "U%d", i);
			}

			//for (int i = 5; i < 9; i++){
			//	

			//	wattron(win, COLOR_PAIR(1));
			//	mvwprintw(win, i, 60, "K%d", i);
			//}

			wattron(win, COLOR_PAIR(1));
			for (int i = 1; i < 9; i++){
				
				mvwprintw(win2, i, 60, "K%d", i+8);
			}

			box(win, '|', '-');
			box(win2, '|', '-');
			touchwin(win);
			touchwin(win2);
			wrefresh(win);
			wrefresh(win2);

			


			Sleep(250);
			refresh();
			clear();
			i++;


			//pthread_join(kop, NULL);

			if (fin){

				for (int i = 0; i < 8; i++){
					kop[i].join();
				}

				n1.join();
				break;
			}


		}


		

		refresh();
		clear();
		endwin();

		printf("Trwa zamykanie watkow ...");


		pthread_join(cyk, NULL);
		

	}
	else
	{
		printw("Twoja Konsolka nie obsluguje kolorow. :/ ");
	}


	return 0;
}