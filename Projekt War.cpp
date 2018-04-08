#include <iostream>
#include <string>
using namespace std;

enum {					// przypisanie numerów danym figurom
	PUSTO = 0,
	PIONEK_BIALY = 1,
	PIONEK_CZERWONY = 2,
	DAMKA_BIALA = 10,
	DAMKA_CZERWONA = 20
};

enum {					// przypisanie numerów danym b³êdom, warunkom
	PRAWIDLOWY_RUCH_Z_BICIEM = 0,
	PRAWIDLOWY_RUCH_BEZ_BICIA = 1,
	POLE_ZAJETE = 2,
	COFANIE = 3,
	WYMAGANE_BICIE = 4,
	WYMAGANE_BICIE_INNYM_PIONKIEM = 5,
	WYMAGANE_BICIE_PO_KOLEI = 6,
	RUCH_W_BOK = 7,
	ZA_DALEKO = 8,
	NIEPRAWIDLOWO_WYBRANY_PIONEK = 9,
	PUSTE_POLE = 10,
	INNY_BLAD = 999
};

void zamiana_pionka_w_damke(int a[8][8])
{
	for(int k = 0; k < 4; k++)              // przechodzi po polach 2 krañców planszy (tylko po tych, po których mog¹ siê poruszaæ figury
	{										// i zamienia zwyk³e pionki w damki
		if(a[0][2 * k] == PIONEK_BIALY)
			a[0][2 * k] = DAMKA_BIALA;

		if(a[7][2 * k + 1] == PIONEK_CZERWONY)
			a[7][2 * k + 1] = DAMKA_CZERWONA;
	}
}

void drukuj_plansze(int a[8][8], int liczba_bialych, int liczba_czerwonych) 		//drukuje plansze
{
	cout << "  Liczba bialych: " << liczba_bialych << ", Liczba czerwonych: " << liczba_czerwonych << "\n";
	cout << "  -----------------------------------------------------------------\n\n";
	cout << "      A       B       C       D       E       F       G       H    \n";

	for(int i = 0; i < 8; i++)
	{
		cout << "  +-------+-------+-------+-------+-------+-------+-------+-------+\n";
		cout <<	"  |       |       |       |       |       |       |       |       |\n";
		cout << 8 - i << " ";
		for(int j = 0; j < 8; j++)
		{
			char p = ' ';  							// je¿eli pole jest puste, to "wpisuje" spacje
			switch(a[7 - i][j])
			{
				case PIONEK_BIALY:          
					p = 'B'; break;
				case PIONEK_CZERWONY:
				    p = 'C'; break;
				case DAMKA_BIALA: 	   		
					p = '#'; break;
				case DAMKA_CZERWONA:   	 	
					p = '@'; break;
			}
			cout <<	"|" << "   " << p <<"   ";
		}
		cout << "|\n";
		cout <<	"  |       |       |       |       |       |       |       |       |\n";
	}
	cout <<	"  +-------+-------+-------+-------+-------+-------+-------+-------+\n\n";
}	


void wyczysc_pozycje(int a[8][8])          // "zeruje" ca³¹ planszê
{
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			a[i][j] = PUSTO;
		}
	}
}

void nadaj_pozycje_startowe(int a[8][8])          // umieszcza pionki na odpowiednich pozycjach
{
	wyczysc_pozycje(a);
	
	for(int k = 0; k <= 3; k++)   // wype³nianie odpowiednich pol (czerwonymi pionkami)
	{
		a[0][2*k] = PIONEK_CZERWONY;
		a[1][2*k + 1] = PIONEK_CZERWONY;
		a[2][2*k] = PIONEK_CZERWONY;
	}
	for(int k = 0; k <= 3; k++)   // wype³nianie odpowiednich pol (bia³ymi pionkami)
	{
		a[5][2*k + 1] = PIONEK_BIALY;
		a[6][2*k] = PIONEK_BIALY;
		a[7][2*k + 1] = PIONEK_BIALY;
	}
}

