#include<iostream>
#include<regex>
#include<mutex>
#include<thread>
#include<string>
#include<exception>
using namespace std;
const char* crt = "\n-------------------------------------------\n";
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };
const char* charRazred[] = { "PRVI", "DRUGI", "TRECI", "CETVRTI" };
enum SortirajPo { T1, T2 };
mutex m;
char* Alociraj(const char* sadrzaj) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}
template<class T1, class T2>
class Kolekcija {
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
	bool _omoguciDupliranje;
public:
	Kolekcija(const Kolekcija& obj)
	{
		_trenutno = obj._trenutno;
		_omoguciDupliranje = obj._omoguciDupliranje;
		_elementi1 = new T1[obj._trenutno];
		_elementi2 = new T2[obj._trenutno];
		for (size_t i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
	}
	Kolekcija(bool omoguciDupliranje = true) {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_omoguciDupliranje = omoguciDupliranje;
	}
	~Kolekcija() {
		delete[]_elementi1; _elementi1 = nullptr;
		delete[]_elementi2; _elementi2 = nullptr;
	}

	void AddElement(T1 el1, T2 el2)
	{
		if (!_omoguciDupliranje)
		{
			for (size_t i = 0; i < _trenutno; i++)
			{
				if (_elementi1[i] == el1 || _elementi2[i] == el2)
					throw exception("dupliranje nije dozvoljeno !");
			}
		}
		T1* temp1 = new T1[_trenutno+1];
		T2* temp2 = new T2[_trenutno+1];
		for (size_t i = 0; i < _trenutno; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		temp1[_trenutno] = el1;
		temp2[_trenutno] = el2;
		delete[] _elementi1;
		delete[] _elementi2;

		_elementi1 = temp1;
		_elementi2 = temp2;
		_trenutno++;
	}
	void SortirajRastuci(SortirajPo key)
	{
		bool provjera = true;
		if (key == SortirajPo::T1)
		{
			while (provjera)
			{
				provjera = false;
				for (size_t i = 0; i < _trenutno - 1; i++)
				{
					if (_elementi1[i] > _elementi1[i + 1])
					{
						swap(_elementi1[i], _elementi1[i + 1]);
						swap(_elementi2[i], _elementi2[i + 1]);
						provjera = true;
					}
				}
			}
		}
		if (key == SortirajPo::T2)
		{
			while (provjera)
			{
				provjera = false;
				for (size_t i = 0; i < _trenutno - 1; i++)
				{
					if (_elementi2[i] > _elementi2[i + 1])
					{
						swap(_elementi1[i], _elementi1[i + 1]);
						swap(_elementi2[i], _elementi2[i + 1]);
						provjera = true;
					}
				}
			}
		}	
	}

	T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
	int getTrenutno() { return _trenutno; }

	friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
	Kolekcija& operator=(const Kolekcija& obj)
	{
		if (this != &obj)
		{
			_trenutno = obj._trenutno;
			_omoguciDupliranje = obj._omoguciDupliranje;

			delete[] _elementi1;
			delete[] _elementi2;
			_elementi1 = new T1[obj._trenutno];
			_elementi2 = new T2[obj._trenutno];
			for (size_t i = 0; i < _trenutno; i++)
			{
				_elementi1[i] = obj._elementi1[i];
				_elementi2[i] = obj._elementi2[i];
			}
		}
		return *this;
	}
};

class DatumVrijeme {
	int* _dan, * _mjesec, * _godina, * _sati, * _minuti;
public:
	DatumVrijeme(const DatumVrijeme& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
		_sati = new int(*obj._sati);
		_minuti = new int(*obj._minuti);
	}
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);
	}
	~DatumVrijeme() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;
	}


	friend ostream& operator<< (ostream& COUT, const DatumVrijeme& obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti << endl;
		return COUT;
	}
	bool operator ==(const DatumVrijeme& obj)
	{
		return *_dan == *obj._dan && *_mjesec == *obj._mjesec && *_godina == *obj._godina && *_sati == *obj._sati && *_minuti == *obj._minuti;
	}
	DatumVrijeme& operator =(const DatumVrijeme obj)
	{
		if (this != &obj)
		{
			delete[] _dan;
			delete[] _mjesec;
			delete[] _godina;
			delete[] _sati;
			delete[] _minuti;

			_dan = new int(*obj._dan);
			_mjesec = new int(*obj._mjesec);
			_godina = new int(*obj._godina);
			_sati = new int(*obj._sati);
			_minuti = new int(*obj._minuti);
		}
		return *this;
	}
	bool operator >(const DatumVrijeme& obj)
	{
		if (*_godina > * obj._godina)
			return true;
		if (*_godina == *obj._godina && *_mjesec > * obj._mjesec)
			return true;
		if (*_godina == *obj._godina && *_mjesec == *obj._mjesec && *_dan > * obj._dan)
			return true;
		if (*_godina == *obj._godina && *_mjesec == *obj._mjesec && *_dan == *obj._dan && *_sati > * obj._sati)
			return true;
		return false;
		
    }
	bool Vise5Min(DatumVrijeme obj)
	{
		if (*_godina < *obj._godina)
			return true;
		if (*_godina == *obj._godina && *_mjesec < *obj._mjesec)
			return true;
		if (*_godina == *obj._godina && *_mjesec == *obj._mjesec && *_dan < *obj._dan)
			return true;
		if (*_godina == *obj._godina && *_mjesec == *obj._mjesec && *_dan == *obj._dan && *_sati < *obj._sati)
			return true;
		if (*_godina == *obj._godina && *_mjesec == *obj._mjesec && *_dan == *obj._dan && *_sati == *obj._sati)
		{
			if (abs(*_minuti - *obj._minuti) > 5)
				return true;
		}
		return false;
	}
	bool operator <(const DatumVrijeme& obj)
	{
		if (*_godina < * obj._godina)
			return true;
		if (*_godina == *obj._godina && *_mjesec < * obj._mjesec)
			return true;
		if (*_godina == *obj._godina && *_mjesec == *obj._mjesec && *_dan < * obj._dan)
			return true;
		if (*_godina == *obj._godina && *_mjesec == *obj._mjesec && *_dan == *obj._dan && *_sati < * obj._sati)
			return true;
		return false;
	}
	
};

