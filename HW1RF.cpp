
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <time.h>
#include <iomanip>

using namespace std;
struct iris
{
	double value[4];
	string species;
};
struct node
{
	int feature; //0~3
	bool isLeaf;
	string species;
	double gap_value;
	struct node* higher;
	struct node* lower;
	vector<iris> data;
};

void InputData(vector<iris> &data)
{
	ifstream input;
	input.open("bezdekIris.txt");
	string buffer, buffer2;
	for (int i = 0; i < 150; i++)
	{
		input >> buffer;
		buffer2 = buffer2.assign(buffer, 0, 3);
		data[i].value[0] = stod(buffer2);
		buffer2 = buffer2.assign(buffer, 4, 3);
		data[i].value[1] = stod(buffer2);
		buffer2 = buffer2.assign(buffer, 8, 3);
		data[i].value[2] = stod(buffer2);
		buffer2 = buffer2.assign(buffer, 12, 3);
		data[i].value[3] = stod(buffer2);
		buffer2 = buffer2.assign(buffer, 16, buffer.length());
		data[i].species = buffer2;
	}
}

double CalEntropy(int numSetosa, int numVersicolor, int numVirginica, int datalength)
{
	double result = 0;
	if (datalength == 0)
	{
		return 0;
	}
	if (numSetosa != 0)
	{
		result += -(numSetosa / (double)datalength) * (log(numSetosa / (double)datalength) / log(2));
	}
	if (numVersicolor != 0)
	{
		result += -(numVersicolor / (double)datalength) * (log(numVersicolor / (double)datalength) / log(2));
	}
	if (numVirginica != 0)
	{
		result += -(numVirginica / (double)datalength) * (log(numVirginica / (double)datalength) / log(2));
	}
	return result;

}

double CalculateGain(vector<iris>data, vector<iris> higher, vector<iris> lower, int feature) // Gain = entropy - remainder
{
	int num_Setosa, num_Versicolor, num_Virginica;
	double data_Setosa_percent, data_Versicolor_percent, data_Virginica_percent;
	double higher_Setosa_percent, higher_Versicolor_percent, higher_Virginica_percent;
	double lower_Setosa_percent, lower_Versicolor_percent, lower_Virginica_percent;
	double remainder;
	double data_entropy, higher_entropy, lower_entropy;
	num_Setosa = num_Versicolor = num_Virginica = 0;
	for (int i = 0; i < data.size(); i++)
	{
		if (data[i].species == "Iris-setosa")
		{
			num_Setosa++;
		}
		else if (data[i].species == "Iris-versicolor")
		{
			num_Versicolor++;
		}
		else if (data[i].species == "Iris-virginica")
		{
			num_Virginica++;
		}
		else cout << "error." << endl;
	}
	data_Setosa_percent = num_Setosa / (double)data.size();
	data_Versicolor_percent = num_Versicolor / (double)data.size();
	data_Virginica_percent = num_Virginica / (double)data.size();
	data_entropy = 0;
	if (num_Setosa > 0)
	{
		data_entropy += -(data_Setosa_percent * log2(data_Setosa_percent));
	}
	if (num_Versicolor > 0)
	{
		data_entropy += -(data_Versicolor_percent * log2(data_Versicolor_percent));
	}
	if (num_Virginica > 0)
	{
		data_entropy += -(data_Virginica_percent * log2(data_Virginica_percent));
	}//complete
	 //cout << data_entropy << endl;
	num_Setosa = num_Versicolor = num_Virginica = 0;
	for (int i = 0; i < higher.size(); i++)
	{
		if (higher[i].species == "Iris-setosa")
		{
			num_Setosa++;
		}
		else if (higher[i].species == "Iris-versicolor")
		{
			num_Versicolor++;
		}
		else if (higher[i].species == "Iris-virginica")
		{
			num_Virginica++;
		}
		else cout << "error." << endl;
	}
	higher_Setosa_percent = num_Setosa / (double)higher.size();
	higher_Versicolor_percent = num_Versicolor / (double)higher.size();
	higher_Virginica_percent = num_Virginica / (double)higher.size();
	higher_entropy = 0;
	if (num_Setosa > 0)
	{
		higher_entropy += -(higher_Setosa_percent * log2(higher_Setosa_percent));
	}
	if (num_Versicolor > 0)
	{
		higher_entropy += -(higher_Versicolor_percent * log2(higher_Versicolor_percent));
	}
	if (num_Virginica > 0)
	{
		higher_entropy += -(higher_Virginica_percent * log2(higher_Virginica_percent));
	}//complete
	 //cout << higher_entropy << endl;
	num_Setosa = num_Versicolor = num_Virginica = 0;
	for (int i = 0; i < lower.size(); i++)
	{
		if (lower[i].species == "Iris-setosa")
		{
			num_Setosa++;
		}
		else if (lower[i].species == "Iris-versicolor")
		{
			num_Versicolor++;
		}
		else if (lower[i].species == "Iris-virginica")
		{
			num_Virginica++;
		}
		else cout << "error." << endl;
	}
	lower_Setosa_percent = num_Setosa / (double)lower.size();
	lower_Versicolor_percent = num_Versicolor / (double)lower.size();
	lower_Virginica_percent = num_Virginica / (double)lower.size();
	lower_entropy = 0;
	if (num_Setosa > 0)
	{
		lower_entropy += -(lower_Setosa_percent * log2(lower_Setosa_percent));
	}
	if (num_Versicolor > 0)
	{
		lower_entropy += -(lower_Versicolor_percent * log2(lower_Versicolor_percent));
	}
	if (num_Virginica > 0)
	{
		lower_entropy += -(lower_Virginica_percent * log2(lower_Virginica_percent));
	}//complete
	 //cout << lower_entropy << endl;
	remainder = ((higher.size() / (double)data.size()) * higher_entropy) + ((lower.size() / (double)data.size()) * lower_entropy);
	return data_entropy - remainder;
}