bool czy_jest_na_planszy(int x, int y)		// sprawdza, czy pionek nie wykracza poza planszê
{
	if((x >= 0 && x <= 7)  &&  (y >= 0 && y <= 7))
		return true;
	else
		return false;
}

bool czy_pole_jest_puste(int x, int y, int a[8][8])		// sprawdza, czy pole jest puste
{
	if(!czy_jest_na_planszy(x, y))
		return false;
	
	if(a[y][x] == PUSTO)
		return true;
	else
		return false;
}

bool czy_jest_damka(int aktualny_gracz)			// czy gracz wykonuje ruch damk¹
{
	return aktualny_gracz == DAMKA_BIALA || aktualny_gracz == DAMKA_CZERWONA;	
}

bool czy_moze_zbic(int aktualny_gracz, int przeciwnik) 		// 
{
	if(aktualny_gracz == PIONEK_BIALY || aktualny_gracz == DAMKA_BIALA)
		return przeciwnik == PIONEK_CZERWONY || przeciwnik == DAMKA_CZERWONA;
	else
		return przeciwnik == PIONEK_BIALY || przeciwnik == DAMKA_BIALA;
}

bool czy_pole_za_pionkiem_do_bicia_jest_puste(int x, int y, int kx, int ky, int zasieg, int a[8][8])	// sprawdza, czy pole za
{																				// zbijan¹ figur¹ jest puste (w przypadku
	for(int i = 1; i <= zasieg; i++)											// damki sprawdza czy jakiekolwiek pole jest puste
		if(czy_pole_jest_puste(x + kx * i, y + ky * i, a))
			return true;
		
	return false;
}

bool czy_pole_za_pionkiem_do_bicia_to_prawidlowe_pole_docelowe(int x, int y, int kx, int ky, int zasieg, int dx, int dy, int a[8][8])
{												// sprawdza, czy nie gracz nie przeskakuje wiêcej ni¿ 1 figury
	int liczba_zajetych = 0;
	for(int i = 1; i <= zasieg; i++)
	{
		int wx = x + kx * i;
		int wy = y + ky * i;
		
		if(!czy_pole_jest_puste(wx, wy, a))
			liczba_zajetych += 1;
		
		if(liczba_zajetych == 0 && wx == dx && wy == dy)
			return true;
	}
		
	return false;
}

//------------------------------------------------------------------------------------------------
// sprawdza, czy wokó³ pola x0, y0 jest przeciwnik, w przypadku damki skanuje ca³¹ przek¹tn¹.
// Je¿eli jest gdzies przeciwnik, to sprawdza czy pole x1, y1, które wskaza³ gracz jako pole docelowe
// le¿y faktycznie za pionkiem do bicia (na przek¹tnej do bicia, a nie gdzies poza). 
// Je¿eli na przek¹tnej by³y pionki do zbicia to funkcja zwraca liczbê tych pionków oraz wspó³rzêdne pierwszego napotkanego
// pionka do zbicia (xz, yz)

int czy_gracz_wykonal_poprawne_bicie(int x0, int y0, int x1, int y1, int* xz, int* yz, int a[8][8])
{
	int kierunek[] = {
		-1, -1,
		-1,  1,
		 1, -1,
		 1,  1
	};
	
	bool damka = czy_jest_damka(a[y0][x0]);
	int zasieg = damka ? 8 : 1;
	int liczba_zbitych = 0;
	
	for(int j = 1; j <= zasieg; j++)
	{
		for(int i = 0; i < 4; i++)
		{
			int kx = kierunek[i * 2 + 0];
			int ky = kierunek[i * 2 + 1];
			
			int x = x0 + kx * j;
			int y = y0 + ky * j;
			
			if(czy_jest_na_planszy(x, y) && czy_moze_zbic(a[y0][x0], a[y][x]))
			{
				if(czy_pole_za_pionkiem_do_bicia_to_prawidlowe_pole_docelowe(x, y, kx, ky, zasieg, x1, y1, a))
				{
					liczba_zbitych += 1;
				
					if(liczba_zbitych == 1)
					{
						*xz = x;
						*yz = y;
					}
				}
			}
		}
	}
	return liczba_zbitych;
}

