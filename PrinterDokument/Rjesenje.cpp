#include<iostream>
#include<string>
#include<regex>
#include<vector>
#include<mutex>
#include<thread>
#include<exception>
#include<chrono>
using namespace std;
const char* crt = "\n---------------------------------------\n";
mutex m;
class DatumVrijeme {
	int* _dan, * _mjesec, * _godina, * _sati, * _minuti;
public:
	DatumVrijeme(const DatumVrijeme& obj)
	{
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
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti;
		return COUT;
	}
	DatumVrijeme& operator =(const DatumVrijeme& obj)
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
	bool operator ==(const DatumVrijeme& obj)
	{
		return *_dan == *obj._dan && *_mjesec == *obj._mjesec && *_godina == *obj._godina && *_sati == *obj._sati;
	}
};

template<class T1, class T2 = int>
class FITKolekcija {
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
public:
	FITKolekcija(const FITKolekcija& obj)
	{
		_trenutno = obj._trenutno;
		delete[] _elementi1;
		delete[] _elementi2;
		_elementi1 = new T1[obj._trenutno];
		_elementi2 = new T2[obj._trenutno];
		for (size_t i = 0; i < obj._trenutno; i++)
		{
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
	}
	FITKolekcija() {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = 0;
	}
	~FITKolekcija() {
		delete[] _elementi1; _elementi1 = nullptr;
		delete[] _elementi2; _elementi2 = nullptr;
	}

	void Dodaj(T1 el1, T2 el2)
	{
		T1* temp1 = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];
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
	void Sortiraj()
	{
		bool provjera = true;
		while(provjera)
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
	void DodajNaLokaciju(T1 el1, T2 el2, int lokacija)
	{
		lokacija = lokacija - 1;
		int b = 0;
		T1* temp1 = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];
		temp1[lokacija] = el1;
		temp2[lokacija] = el2;

		for (size_t i = 0; i < _trenutno + 1; i++)
		{
			if (i != lokacija)
			{
				temp1[i] = _elementi1[b];
				temp2[i] = _elementi2[b++];
			}
		}
	
		delete[] _elementi1;
		delete[] _elementi2;
		_trenutno++;
		_elementi1 = temp1;
		_elementi2 = temp2;
	}
	FITKolekcija *GetJedinstveni()
	{
		FITKolekcija* temp = new FITKolekcija;
		bool provjera = false;
		for (size_t i = 0; i < _trenutno; i++)
		{
			for (size_t j = i; j < _trenutno; j++)
			{
				if (_elementi1[i] == _elementi1[j] && _elementi2[i] == _elementi2[j] && i != j)
				{
					provjera = true;
				}
			}
			if (provjera == false)
			{
				temp->Dodaj(_elementi1[i], _elementi2[i]);
			}
			provjera = false;
		}
		return temp;
	}

	T1* GetT1() { return _elementi1; }
	T2* GetT2() { return _elementi2; }
	T1 GetElement1(int lokacija) { return _elementi1[lokacija]; }
	T2 GetElement2(int lokacija) { return _elementi2[lokacija]; }

	int GetTrenutno() { return _trenutno; }

	friend ostream& operator<< (ostream& COUT,const FITKolekcija& obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj._elementi1[i] << " " << obj._elementi2[i] << endl;
		return COUT;
	}
	FITKolekcija& operator =(const FITKolekcija& obj)
	{
		if (this != &obj)
		{
			_trenutno = obj._trenutno;
			delete[] _elementi1;
			delete[] _elementi2;
			_elementi1 = new T1[obj._trenutno];
			_elementi2 = new T2[obj._trenutno];
			for (size_t i = 0; i < obj._trenutno; i++)
			{
				_elementi1[i] = obj._elementi1[i];
				_elementi2[i] = obj._elementi2[i];
			}
		}
		return *this;
	}
};

enum vrstaDokumenta { PDF, DOC, TXT, HTML };

class Dokument {
	vrstaDokumenta _vrsta;
	string _naziv;
	string _sadrzaj;
	int _brojStranica;
public:
	Dokument(const Dokument& obj):_vrsta(obj._vrsta),_naziv(obj._naziv),_sadrzaj(obj._sadrzaj),_brojStranica(obj._brojStranica) {}
	Dokument() { _brojStranica = 0; }
	Dokument(vrstaDokumenta vrsta, string naziv) {
		_naziv = naziv; _vrsta = vrsta; _brojStranica = 0;
	}

	void DodajSadrzaj(string sadrzaj)
	{
		_sadrzaj += sadrzaj + " ";
		_brojStranica = _sadrzaj.length() / 30;
	}
	string GetNaziv() { return _naziv; };
	string GetSadrzaj() { return _sadrzaj; };
	vrstaDokumenta GetVrsta() { return _vrsta; };
	void UvecajBrojStranica() { _brojStranica++; }
	int GetBrojStranica() { return _brojStranica; }

