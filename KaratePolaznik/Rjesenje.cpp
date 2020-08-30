#include<mutex>
#include<string>
#include<regex>
#include<exception>
#include<iterator>
#include<thread>
#include<iostream>
using namespace std;
const char* crt = "\n-------------------------------------------\n";
enum Pojas { BIJELI, ZUTI, NARANDZASTI, ZELENI, PLAVI, SMEDJI, CRNI };
const char * pojasChar[] = {"BIJELI", "ZUTI"," NARANDZASTI", "ZELENI", "PLAVI", "SMEDJI", "CRNI" };
const int brojTehnika = 6;
const char* NIJE_VALIDNA = "<VRIJEDNOST_NIJE_VALIDNA>";

mutex m;
char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	if (dealociraj)
		delete[]sadrzaj;
	return temp;
}

template<class T1, class T2, int max = 10>
class Kolekcija {
	T1* _elementi1[max] = { nullptr };
	T2* _elementi2[max] = { nullptr };
	int  _trenutno;
public:
	Kolekcija(const Kolekcija& obj):_trenutno(obj._trenutno)
	{
		_trenutno = obj._trenutno;
		for (size_t i = 0; i < obj._trenutno; i++)
		{
			_elementi1[i]  = new T1(*obj._elementi1[i]);
			_elementi2[i]  = new T2(*obj._elementi2[i]);
		}
	}
	Kolekcija() { _trenutno = 0; }
	~Kolekcija() {
		for (size_t i = 0; i < _trenutno; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
	}

	void AddElement(T1 el1, T2 el2)
	{
		if (_trenutno == max)
			throw exception("pokusali ste dodati element u pun niz !");

		_elementi1[_trenutno] = new T1(el1);
		_elementi2[_trenutno++] = new T2(el2);
	}
	void AddElement(T1 el1, T2 el2, int lokacija)
	{
		if (_trenutno == max)
			return;

		_trenutno++;
		lokacija -= 1;
		int b = 0;

		T1* temp1[max] = { nullptr };
		T2* temp2[max] = { nullptr };

		temp1[lokacija] = new T1(el1);
		temp2[lokacija] = new T2(el2);
		for (size_t i = 0; i < _trenutno; i++)
		{
			if (i != lokacija)
			{
				temp1[i] = new T1(*_elementi1[b]);
				temp2[i] = new T2(*_elementi2[b++]);
			}
		}
		
		for (size_t i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = new T1(*temp1[i]);
			_elementi2[i] = new T2(*temp2[i]);
		}

	}
	void RemoveAt(int lokacija)
	{
		lokacija -= 1;
		for (size_t i = lokacija; i < _trenutno - 1; i++)
		{
			_elementi1[i] = _elementi1[i + 1];
			_elementi2[i] = _elementi2[i + 1];
		}
		_trenutno--;
	}

	T1& getElement1(int lokacija)const { return *_elementi1[lokacija]; }
	T2& getElement2(int lokacija)const { return *_elementi2[lokacija]; }
	int getTrenutno() { return _trenutno; }

	friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
	Kolekcija& operator=(const Kolekcija& k) {
		if (this != &k) {
			if (k._trenutno >= max) throw exception("Kolekcije su razlicite");
			_trenutno = k._trenutno;
			for (size_t i = 0; i < k._trenutno; i++)
			{
				delete _elementi1[i];
				delete _elementi2[i];
				_elementi1[i] = new T1(*k._elementi1[i]);
				_elementi2[i] = new T2(*k._element2[i]);
			}
		}
		return *this;
	}
	T2 & operator [](T2 el1)
	{
		for (size_t i = 0; i < _trenutno; i++)
		{
			if (*_elementi1[i] == el1)
			{
				return *_elementi2[i];
			}
		}
	}
};
class Datum {
	int* _dan, * _mjesec, * _godina;
public:
	Datum(const Datum& obj)
	{
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
	}
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}