//------------------------------------------------------------------------------------------------
// sprawdza, czy wokó³ pola x0, y0 jest przeciwnik, w przypadku damki skanuje ca³¹ przek¹tn¹

bool czy_jest_przeciwnik_do_zbicia(int x0, int y0, int a[8][8])
{
	int kierunek[] = {
		-1, -1,
		-1,  1,
		 1, -1,
		 1,  1
	};
	
	bool damka = czy_jest_damka(a[y0][x0]);
	int zasieg = damka ? 8 : 1;
	
	for(int j = 1; j <= zasieg; j++)
	{
		for(int i = 0; i < 4; i++)
		{
			int kx = kierunek[i * 2 + 0];
			int ky = kierunek[i * 2 + 1];
			
			int x = x0 + kx * j;
			int y = y0 + ky * j;
			
			if(czy_jest_na_planszy(x, y) && czy_moze_zbic(a[y0][x0], a[y][x]) && czy_pole_za_pionkiem_do_bicia_jest_puste(x, y, kx, ky, zasieg, a))
				return true;
		}
	}
	return false;
}

bool czy_jest_kolor(int pionek_lub_damka, int kolor) 		//
{
	if(kolor == PIONEK_CZERWONY || kolor == DAMKA_CZERWONA)
		return pionek_lub_damka == PIONEK_CZERWONY || pionek_lub_damka == DAMKA_CZERWONA;
	else
		return pionek_lub_damka == PIONEK_BIALY || pionek_lub_damka == DAMKA_BIALA;
}

bool czy_jest_gdzies_przeciwnik_do_zbicia(int kolor, int a[8][8])			// sprawdza, czy na planszy nie ma obowi¹zkowego bicia
{
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			if(czy_jest_kolor(a[i][j], kolor))
			{
				if(czy_jest_przeciwnik_do_zbicia(j, i, a))
					return true;
			}
		}
	}
	
	return false;
}

void przenies_pionka(int x0, int y0, int x1, int y1, int a[8][8]) 		// przenosi figurê na podane przez gracza pole
{
	a[y1][x1] = a[y0][x0];
	a[y0][x0] = PUSTO;
}

void usun_pionka(int x, int y, int a[8][8])				// usuwa zbijan¹ figurê
{
	a[y][x] = PUSTO;
}
//-------------------------------------------------------------------------------------------------------
// Funkcja wykonuj¹ca ruch (bada, czy ruszamy siê damk¹, czy zwyk³ym pionkiem)
int wykonaj_ruch(int x0, int y0, int x1, int y1, int a[8][8], int* liczba_bialych, int* liczba_czerwonych)
{
	bool damka = czy_jest_damka(a[y0][x0]);
	
	if(!czy_pole_jest_puste(x1, y1, a))
		return POLE_ZAJETE;

	if(y1 == y0 || x1 == x0)
		return RUCH_W_BOK;

	// zmienne do zapisania wspó³rzêdnych zbitego pionka
	int xz = -1, yz = -1;
	
	bool bicie = czy_jest_przeciwnik_do_zbicia(x0, y0, a);
	bool poprawne_bicie = false;
	
	if(bicie)
	{
		int liczba_zbitych = czy_gracz_wykonal_poprawne_bicie(x0, y0, x1, y1, &xz, &yz, a);
		if(liczba_zbitych == 0)
			return WYMAGANE_BICIE;
		else if(liczba_zbitych > 1)
			return WYMAGANE_BICIE_PO_KOLEI;
		else
			poprawne_bicie = true;
	}
	
	if(xz < 0 && yz < 0 && czy_jest_gdzies_przeciwnik_do_zbicia(a[y0][x0], a))
		return WYMAGANE_BICIE_INNYM_PIONKIEM;
	
	int lc = *liczba_czerwonych;
	int lb = *liczba_bialych;
	
	if(a[y0][x0] == PIONEK_BIALY || a[y0][x0] == DAMKA_BIALA)
	{
		if(!damka)
		{
			if(!poprawne_bicie && y1 > y0)
				return COFANIE;
	
			//jesli jest bicie to mo¿na siê ruszyæ maksymalnie o 2 pola, je¿eli nie to o 1 - (int) bicie => false -> 0, true -> 1
			if(y0 - y1 >= (2 + (int) bicie))
				return ZA_DALEKO;
		}
		
		if(xz >= 0 && yz >= 0)
		{
			*liczba_czerwonych -= 1;
			usun_pionka(xz, yz, a);
		}
	}

	if(a[y0][x0] == PIONEK_CZERWONY || a[y0][x0] == DAMKA_CZERWONA)
	{
		if(!damka)
		{
			if(!poprawne_bicie && y1 < y0)
				return COFANIE;
	
			if(y1 - y0 >= (2 + (int) bicie))
				return ZA_DALEKO;
		}
		
		if(xz >= 0 && yz >= 0)
		{
			*liczba_bialych -= 1;
			usun_pionka(xz, yz, a);
		}
		
	}
	
	przenies_pionka(x0, y0, x1, y1, a);

	if(lc != *liczba_czerwonych || lb != *liczba_bialych)
		return PRAWIDLOWY_RUCH_Z_BICIEM;
	else
		return PRAWIDLOWY_RUCH_BEZ_BICIA;
}