class Predmet {
	char* _naziv;
	int _ocjena;
	string _napomena;
public:
	Predmet(const Predmet& obj):_napomena(obj._napomena), _ocjena(obj._ocjena) { _naziv = Alociraj(obj._naziv); }	
	Predmet(const char* naziv = "", int ocjena = 0, string napomena = ""):_napomena(napomena),_ocjena(ocjena) {_naziv = Alociraj(naziv);}

	~Predmet() {
		delete[] _naziv; _naziv = nullptr;
	}

	string GetNapomena() { return _napomena; }
	char* GetNaziv() { return _naziv; }
	int GetOcjena() { return _ocjena; }
	void DodajNapomenu(string napomena) {
		_napomena += " " + napomena;
	}

	friend ostream& operator<< (ostream& COUT, Predmet& obj) {
		COUT << obj._naziv << " (" << obj._ocjena << ") " << obj._napomena << endl;
		return COUT;
	}
	bool operator ==(const Predmet& obj)
	{
		return strcmp(_naziv, obj._naziv) == 0;
	}
	Predmet operator =(const Predmet& obj)
	{
		if (this != &obj)
		{
			delete [] _naziv;
			_napomena = obj._napomena;
			_naziv = Alociraj(obj._naziv);
			_ocjena = obj._ocjena;		
		}
		return *this;
	}
};

class Uspjeh {
	eRazred* _razred;
	//formalni argument datumvrijeme se odnosi na vrijeme evidentiranja polozenog predmeta
	Kolekcija<Predmet, DatumVrijeme> _predmeti;
public:
	Uspjeh(const Uspjeh & obj):_razred(new eRazred(*obj._razred)),_predmeti(obj._predmeti){}
	Uspjeh(eRazred razred){
		_razred = new eRazred(razred);}
	~Uspjeh() { delete _razred; _razred = nullptr; }


	void SetPredmeti(Predmet predmet, DatumVrijeme datumPolaganja) {_predmeti.AddElement(predmet, datumPolaganja);}
	Kolekcija<Predmet, DatumVrijeme>* GetPredmeti() { return &_predmeti; }
	eRazred* GeteRazred() { return _razred; }

	friend ostream& operator<< (ostream& cout, const Uspjeh& obj) {
		cout << *obj._razred << " " << obj._predmeti << endl;
		return cout;
	}
};
string ValidirajEmail(string email)
{
	if (regex_match(email, regex("([A-Za-z]{1,})([@])(outlook.com|edu.fit.ba)"))) 
		return email;
	    return "notSet@edu.fit.ba";

}
class Kandidat {
	char* _imeprezime;
	string _emailadresa;
	string _brojtelefona;
	vector<Uspjeh> _uspjeh;
public:
	Kandidat(const char* imeprezime = nullptr, string emailadresa= "", string brojtelefona="") {
		_imeprezime = Alociraj(imeprezime);
		_emailadresa = ValidirajEmail(emailadresa);
		_brojtelefona = brojtelefona;
	}
	~Kandidat() {
		delete[] _imeprezime; _imeprezime = nullptr;
	}