	friend ostream& operator<<(ostream& COUT, const Dokument& obj) {
		COUT << obj._naziv << "." << obj._vrsta << " (" << obj._brojStranica << ")\n" << obj._sadrzaj << endl;
		return COUT;
	}
	Dokument& operator =(const Dokument & obj)
	{
		if (this != &obj)
		{
			_vrsta = obj._vrsta;
			_naziv = obj._naziv;
			_sadrzaj = obj._sadrzaj;
			_brojStranica = obj._brojStranica;
		}
		return *this;
	}
	bool operator ==(const Dokument& obj)
	{
		return strcmp(_naziv.c_str(), obj._naziv.c_str()) == 0;
	}

};
class Uredjaj {
protected:
	char* _proizvodjac;
	char* _model;
public:
	Uredjaj(const Uredjaj& obj)
	{
		int vel = strlen(obj._proizvodjac) + 1;
		_proizvodjac = new char[vel];
		strcpy_s(_proizvodjac, vel, obj._proizvodjac);

		vel = strlen(obj._model) + 1;
		_model = new char[vel];
		strcpy_s(_model, vel,obj._model);
	}
	Uredjaj(const char* proizvodjac = nullptr, const char* model = nullptr) {
		int vel = strlen(proizvodjac) + 1;
		_proizvodjac = new char[vel];
		strcpy_s(_proizvodjac, vel, proizvodjac);
		vel = strlen(model) + 1;
		_model = new char[vel];
		strcpy_s(_model, vel, model);
	}
	~Uredjaj() {
		delete[] _proizvodjac; _proizvodjac = nullptr;
		delete[] _model; _model = nullptr;
	}

	friend ostream& operator<<(ostream& COUT, const Uredjaj& obj) {
		COUT << obj._proizvodjac << "." << obj._model << endl;
		return COUT;
	}
};
class Printer:public Uredjaj {
	FITKolekcija<DatumVrijeme, Dokument> _printaniDokumenti;
	vector<string> _zabranjeneRijeci;
public:
	Printer(const Printer& obj) :_printaniDokumenti(obj._printaniDokumenti), _zabranjeneRijeci(obj._zabranjeneRijeci), Uredjaj(obj._proizvodjac, obj._model){}
	Printer(const char * proizvodac,const char * model):Uredjaj(proizvodac,model){}