bool koniec_gry(int a[8][8], int liczba_czerwonych, int liczba_bialych) 		// // warunek koñca gry
{
	drukuj_plansze(a, liczba_bialych, liczba_czerwonych);
	if(liczba_czerwonych <= 0)
	{
		cout << "==================================\n";
		cout << "	WYGRAL GRACZ BIALY		   \n";
		cout << "==================================";
		return true;
	}
	else if(liczba_bialych <= 0)
	{		
		cout << "=====================================\n";
		cout << "	WYGRAL GRACZ CZERWONY		  \n";
		cout << "=====================================";
		return true;	
	}
	return false;
}

int odczytaj_x(string wspolrzedne)  		// odczytywanie i poprawianie wspó³rzêdnych
{
	for(unsigned int i = 0; i < wspolrzedne.length(); i++)
	{
		char c = toupper(wspolrzedne[i]);
		if(c >= 'A' && c <= 'H')
			return c - 'A';
	}
	
	return -1;
}

int odczytaj_y(string wspolrzedne)  		// odczytywanie i poprawianie wspó³rzêdnych
{
	for(unsigned int i = 0; i < wspolrzedne.length(); i++)
	{
		char c = wspolrzedne[i];
		if(c >= '1' && c <= '8')
			return c - '1';
	}
	
	return -1;
}

void drukujBlad(int stan)
{
	cout << "  ----------------------------! BLAD !-----------------------------\n  ";
	if(stan == POLE_ZAJETE)
	{
		cout << "Pole zajete\n";
	}
	else if(stan == COFANIE)
	{
		cout << "Nie mozna sie cofac\n";
	}
	else if(stan == WYMAGANE_BICIE)
	{
		cout << "Wymagane bicie\n";
	}
	else if(stan == WYMAGANE_BICIE_INNYM_PIONKIEM)
	{
		cout << "Wymagane bicie innym pionkiem\n";
	}
	else if(stan == WYMAGANE_BICIE_PO_KOLEI)
	{
		cout << "Wymagane bicie po kolei\n";
	}
	else if(stan == RUCH_W_BOK)
	{
		cout << "Ruch moze odbywac sie tylko po skosach\n";
	}
	else if(stan == ZA_DALEKO)
	{
		cout << "Za daleki ruch\n";
	}
	else
		cout << "Zly ruch. Przyczyna nieznana\n";
	cout << "  -----------------------------------------------------------------\n";
}

void drukujAktualnegoGracza(int aktualny_gracz)
{
	cout << "Gracz: ";
	if(aktualny_gracz == PIONEK_BIALY)
		cout << " pionek bialy";
	else if(aktualny_gracz == PIONEK_CZERWONY)
		cout << " pionek czerwony";
	else if(aktualny_gracz == DAMKA_BIALA)
		cout << " damka biala";
	else if(aktualny_gracz == DAMKA_CZERWONA)
		cout << " damka czerwona";
	cout << "\n";
}

