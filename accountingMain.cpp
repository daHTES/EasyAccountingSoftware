#include "accountingInterface.h"

int main()
{
	setlocale(LC_ALL, "Russian");
	userInterface theUserInterface;

	theUserInterface.interface();





	return 0;
}

//------------------------------------------------------------------------------------
// получение строки текста 
void getLine (string& inStr)
{
	char temp[30];
	cin.get(temp, 20, '\n');
	cin.ignore(20, '\n');
	inStr = temp;
}
//------------------------------------------------------------------------------------
// получение символа
char getChar() 
{
	char ch = cin.get();
	cin.ignore(80, '\n');
	return ch;
}
//------------------------------------------------------------------------------------
// методы класса Tenant
tenant::tenant(string n, int ap) : name(n), aptNumber(ap) {}
tenant::tenant(string n, string sername, int Ano, int num) : name(n), SerName(sername), aptNumber(Ano), phoneNumber(num) {}
tenant::~tenant() {}
int tenant::getAptNumber() { return aptNumber; }
bool operator < (const tenant& p1, const tenant& p2) { return p1.name < p2.name || p1.SerName < p2.SerName; }
bool operator == (const tenant& p1, const tenant& p2) { return p1.name == p2.name || p1.SerName == p2.SerName; }
ostream& operator << (ostream& s, const tenant& temp) 
{
	s << temp.aptNumber << '\t' << temp.name << '\t' << temp.SerName << '\t'
		<< temp.phoneNumber << '\t' << temp.phoneNumber;
	return s;
}

//------------------------------------------------------------------------------------
// методы класса TenantInputScreen
void tenantInputScreen::getTenant() // получение данных о жильце
{
	cout << "Введите имя жильца: \n";
	getLine(tName);
	cout << "Введите фамилию жильца: \n";
	getLine(tSerName);
	cout << "Введите номер комнаты: \n";
	cin >> aptNo;
	cout << "Введите номер телефона: \n";
	cin >> tPhoneNumber;
	tenant* ptrTenant = new tenant(tName, tSerName, aptNo, tPhoneNumber);
	ptrTenantList->insertTenant(ptrTenant);
}

bool compareTenants::operator () (tenant* p1, tenant* p2) const{return *p1 < *p2;}
//------------------------------------------------------------------------------------
// методы класса tenantList
tenantList::~tenantList() 
{
	while (!setPtrsTens.empty()) 
	{
		iterPtrTens = setPtrsTens.begin();
		delete* iterPtrTens;
		setPtrsTens.erase(iterPtrTens);
	}
}
void tenantList::insertTenant(tenant* ptr) 
{
	setPtrsTens.insert(ptr); // вставка
}
int tenantList::getAptno(string Name) 
{
	int aptNo;
	tenant dummy(Name, 0);
	iterPtrTens = setPtrsTens.begin();
	while (iterPtrTens != setPtrsTens.end()) 
	{
		aptNo = (*iterPtrTens)->getAptNumber(); // поиск жильца в списке да?
		if (dummy == **iterPtrTens++)
			return aptNo;
	}
	return -1;
}

void tenantList::display() 
{
	cout << "\nApt#\tИмя жильца\n--------------\n";
	if (setPtrsTens.empty())
		cout << "****Нет жильцов***\n";
	else 
	{
		iterPtrTens = setPtrsTens.begin();
		while (iterPtrTens != setPtrsTens.end())
			cout << **iterPtrTens++;
	}
}
//------------------------------------------------------------------------------------
// методы класса rentRow
rentRow::rentRow(int an) : aptNo(an) { fill(&rent[0], &rent[12], 0); }
void rentRow::setrent(int m, float am) { rent[m] = am; }
float rentRow::getSumOfRow() { return accumulate(&rent[0], &rent[12], 0); }
bool operator < (const rentRow& t1, const rentRow& t2) 
{
	return t1.aptNo < t2.aptNo;
}
bool operator == (const rentRow& t1, const rentRow& t2) 
{
	return t1.aptNo == t2.aptNo;
}
ostream& operator << (ostream& s, const rentRow& an) 
{
	s << an.aptNo << "\t";
	for (int j = 0; j < 12; j++) // вывводим номера комнат и 12 арнедных платежей
	{
		if (an.rent[j] == 0)
			s << "  0  ";
		else
			s << an.rent[j] << " ";
	}
	s << endl;
	return s;
}
bool compareRow::operator() (rentRow* ptrR1, rentRow* ptrR2) const
{
	return *ptrR1 < *ptrR2;
}
//------------------------------------------------------------------------------------
// методы класса rentRecord
rentRecord::~rentRecord() 
{
	while (!setPtrRR.empty())
	{
		iter = setPtrRR.begin();
		delete* iter;
		setPtrRR.erase(iter);
	}
}
void rentRecord::insertRent(int aptNo, int month, float amount) 
{
	rentRow searchRow(aptNo);
	iter = setPtrRR.begin();
	while (iter != setPtrRR.end())
	{
		if (searchRow == **iter)
		{
			(*iter)->setrent(month, amount);
			return;
		}
		else
			iter++;
	}
	rentRow* ptrRow = new rentRow(aptNo); // не найден новая строка
	ptrRow->setrent(month, amount); // занести её в платеж
	setPtrRR.insert(ptrRow); // заносим платеж в вектор
}
void rentRecord::display() 
{
	cout << "\nAptNo\t Янв  Фев  Мар  Апр  Май  Июн "
		<< "           Июл  Авг  Сен  Окт  Ноя  Дек\n"
		<< "-----------------------------------------"
		<< "-----------------------------------------\n";
	if (setPtrRR.empty())
		cout << "***Нет платежей!****\n";
	else 
	{
		iter = setPtrRR.begin();
		while (iter != setPtrRR.end())
			cout << **iter++;
	}
}