	friend ostream& operator<< (ostream& COUT, const Datum& obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
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
	int operator -(const Datum& obj)
	{
		if (*_godina < * obj._godina)
			return true;
		if (*_godina == * obj._godina && *_mjesec < *obj._mjesec)
			return true;
		if (*_godina == *obj._godina && *_mjesec == *obj._mjesec)
		{
			return abs(*_dan - *obj._dan);
		}
	} 
	bool operator ==(const Datum& obj)
	{
		return *_dan == *obj._dan &&  *_mjesec == *obj._mjesec && *_godina == *obj._godina;
	}

};

class Tehnika {
	char* _naziv;
	//int se odnosi na ocjenu u opsegu od 1 – 5, a Datum na datum kada je ocijenjena odredjena tehnika 
	Kolekcija<int, Datum, brojTehnika>* _ocjene;
public:
	Tehnika(const Tehnika & obj):_ocjene(new Kolekcija<int, Datum, brojTehnika>(*obj._ocjene)), _naziv(GetNizKaraktera(obj._naziv))
	{
	}
	Tehnika(const char* naziv = nullptr) {
		_naziv = GetNizKaraktera(naziv);
		_ocjene = new Kolekcija<int, Datum, brojTehnika>();
	}
	~Tehnika() {
		delete[] _naziv; _naziv = nullptr;
		delete _ocjene; _ocjene = nullptr;
	}

	bool AddOcjena(int ocjena, Datum datum)
	{
		for (size_t i = 0; i < _ocjene->getTrenutno(); i++)
		{
			if ((_ocjene->getElement2(i) - datum) < 3 || _ocjene->getElement2(i) == datum)
			return false;
		}		
		_ocjene->AddElement(ocjena, datum);
		return true;
	}
	
	char* GetNaziv() { return _naziv; }
	Kolekcija<int, Datum, brojTehnika>& GetOcjene() { return *_ocjene; }
	friend ostream& operator <<(ostream& c, const Tehnika& obj)
	{
		float prosjek = 0;
		int brojac = 0;
		cout << "naziv tehnike -----> " << obj._naziv << endl;
		for (size_t i = 0; i < obj._ocjene->getTrenutno(); i++)
		{
			prosjek += obj._ocjene->getElement1(i);
			brojac++;
		}
		if (brojac > 0)
			prosjek /= float(brojac);
		cout << "prosjek tehnike je ---> " << prosjek << endl;
		cout << "ocjena i datim tehnike : " << endl;
		cout<< *obj._ocjene << endl;
		return c;
	}
	bool operator == (const Tehnika& t) {
		return strcmp(_naziv, t._naziv) == 0;
	}
};

class Polaganje {
	Pojas _pojas;
	vector<Tehnika*> _polozeneTehnike;
public:
	
	Polaganje(const Polaganje & obj):_pojas(obj._pojas)
	{
	for (size_t i = 0; i < obj._polozeneTehnike.size(); i++)
		_polozeneTehnike.push_back(new Tehnika(*obj._polozeneTehnike[i]));

	}
	Polaganje(Pojas pojas = BIJELI) {
		_pojas = pojas;
	}
	~Polaganje() {
		for (size_t i = 0; i < _polozeneTehnike.size(); i++) {
			delete _polozeneTehnike[i];
			_polozeneTehnike[i] = nullptr;
		}
	
	}

	void AddTehnika(Tehnika tehnika)
	{
		_polozeneTehnike.push_back(new Tehnika(tehnika));
	}

