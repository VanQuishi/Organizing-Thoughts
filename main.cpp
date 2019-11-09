#include "ArgumentManager.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

bool equal(string f, string l)
{
	char special[6] = { '.', '?', '!', '(', ')','-' };
	bool erased = false;
	bool value = true;

	for (int i = 0; i < f.size(); i++)
	{
		//string s(1, f[i]);diff
		for (int j = 0; j < 6; j++)
		{
			if (f[i] == special[j])
			{
				f.erase(f.begin() + i, f.end());
				erased = true;
			}
		}

		if (erased == true)
		{
			break;
		}
	}

	erased = false;
	for (int i = 0; i < l.size(); i++)
	{
		//string z(1, l[i]);
		for (int j = 0; j < 6; j++)
		{
			if (l[i] == special[j])
			{
				l.erase(l.begin() + i, l.end());
				erased = true;
			}
		}

		if (erased == true)
		{
			break;
		}
	}

	//==========================
	if (l.size() == f.size())
	{
		for (int i = 0; i < l.size(); i++)
		{
			if (tolower(l[i]) != tolower(f[i]))
			{
				value = false;
			}
		}
	}

	else
	{
		value = false;
	}

	return value;
}

struct sentence
{
	string firstWord;
	string lastWord;
	string line;
	bool in_order;
	bool is_head;
	sentence *next;
};

class thought
{
public:
	int size;
	sentence *head;
	sentence *tail;

	thought();
	void add(string _firstword, string _lastword, string _line);
	void sort(thought &final_thought, ofstream &outFile);
};

thought::thought()
{
	size = 0;
	head = NULL;
	tail = NULL;
}

void thought::add(string _firstword, string _lastword, string _line)
{
	sentence *temp = new sentence;
	temp->firstWord = _firstword;
	temp->lastWord = _lastword;
	temp->line = _line;
	temp->in_order = false;
	temp->is_head = true;
	temp->next = NULL;

	if (head == NULL && tail == NULL)
	{
		head = temp;
		tail = temp;
		size++;
	}

	else
	{
		tail->next = temp;
		tail = temp;
		size++;
	}

	//delete temp;
}

void thought::sort(thought &final_thought, ofstream &outFile)
{
	if (size == 0)
	{
		return;
	}

	if (size == 1)
	{
		outFile << head->line << endl;
	}

	else
	{

		sentence *temp = head;
		for (int i = 0; i < size; i++)
		{

			sentence *temp_1 = head;

			for (int j = 0; j < size; j++)
			{
				if (equal(temp->firstWord, temp_1->lastWord) == true)
				{
					temp->is_head = false;
				}

				temp_1 = temp_1->next;
			}

			temp = temp->next;
		}

		temp = head;

		for (int i = 0; i < size; i++)
		{
			if (temp->is_head == true)
			{
				final_thought.add(temp->firstWord, temp->lastWord, temp->line);
				head->in_order = true;
				tail->in_order = true;
				break;
			}

			temp = temp->next;
		}

		outFile << final_thought.head->line << endl;
		sentence *sorted = final_thought.head;
		sentence *curr = NULL;


		for (int i = 0; i < size - 1; i++)    //loop that counts how many lines are printed
		{
			curr = head;

			for (int j = 0; j < size ; j++)
			{
				if (equal(curr->firstWord, sorted->lastWord) == true)
				{
					outFile << curr->line << endl;
					sorted = curr;
				}

				curr = curr->next;
			}
			//delete curr;
		}

		//delete temp;
		//delete sorted;

	}
}

int main(int argc, char* argv[])
{
	ArgumentManager am(argc, argv);
	const string in = am.get("input");
	const string out = am.get("output");

	ifstream inFile(in);

	string first, last, l;
	thought dayDream;
	thought final_thought;

	if (!inFile.is_open())
	{
		cout << "Input file cannot be opened." << endl;
	}

	while (!inFile.eof())
	{
		getline(inFile, l);              //read each line of the file
		if (l != "") {
			istringstream ss(l);             //separate words in a line
			int idx = 0;
			string arr[1000];

			do
			{
				string w;
				ss >> w;
				arr[idx] = w;
				idx++;
			} while (ss);


			first = arr[0];
			last = arr[idx - 2];

			dayDream.add(first, last, l);
		}
	}

	inFile.close();

	ofstream outFile(out);
	dayDream.sort(final_thought, outFile);
	outFile.close();

	//system("pause");
	return 0;

}