float rentRecord::getSumRents() 
{
	float sumRents = 0.0;
	iter = setPtrRR.begin();
	while (iter != setPtrRR.end()) 
	{
		sumRents += (*iter)->getSumOfRow();
		iter++;
	}
	return sumRents;
}
//------------------------------------------------------------------------------------
// методы класса rentInputScreen
void rentInputScreen::getRent() 
{
	cout << "Введите имя Жильца: \n";
	getLine(renterName);
	aptNo = ptrTenantList->getAptno(renterName);
	if (aptNo > 0) // если имя найдено получить сумму платежа
	{
		cout << "Введите сумму платежа (355.67): ";
		cin >> rentPaid;
		cin.ignore(80, '\n');
		cout << "Введите номера месяца оплаты (1-12): ";
		cin >> month;
		cin.ignore(80, '\n');
		month--; // вну. нумерация
		ptrRentRecord->insertRent(aptNo, month, rentPaid);
	}
	else
		cout << "Такого жильца нет\n";
}
//------------------------------------------------------------------------------------
// методы класса expense
bool operator < (const expense& e1, const expense& e2) 
{
	if (e1.month == e2.month)
		return e1.day < e2.day;
	else
		return e1.month < e2.month;
}
bool operator == (const expense& s1, const expense& s2) 
{
	return s1.month == s2.month && s1.day == s2.day;
}
ostream& operator << (ostream& s, const expense& exp) 
{
	s << exp.month << '/' << exp.day << '\t' << exp.payee << '\t';
	s << exp.amount << '\t' << exp.category << endl;
	return s;
}
bool compareDates:: operator() (expense* ptr1, expense* ptr2) const
{
	return *ptr1 < *ptr2;
}
bool compareCategories::operator() (expense* ptr1, expense* ptr2) const 
{
	return ptr1->category < ptr2->category;
}
//------------------------------------------------------------------------------------
// методы класса expenseRecord
expenseRecord::~expenseRecord() 
{
	while (!vectPtrsExpenses.empty()) 
	{
		iter = vectPtrsExpenses.begin();
		delete* iter;
		vectPtrsExpenses.erase(iter);

	}
}
void expenseRecord::insertExp(expense* ptrExp)
{
	vectPtrsExpenses.push_back(ptrExp);
}
void expenseRecord::display()
{
	cout << "\nДата\tПолучатель\t\tСумма\tКатегтрия\n"
		<< "-----------------------------------------\n";
	if (vectPtrsExpenses.size() == 0)
		cout << "***Расходов нет***\n";
	else 
	{
		sort(vectPtrsExpenses.begin(), vectPtrsExpenses.end(), compareDates());
		iter = vectPtrsExpenses.begin();
		while (iter != vectPtrsExpenses.end())
			cout << **iter++;
	}
}
float expenseRecord::displaySummary() 
{
	float totalExpenses = 0;
	if (vectPtrsExpenses.size() == 0) 
	{
		cout << "\t Все категории \t0\n";
		return 0;
	}
	// сортировка по категории
	sort(vectPtrsExpenses.begin(), vectPtrsExpenses.end(), compareCategories());
	// по каждой категории сумма записей
	iter = vectPtrsExpenses.begin();
	string tempCat = (*iter)->category;
	float sumCat = 0.0;
	while (iter != vectPtrsExpenses.end())
	{
		if (tempCat == (*iter)->category)
			sumCat += (*iter)->amount;

		else 
		{
			cout << '\t ' << tempCat << '\t' << sumCat << endl;
			totalExpenses += sumCat; // прибавить предыдущие значе
			tempCat = (*iter)->category; 
			sumCat = (*iter)->amount; // прибавить конечные суммы
		}
		iter++;
	}
	totalExpenses += sumCat; // прибавить сумму конечной категории
	cout << '\t' << tempCat << '\t' << sumCat << endl;
	return totalExpenses;
}
//------------------------------------------------------------------------------------
// методы класса expenseInputScreen
expenseInputScreen::expenseInputScreen(expenseRecord* per) : ptrExpenseRecord(per) {}
void expenseInputScreen::getExpense() 
{
	int month, day;
	string category, payee;
	float amount;
	cout << "Введите месяц 1-12: ";
	cin >> month;
	cin.ignore(80, '\n');
	cout << "Введите день 1 - 31: ";
	cin >> day;
	cin.ignore(80, '\n');
	cout << "Введите категорию расходов (ремонт, налоги): ";
	getLine(category);
	cout << "Введите получателя "
		<< "ВАТ Банк Русь: ";
	getLine(payee);
	cout << "Введите сумму (39.5): ";
	cin >> amount;
	cin.ignore(80, '\n');
	expense* ptrExpense = new expense(month, day, category, payee, amount);
	ptrExpenseRecord->insertExp(ptrExpense);
}
//------------------------------------------------------------------------------------
// методы класса annualReport
annualReport::annualReport(rentRecord* pRR, expenseRecord* pER) : ptrRR(pRR), ptER(pER) {}