void Shuffle(vector<iris> &data)
{
	vector<iris> temp;
	bool isPut[155] = { false };
	temp.assign(data.begin(), data.end());
	int buffer = 0;
	int location = 0;
	//system("pause");
	for (int i = 150; i > 0; i--)
	{
		buffer = rand() % i + 1;
		location = -1;
		for (int j = 0; j < buffer; j)
		{
			location++;
			if (isPut[location] == true)
			{
				continue;
			}
			j++;
		}
		//cout << location << " " << i - 1 << endl;
		data[location] = temp[i - 1];
		isPut[location] = true;
		//cout << "a" << endl;
	}
}

bool Sort_compare0(iris a, iris b)
{
	return (a.value[0] < b.value[0]);
}
bool Sort_compare1(iris a, iris b)
{
	return (a.value[1] < b.value[1]);
}
bool Sort_compare2(iris a, iris b)
{
	return (a.value[2] < b.value[2]);
}
bool Sort_compare3(iris a, iris b)
{
	return (a.value[3] < b.value[3]);
}

void Construct_tree(vector<iris> &data, struct node* &root, int num)
{

	//cout << "new node created" << " " << data.size() << endl;
	root = new struct node;  //NULL to thing
	root->data.assign(data.begin(), data.end());  //put data into node
	root->isLeaf = true;
	root->species = data[0].species;
	for (int i = 1; i < data.size(); i++)
	{
		if (data[i].species != data[i - 1].species)
		{
			root->isLeaf = false;
			root->species = "";
			break;
		}
	}
	if (root->isLeaf == true)
	{
		//cout << "leaf" << " " << root->species << endl;
		return;
	}
	vector<int> species_change_num;
	double highest_gain = 0;
	double highest_gap[2];  // [feature][gap_location]
	vector<iris> lower_data, higher_data;
	for (int i = 0; i < 4; i++)
	{
		if (num == i)
		{
			continue;
		}
		species_change_num.clear();
		switch (i)
		{
		case 0:
			sort(data.begin(), data.end(), Sort_compare0);
			break;
		case 1:
			sort(data.begin(), data.end(), Sort_compare1);
			break;
		case 2:
			sort(data.begin(), data.end(), Sort_compare2);
			break;
		case 3:
			sort(data.begin(), data.end(), Sort_compare3);
			break;
		}
		for (int j = 1; j < data.size(); j++)
		{
			if (data[j].species != data[j - 1].species)
			{
				species_change_num.push_back(j);
			}
		}
		for (int j = 0; j < species_change_num.size(); j++)
		{
			vector<iris> lower;
			vector<iris> higher;
			lower.clear();
			higher.clear();
			lower.assign(data.begin(), data.begin() + species_change_num[j]);
			higher.assign(data.begin() + species_change_num[j], data.end());
			double temp = CalculateGain(data, higher, lower, i);
			//cout << temp << endl;
			if (temp >= highest_gain)
			{
				highest_gain = temp;
				highest_gap[0] = i;
				highest_gap[1] = data[species_change_num[j]].value[i];
				lower_data.assign(lower.begin(), lower.end());
				higher_data.assign(higher.begin(), higher.end());
			}
		}
	}
	root->feature = highest_gap[0];
	//cout << "feature: " << root->feature << " highest gain: " << highest_gain << endl;
	root->gap_value = highest_gap[1];
	root->higher = NULL;
	root->lower = NULL;
	//cout <<" higher size "<< higher_data.size() << " lower size " << lower_data.size() << endl;
	Construct_tree(higher_data, root->higher, num);
	Construct_tree(lower_data, root->lower, num);
	return;
}

