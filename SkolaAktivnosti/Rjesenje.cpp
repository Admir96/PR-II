#include<string>
#include<regex>
#include<mutex>
#include<thread>
#include<exception>
#include<iostream>
#include<tuple>
using namespace std;

/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
****************************************************************************/

const char* nedozvoljena_operacija = "Nedozvoljena operacija";
const char* not_set = "NOT_SET";
const char* crt = { "\n--------------------------------------------\n" };
enum Sortiranje { ASC, DESC };
enum Predmet { MATEMATIKA, FIZIKA, HEMIJA, GEOGRAFIJA, NOT_SET };
enum Razred { I = 1, II, III, IV };
const char * razred[] = {"I", "II", "III", "IV" };
mutex m;
char* AlocirajNizKaraktera(const char* sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}
template <class T1, class T2, int max>
class Kolekcija {
	T1* _elementi1[max] = { nullptr };
	T2* _elementi2[max] = { nullptr };
	int _trenutnoElemenata;
	bool _dozvoliDupliranje;
	Sortiranje _sortiranje;
public:
	Kolekcija(const Kolekcija& obj)
	{
		_trenutnoElemenata = obj._trenutnoElemenata;
		_dozvoliDupliranje = obj._dozvoliDupliranje;
		_sortiranje = obj._sortiranje;

		for (size_t i = 0; i < obj._trenutnoElemenata; i++)
		{
			_elementi1[i] = new T1(*obj._elementi1[i]);
			_elementi2[i] = new T2(*obj._elementi2[i]);
		}
    }
	Kolekcija(Sortiranje sortiranje = ASC, bool dozvoliDupliranje = true) {
		_trenutnoElemenata = 0;
		_sortiranje = sortiranje;
		_dozvoliDupliranje = dozvoliDupliranje;
	}
	~Kolekcija() {
		for (size_t i = 0; i < _trenutnoElemenata; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
	}

	void AddElement(T1 el1, T2 el2)
	{
		if (!_dozvoliDupliranje)
		{
			for (size_t i = 0; i < _trenutnoElemenata; i++)
			{
				if (*_elementi1[i] == el1 && *_elementi2[i] == el2)
					throw exception("nedozovljeno dupliranje");
			}
		}

		if (_trenutnoElemenata == max)
			throw exception("prekoracili ste maksimalan broj elemenata !");

		_elementi1[_trenutnoElemenata] = new T1(el1);
		_elementi2[_trenutnoElemenata++] = new T2(el2);

		bool provjera = true;
		if (_sortiranje == ASC)
		{
			while (provjera)
			{
				provjera = false;
				for (size_t i = 0; i < _trenutnoElemenata - 1; i++)
				{
					if (*_elementi1[i] > *_elementi1[i + 1])
					{
						swap(_elementi1[i], _elementi1[i + 1]);
						swap(_elementi2[i], _elementi2[i + 1]);
						provjera = true;
					}
				}
			}
		}
		if(_sortiranje == DESC)
		{

			while (provjera)
			{
				provjera = false;
				for (size_t i = 0; i < _trenutnoElemenata - 1; i++)
				{
					if (*_elementi1[i] < *_elementi1[i + 1])
					{
						swap(_elementi1[i], _elementi1[i + 1]);
						swap(_elementi2[i], _elementi2[i + 1]);
						provjera = true;
					}
				}
			}
		}
	}

	int GetTrenutno() const { return _trenutnoElemenata; }
	int GetMax() const { return max; }

	T1& GetElement1(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception(nedozvoljena_operacija);
		return *_elementi1[lokacija];
	}
	T2& GetElement2(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception(nedozvoljena_operacija);
		return *_elementi2[lokacija];
	}
	void SetElement2(int lokacija, T2 ele2) {
		_elementi2[lokacija] = ele2;
	}

	friend ostream& operator<<(ostream & COUT, const Kolekcija & obj) {
		for (size_t i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj.GetElement1(i) << " " << obj.GetElement2(i) << endl;
		return COUT;
	}

};

class Aktivnost {
	shared_ptr<Razred> _razred;
	string _opis;
	int _ocjena;//1-5
public:
	Aktivnost(const Aktivnost& obj) :_razred(obj._razred), _opis(obj._opis), _ocjena(obj._ocjena){}
	Aktivnost(Razred razred = I, int ocjena = 0, string opis = not_set) {
		_ocjena = ocjena;
		_opis = opis;
		_razred = make_shared<Razred>(razred);
	}



	int  GetOcjenu() const { return _ocjena; }
	string GetOpis() const { return _opis; }
	Razred GetRazred() const { return *_razred; }

	friend ostream& operator<<(ostream& COUT, const Aktivnost& obj) {
		COUT << *obj._razred << " " << obj._ocjena << " " << obj._opis << endl;
		return COUT;
	}
	Aktivnost& operator =(const Aktivnost& obj)
	{
		if (this != &obj)
		{
		 _razred = obj._razred;
			_opis = obj._opis;
			_ocjena = obj._ocjena;
		}
		return *this;
	}
	bool operator ==(const Aktivnost& obj)
	{
		return _opis == obj._opis;
	}
};

class Polaznik {
protected:
	char* _imePrezime;
	string _brojTelefona;
public:
	Polaznik(string imePrezime ="", string brojTelefona = "") : _imePrezime(AlocirajNizKaraktera(imePrezime.c_str())) {
		_brojTelefona = brojTelefona;
	}
	~Polaznik() { delete[] _imePrezime; }



	char* GetImePrezime() { return _imePrezime; }
	string GetTelefon() { return _imePrezime; }
	virtual void PredstaviSe() = 0;
	friend ostream& operator <<(ostream & c,const Polaznik& obj)
	{
		cout << crt;
		cout << "ime Polaznika -- > " << obj._imePrezime << endl;
		cout << "broj telefona Polaznika -- > " << obj._brojTelefona << endl;
		cout << crt;
		return c;
	}
};

class Ucenik:public Polaznik {
	Kolekcija<Predmet, Aktivnost, 16>* _aktivnosti;
public:
	Ucenik(const Ucenik& obj):_aktivnosti(new Kolekcija<Predmet, Aktivnost, 16>(*obj._aktivnosti)),Polaznik(obj._imePrezime,obj._brojTelefona){}
	Ucenik(string imeUcenika = "",string brojTelefona = ""):Polaznik(imeUcenika.c_str(),brojTelefona)
	{
		_aktivnosti = new Kolekcija<Predmet, Aktivnost, 16>;
	}
	~Ucenik() { delete _aktivnosti; _aktivnosti = nullptr; }



	Kolekcija<Predmet, Aktivnost, 16>& GetAktivnosti() { return *_aktivnosti; };
	void PredstaviSe()
	{
		cout << *this << endl;
	}

	friend ostream& operator<<(ostream& COUT,const Ucenik& obj)
	{
		cout << crt;
		cout << "ime Ucenika -- > " << obj._imePrezime << endl;
		cout << "broj telefona Ucenika -- > " << obj._brojTelefona << endl;
		cout << "aktivnosti Ucenika : " << endl;
		cout << *obj._aktivnosti << endl;
		cout << crt;
		return COUT;
	}
};
class Skola {
	char* _naziv;
	vector<Ucenik> _ucenici;
public:
	Skola(const char* naziv = nullptr) {
		_naziv = AlocirajNizKaraktera(naziv);
	}
	~Skola() {
		delete[] _naziv; _naziv = nullptr;
	}

	bool DodajAktivnost(string imeUcenika, Predmet predmet, Aktivnost aktivnost)
	{
		bool provjera = true;
		int bI = 0;
		for (size_t i = 0; i < _ucenici.size(); i++)
		{
			if (strcmp(_ucenici[i].GetImePrezime(), imeUcenika.c_str()) == 0)
			{
				for (size_t j = 0; j < _ucenici[i].GetAktivnosti().GetTrenutno(); j++)
				{
					if (_ucenici[i].GetAktivnosti().GetElement2(j).GetRazred() == aktivnost.GetRazred() && _ucenici[i].GetAktivnosti().GetElement1(j) == predmet)
						return false;
					if (_ucenici[i].GetAktivnosti().GetElement2(j).GetRazred() == aktivnost.GetRazred())
						bI++;
				}
			}
			if (bI == 4)
				return false;
		}

		for (size_t i = 0; i < _ucenici.size(); i++)
		{
			if (strcmp(_ucenici[i].GetImePrezime(), imeUcenika.c_str()) == 0)
			{
				_ucenici[i].GetAktivnosti().AddElement(predmet, aktivnost);
				provjera = true;
			}
		}	
		if (provjera == true)
		{
			thread t([this, aktivnost]() {
				m.lock();
				cout << "uspjesno ste okoncali u okviru razreda " << razred[aktivnost.GetRazred()] << endl;
				cout << "sa prosjecnom ocjenom : " << aktivnost.GetOcjenu() << endl;
				m.unlock();
				});
			t.join();
		}
		return true;
	}
	pair <Polaznik*, float> GetNajboljegUcenika()
	{
		float prosjecnaOcjena = 0;
		float prosjecnaOcjenaMax = 0;
		int brojac = 0;
		Polaznik* temp = nullptr;

		for (size_t i = 0; i < _ucenici.size(); i++)
		{
			for (size_t j = 0; j < _ucenici[i].GetAktivnosti().GetTrenutno(); j++)
			{
				prosjecnaOcjena += _ucenici[i].GetAktivnosti().GetElement2(j).GetOcjenu();
				brojac++;
			}
			if(brojac > 0)
			prosjecnaOcjena /= float(brojac);
			if (prosjecnaOcjena > prosjecnaOcjenaMax)
			{
				prosjecnaOcjenaMax = prosjecnaOcjena;
				temp = &_ucenici[i];
				brojac = 0;
			}
		}
		return make_pair(temp, prosjecnaOcjenaMax);
	}

	char* GetNaziv()const { return _naziv; }
	vector<Ucenik>& GetUcenici() { return _ucenici; };

	friend ostream& operator<<(ostream& COUT,const Skola& obj) {
		COUT << "Skola: " << obj._naziv << endl;
		COUT << "Ucenici: " << endl;
		for (size_t i = 0; i < obj._ucenici.size(); i++)
			COUT << obj._ucenici[i] << endl;
		return COUT;
	}
	void operator()(string imeUcenika, string brojTelefona)
	{
		//koristeci regex, osigurati sljedeci format za broj telefona: +387(6X)XXX-XXX ili +387 6X XXX-XXX
	  //onemoguciti pojavljivanje samo jedne zagrade, a ukoliko format nije adekvatna koristiti vrijednost not_set
		bool provjera = false;
		if (regex_match(brojTelefona, regex("([+]387([(]6[0-9]{1}[)])[0-9]{3}-[0-9]{3})")) ||
			regex_match(brojTelefona, regex("([+]387 (6[0-9]{1}) [0-9]{3}-[0-9]{3})")))
			provjera = true;

		if (provjera == false)
			brojTelefona = "not_set";

		for (size_t i = 0; i < _ucenici.size(); i++)
		{
			if (strcmp(_ucenici[i].GetImePrezime(), imeUcenika.c_str()) == 0 || _ucenici[i].GetTelefon() == brojTelefona)
				throw exception("pokusali ste dodati ucenika sa istim brojem telefona !");
		}
		Ucenik* temp = new Ucenik(imeUcenika, brojTelefona);
		_ucenici.push_back(*temp);
	}


};

void main() {
	Kolekcija<int, int, 10> kolekcija1(DESC, false);
	try {
		kolekcija1.AddElement(1, 2);
		//dupliranje elemenata nije dozvoljeno
		kolekcija1.AddElement(1, 2);
	}
	catch (exception & ex) {
		cout << ex.what();
	}


		/*nakon svakog dodavanja, elemente sortirati prema T1 i vrijednosti atributa _sortiranje*/
	for (size_t i = 1; i < kolekcija1.GetMax() - 1; i++)
		kolekcija1.AddElement(rand(), rand());
	
	  cout << kolekcija1 << endl;

	try {
		//prekoracen maksimalan broj elemenata
		kolekcija1.AddElement(rand(), rand());
	}
	catch (exception & ex) {
		cout << ex.what() << endl;
	}
	cout << kolekcija1 << endl;

	Kolekcija<int, int, 10> kolekcija2(kolekcija1);
	cout << kolekcija2 << endl;

	Skola gimnazijaMostar("GIMNAZIJA MOSTAR");
	//dodaje novog ucenika u skolu
	gimnazijaMostar("Jasmin Azemovic", "+387(61)111-222");
	gimnazijaMostar("Adel Handzic", "+387(61)333-444");

	/*
	koristeci regex, osigurati sljedeci format za broj telefona: +387(6X)XXX-XXX ili +387 6X XXX-XXX
	onemoguciti pojavljivanje samo jedne zagrade, a ukoliko format nije adekvatna koristiti vrijednost not_set
	*/
	gimnazijaMostar("Telefon NotValidFormat", "387 61)333-444");

	try
	{
		/*onemoguciti dodavanje ucenika sa istim imenom i prezimenom ili brojem telefona*/
		gimnazijaMostar("Adel Handzic", "+387(61)333-444");
	}
	catch (exception & ex)
	{
		cout << ex.what() << endl;
	}

	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", MATEMATIKA, Aktivnost(I, 4, "Priprema za takmicenje iz Matematije koje se odrzava u Konjicu 07.02.2019")))
		cout << "Aktivnost uspjesno dodana" << endl;
	/*na nivou svakog razreda se mogu evidentirati maksimalno 4 aktivnosti, a takodjer, na nivou razreda se ne smiju ponavljati aktivnosti iz istog predmeta*/
	if (!gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", MATEMATIKA, Aktivnost(I, 4, "Aktivnosti iz matematike")))
		cout << "Aktivnost nije uspjesno dodana" << endl;
	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", HEMIJA, Aktivnost(I, 5, "Priprema otopina za vjezbe iz predmeta Hemija")))
		cout << "Aktivnost uspjesno dodana" << endl;
	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", FIZIKA, Aktivnost(I, 4, "Analiza stepena apsorpcije materijala ")))
		cout << "Aktivnost uspjesno dodana" << endl;
	/*u slucaju da je ucenik uspjesno (ocjenom vecom od 1) realizovao aktivnosti na nivou odredjenog razreda, te posjeduje validan broj telefona,
	u okviru zasebnog thread-a se salje SMS sa sadrzajem "Uspjesno ste okoncali aktivnosti u okviru X razreda sa prosjecnom ocjenom X.X"*/
	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", GEOGRAFIJA, Aktivnost(I, 4, "Izrada 5 reljefa Mostara")))
		cout << "Aktivnost uspjesno dodana" << endl;
	if (gimnazijaMostar.DodajAktivnost("Adel Handzic", MATEMATIKA, Aktivnost(I, 5, "Izrada skripte na temu integralni racun")))
		cout << "Aktivnost uspjesno dodana" << endl;

	//ispisuje sve ucenike i njihove aktivnosti
	cout << gimnazijaMostar << endl;


	pair<Polaznik*, float> par = gimnazijaMostar.GetNajboljegUcenika();
	cout << "Najbolji ucenik je " << par.first->GetImePrezime() << " sa prosjekom " << par.second << endl;

	system("pause>0");

}