	vector<Tehnika*> GetTehnike() { return _polozeneTehnike; }
	Pojas GetPojas() { return _pojas; }
	friend ostream& operator<< (ostream& COUT, const Polaganje& obj) {
		COUT << obj._pojas << endl;
		for (size_t i = 0; i < obj._polozeneTehnike.size(); i++)
			COUT << *obj._polozeneTehnike[i];
		return COUT;
	}
};
bool ValidirajLozinku(string lozinka) {
	return regex_search(lozinka, regex("(?=.*[A-Z])(?=.*[a-z])(?=.*[0-9])(?=.*[@\\-*]+)(?=.{7,})"));
}
class Korisnik {
	char* _imePrezime;
	string _emailAdresa;
	string _lozinka;
public:
	Korisnik(const char* imePrezime = nullptr, string emailAdresa = "", string lozinka = "VRIJEDNOST_NIJE_VALIDNA")
	{
		_imePrezime = GetNizKaraktera(imePrezime);
		_emailAdresa = emailAdresa;
		_lozinka = ValidirajLozinku(lozinka) ? lozinka : NIJE_VALIDNA;
	}
	virtual ~Korisnik() { delete[] _imePrezime; _imePrezime = nullptr; }
	string GetEmail() { return _emailAdresa; }
	string GetLozinka() { return _lozinka; }
	char* GetImePrezime() { return _imePrezime; }
};
bool ValidirajPojas(Polaganje _polozeniPojasevi, Pojas pojas)
{
	float prosjek = 0;
	int brojac = 0;
	int brojTehnika = 0;
	
	if (pojas == ZUTI)
		return true;

		for (size_t j = 0; j < _polozeniPojasevi.GetTehnike().size(); j++)
		{
			for (size_t k = 0; k < _polozeniPojasevi.GetTehnike()[j]->GetOcjene().getTrenutno(); k++)
			{
				prosjek += _polozeniPojasevi.GetTehnike()[j]->GetOcjene().getElement1(k);
				brojac++;
			}
		}
	
	if (pojas == NARANDZASTI && _polozeniPojasevi.GetPojas() == ZUTI)
	{
		for (size_t j = 0; j < _polozeniPojasevi.GetTehnike().size(); j++)
			brojTehnika++;
	}
	if (pojas == ZELENI && _polozeniPojasevi.GetPojas() == NARANDZASTI)
	{
		for (size_t j = 0; j < _polozeniPojasevi.GetTehnike().size(); j++)
			brojTehnika++;
	}
	if (pojas == PLAVI && _polozeniPojasevi.GetPojas() == SMEDJI)
	{
		for (size_t j = 0; j < _polozeniPojasevi.GetTehnike().size(); j++)
			brojTehnika++;

	}
	if (pojas == CRNI && _polozeniPojasevi.GetPojas() == SMEDJI)
	{
		for (size_t j = 0; j < _polozeniPojasevi.GetTehnike().size(); j++)		
			brojTehnika++;
	}
	if (brojac > 0)
		prosjek /= float(brojac);

	if (prosjek > 3.5 || brojTehnika >= 3)
		return true;
}

class KaratePolaznik : public Korisnik {
	vector<Polaganje> _polozeniPojasevi;
public:
	KaratePolaznik(const char* imePrezime, string emailAdresa, string lozinka):Korisnik(imePrezime, emailAdresa, lozinka) {
	}
	~KaratePolaznik() {
		cout << crt << "DESTRUKTOR -> KaratePolaznik" << crt;
	}