void Test_tree(vector<iris> &test_data, struct node* root, vector<vector<string>> &result, int num) ////[tree_id][data_id][prediction & fact]
{
	for (int i = 0; i < 150; i++)
	{
		struct node* current;
		current = root;
		while (current->isLeaf == false)
		{
			if (test_data[i].value[current->feature] >= current->gap_value)
			{
				current = current->higher;
			}
			else
			{
				current = current->lower;
			}
		}
		result[num][i] = current->species;
	}
	return;
}

vector<double> CalResult(vector<iris> test_data, vector<vector<string>> result)
{
	vector<string> vote;
	vote.resize(150);
	int num_Setosa, num_Versicolor, num_Virginica, num_correct;
	for (int i = 0; i < 150; i++)
	{
		num_Setosa = num_Versicolor = num_Virginica = 0;
		for (int j = 0; j < 5; j++)
		{
			if (result[j][i] == "Iris-setosa")
			{
				num_Setosa++;
			}
			else if (result[j][i] == "Iris-versicolor")
			{
				num_Versicolor++;
			}
			else if (result[j][i] == "Iris-virginica")
			{
				num_Virginica++;
			}
		}
		int highest = num_Setosa;
		string answer = "Iris-setosa";
		if (num_Versicolor > highest)
		{
			highest = num_Versicolor;
			answer = "Iris-versicolor";
		}
		if (num_Virginica > highest)
		{
			highest = num_Virginica;
			answer = "Iris-virginica";
		}
		vote[i] = answer;
		//cout << num_Setosa << " " << num_Versicolor << " " << num_Virginica << " prediction: " << vote[i] << ", fact: " << test_data[i].species << endl;

	}
	num_correct = 0;
	for (int i = 0; i < 150; i++)
	{
		if (vote[i] == test_data[i].species)
		{
			num_correct++;
		}
	}
	int TP[3] = { 0 }, TN[3] = { 0 }, FP[3] = { 0 }, FN[3] = { 0 }; //0:setosa 1:versicolor 2:virginica
	for (int i = 0; i < 150; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			switch (j)
			{
			case 0:
				if (vote[i] == "Iris-setosa" && test_data[i].species == "Iris-setosa")
				{
					TP[j]++;
				}
				else if (vote[i] == "Iris-setosa" && test_data[i].species != "Iris-setosa")
				{
					FP[j]++;
				}
				else if (vote[i] != "Iris-setosa" && test_data[i].species == "Iris-setosa")
				{
					FN[j]++;
				}
				else
				{
					TN[j]++;
				}
				break;
			case 1:
				if (vote[i] == "Iris-versicolor" && test_data[i].species == "Iris-versicolor")
				{
					TP[j]++;
				}
				else if (vote[i] == "Iris-versicolor" && test_data[i].species != "Iris-versicolor")
				{
					FP[j]++;
				}
				else if (vote[i] != "Iris-versicolor" && test_data[i].species == "Iris-versicolor")
				{

					FN[j]++;
				}
				else
				{
					TN[j]++;
				}
				break;
			case 2:
				if (vote[i] == "Iris-virginica" && test_data[i].species == "Iris-virginica")
				{
					TP[j]++;
				}
				else if (vote[i] == "Iris-virginica" && test_data[i].species != "Iris-virginica")
				{
					FP[j]++;
				}
				else if (vote[i] != "Iris-virginica" && test_data[i].species == "Iris-virginica")
				{
					FN[j]++;
				}
				else
				{
					TN[j]++;
				}
				break;
			}
		}
	}
	double accuracy = num_correct / 150.0;
	double setosa_precision = TP[0] + FP[0] > 0 ? TP[0] / (double)(TP[0] + FP[0]) : 0;
	double versicolor_precision = TP[1] + FP[1] > 0 ? TP[1] / (double)(TP[1] + FP[1]) : 0;
	double virginica_precision = TP[2] + FP[2] > 0 ? TP[2] / (double)(TP[2] + FP[2]) : 0;
	double setosa_recall = TP[0] + FN[0] > 0 ? TP[0] / (double)(TP[0] + FN[0]) : 0;
	double versicolor_recall = TP[1] + FN[1] > 0 ? TP[1] / (double)(TP[1] + FN[1]) : 0;
	double virginica_recall = TP[2] + FN[2] > 0 ? TP[2] / (double)(TP[2] + FN[2]) : 0;
	

	vector<double> stat;
	stat.push_back(setosa_precision);
	stat.push_back(versicolor_precision);
	stat.push_back(virginica_precision);
	stat.push_back(setosa_recall);
	stat.push_back(versicolor_recall);
	stat.push_back(virginica_recall);
	stat.push_back(accuracy);
	return stat;
}

