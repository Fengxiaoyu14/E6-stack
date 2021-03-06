#include <string>
#include <iostream>
#include <vector>
#include <stack>
#include <iomanip>

using namespace std;


//设置操作符优先级，这里考虑到括号（"("、")"）匹配，定义设置左括号"("的优先级最高，且只有在遇到右括号时才弹出左括号
int priority(const string str)
{
	const char* op = str.c_str();
	switch (*op)
	{
	case ')':
		return 0;
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	case '(':
		return 3;
	default:
		return -1;
	}
}

/*********************中缀表达式转为后缀表达式**************************/
string InfixToPostfix(const string& str)
{
	string operatorstr = "*-/+()";      //用于string搜索
	string numbers = "0123456789.";

	//对输入的中缀表达式中每个元素进行切片，每个元素存储到vector<string>Inputstr
	vector<string> Inputvec;   //存储切片结果
	for (unsigned int i = 0; i < str.size(); )
	{
		string::size_type operatorindex = str.find_first_of(operatorstr, i);     //搜索str中从i开始的第一个操作符
		if (operatorindex != string::npos)
		{
			//如果从i开始搜索到了操作符
			if (operatorindex == i)
			{
				//如果是两个连续的操作符，即这种形式的表达式   a*(b+c)+d;
				string tempstr = str.substr(operatorindex, 1);
				Inputvec.push_back(tempstr);
				i = i + 1;
			}
			else
			{
				Inputvec.push_back(str.substr(i, operatorindex - i));
				Inputvec.push_back(str.substr(operatorindex, 1));
				i = operatorindex + 1;
			}
		}
		else
		{
			//如果从i开始没有搜索到操作符，则把剩下的字符串放进vector
			Inputvec.push_back(str.substr(i, str.size() - i));
			i = str.size();
		}
	}

	//遍历切片结果vector中每个元素
	stack<string> operatorstack;     //创建空栈，用来存储操作符
	vector<string> PostfiOutvec;     //存储后缀输出
	for (int i = 0; i < Inputvec.size(); i++)
	{
		//如果当前元素是操作符
		if (Inputvec[i].find_first_of(operatorstr) != string::npos)
		{
			if (operatorstack.empty())
			{
				operatorstack.push(Inputvec[i]);      //如果操作符栈空，则直接入栈
			}
			else
			{
				if (Inputvec[i] == ")")     //如果当前操作符是右括号
				{
					while (operatorstack.top() != "(")
					{
						PostfiOutvec.push_back(operatorstack.top());     //将栈顶操作符输出
						operatorstack.pop();    //删除栈顶元素
					}
					operatorstack.pop();    //删除栈顶元素（这里是删除左括号"("）
				}
				else
				{
					int curpri = priority(Inputvec[i]);     //获取操作符的优先级

					//比较当前操作符与栈顶元素优先级，如果小于或等于栈顶元素优先级则弹出栈顶元素，否则当前操作符入栈
					while (!operatorstack.empty())
					{
						string top = operatorstack.top();     //返回栈顶元素
						int toppor = priority(top);     //栈顶元素优先级

						if ((curpri <= toppor) && top != "(")       //左括号优先级最大，但是它只有遇到右括号才输出
						{
							PostfiOutvec.push_back(top);
							operatorstack.pop();    //删除栈顶元素
						}
						else
							break;
					}
					operatorstack.push(Inputvec[i]);
				}
			}
		}
		//如果当前元素是操作数，直接输出
		else
		{
			PostfiOutvec.push_back(Inputvec[i]);
		}
	}


	while (!operatorstack.empty())
	{
		PostfiOutvec.push_back(operatorstack.top());      //输出操作符栈中的其他操作符
		operatorstack.pop();
	}

	//在输出中插入空格
	vector<string>::const_iterator itr = PostfiOutvec.begin() + 1;	//const_iterator指向的值不会改变
	while (itr != PostfiOutvec.end())
	{
		itr = PostfiOutvec.insert(itr, " ");      //这里返回insert之后的指针，因为改变容器的操作会使迭代器失效
		itr += 2;
	}

	PostfiOutvec.push_back(" ");     //添加最后一个空格

	//vector输出为string，作为后缀表达式结果返回
	string result;
	for (int i = 0; i < PostfiOutvec.size(); i++)
	{
		result.append(PostfiOutvec[i]);
	}

	return result;
}


/*********************后缀表达式求值**************************/
double postfixExpression(const string& str)
{
	stack<double> mystack;    //栈空间

	string s = "0123456789+-*/";
	string empty = " ";
	string numbers = ".0123456789";
	string c = "+-*/";

	double firstnum;
	double secondnum;
	double ret;

	for (unsigned int i = 0; i < str.size(); )
	{
		string::size_type start = str.find_first_of(s, i);     //查找第一个数字或算术符号
		string::size_type end = str.find_first_of(empty, i);   //查找第一个空格
		string tempstr = str.substr(start, end - start);     //取出这一个元素

		//判断元素
		if (tempstr == "+" || tempstr == "-" || tempstr == "*" || tempstr == "/")
		{
			secondnum = mystack.top();    //取当前栈顶元素
			mystack.pop();
			firstnum = mystack.top();
			mystack.pop();
			if (tempstr == "+")
			{
				ret = firstnum + secondnum;
				mystack.push(ret);
			}
			if (tempstr == "-")
			{
				ret = firstnum - secondnum;
				mystack.push(ret);
			}
			if (tempstr == "*")
			{
				ret = firstnum * secondnum;
				mystack.push(ret);
			}
			if (tempstr == "/")
			{
				ret = firstnum / secondnum;
				mystack.push(ret);
			}
		}
		else
		{
			double num = stod(tempstr);	// 将string类型转换为double类型
			mystack.push(num);
		}

		i = end + 1;
	}
	return mystack.top();	// 最终的结果
}


int main()
{
	int n;
	cin >> n;	// 表达式的个数

	for (int i = 0; i < n; i++)
	{
		string str;
		cin >> str;
		double ret;

		str = InfixToPostfix(str);
		ret = postfixExpression(str);

		cout << setiosflags(ios::fixed) << setprecision(2); // 保留两位小数
		cout << ret << endl;
	}


	return 0;
}