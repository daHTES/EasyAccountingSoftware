#pragma warning (disable:4786)

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <numeric>
using namespace std;
// Global method
void getLine(string& inStr);
char getChar();

/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
// Class жильцы
class tenant 
{
private:
	string name; // имя жильца
	string SerName; // фамилия
	int aptNumber; // номер квартиры
	int phoneNumber; // телефон
public:
	tenant(string name, int ap);
	tenant(string name, string serName, int aptNum, int phone);
	~tenant();
	int getAptNumber();
	// сравнение для множеств
	friend bool operator < (const tenant&, const tenant&);
	friend bool operator == (const tenant&, const tenant&);
	// для операций ввода-вывода
	friend ostream& operator << (ostream&, const tenant&);
};

/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
// functions объект для сравнения имен жильцов
class compareTenants 
{
public:
	bool operator () (tenant*, tenant*) const;
};


/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
// list жильцов
class tenantList 
{
private:
	// установ. указатели на жильцов
	set <tenant*, compareTenants> setPtrsTens;
	set <tenant*, compareTenants>::iterator iterPtrTens;

public:
	~tenantList();

	void insertTenant(tenant*); // внесение жильца в список
	int getAptno(string); // возвр. номер комнаты
	void display();
};

/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
// class запроса добавления нового жильца
class tenantInputScreen 
{
private:
	tenantList* ptrTenantList;
	string tName;
	int aptNo;
	string tSerName; // фамилия
	int tPhoneNumber; // телефон

public:
	tenantInputScreen(tenantList* ptrTL) : ptrTenantList(ptrTL) {}
	void getTenant();
};

/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
// класс таблица
class rentRow
{
private:
	int aptNo;
	float rent[12];

public:
	rentRow(int);
	void setrent(int, float); // запись платы за месяц
	float getSumOfRow(); // сумма платежей из одной строки таблицы

	friend bool operator < (const rentRow&, const rentRow&);
	friend bool operator == (const rentRow&, const rentRow&);
	// вывод
	friend ostream& operator << (ostream&, const rentRow&);
};

/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
// функциональный объект для rentRows
class compareRow
{
public:
	bool operator () (rentRow*, rentRow*) const;
};

/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
// ввод ренты 
class rentRecord 
{
private:
	// мн. указа. на объекты rentRow (1==1)
	set<rentRow*, compareRow> setPtrRR;
	set<rentRow*, compareRow>::iterator iter;
public:
	~rentRecord();
	void insertRent(int, int, float);
	void display(); 
	float getSumRents(); // сумма всех платежей		
};

/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
// Вывод ренты
class rentInputScreen 
{
private:
	tenantList* ptrTenantList;
	rentRecord* ptrRentRecord;
	string renterName;
	string renterSername;
	int rentPhoneNumber;
	float rentPaid;
	int month;
	int aptNo;
public:
	rentInputScreen(tenantList* ptrTL, rentRecord* ptrRR): ptrTenantList(ptrTL), ptrRentRecord(ptrRR){}
	void getRent(); // аредн. платаная одного жильца за 1 месяц
};

/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
// класс расходы
class expense 
{
public:
	int month, day;
	string category, payee;
	float amount;
	expense() {}
	expense(int m, int d, string c, string p, float a) : month(m), day(d), category(c), payee(p), amount(a) {}

	friend bool operator < (const expense&, const expense&);
	friend bool operator == (const expense&, const expense&);
	// для вывода
	friend ostream& operator << (ostream&, const expense&);
};

/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
// фу-льный класс расходов
class compareDates 
{
public:
	bool operator () (expense*, expense*) const;
};

/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
// фу-льный класс расходов
class compareCategories 
{
public:
	bool operator () (expense*, expense*) const;
};

/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
// класс занесения расходов
class expenseRecord 
{
private:
	// вектор указателей на расходы
	vector<expense*> vectPtrsExpenses;
	vector<expense*>::iterator iter;

public:
	~expenseRecord();
	void insertExp(expense*);
	void display();
	float displaySummary();
};

/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
// вывод класса расходов 
class expenseInputScreen 
{
private:
	expenseRecord* ptrExpenseRecord;
public:
	expenseInputScreen(expenseRecord*);
	void getExpense();
};

/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
// отчет класс
class annualReport
{
private:
	rentRecord* ptrRR;
	expenseRecord* ptER;
	float expenses, rents;
public:
	annualReport(rentRecord*, expenseRecord*);
	void display();
};

/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
// класс юзера
class userInterface 
{
private:
	tenantList* ptrTenantList;
	tenantInputScreen* ptrTenantInputScreen;
	rentRecord* ptrRentRecord;
	rentInputScreen* ptrRentInputScreen;
	expenseRecord* ptrExpenseRecord;
	expenseInputScreen* ptrExpenseInputScreen;
	annualReport* ptrAnnualReport;
	char ch;

public:
	userInterface();
	~userInterface();
	void interface();
};


