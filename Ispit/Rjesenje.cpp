// 1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
// 2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR CE BITI OZNACENO KAO "RE"
// 3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA
#include <iostream>
#include<vector>
#include<string>
#include<regex>
#include<mutex>
#include<thread>
#include<exception>
using namespace std;
const char* crt = "\n--------------------------------------------------\n";
mutex m;
template<class T1, class T2>
class Kolekcija
{
	T1* _elementi1;
	T2* _elementi2;
	int _trenutnoElemenata;
public:
	Kolekcija(const Kolekcija& obj)
	{
		_trenutnoElemenata = obj._trenutnoElemenata;
		_elementi1 = new T1[obj._trenutnoElemenata];
		_elementi2 = new T2[obj._trenutnoElemenata];
		for (size_t i = 0; i < obj._trenutnoElemenata; i++)
		{
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
	}
	Kolekcija(): _trenutnoElemenata(0)
	{
		_elementi1 = nullptr;
		_elementi2 = nullptr;
	}
	~Kolekcija()
	{
		delete[]_elementi1;
		delete[]_elementi2;
	}

	void AddElement(T1 el1, T2 el2)
	{
		T1* temp1 = new T1[_trenutnoElemenata + 1];
		T2* temp2 = new T2[_trenutnoElemenata + 1];
		for (size_t i = 0; i < _trenutnoElemenata; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		temp1[_trenutnoElemenata] = el1;
		temp2[_trenutnoElemenata] = el2;
		delete[] _elementi1;
		delete[] _elementi2;
		_elementi1 = temp1;
		_elementi2 = temp2;
		_trenutnoElemenata++;
	}
	void RemoveElement(int lokacija)
	{
		for (size_t i = lokacija-1; i < _trenutnoElemenata - 1; i++)
		{
			_elementi1[i] = _elementi1[i + 1];
			_elementi2[i] = _elementi2[i + 1];
		}
		_trenutnoElemenata--;
	}

	T1 GetElement1(int index) { return _elementi1[index]; }
	T2 GetElement2(int index) { return _elementi2[index]; }
	int GetTrenutno() { return _trenutnoElemenata; }

	friend ostream& operator<<(ostream& cout, Kolekcija& obj)
	{
		for (size_t i = 0; i < obj.GetTrenutno(); i++)
			cout << obj.GetElement1(i) << " " << obj.GetElement2(i) << endl;
		return cout;
	}
	Kolekcija& operator =(const Kolekcija& obj)
	{
		if (this != &obj)
		{
			_trenutnoElemenata = obj._trenutnoElemenata;
			delete[] _elementi1;
			delete[] _elementi2;
			_elementi1 = new T1[obj._trenutnoElemenata];
			_elementi2 = new T2[obj._trenutnoElemenata];
			for (size_t i = 0; i < obj._trenutnoElemenata; i++)
			{
				_elementi1[i] = obj._elementi1[i];
				_elementi2[i] = obj._elementi2[i];
			}
		}
		return *this;
	}
};
class Datum
{
	int* _dan, * _mjesec, * _godina;
public:
	Datum(const Datum& obj):_dan(new int(*obj._dan)), _mjesec(new int(*obj._mjesec)), _godina(new int(*obj._godina)){	}
	Datum(int dan = 1, int mjesec = 1, int godina = 2000)
	{
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	~Datum()
	{
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}



	int GetDan() { return *_dan; }
	int GetMjesec() { return *_mjesec; }
	int GetGodina() { return *_godina; }

	friend ostream& operator<< (ostream& COUT, const Datum& obj)
	{
		COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina;
		return COUT;
	}
	Datum& operator =(const Datum& obj)
	{
		if (this != &obj)
		{
			delete[] _dan;
			delete[] _mjesec;
			delete[] _godina;

			_dan = new int(*obj._dan);
			_mjesec = new int(*obj._mjesec);
			_godina = new int(*obj._godina);
		}
		return *this;
	}
};
class Student
{
	char* _imePrezime;
	Datum* _datumRodjenja;
public:
	Student(const Student& obj):_datumRodjenja(new Datum(*obj._datumRodjenja))
	{
		_imePrezime = new char[strlen(obj._imePrezime) + 1];
		strcpy_s(_imePrezime, strlen(obj._imePrezime) + 1, obj._imePrezime);
	}
	Student(const char* imePrezime = "---", Datum d = Datum())
	{
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		_datumRodjenja = new Datum(d);
	}
	~Student() { delete[] _imePrezime; delete _datumRodjenja; }



	char* GetImePrezime() { return _imePrezime; }
	Datum GetDatumRodenja() { return *_datumRodjenja; }

	friend ostream& operator <<(ostream& cout, const Student& obj)
	{
		cout << "ime studenta ---> " << obj._imePrezime << endl;
		cout << "datum rodenja studenta ---> " << *obj._datumRodjenja << endl;
		return cout;
	}
	Student& operator =(const Student& obj)
	{
		if (this != &obj)
		{
			delete[] _imePrezime;
			delete _datumRodjenja;

			_imePrezime = new char[strlen(obj._imePrezime) + 1];
			strcpy_s(_imePrezime, strlen(obj._imePrezime) + 1, obj._imePrezime);

			_datumRodjenja = new Datum(*obj._datumRodjenja);
		}
		return *this;
	}
	bool operator ==(const Student& obj)
	{
		return strcmp(_imePrezime, obj._imePrezime) == 0;
	}
};
class Ispit
{
	Datum _datumOdrzavanja;
	char* _opisIspita;
	Kolekcija<Student, Datum> _prijave;
	Kolekcija<Student, int> _rezultati;
public:
	Ispit(const Ispit& obj):_datumOdrzavanja(obj._datumOdrzavanja),
		_prijave(obj._prijave),_rezultati(obj._rezultati)
	{	
		_opisIspita = new char[strlen(obj._opisIspita) + 1];
		strcpy_s(_opisIspita, strlen(obj._opisIspita) + 1, obj._opisIspita);
	}
	Ispit(const char* opis = nullptr, Datum datum = Datum()) :_datumOdrzavanja(datum)
	{
		_opisIspita = new char[strlen(opis) + 1];
		strcpy_s(_opisIspita, strlen(opis) + 1, opis);
	}
	~Ispit()
	{
		delete[] _opisIspita;
	}

	bool DodajPrijavu(Student student,Datum datum)
	{
		for (size_t i = 0; i < _prijave.GetTrenutno(); i++)
		{
			if (_prijave.GetElement1(i) == student)
				return false;
		}
		_prijave.AddElement(student, datum);
		return true;
	}
	bool DodajRezultat(Student student, int ocjena)
	{
		for (size_t i = 0; i < _prijave.GetTrenutno(); i++)
		{
			if (_prijave.GetElement1(i) == student)
			{
				_rezultati.AddElement(student, ocjena);
				return true;
			}
		}
		return false;
	}
	void Sortiraj()
	{
		Student * temp = new Student;
		bool provjera = true;
		while (provjera)
		{
			provjera = false;
			for (size_t i = 0; i < _rezultati.GetTrenutno() - 1; i++)
			{
				if (_rezultati.GetElement2(i) > _rezultati.GetElement2(i + 1))
				{
					*temp = _rezultati.GetElement1(i + 1);
					_rezultati.GetElement1(i + 1) = _rezultati.GetElement1(i);
					_rezultati.GetElement1(i) = *temp;
				    provjera = true;
				}
			}
		}
	}
	void ZakljuciRezultate()
	{
		for (size_t i = 0; i < _prijave.GetTrenutno(); i++)
		{
		bool imaOcjena = false;
			for (size_t j = 0; j < _rezultati.GetTrenutno(); j++)
			{
				if (_prijave.GetElement1(i) == _rezultati.GetElement1(j))
				{
					imaOcjena = true;
				}
			}
			if (imaOcjena == false)
				_rezultati.AddElement(_prijave.GetElement1(i), 5);
		}
	}

	Datum GetDatum() { return _datumOdrzavanja; }
	char* GetOpis() { return _opisIspita; }
	Kolekcija<Student, Datum> GetPrijave() { return _prijave; }
	Kolekcija<Student, int> GetRezultati() { return _rezultati; }

	friend ostream& operator<<(ostream& cout, Ispit& obj)
	{
		
		cout << "datum odrzavanja -->> " << obj._datumOdrzavanja << endl;
		cout << "opsi ispita -->> " << obj._opisIspita << endl;
		cout << crt;
		cout << "prijave :" << endl;
		cout << obj._prijave << endl;
		cout << "Rezultati :" << endl;
		cout << obj._rezultati << endl;
		cout << crt;
		return cout;
	}
};
void main()
{
	//REGIONI olaksavaju organizaciju programskog code-a, te ih mozete ukloniti ukoliko vam otezavaju preglednost

	Kolekcija<int, double> kolekcija1;
	for (size_t i = 0; i < 10; i++)
		kolekcija1.AddElement(i, i + 0.3);
	cout << kolekcija1 << endl;
	cout << crt;

	Kolekcija<int, double> kolekcija2;
	kolekcija2 = kolekcija1;
	kolekcija1.RemoveElement(2);//UMANJITI VELIÈINU NIZA
	cout << kolekcija2 << crt;
	cout << kolekcija1 << crt;

	if (kolekcija1.GetTrenutno() == kolekcija2.GetTrenutno())
		cout << "ISTI BROJ ELEMENATA" << endl;

	Kolekcija<int, double> kolekcija3(kolekcija2);
	cout << kolekcija3 << crt;

	Datum danas(26, 11, 2015);
	Datum sutra(danas);
	Datum prekosutra;
	prekosutra = danas;
	cout << danas << endl << sutra << endl << prekosutra << crt;

	Student denis("Denis Music", Datum(26, 11, 1990));
	Student jasmin("Jasmin Azemovic", Datum(22, 11, 1990));
	Student goran;
	goran = jasmin;
	Student adel("Adel Handzic", Datum(25, 8, 1990));
	Student marija("Marija Herceg", Datum(15, 6, 1990));

	Ispit prIII("PRIII::26.11.2015", danas);
	//DodajPrijavu - dodaje novog studenta u listu prijavljenih za ispti. 
	//onemoguciti dodavanje dva ista studenta
	if (prIII.DodajPrijavu(denis, danas))
		cout << denis << " DODAT na spisak" << endl;
	if (prIII.DodajPrijavu(jasmin, danas))
		cout << jasmin << " DODAT na spisak" << endl;
	if (prIII.DodajPrijavu(goran, danas))
		cout << jasmin << " DODAT na spisak" << endl;
	if (prIII.DodajPrijavu(adel, danas))
		cout << adel << " DODAT na spisak" << endl;
	if (prIII.DodajPrijavu(marija, danas))
		cout << adel << " DODAT na spisak" << endl;

	cout << crt << endl;
	/*DodajRezultat - dodaje ocjenu koju je student ostvario na ispitu.
	za dodavanje ocjene student je prethodno trebao biti prijavljen na ispit
	*/
	if (prIII.DodajRezultat(goran, 7))
		cout << "rezultat dodan !" << endl;
	if (prIII.DodajRezultat(jasmin, 6))
		cout << "rezultat dodan !" << endl;
	if (prIII.DodajRezultat(adel, 8))
		cout << "rezultat dodan !" << endl;
	if (prIII.DodajRezultat(marija, 9))
		cout << "rezultat dodan !" << endl;

	/*Sortiraj - sortira rezultate ispita prema ocjeni*/
	//prIII.Sortiraj();
	/*ZakljuciRezultate - dodaje ocjenu 5 svim studentima koji su prijavili ispit
	a nije im evidentirana ocjena*/
	prIII.ZakljuciRezultate();
	//rezultati ispita se smiju ispisivati jedino ukoliko su prethodno zakljuèeni
	cout << prIII << crt;

	Ispit prIII_2(prIII);
	cout << prIII_2 << crt;
	system("pause");
}