	bool AddPredmet(eRazred razred, Predmet predmet, DatumVrijeme datumPolaganja)
	{
		float prosjek = 0;
		int brojacZaProsjek = 0;
		for (size_t i = 0; i < _uspjeh.size(); i++)
		{
			if (*_uspjeh[i].GeteRazred() == razred)
			{
				for (size_t j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++)
				{
					if (_uspjeh[i].GetPredmeti()->getElement2(j).Vise5Min(datumPolaganja) == false || _uspjeh[i].GetPredmeti()->getElement1(j) == predmet)
						return false;
				}
			}
		}
		Uspjeh* temp = new Uspjeh(razred);
		temp->SetPredmeti(predmet, datumPolaganja);
		_uspjeh.push_back(*temp);

		for (size_t i = 0; i < _uspjeh.size(); i++)
		{
			if (*_uspjeh[i].GeteRazred() == razred)
			{
				for (size_t j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++)
				{
					prosjek += _uspjeh[i].GetPredmeti()->getElement1(j).GetOcjena();
					brojacZaProsjek++;
				}
			}			
		}
		if (brojacZaProsjek != 0)
			prosjek /= float(brojacZaProsjek);
			
		thread t([this,razred, prosjek]() {
			m.lock();
			cout << crt;
			cout<< "FROM: " <<"info@edu.fit.ba"<<endl;
			cout << "TO: " << _emailadresa << endl;
			cout << "Postovani " << _imeprezime << " , evidentirali ste uspjeh za " << charRazred[razred] <<" razred "<< endl;
			cout<<" Pozdrav."<<endl;
			cout<<" FIT Team."<<endl;
			if (prosjek > 4.5)
				cout << "Svaka cast za uspjeh " << prosjek << " u " << razred << " razredu" << endl;
			cout << crt;
			m.unlock();
			});
		t.join();
		return true;
	}
	int BrojPonavljanjaRijeci(string rijec)
	{
		int brojPonavljanja = 0;
		for (size_t i = 0; i < _uspjeh.size(); i++)
		{
			regex r("(\\b" + rijec + "\\b|\\B" + rijec + "\\B)");
			for (size_t j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++)
			{
				string napomena =_uspjeh[i].GetPredmeti()->getElement1(j).GetNapomena();
				auto start = sregex_iterator(napomena.begin(), napomena.end(), r);
				auto end = sregex_iterator();
				brojPonavljanja += distance(start, end);
			}
		}
		return brojPonavljanja;
	}

	vector<Uspjeh>* GetUspjeh() { return &_uspjeh; }
	string GetEmail() { return _emailadresa; }
	string GetBrojTelefona() { return _brojtelefona; }
	char* GetimePrezime() { return _imeprezime; }

	friend ostream& operator<< (ostream& cout, const Kandidat& obj) {
		cout << obj._imeprezime << " " << obj._emailadresa << " " << obj._brojtelefona << endl;
 
		return cout;
	}
	Uspjeh* operator [](eRazred razred)
	{
		Uspjeh* temp = new Uspjeh(razred);
		for (size_t i = 0; i <_uspjeh.size() ; i++)
		{
			if (*_uspjeh[i].GeteRazred() == razred)
			{
				for (size_t j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++)
				{
					temp->SetPredmeti(_uspjeh[i].GetPredmeti()->getElement1(j), _uspjeh[i].GetPredmeti()->getElement2(j));
				}
			}
		}
		return temp;
	}
	vector<Predmet>& operator ()(const DatumVrijeme& start, const DatumVrijeme& end)
	{
		vector<Predmet>* temp = new vector<Predmet>();
		for (size_t i = 0; i < _uspjeh.size(); i++)
		{
			for (size_t j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++)
			{
				if (_uspjeh[i].GetPredmeti()->getElement2(j) > start && _uspjeh[i].GetPredmeti()->getElement2(j) < end)
				{
					temp->push_back(_uspjeh[i].GetPredmeti()->getElement1(j));
				}
			}
		}
		return *temp;
	}


};