int main()
{
	int aktualny_gracz = PIONEK_BIALY;
	int liczba_bialych = 12;
	int liczba_czerwonych = 12;
	
	int a[8][8];
	nadaj_pozycje_startowe(a);	
	
	while(true)
	{
		drukuj_plansze(a, liczba_bialych, liczba_czerwonych);
		
		int x0, y0;
		int x1, y1;
		
		drukujAktualnegoGracza(aktualny_gracz);
			
		while(true)
		{
			string pozycja;
			cout << "Podaj wspolrzedne pionka, ktorym chcesz wykonac ruch: ";
			cin >> pozycja;
			
			x0 = odczytaj_x(pozycja);
			y0 = odczytaj_y(pozycja);
			
			//--------------------------------------------------
			int warunek = 1;
			
			if(x0 < 0 || y0 < 0)
			{
				cout << "Niepoprawna wspolrzedna\n";
				warunek = 0;
			}
			else 
			{
				if(a[y0][x0] == PUSTO)
				{
					cout << "To pole jest puste\n";
					warunek = 0;
				}
				if(aktualny_gracz == PIONEK_BIALY && a[y0][x0] == PIONEK_CZERWONY)
				{
					cout << "Tu nie ma bialego pionka\n";
					warunek = 0;
				}
				if(aktualny_gracz == PIONEK_CZERWONY && a[y0][x0] == PIONEK_BIALY)
				{
					cout << "Tu nie ma czerwonego pionka\n";
					warunek = 0;
				}
			}
			
			if(warunek > 0)
				break;
		}
		
		while(true)
		{
			string pozycja;
			cout << "Podaj wspolrzedne pola, na ktore chcesz ruszyc pionka: ";
			cin >> pozycja;

			x1 = odczytaj_x(pozycja);
			y1 = odczytaj_y(pozycja);
			
			if(x1 < 0 || y1 < 0)
				cout << "Niepoprawna wspolrzedna\n";
			else
				break;
		}

		int stan = wykonaj_ruch(x0, y0, x1, y1, a, &liczba_bialych, &liczba_czerwonych);
		
		bool czy_byly_dalsze_bicia = false;

		if(stan == PRAWIDLOWY_RUCH_Z_BICIEM)
		{
			while(czy_jest_przeciwnik_do_zbicia(x1, y1, a))
			{
				czy_byly_dalsze_bicia = true;
				
				drukuj_plansze(a, liczba_bialych, liczba_czerwonych);
				
				drukujAktualnegoGracza(aktualny_gracz);
				cout << "Wymagane dalsze bicie. Podaj wspolrzedne do zbicia pionka\n";
				
				string pozycja;
				cin >> pozycja;
				
				int nx = odczytaj_x(pozycja);
				int ny = odczytaj_y(pozycja);
				
				if(nx < 0 || ny < 0)
					cout << "Niepoprawna wspolrzedna\n";
				else
				{
					int stan = wykonaj_ruch(x1, y1, nx, ny, a, &liczba_bialych, &liczba_czerwonych);
					
					if(stan == PRAWIDLOWY_RUCH_Z_BICIEM)
					{
						y1 = ny;
						x1 = nx;
					}
					else
					{
						drukujBlad(stan);
					}
				}
			}
		}

		if(stan == PRAWIDLOWY_RUCH_BEZ_BICIA || stan == PRAWIDLOWY_RUCH_Z_BICIEM)		// zmienia gracza tylko w przypadku
		{																				// wykonania prawid³owego ruchu
			//***********************************************************
			zamiana_pionka_w_damke(a);
			//***********************************************************
			if(aktualny_gracz == PIONEK_BIALY)
				aktualny_gracz = PIONEK_CZERWONY;
			else
				aktualny_gracz = PIONEK_BIALY;
		}
		else if(czy_byly_dalsze_bicia == false)
		{
			drukujBlad(stan);
		}

		if(koniec_gry(a, liczba_czerwonych, liczba_bialych))
			break;
	}
}