void K_fold_Vaidation(vector<iris> &data)
{
	vector<double> value_result;
	vector<vector<string>> result; //[tree_id][data_id]//prediction
	result.resize(5);
	for (int i = 0; i < 5; i++)
	{
		result[i].resize(150);
	}
	vector<double> mean_total_result;
	vector<iris> train_data, test_data, temp, temp2;
	mean_total_result.resize(7);
	for (int i = 0; i < 7; i++)
	{
		mean_total_result[i] = 0;
	}
	for (int i = 0; i < 5; i++)
	{
		train_data.clear();
		test_data.clear();
		test_data.assign(data.begin() + i * 30, data.begin() + (i + 1) * 30);
		temp.assign(data.begin(), data.begin() + i * 30);
		temp2.assign(data.begin() + (i + 1) * 30, data.begin() + 150);
		train_data.insert(train_data.end(), temp.begin(), temp.end());
		train_data.insert(train_data.end(), temp2.begin(), temp2.end());
		struct node* root[5] = { NULL };  //(point to NULL)
		for (int j = 0; j < 5; j++) // 5 trees
		{
			Construct_tree(train_data, root[j], j); //j=0: 123, i=1: 023, i=2: 013, i=3: 012, i=4:0123;
			Test_tree(data, root[j], result, j);
		}
		value_result = CalResult(data, result);
		for (int k = 0; k < 7; k++)
		{
			mean_total_result[k] += value_result[k];
		}
	}
	for (int i = 0; i < 7; i++)
	{
		mean_total_result[i] = mean_total_result[i] / 5.0;
	}	
	cout << mean_total_result[6] << endl;
	cout << mean_total_result[0] << " " << mean_total_result[3] << endl;
	cout << mean_total_result[1] << " " << mean_total_result[4] << endl;
	cout << mean_total_result[2] << " " << mean_total_result[5] << endl;
}

int main()
{
	srand(time(NULL));
	vector<iris> data;
	data.resize(150);
	InputData(data);
	Shuffle(data);
	K_fold_Vaidation(data);
	
	return 0;
}