void main() {
	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR AE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEEILO GUBLJENJE URA?ENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKO?ER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIENI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUEAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOANE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAEENO.
	7. IMATE POTPUNU SLOBODU DA U MAIN FUNKCIJI, ZA POTREBE TESTIRANJA, DODAJETE VISE POZIVA ILI SALJETE DRUGE VRIJEDNOSTI PARAMETARA
	****************************************************************************/

	DatumVrijeme temp,
		datum19062019_1015(19, 6, 2019, 10, 15),
		datum20062019_1115(20, 6, 2019, 11, 15),
		datum30062019_1215(30, 6, 2019, 12, 15),
		datum05072019_1231(5, 7, 2019, 12, 31);

	const int kolekcijaTestSize = 9;
	Kolekcija<int, int> kolekcija1(false);
	for (size_t i = 0; i < kolekcijaTestSize; i++)
		kolekcija1.AddElement(i + 1, 20 - i);

	try {
		//ukoliko nije dozvoljeno dupliranje elemenata, metoda AddElement baca izuzetak
		kolekcija1.AddElement(6, 15);
	}
	catch (exception & err) {
		cout << err.what() << endl;
	}
	cout << kolekcija1 << endl;

	//na osnovu vrijednosti parametra sortira clanove kolekcije u rastucem redoslijedu 
	kolekcija1.SortirajRastuci(SortirajPo::T2);
	cout << kolekcija1 << endl;


	Kolekcija<int, int> kolekcija2 = kolekcija1;
	cout << kolekcija2 << crt;

	Kolekcija<int, int> kolekcija3;
	kolekcija3 = kolekcija1;
	cout << kolekcija3 << crt;

	//napomena se moze dodati i prilikom kreiranja objekta
	Predmet Matematika("Matematika", 5, "Ucesce na takmicenju"),
		Fizika("Fizika", 5),
		Hemija("Hemija", 2),
		Engleski("Engleski", 5);
	Fizika.DodajNapomenu("Pohvala za ostvareni uspjeh");
	cout << Matematika << endl;

	/*
	email adresa mora biti u formatu: text@outlook.com ili text@edu.fit.ba
	u slucaju da email adresa nije validna, postaviti je na defaultnu: notSet@edu.fit.ba
	za provjeru koristiti regex
	*/
	Kandidat jasmin("Jasmin Azemovic", "jasmin@outlook.com", "033 281 172");
	Kandidat adel("Adel Handzic", "adel@edu.fit.ba", "033 281 170");
	Kandidat emailNotValid("Ime Prezime", "korisnik@klix.ba", "033 281 170");

	/*
	uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
	tom prilikom onemoguciti:
	- dodavanje istoimenih predmeta na nivou jednog razreda,
	- dodavanje vise predmeta u kratkom vremenskom periodu (na nivou jednog razreda, razmak izmedju dodavanja pojedinih predmeta mora biti najmanje 5 minuta).
	razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za ii razred, pa onda za i razred i sl.).
	funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	*/
	if (jasmin.AddPredmet(DRUGI, Fizika, datum20062019_1115))
		cout << "predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(DRUGI, Hemija, datum30062019_1215))
		cout << "predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(PRVI, Engleski, datum19062019_1015))
		cout << "predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(PRVI, Matematika, datum20062019_1115))
		cout << "predmet uspjesno dodan!" << crt;
	//ne treba dodati matematiku jer je vec dodana u prvom razredu
	if (jasmin.AddPredmet(PRVI, Matematika, datum05072019_1231))
		cout << "predmet uspjesno dodan!" << crt;
	//ne treba dodati fiziku jer nije proslo 5 minuta od dodavanja posljednjeg predmeta
	if (jasmin.AddPredmet(PRVI, Fizika, datum20062019_1115))
		cout << "predmet uspjesno dodan!" << crt;
	/*nakon evidentiranja uspjeha na bilo kojem predmetu kandidatu se salje email sa porukom:
	FROM:info@edu.fit.ba
	TO: emailKorisnika
	Postovani ime i prezime, evidentirali ste uspjeh za X razred.
	Pozdrav.
	FIT Team.

	ukoliko je prosjek na nivou tog razreda veci od 4.5 kandidatu se salje SMS sa porukom: "Svaka cast za uspjeh X.X u X razredu".
	slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
	*/
	cout << "USPJEH ISPISATI KORISTECI OSTREAM_ITERATOR" << endl;
	cout << jasmin << endl;
	//vraca broj ponavljanja odredjene rijeci u napomenama, koristiti sregex_iterator
	cout << "Rijec takmicenje se pojavljuje " << jasmin.BrojPonavljanjaRijeci("takmicenju") << " puta." << endl;

	//vraca niz predmeta koji su evidentiranih u periodu izmedju vrijednosti proslijedjenih parametara
	vector<Predmet> jasminUspjeh = jasmin(DatumVrijeme(18, 06, 2019, 10, 15), DatumVrijeme(21, 06, 2019, 10, 10));
	for (size_t i = 0; i < jasminUspjeh.size(); i++)
		cout << jasminUspjeh[i] << endl;

	Uspjeh* uspjeh_Irazred = jasmin[PRVI];//vraca uspjeh kandidata ostvaren u prvom razredu
	if (uspjeh_Irazred != nullptr)
		cout << *uspjeh_Irazred << endl;

	system("pause>0");

}