void annualReport::display() 
{
	cout << "Годовой отчет \n-----\n";
	cout << "Доходы\n";
	cout << "\tАрендная плата \t\t";
	rents = ptrRR->getSumRents();
	cout << rents << endl;

	cout << "Расходы\n";
	expenses = ptER->displaySummary();
	cout << "\nБаланс\t\t\t" << rents - expenses << endl;
}

//------------------------------------------------------------------------------------
// методы userInterface
userInterface::userInterface() 
{
	ptrTenantList = new tenantList;
	ptrRentRecord = new rentRecord;
	ptrExpenseRecord = new expenseRecord;
}
userInterface::~userInterface() 
{
	delete ptrTenantList;
	delete ptrRentRecord;
	delete ptrExpenseRecord;
}
void userInterface::interface() 
{
	while (true) 
	{
		cout << "Для ввода данных нажмите 'y', \n"
			<< "                 'd' для вывода отчета, \n"
			<< "                  'q' для выхода:  ";
		ch = getChar();
		if (ch == 'y')
		{
			cout << " 't'  для добавления жильца, \n"
				<< "     'r'  для записи арендной платы, \n"
				<< "      'e' для записи расходов:  ";
			ch = getChar();
			switch (ch)
			{
				// экраны для ввода существующих
			case 't': ptrTenantInputScreen = new tenantInputScreen(ptrTenantList);
				ptrTenantInputScreen->getTenant();
				delete ptrTenantInputScreen;
				break;
			case 'r': ptrRentInputScreen = new rentInputScreen(ptrTenantList, ptrRentRecord);
				ptrRentInputScreen->getRent();
				delete ptrRentInputScreen;
				break;
			case 'e': ptrExpenseInputScreen = new expenseInputScreen(ptrExpenseRecord);
				ptrExpenseInputScreen->getExpense();
				delete ptrExpenseInputScreen;
				break;
			default:
				cout << "неизвестная функция\n";
				break;
			}
		}
		else if (ch == 'd')
		{
			cout << " 't'  для вывода жильцов, \n"
				<< "    'r'  для вывода арендной платы \n"
				<< "     'e' для вывода расходов \n"
				<< "    'a'  дял вывода годового отчета:  ";
			ch = getChar();
			switch (ch)
			{
			case 't': ptrTenantList->display();
				break;
			case 'r': ptrRentRecord->display();
				break;
			case 'e': ptrExpenseRecord->display();
				break;
			case 'a':
				ptrAnnualReport = new annualReport(ptrRentRecord, ptrExpenseRecord);
				ptrAnnualReport->display();
				delete ptrAnnualReport;
				break;
			default: cout << "Неизвестная функция вывода\n";
				break;
			}
		}
		else if (ch == 'q')
			return;
		else
			cout << "неизвестная функция. нажимайфте только 'y', 'd', 'q' \n";
	}
}