	void DodajZabranjenuRijec(string zabranjenaRijec)
	{
		_zabranjeneRijeci.push_back(zabranjenaRijec);
	}
	void Printaj(DatumVrijeme datumPrintanja, Dokument dokument)
	{
		for (size_t i = 0; i < _printaniDokumenti.GetTrenutno(); i++)
		{
			if (_printaniDokumenti.GetElement1(i) == datumPrintanja && _printaniDokumenti.GetElement2(i) == dokument)
				throw exception("pokusali ste printati isti dokument !");
		}
		_printaniDokumenti.Dodaj(datumPrintanja, dokument);

		if (!regex_match(dokument.GetNaziv(), regex("([A-Za-z]{5,15})(\\.)(doc|txt|pdf|html)")))
			throw exception("dokument je u losem formatu !");
		for (size_t i = 0; i < _zabranjeneRijeci.size(); i++)
		{
			if (regex_search(dokument.GetSadrzaj(), regex(_zabranjeneRijeci[i])))
				throw exception("dokument posjeduje zabranjene rijeci !");
		}
		thread t([this,&dokument]() {
			m.lock();
			cout << crt;
			cout << "naziv dokumenta ---- > " << dokument.GetNaziv() << endl;
			cout << "sadrzaj dokumenta : " << endl;
			cout << dokument.GetSadrzaj() << endl;
			this_thread::sleep_for(chrono::seconds(1));
			cout << crt;
			m.unlock();
			});
		t.join();

	}
	float GetProsjecanBrojStranicaPoDatumu(DatumVrijeme datum)
	{


		float prosjecanBrojStranica = 0;
		int brojac = 0;
		bool provjera = false;
		for (size_t i = 0; i < _printaniDokumenti.GetTrenutno(); i++)
		{
			if (_printaniDokumenti.GetElement1(i) == datum)
			{
				for (size_t j = 0; j < _zabranjeneRijeci.size(); j++)
				{
					string n = _printaniDokumenti.GetElement2(i).GetSadrzaj();
					if (regex_search(_printaniDokumenti.GetElement2(i).GetSadrzaj(), regex(_zabranjeneRijeci[j])))
						provjera = true;
				}
				if (regex_match(_printaniDokumenti.GetElement2(i).GetNaziv(), regex("([A-Za-z]{5,15})(\\.)(doc|txt|pdf|html)")) && provjera == false)
				{
					prosjecanBrojStranica += _printaniDokumenti.GetElement2(i).GetBrojStranica();
					brojac++;
				}
			}
		}
		if (brojac > 0)
			prosjecanBrojStranica /= float(brojac);
		return prosjecanBrojStranica;
	}
	string GetTopZabranjenuRijec()
	{
		string rijec = "";
		int brojacMax = 0;
		int brojacMin = 0;
		for (size_t j = 0; j < _zabranjeneRijeci.size(); j++)
		{
			for (size_t i = 0; i < _printaniDokumenti.GetTrenutno(); i++)
			{
				if (regex_search(_printaniDokumenti.GetElement2(i).GetSadrzaj(), regex(_zabranjeneRijeci[j])))
				{
					brojacMax++;
				}
			}
			if (brojacMax > brojacMin)
			{
				brojacMin = brojacMax;
				rijec = _zabranjeneRijeci[j];
			}
			brojacMax = 0;
		}
		return rijec;
	}
	void UkloniDokumente()
	{
		FITKolekcija<DatumVrijeme, Dokument> temp;
		bool provjeraZaSadrzaj = false;
		bool provjeraZaNaziv = false;
		//if (!regex_match(dokument.GetNaziv(), regex("([A-Za-z]{5,15})(\\.)(doc|txt|pdf|html)")))
		//if (regex_search(dokument.GetSadrzaj(), regex(_zabranjeneRijeci[i])))
		for (size_t i = 0; i < _printaniDokumenti.GetTrenutno(); i++)
		{
			for (size_t j = 0; j < _zabranjeneRijeci.size(); j++)
			{
				if (regex_search(_printaniDokumenti.GetElement2(i).GetSadrzaj(), regex(_zabranjeneRijeci[j])))
				{
					provjeraZaSadrzaj = true;
					break;
				}
			}
			  if (!regex_match(_printaniDokumenti.GetElement2(i).GetNaziv(), regex("([A-Za-z]{5,15})(\\.)(doc|txt|pdf|html)")))
				  provjeraZaNaziv = true;

			  if (provjeraZaNaziv == false && provjeraZaSadrzaj == false)
			  {				
				  temp.Dodaj(_printaniDokumenti.GetElement1(i), _printaniDokumenti.GetElement2(i));				 
			  }
			  provjeraZaNaziv = false;
			  provjeraZaSadrzaj = false;
		 }
		_printaniDokumenti = temp;
	}

	FITKolekcija<DatumVrijeme, Dokument>& GetPrintaniDokumenti() { return _printaniDokumenti; };
	vector<string>& GetZabranjeneRijeci() { return _zabranjeneRijeci; };
	friend ostream& operator <<(ostream& c,  Printer& obj)
	{
		cout << " printer : " << obj._proizvodjac << "modela :" << obj._model << endl;
		for (size_t i = 0; i < obj._printaniDokumenti.GetTrenutno(); i++)
		{
			cout << crt;
			cout << "datum printanja ---- > " << obj._printaniDokumenti.GetElement1(i) << endl;
			cout << "dokument : " << endl;
			cout << obj._printaniDokumenti.GetElement2(i) << endl;
			cout << crt;
		}
		return c;
	}
};

void main()
{
	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKOĐER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
	****************************************************************************/

	DatumVrijeme danas(1, 2, 2017, 10, 15);
	DatumVrijeme sutra(danas);
	DatumVrijeme prekosutra;
	prekosutra = danas;
	cout << danas << endl << sutra << endl << prekosutra << crt;

	int v6 = 6, v13 = 13, v32 = 32, v63 = 63, v98 = 98, v109 = 109, v196 = 196;

	FITKolekcija<int, int> brojevi;
	brojevi.Dodaj(v196, v6);
	brojevi.Dodaj(v13, v32);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v63, v13);
	brojevi.Dodaj(v196, v6);

	cout << brojevi << crt;
	//SORTIRANJE ČLANOVA KOLEKCIJE SE VRŠI U RASTUĆEM REDOSLIJEDU NA OSNOVU VRIJEDNOSTI ELEMENTA TIPA T1
	brojevi.Sortiraj();
	cout << brojevi << crt;
	//BROJ 2 SE ODNOSI NA LOKACIJU/INDEKS UNUTAR LISTE NA KOJI JE POTREBNO DODATI NOVE ELEMENTE
	brojevi.DodajNaLokaciju(v109, v6, 2);
	cout << brojevi << crt;
	brojevi.Sortiraj();
	cout << brojevi << crt;
	/*METODA GetJedinstveni VRAĆA LISTU JEDINSTVENIH ELEMENATA TJ. ELEMENATA KOJI NE SADRŽE DUPLIKATE (POJAM DUPLIKAT SE ODNOSI NA ISTE VRIJEDNOSTI ELEMENATA T1 I T2). ELEMENTI KOJI SE DUPLIRAJU SE U OVOJ LISTI TREBAJU POJAVITI SAMO JEDNOM.*/
	FITKolekcija<int, int>* jedinstveni = brojevi.GetJedinstveni();
	cout << *jedinstveni << crt;
	*jedinstveni = brojevi;
	cout << *jedinstveni << crt;

