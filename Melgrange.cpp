#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>


using namespace std;

struct adjacencyMatrix
{
	int vertex=0;
	bool **relation;

	void showMatrix()
	{
		cout << endl;
		cout << "     ";
		for (int x = 0; x < vertex; x++)
		{
			cout << "  x";
			printf("%-2i", x);
			cout << " ";
		}
		for (int x = 0; x < vertex; x++)
		{
			cout << endl << " x" << x << " ";
			for (int j = 0; j < vertex; j++)
			{
				cout << "  ";
				printf("%3i", relation[j][x]);
				cout << " ";
			}
		}
	}
	void fillRandom(int n)
	{
		srand(time(NULL));
		relation = new bool* [n];
		for (int x = 0; x < n; x++)
			relation[x] = new bool[n];

		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				relation[i][j] = 0;

		for (int i=0; i<n; i++)
			for (int j = 0; j < i; j++)
			{
					relation[i][j] = rand() % 2;
					relation[j][i] = relation[i][j];
			}
		vertex = n;
	}
	int sumRelation()
	{
		int counter = 0;
		for (int i=0; i<vertex; i++)
			for (int j = 0; j <= i; j++)
				if (relation[i][j])
					counter++;
		return counter;
	}
	~adjacencyMatrix()
	{
		for (int i = 0; i < vertex; i++)
		{
			delete[] relation[i];
		}
	}
	void Melgrange()
	{
		struct combination
		{
			vector<int> lines;
			vector<int> columns;
			
			//A<=B
			combination()
			{
			}
			combination(int i)
			{
				lines.push_back(i);
			}
			combination(const combination& copy)
			{
				this->lines = copy.lines;
				this->columns = copy.columns;
			}
			combination& operator= (const combination& CopySource)
			{
				this->lines = CopySource.lines;
				this->columns = CopySource.columns;
				return *this;
			}
			combination& operator+= (const combination& compare)//overload operator += for U
			{
				int key;
				for (int i = 0; i < compare.lines.size(); i++)
				{
					this->lines.push_back(compare.lines[i]);//unite lines
				}
				for (int i = 0; i < this->columns.size(); i++)
				{
					key = 0;
					for (int j = 0; j < compare.columns.size(); j++)
					{
						if (compare.columns[j] == this->columns[i])
						{
							key = 1;
							break;
						}
					}
					if (!key)
					{
						this->columns.erase(i + this->columns.begin()); //it ramains only crossing of columns
					}
				}
				sort(this->lines.begin(), this->lines.end());
				this->lines.erase(std::unique(this->lines.begin(), this->lines.end()), this->lines.end()); //delete duplicates
				return *this;
			}
			combination& operator-= (const combination& compare) //overload operator -= for func crossing
			{
				int key;
				for (int i = 0; i < compare.columns.size(); i++)
				{
					this->columns.push_back(compare.columns[i]);  //unite columns
				}
				for (int i = 0; i < this->lines.size(); i++)
				{
					key = 0;
					for (int j = 0; j < compare.lines.size(); j++)
					{
						if (compare.lines[j] == this->lines[i])
						{
							key = 1;
							break;
						}
					}
					if (!key)
					{
						this->lines.erase(this->lines.begin() + i); //it remains only crossing of lines
					}
				}
				sort(this->columns.begin(), this->columns.end());
				this->columns.erase(std::unique(this->columns.begin(), this->columns.end()), this->columns.end()); //delete duplicates
				return *this;
			} 
			bool operator<= ( combination& compare)//it's overload operator == is necessary to compare combinations
			{
				sort(compare.lines.begin(), compare.lines.end());  //it is not necessary, just makes comparison more comfortable
				sort(compare.columns.begin(), compare.columns.end());

				sort(this->lines.begin(), this->lines.end());
				sort(this->columns.begin(), this->columns.end());

				bool key = 0;
				
				for (int i = 0; (i != this->lines.size()); i++)
				{
					for (int j = 0, key=0; (j != compare.lines.size()); j++)
					{
						if (this->lines[i] == compare.lines[j])
						{
							key = 1;
							break;
						}
					}
				}
				if (key == 0)
					return 0;
				for (int i = 0; (i != this->columns.size()); i++)
				{
					for (int j = 0, key = 0; (j != compare.columns.size()); j++)
					{
						if (this->columns[i] == compare.columns[j])
						{
							key = 1;
							break;
						}
					}
				}
				return key;
			}
			bool operator== ( combination& compare)//it's overload operator == is necessary to compare combinations
			{
				sort(compare.lines.begin(), compare.lines.end());  //it is not necessary, just makes comparison more comfortable
				sort(compare.columns.begin(), compare.columns.end());

				sort(this->lines.begin(), this->lines.end());
				sort(this->columns.begin(), this->columns.end());
				bool key = ((this->lines.size()== compare.lines.size()) and (this->columns.size() == compare.columns.size()));
				if (!key)
					return 0;
				for (int i = 0; i < this->lines.size(); i++)
				{
					if (this->lines[i] != compare.lines[i])
					{
						key = 0;
						break;
					}
				}
				if (!key)
					return 0;
				for (int i = 0; i < this->columns.size(); i++)
				{
					if (this->columns[i] != compare.columns[i])
					{
						key = 0;
						break;
					}
				}
				return key;
			}
			bool full()
			{
				return (lines.empty() and columns.empty());  //if one of them is empty we can drop this combination;
			}
			bool columnsAreLikeLines()
			{
				sort(this->lines.begin(), this->lines.end());
				sort(this->columns.begin(), this->columns.end());
				int key = 0;
				if (!(columns.size() == lines.size()))
					return key;
				else
				{
					for (int i = 0, key=1; i < columns.size(); i++)
					{
						if (columns[i] != lines[i])
							key = 0;
						break;
					}
				}
				return key;
			}
		};
		combination T;
		vector<combination> family(vertex);
		vector<combination> copy/*(vertex)*/;
		for (int i = 0; i < vertex; i++)
		{
			family[i].lines.push_back(i);
			for (int j = 0; j < vertex; j++)
			{
				if (!relation[i][j])
					family[i].columns.push_back(j);
			}
			copy.push_back(family[i]);

		}
		int key=0;
		do
		{
			for (int i = 0; i < copy.size(); i++) //we will find all combinations
			{
				for (int j = i+1; j < copy.size(); j++)
				{
					T = copy[i];
					T += copy[j];
					family.push_back(T);
					if (T.full())
					{
						family.push_back(T);
					}
					T = copy[i];
					T -= copy[j];
					if (T.full())
					{
						family.push_back(T);
					}
				}
			}
			
			for (int i = 0; i < copy.size(); i++) //and now we delete duplicates and subArray
			{
				for (int j = 0; j < copy.size(); j++)
				{
					if (i == j) continue;
					if ((family[i] <= family[j]) or !family[i].full())
					{
						family.erase(family.begin() + i);
					}
				}
			}
			key = 1;
			if (family.size()==copy.size())
			for (int i = 0; i < family.size(); i++)
			{
				key = 1;
				for (int j = 0; j < copy.size(); j++)
				{
					key = !(family[i] == copy[j]);
					if (!key)
						break;
				}
				if (key)
					break;
			}
			copy = family;
			/*
			for (int i = 0; i < family.size(); i++)
			{
				{
					cout << endl;
					for (int j = 0; j < family[i].lines.size(); j++)
					{
						cout << family[i].lines[j] << " ";
					}
					cout << "| ";
					for (int j = 0; j < family[i].columns.size(); j++)
					{
						cout << family[i].columns[j] << " ";
					}
				}
			}*/
			cout << endl<<"iteration ends"<<endl;

		} while (key);
		
		for (int i = 0; i < family.size(); i++)
		{
			if (family[i].columnsAreLikeLines())
			{
				cout<<endl;
				for (int j = 0; j < family[i].lines.size(); j++)
				{
					cout << family[i].lines[j] << " ";
				}
			}
		}
		cout << endl;
	}
};

int main()
{
    adjacencyMatrix graph;
    graph.fillRandom(10);
    graph.showMatrix();
    graph.Melgrange();
    system("Pause>NULL");
    system("cls");
    return 0;
}