	/*
sve tehnike na nivou jednog pojasa (ZUTI, ZELENI ... ) se evidentiraju unutar istog objekta tipa Polaganje,
tom prilikom onemoguciti:
- dodavanje istih (moraju biti identicne vrijednosti svih atributa) tehnika na nivou jednog pojasa,
- dodavanje tehnika za visi pojas ako prethodni pojas nema evidentirane najmanje 3 tehnike ili nema prosjecnu ocjenu svih tehnika vecu od 3.5
(onemoguciti dodavanje tehnike za NARANDZASTI ako ne postoji najmanje 3 tehnike za ZUTI pojas ili njihov prosjek nije veci od 3.5)
funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja 
*/
	bool AddTehniku(Pojas pojas, Tehnika & tehnika)
	{
		float UspjehP = 0, UspjehU = 0;
		int bP = 0, bU = 0;
		
		for (size_t i = 0; i < _polozeniPojasevi.size(); i++)
		{
			if (_polozeniPojasevi[i].GetPojas() == pojas)
			{
				for (size_t j = 0; j < _polozeniPojasevi[i].GetTehnike().size(); j++)
				{
					if (strcmp(_polozeniPojasevi[i].GetTehnike()[j]->GetNaziv(), tehnika.GetNaziv()) == 0)
						return false;
				}
			}
		}
		for (size_t i = 0; i < _polozeniPojasevi.size(); i++)
		{
			if (!ValidirajPojas(_polozeniPojasevi[i], pojas))
				return false;
		}

		Polaganje polaganje(pojas);
		polaganje.AddTehnika(tehnika);
		_polozeniPojasevi.push_back(polaganje);

		for (size_t i = 0; i < tehnika.GetOcjene().getTrenutno(); i++)
		{
			UspjehP += tehnika.GetOcjene().getElement1(i);
			bP++;
		}
		for (size_t i = 0; i < _polozeniPojasevi.size(); i++)
		{
			for (size_t j = 0; j < _polozeniPojasevi[i].GetTehnike().size(); j++)
			{
				for (size_t k = 0; k < _polozeniPojasevi[i].GetTehnike()[j]->GetOcjene().getTrenutno(); k++)
				{
					UspjehU += _polozeniPojasevi[i].GetTehnike()[j]->GetOcjene().getElement1(k);
					bU++;
				}
			}
		
		}
		if (bP > 0)
			UspjehP /= float(bP);
		if (bU > 0)
			UspjehU /= float(bU);


		thread t([this,pojas,&UspjehP,&UspjehU,tehnika]() {
			m.lock();
			cout << crt;
			cout << "FROM : info@karate.ba<<endl" << endl;
			cout << "TO :"<< GetEmail() << endl;
			cout << " Postovani "<< GetImePrezime() << endl;
			cout << " evidentirana vam je thenika" << tehnika << "za " <<pojasChar[ pojas]<< " pojas."<<endl;
		    cout<<" Dosadasnji uspjeh(prosjek ocjena)" << endl;
			cout << "na pojasu " << pojasChar[pojas] << " iznosi" << UspjehP << endl;
			cout<< ", a ukupni uspjeh(prosjek ocjena) na svim pojasevima iznosi " << UspjehU << endl;
		    cout<<"Pozdrav. "<< endl;
		    cout<<"KARATE Team."<<endl;
			cout << crt;
			m.unlock();
			});
		t.join();
		

		return true;
	}
	friend ostream& operator<< (ostream& COUT, KaratePolaznik& obj) {
		COUT << obj.GetImePrezime() << " " << obj.GetEmail() << " " << obj.GetLozinka() << endl;
		for (size_t i = 0; i < obj._polozeniPojasevi.size(); i++)
			COUT << obj._polozeniPojasevi[i];
		return COUT;
	}
	vector<Polaganje>& GetPolozeniPojasevi() { return _polozeniPojasevi; }
};

void main() {


  Datum datum19062020(19, 6, 2020),
		datum20062020(20, 6, 2020),
		datum30062020(30, 6, 2020),
		datum05072020(5, 7, 2020);

	int kolekcijaTestSize = 10;

	Kolekcija<int, int> kolekcija1;
	for (int i = 0; i < kolekcijaTestSize; i++)
		kolekcija1.AddElement(i, i);

	cout << kolekcija1 << endl;

	try {
		/*metoda AddElement baca izuzetak u slucaju da se pokusa
		dodati vise od maksimalnog broja elemenata*/
		kolekcija1.AddElement(11, 11);
	}
	catch (exception & err) {
		cout << crt << "Greska -> " << err.what() << crt;
	}
	cout << kolekcija1 << crt;


	kolekcija1.RemoveAt(2);
	/*uklanja par (T1 i T2) iz kolekcije koji se nalazi na lokaciji sa proslijedjenim indeksom.
	nakon uklanjanja vrijednosti onemoguciti pojavu praznog prostora unutar kolekcije tj.
	pomjeriti sve elemente koji se nalaze nakon proslijedjene lokacije za jedno mjesto unazad
	npr. ako unutar kolekcije postoje elementi
	0 0
	1 1
	2 2
	3 3
	nakon uklanjanja vrijednosti na lokaciji 1, sadrzaj kolekcije ce biti sljedeci
	0 0
	2 2
	3 3
	*/

	cout << kolekcija1 << crt;

	kolekcija1.AddElement(9, 9, 2);
	/*funkciji AddElement se, kao treci parametar, moze proslijediti i lokacija na koju se dodaju
	nove vrijednosti pri cemu treba zadrzati postojece vrijednosti pomjerene za jedno mjesto unaprijed
	u odnosu na definisanu lokaciju npr. ako unutar kolekcije postoje elementi
	0 0
	1 1
	2 2
	3 3
	nakon dodavanja vrijednosti 9 i 9 na lokaciju 1, sadrzaj kolekcije ce biti sljedeci
	0 0
	9 9
	1 1
	2 2
	3 3
	//*/

	cout << kolekcija1 << crt;

	Kolekcija<int, int> kolekcija2 = kolekcija1;
	cout << kolekcija1 << crt;

	//na osnovu vrijednosti T1 mijenja vrijednost T2. 
	kolekcija1[9] = 2;
	cout << kolekcija1 << crt;
	/* npr.ako unutar kolekcije postoje elementi:
	0 0
	9 9
	1 1
	2 2
	3 3
	nakon promjene vrijednosti sadrzaj kolekcije ce biti sljedeci
	0 0
	9 2
	1 1
	2 2
	3 3
	*/

	Tehnika choku_zuki("choku_zuki"),
		gyaku_zuki("gyaku_zuki"),
		kizami_zuki("kizami_zuki"),
		oi_zuki("oi_zuki");

	/*svaka tehnika moze imati vise ocjena tj. moze se polagati u vise navrata.
		-   razmak izmedju polaganja dvije tehnike mora biti najmanje 3 dana AddOcjena
		-   nije dozvoljeno dodati ocjenu sa ranijim datumom u odnosu na vec evidentirane (bez obzira sto je stariji od 3 dana)
	*/
	if (choku_zuki.AddOcjena(1, datum19062020))
		cout << "Ocjena evidentirana!" << endl;
	if (!choku_zuki.AddOcjena(5, datum20062020))
		cout << "Ocjena NIJE evidentirana!" << endl;
	if (choku_zuki.AddOcjena(5, datum30062020))
		cout << "Ocjena evidentirana!" << endl;

	/* ispisuje: naziv tehnike, ocjene (zajedno sa datumom) i prosjecnu ocjenu za tu tehniku
	   ukoliko tehnika nema niti jednu ocjenu prosjecna treba biti 0*/
	cout << choku_zuki << endl;

	if (ValidirajLozinku("john4Do*e"))
		cout << "OK" << crt;
	if (!ValidirajLozinku("john4Doe"))
		cout << "Specijalni znak?" << crt;
	if (!ValidirajLozinku("jo*4Da"))
		cout << "7 znakova?" << crt;
	if (!ValidirajLozinku("4jo-hnoe"))
		cout << "Veliko slovo?" << crt;
	if (ValidirajLozinku("@john2Doe"))
		cout << "OK" << crt;

	/*
	za autentifikaciju svaki korisnik mora posjedovati lozinku koja sadrzi:
	-   najmanje 7 znakova
	-   velika i mala slova
	-   najmanje jedan broj
	-   najmanje jedan specijalni znak
	za provjeru validnosti lozinke koristiti globalnu funkciju ValidirajLozinku, a unutar nje regex metode.
	validacija lozinke se vrsi unutar konstruktora klase Korisnik, a u slucaju da nije validna
	postaviti je na podrazumijevanu vrijednost: <VRIJEDNOST_NIJE_VALIDNA>
	*/

	Korisnik* jasmin = new KaratePolaznik("Jasmin Azemovic", "jasmin@karate.ba", "j@sm1N*");
	Korisnik* adel = new KaratePolaznik("Adel Handzic", "adel@edu.karate.ba", "4Ade1*H");
	Korisnik* emailNijeValidan = new KaratePolaznik("John Doe", "john.doe@google.com", "johndoe");

	/*
	sve tehnike na nivou jednog pojasa (ZUTI, ZELENI ... ) se evidentiraju unutar istog objekta tipa Polaganje,
	tom prilikom onemoguciti:
	- dodavanje istih (moraju biti identicne vrijednosti svih atributa) tehnika na nivou jednog pojasa,
	- dodavanje tehnika za visi pojas ako prethodni pojas nema evidentirane najmanje 3 tehnike ili nema prosjecnu ocjenu svih tehnika vecu od 3.5
	(onemoguciti dodavanje tehnike za NARANDZASTI ako ne postoji najmanje 3 tehnike za ZUTI pojas ili njihov prosjek nije veci od 3.5)
	funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja 
	*/

	//doraditi klase da nacin da omoguce izvrsenje naredne linije koda
	KaratePolaznik* jasminPolaznik = dynamic_cast<KaratePolaznik*>(jasmin);

	if (jasminPolaznik != nullptr) {
		if (jasminPolaznik->AddTehniku(ZUTI, gyaku_zuki))
			cout << "Tehnika uspjesno dodan!" << crt;
		//ne treba dodati kizami_zuki jer ne postoje evidentirane 3 tehnike za ZUTI pojas
		if (!jasminPolaznik->AddTehniku(NARANDZASTI, kizami_zuki))
			cout << "Tehnika NIJE uspjesno dodana!" << crt;
		if (jasminPolaznik->AddTehniku(ZUTI, choku_zuki))
			cout << "Tehnika uspjesno dodan!" << crt;
		//ne treba dodati choku_zuki jer je vec dodana za zuti pojas
		if (!jasminPolaznik->AddTehniku(ZUTI, choku_zuki))
			cout << "Tehnika NIJE uspjesno dodana!" << crt;

		//ispisuje sve dostupne podatke o karate polazniku
		cout << *jasminPolaznik << crt;
	}

	/*nakon evidentiranja tehnike na bilo kojem pojasu kandidatu se salje email sa porukom:
	FROM:info@karate.ba
	TO: emailKorisnika
	Postovani ime i prezime, evidentirana vam je thenika X za Y pojas. Dosadasnji uspjeh (prosjek ocjena)
	na pojasu Y iznosi F, a ukupni uspjeh (prosjek ocjena) na svim pojasevima iznosi Z.
	Pozdrav.
	KARATE Team.
	slanje email poruka implemenitrati koristeci zasebne thread-ove.
	*/

	//osigurati da se u narednim linijama poziva i destruktor klase KaratePolaznik
	delete jasmin;
	delete adel;
	delete emailNijeValidan;

	system("pause>0");
}