	Dokument ispitPRIII(DOC, "ispitPRIII.doc");
	//BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREĐUJE PRILIKOM DODAVANJA SADRŽAJA. 
	//ZA POTREBE ISPITA PRETPOSTAVLJAMO DA NA JEDNU STRANICU MOŽE STATI 30 ZNAKOVA UKLJUČUJUĆI I RAZMAKE
	ispitPRIII.DodajSadrzaj("NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO RE");
	cout << "Broj stranica -> " << ispitPRIII.GetBrojStranica() << endl;
	Dokument ispitBaze(DOC, "ispitBaze.doc");
	ispitBaze.DodajSadrzaj("PROGRAMSKI CODE SE TAKOĐER NALAZI U FAJLU CODE.TXT");
	cout << ispitBaze << endl;//ISPISUJE SVE DOSTUPNE PODATKE O DOKUMENTU

	Printer hp3200("HP", "3200");
	//PRINTER NEĆE DOZVOLITI PRINTANJE DOKUMENATA U ČIJEM SADRŽAJU SE NALAZI NEKA OD ZABRANJENIH RIJEČI
	hp3200.DodajZabranjenuRijec("RE");// :)
	hp3200.DodajZabranjenuRijec("RAT");
	hp3200.DodajZabranjenuRijec("UBITI");
	hp3200.DodajZabranjenuRijec("RE");// ONEMOGUĆITI PONAVLJANJE ZABRANJENIH RIJEČI, KORISTITI ITERATORE
	hp3200.DodajZabranjenuRijec("MRZITI");
	try
	{
		/*
		DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDEĆA PRAVILA:
		1. NAZIV DOKUMENTA MORA BITI U SLJEDEĆEM FORMATU npr: ispitPRIII.doc
		NAZIV DOKUMENTA MOŽE SADRŽAVATI SAMO SLOVA (NAJMANJE 5, NAJVIŠE 15), A EKSTENZIJA MOŽE
		BITI SAMO NEKA OD DOZVOLJENIH VRSTOMDOKUMENTA
		2. SADRŽAJ DOKUMENTA U SEBI NE SMIJE POSJEDOVATI NITI JEDNU ZABRANJENU RIJEČ
		UKOLIKO NEKI OD NAVEDENIH USLOVA NIJE ZADOVOLJEN FUNKCIJA TREBA DA BACI EXCEPTION SA
		ODGOVARAJUĆOM PORUKOM, ALI ĆE DOKUMENT BITI POHRANJEN U KOLEKCIJU _printaniDokumenti.
		UKOLIKO DOKUMENT ISPUNJAVA SVE USLOVE ZA PRINTANJE POTREBNO JE NA KONZOLU ISPISATI
		SADRŽAJ DOKUMENTA KOJI SE PRINTA Printaj
		*/
		hp3200.Printaj(danas, ispitBaze);
		hp3200.Printaj(danas, ispitPRIII);

		/*
		IMPLEMENTIRATI PRINTANJE U ZASEBNIM NITIMA UZ KORIŠTENJE sleep_for FUNKCIJE
		VODITI RAČUNA O DIJELJENJU ZAJEDNIČKIH RESURSA
		*/

	}
	catch (exception & err)
	{
		cout << err.what() << endl;
	}

	//KOD POREĐENJA DATUMA PRINTANJA NIJE POTREBNO POREDITI MINUTE, SAMO DATUM I SATE
	cout << "Prosjecan broj printanih stranica za " << danas << " je -> " << hp3200.GetProsjecanBrojStranicaPoDatumu(danas) << crt;
	cout << "Najcesce koristena zabranjena rijec je -> " << hp3200.GetTopZabranjenuRijec() << crt;

	Printer hp4000(hp3200);
	cout << "Prosjecan broj printanih stranica za " << danas << " je -> " << hp4000.GetProsjecanBrojStranicaPoDatumu(danas) << crt;
	cout << "Najcesce koristena zabranjena rijec je -> " << hp4000.GetTopZabranjenuRijec() << crt;
	hp4000.UkloniDokumente();//UKLANJA SVE DOKUMENTE KOJI NISU ZADOVOLJILI USLOVE ZA PRINTANJE
	cout << hp4000 << crt;
	system("pause");
}