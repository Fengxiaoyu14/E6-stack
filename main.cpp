#include <string>
#include <iostream>
#include <vector>
#include <stack>
#include <iomanip>

using namespace std;


//���ò��������ȼ������￼�ǵ����ţ�"("��")"��ƥ�䣬��������������"("�����ȼ���ߣ���ֻ��������������ʱ�ŵ���������
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

/*********************��׺���ʽתΪ��׺���ʽ**************************/
string InfixToPostfix(const string& str)
{
	string operatorstr = "*-/+()";      //����string����
	string numbers = "0123456789.";

	//���������׺���ʽ��ÿ��Ԫ�ؽ�����Ƭ��ÿ��Ԫ�ش洢��vector<string>Inputstr
	vector<string> Inputvec;   //�洢��Ƭ���
	for (unsigned int i = 0; i < str.size(); )
	{
		string::size_type operatorindex = str.find_first_of(operatorstr, i);     //����str�д�i��ʼ�ĵ�һ��������
		if (operatorindex != string::npos)
		{
			//�����i��ʼ�������˲�����
			if (operatorindex == i)
			{
				//��������������Ĳ���������������ʽ�ı��ʽ   a*(b+c)+d;
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
			//�����i��ʼû�������������������ʣ�µ��ַ����Ž�vector
			Inputvec.push_back(str.substr(i, str.size() - i));
			i = str.size();
		}
	}

	//������Ƭ���vector��ÿ��Ԫ��
	stack<string> operatorstack;     //������ջ�������洢������
	vector<string> PostfiOutvec;     //�洢��׺���
	for (int i = 0; i < Inputvec.size(); i++)
	{
		//�����ǰԪ���ǲ�����
		if (Inputvec[i].find_first_of(operatorstr) != string::npos)
		{
			if (operatorstack.empty())
			{
				operatorstack.push(Inputvec[i]);      //���������ջ�գ���ֱ����ջ
			}
			else
			{
				if (Inputvec[i] == ")")     //�����ǰ��������������
				{
					while (operatorstack.top() != "(")
					{
						PostfiOutvec.push_back(operatorstack.top());     //��ջ�����������
						operatorstack.pop();    //ɾ��ջ��Ԫ��
					}
					operatorstack.pop();    //ɾ��ջ��Ԫ�أ�������ɾ��������"("��
				}
				else
				{
					int curpri = priority(Inputvec[i]);     //��ȡ�����������ȼ�

					//�Ƚϵ�ǰ��������ջ��Ԫ�����ȼ������С�ڻ����ջ��Ԫ�����ȼ��򵯳�ջ��Ԫ�أ�����ǰ��������ջ
					while (!operatorstack.empty())
					{
						string top = operatorstack.top();     //����ջ��Ԫ��
						int toppor = priority(top);     //ջ��Ԫ�����ȼ�

						if ((curpri <= toppor) && top != "(")       //���������ȼ���󣬵�����ֻ�����������Ų����
						{
							PostfiOutvec.push_back(top);
							operatorstack.pop();    //ɾ��ջ��Ԫ��
						}
						else
							break;
					}
					operatorstack.push(Inputvec[i]);
				}
			}
		}
		//�����ǰԪ���ǲ�������ֱ�����
		else
		{
			PostfiOutvec.push_back(Inputvec[i]);
		}
	}


	while (!operatorstack.empty())
	{
		PostfiOutvec.push_back(operatorstack.top());      //���������ջ�е�����������
		operatorstack.pop();
	}

	//������в���ո�
	vector<string>::const_iterator itr = PostfiOutvec.begin() + 1;	//const_iteratorָ���ֵ����ı�
	while (itr != PostfiOutvec.end())
	{
		itr = PostfiOutvec.insert(itr, " ");      //���ﷵ��insert֮���ָ�룬��Ϊ�ı������Ĳ�����ʹ������ʧЧ
		itr += 2;
	}

	PostfiOutvec.push_back(" ");     //������һ���ո�

	//vector���Ϊstring����Ϊ��׺���ʽ�������
	string result;
	for (int i = 0; i < PostfiOutvec.size(); i++)
	{
		result.append(PostfiOutvec[i]);
	}

	return result;
}


/*********************��׺���ʽ��ֵ**************************/
double postfixExpression(const string& str)
{
	stack<double> mystack;    //ջ�ռ�

	string s = "0123456789+-*/";
	string empty = " ";
	string numbers = ".0123456789";
	string c = "+-*/";

	double firstnum;
	double secondnum;
	double ret;

	for (unsigned int i = 0; i < str.size(); )
	{
		string::size_type start = str.find_first_of(s, i);     //���ҵ�һ�����ֻ���������
		string::size_type end = str.find_first_of(empty, i);   //���ҵ�һ���ո�
		string tempstr = str.substr(start, end - start);     //ȡ����һ��Ԫ��

		//�ж�Ԫ��
		if (tempstr == "+" || tempstr == "-" || tempstr == "*" || tempstr == "/")
		{
			secondnum = mystack.top();    //ȡ��ǰջ��Ԫ��
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
			double num = stod(tempstr);	// ��string����ת��Ϊdouble����
			mystack.push(num);
		}

		i = end + 1;
	}
	return mystack.top();	// ���յĽ��
}


int main()
{
	int n;
	cin >> n;	// ���ʽ�ĸ���

	for (int i = 0; i < n; i++)
	{
		string str;
		cin >> str;
		double ret;

		str = InfixToPostfix(str);
		ret = postfixExpression(str);

		cout << setiosflags(ios::fixed) << setprecision(2); // ������λС��
		cout << ret << endl;
	}


	return 0;
}