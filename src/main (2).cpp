#include <iostream>
#include <print>
#include <vector>
#include <algorithm>
#include "obiekty_gry.h"
#include "winbgi2.h"

class Plansza
{
public:
    void inicjuj() {
        for (int i = 1; i < 6; i++) { for (int j = 1; j < 5; j++) { plansza_[i][j] = 0; } }  //aby uproúciÊ odczytywanie wspÛ≥rzÍdnych i zachowaÊ w nim 
        czy_przygotowana = true;                                                            //spÛjnoúÊ indeksowanie zaczynamy od 0 
    }

    void drukuj() {
        if (czy_przygotowana)
            for (int i = 1; i < 6; i++) { std::print("\t\t\t\t\t"); for (int j = 1; j < 5; j++) { std::print("{}  ", plansza_[i][j]); } std::print("\n"); }
        //std::print("\n\n\n");
    }

    void wypelnij(int x, int y, int wartosc) { plansza_[x][y] = wartosc; }
    int sprawdz(int x, int y) { return plansza_[x][y]; }

private:
    int plansza_[7][6];                 //plansza wiÍksza niø pola do gry o wiersz i kolumnÍ o indeksach 0 oraz 
    bool czy_przygotowana{ false };     //kolumnÍ 6 i wiersz 7 - plansza gry jest otoczona "ramkπ" z zer aby uniemoøliwiÊ indeksowanie poza zakres tablicy
};

void rysuj(Element Obiekt)
{
    rectangle(Obiekt.GetX() * 50, Obiekt.GetY() * 50, (Obiekt.GetX() + Obiekt.GetSzer()) * 50, (Obiekt.GetY() + Obiekt.GetWys()) * 50);
    line((double)Obiekt.GetX()*50, (double)Obiekt.GetY()*50, (Obiekt.GetX() + Obiekt.GetSzer())*50., (Obiekt.GetY() + Obiekt.GetWys())*50.);
    line((Obiekt.GetX() + Obiekt.GetSzer()) * 50., Obiekt.GetY() * 50., Obiekt.GetX() * 50., (Obiekt.GetY() + Obiekt.GetWys()) * 50.);
    //rectangle(int left, int top, int right, int bottom)
}

void static UmiescElementNaPlanszy(Plansza& plansza, Element e)
{
    for (int i = e.GetY(); i < e.GetWys() + e.GetY(); i++)
    {
        for (int j = e.GetX(); j < e.GetSzer() + e.GetX(); j++)
        {
            plansza.wypelnij(i, j, e.GetTyp());
            rysuj(e);
        }
    }
}

void static aktualizacjaPlanszy(Plansza& plansza, std::vector <Element> elementy)
{
    plansza.inicjuj();
    clear();
    setlinestyle(2, 1, 4);
    line(50, 50, 250, 50);
    line(50, 50, 50, 300);
    line(250, 50, 250, 300);
    line(50, 300, 100, 300);
    line(250, 300, 200, 300);
    setlinestyle(1, 3, 1);
    for (Element e : elementy)
    {
        UmiescElementNaPlanszy(plansza, e);
    }
}

bool CzyLegalny(Plansza p, Element e, char kierunek) // Sprawdza czy w miejscu w ktÛre chcemy przesunπÊ element sπ tylko 0
{
    if (kierunek == 'd')
    {
        for (int i = e.GetX(); i < e.GetSzer() + e.GetX(); i++) //sprawdzenie czy na ca≥ej szerokoúci obiektu poniøej niego sπ same 0
        {
            if (p.sprawdz(e.GetY() + e.GetWys(), i) != 0)
                return false;
        }
        return true;
    }
    if (kierunek == 'g')
    {
        for (int i = e.GetX(); i < e.GetSzer() + e.GetX(); i++)
        {
            if (p.sprawdz(e.GetY() - 1, i) != 0)
                return false;
        }
        return true;
    }
    if (kierunek == 'p')
    {
        for (int i = e.GetY(); i < e.GetWys() + e.GetY(); i++)
        {
            if (p.sprawdz(i, e.GetX() + e.GetSzer()) != 0)
                return false;
        }
        return true;
    }
    if (kierunek == 'l')
    {
        for (int i = e.GetY(); i < e.GetWys() + e.GetY(); i++)
        {
            int a = e.GetX() - 1;
            if (p.sprawdz(i, a) != 0)
                return false;
        }
        return true;
    }
    return false;
}

void ruch(std::vector<Element>& Obiekty, int typ, char kierunek, Plansza& P) // weü obiekt o danym typie, sprawdü jego po≥oøenie i wymiary, sprawdü czy ruch jest legalny, wykonaj ruch
{
    Element e;
    int iterator = 0;
    for (Element a : Obiekty)
    {
        if (a.GetTyp() == typ)
        {
            e = a;
            break;
        }
        iterator++;
    }

    if (kierunek == 'd' && CzyLegalny(P, e, kierunek))
        e.Set_pozycjaY(e.GetY() + 1);
    else if (kierunek == 'g' && CzyLegalny(P, e, kierunek))
        e.Set_pozycjaY(e.GetY() - 1);
    else if (kierunek == 'p' && CzyLegalny(P, e, kierunek))
        e.Set_pozycjaX(e.GetX() + 1);
    else if (kierunek == 'l' && CzyLegalny(P, e, kierunek))
        e.Set_pozycjaX(e.GetX() - 1);
    else
        std::print("\nBledna komenda lub ruch jest nielegalny!\n");
    Obiekty[iterator] = e;

    aktualizacjaPlanszy(P, Obiekty);
}

void gra(std::vector<Element>& Obiekty, Plansza& P)
{
    P.inicjuj();
    //P.drukuj();
    aktualizacjaPlanszy(P, Obiekty);
    P.drukuj();

    int komenda_typ;
    char komenda_kier;
    std::print("Aby wykonac ruch wprowadz numer elementu ktory chcesz przesunac, a w nastepnym kroku kierunek ruchu. \
Elementy maja numery od 1 do 11, dozwolone polecenia kierunkow to: \n\'d\' - dol\n\'g\' - gora\n\'p\' - prawo\n\'l\' - lewo\n\
Aby zakonczyc, zamiast kierunku wpisz \'k\'");
    std::cin >> komenda_typ;
    std::cin >> komenda_kier;
    do
    {
        ruch(Obiekty, komenda_typ, komenda_kier, P);
        P.drukuj();
        wait();
        std::cin >> komenda_typ;
        std::cin >> komenda_kier;

    } while (komenda_kier != 'k');
}

int main()
{
    Plansza plansza;
    std::vector <Element> obiekty = ObiektyGry();
    graphics(350, 400);
    //animate(2);
    //wait();
    /*rectangle(50, 50, 100, 100);
    wait();
    clear();
    wait();
    rectangle(50, 50, 100, 100);*/
    gra(obiekty, plansza);
    wait();
}

