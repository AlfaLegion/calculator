#include<iostream>
#include<climits>
#include<vector>
#include<string>
#include<stack>
#include<list>
using namespace std;
bool analysis(const string& str, ostream& os)
{
	for (size_t i = 0; i<str.size(); i++)
	{
		if (static_cast<int>(str[i]) > 57 || static_cast<int>(str[i]) < 40)
		{
			size_t j = i;
			os << "Некорректный ввод, строка содержит не допустимое выражение: ";
			while ((j < str.size() && (static_cast<int>(str[j]) > 57 || static_cast<int>(str[j]) < 40)))
			{
				os << str[j];
				j++;
			}
			os << endl;
			return false;
		}
	}
	return true;
}
void origin_infix(string& str)
{
	string temp = "(0-1)*";
	vector<string> vector_comb(5);
	vector_comb = { "(-","*-","/-","+-","--" };
	if (str[0] == '-'&&str[1] == '(')
	{
		str.erase(0, 1);
		str.insert(0, temp);
	}
	if (str[0] == '-' && (str[1] >= 48 && str[1] <= 57))
	{
		str.erase(0, 1);
		str.insert(0, temp);
	}
	bool flag = true;
	for (auto x : vector_comb)
	{
		auto i = str.find(x);
		if (i == string::npos)
		{
			continue;
		}
		else
		{
			str.erase(i + 1, 1);
			str.insert(i + 1, temp);
		}
	}

}
void transform(string& infix, string& postfix)
{
	origin_infix(infix);
	stack<char> stk;
	infix.push_back('@');
	stk.push('@');
	size_t i = 0;
	bool flag = true;
	bool prv_is_digit = false;// предыдущий элемент был числом?
	while (flag)
	{
		char ch;
		if (i >= infix.size())
		{
			while (!stk.empty())
			{
				ch = stk.top();
				postfix.push_back(ch);
				stk.pop();
			}
			postfix.pop_back();
			break;
		}
		ch = infix[i];
		char tm = stk.top();
		switch (ch)
		{
		case '@':
		{
			if (tm == '@')
			{
				flag = false;
				prv_is_digit = false;
				break;
			}
			if (tm == '+' || tm == '-' || tm == '*' || tm == '/')
			{

				if (prv_is_digit)
				{
					postfix.push_back('|');
				}
				postfix.push_back(tm);
				stk.pop();
				prv_is_digit = false;
				break;
			}
			if (tm == '(')
			{
				cout << "Error" << endl;
				postfix.clear();
				infix.clear();
				return;
			}
		}
		case '+':
		{
			if (tm == '@' || tm == '(')
			{
				stk.push(ch);
				if (prv_is_digit)
				{
					postfix.push_back('|');
				}
				prv_is_digit = false;
				break;
			}
			if (tm == '+' || tm == '-' || tm == '*' || tm == '/')
			{
				if (prv_is_digit)
				{
					postfix.push_back('|');
				}
				postfix.push_back(tm);
				stk.pop();
				stk.push(ch);
				prv_is_digit = false;
				break;
			}
		}
		case '-':
		{
			if (tm == '@' || tm == '(')
			{
				if (prv_is_digit)
				{
					postfix.push_back('|');
				}
				stk.push(ch);
				prv_is_digit = false;
				break;
			}
			if (tm == '+' || tm == '-' || tm == '*' || tm == '/')
			{
				if (prv_is_digit)
				{
					postfix.push_back('|');
				}
				postfix.push_back(tm);
				stk.pop();
				stk.push(ch);
				prv_is_digit = false;
				break;
			}
		}
		case '*':
		{
			if (tm == '@' || tm == '-' || tm == '+' || tm == '(')
			{
				if (prv_is_digit)
				{
					postfix.push_back('|');
				}
				stk.push(ch);
				prv_is_digit = false;
				break;
			}
			if (tm == '*' || tm == '/')
			{
				if (prv_is_digit)
				{
					postfix.push_back('|');
				}
				postfix.push_back(tm);
				stk.pop();
				stk.push(ch);
				prv_is_digit = false;
				break;
			}
		}
		case '/':
		{
			if (tm == '@' || tm == '-' || tm == '+' || tm == '(')
			{
				if (prv_is_digit)
				{
					postfix.push_back('|');
				}
				stk.push(ch);
				prv_is_digit = false;
				break;
			}
			if (tm == '*' || tm == '/')
			{
				if (prv_is_digit)
				{
					postfix.push_back('|');
				}
				postfix.push_back(tm);
				stk.pop();
				stk.push(ch);
				prv_is_digit = false;
				break;
			}
		}
		case '(':
		{
			if (tm == '@' || tm == '-' || tm == '+' || tm == '(' || tm == '*' || tm == '/')
			{
				stk.push(ch);
				prv_is_digit = false;
				break;
			}
		}
		case ')':
		{
			if (tm == '@') { cout << "Error" << endl; return; }
			if (tm == '+' || tm == '-' || tm == '*' || tm == '/')
			{
				if (prv_is_digit)
				{
					postfix.push_back('|');
				}
				while (tm != '(')
				{
					postfix.push_back(tm);
					stk.pop();
					tm = stk.top();
				}
				stk.push(ch);
				prv_is_digit = false;
				//break;
			}
			if (tm == '(') { stk.pop(); stk.pop(); break; }
		}
		default:
		{
			if (!prv_is_digit)
			{
				postfix.push_back('|');
			}
			postfix.push_back(ch);
			prv_is_digit = true;
			break;
		}
		}
		i++;
	}
}
double convert(string& str)
{
	double rez;
	char *ch;
	auto i = str.find('.');
	if (i != string::npos)
	{
		str.erase(i, 1);
		str.insert(i, ",");
	}
	rez = strtod(str.c_str(), &ch);
	if (errno == ERANGE)
	{
		cout << "errno=ERANGE" << endl;
		system("pause");
		exit(1);

	}
	return rez;
}
double calculate(string& str)
{
	
		double rez = 0;
		size_t i = 0;
		size_t j = 0;
		stack<double> stk;
		while (i < str.size())
		{
			if (str[i] == '|')
			{
				j = i + 1;
				string buff;
				while (str[j] != '|')
				{
					buff.push_back(str[j]);
					j++;
				}
				stk.push(convert(buff));
				i = j + 1;
			}
			switch (str[i])
			{
			case '-':
			{
				double b = stk.top();
				stk.pop();
				double a = stk.top();
				stk.pop();
				stk.push(a - b);
				i++;
				break;
			}
			case '+':
			{
				double b = stk.top();
				stk.pop();
				double a = stk.top();
				stk.pop();
				stk.push(a + b);
				i++;
				break;
			}
			case '*':
			{
				double b = stk.top();
				stk.pop();
				double a = stk.top();
				stk.pop();
				stk.push(a * b);
				i++;
				break;
			}
			case '/':
			{
				double b = stk.top();
				stk.pop();
				double a = stk.top();
				stk.pop();
				stk.push(a / b);
				i++;
				break;
			}
			default:break;
			}

		}
		return stk.top();
}
int main()
{
	setlocale(LC_ALL, "Rus");
	string infix;
	string postfix;
	char ch = 'A';
	while (ch != 'q')
	{
		cout << "Input: ";
		std::cin >> infix;
		if (analysis(infix, cout))
		{
			transform(infix, postfix);
			if (!postfix.empty())
			{
				cout.setf(std::ios::fixed);
				cout.precision(2);
			 cout <<"Solution: "<<calculate(postfix)<< endl;
			}else cout << "Проверьте корректность выражения" << endl;
		}
		infix.clear();
		postfix.clear();
		cout << endl;
		cout << "Для выхода введите q для продолжения введите любой символ: ";
		std::cin >> ch;
		std::cin.get();
	}
	std::cin.get();
	return 0;
}