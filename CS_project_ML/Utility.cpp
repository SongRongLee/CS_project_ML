#include"Utility.h"
#include <windows.h>
bool mycomp(pair<int, double> a, pair<int, double> b) {
	return a.second < b.second;
}

bool mycomp_label(pair<int, double> a, pair<int, double> b) {
	return a.first < b.first;
}

bool mycomp_index(MyData a, MyData b) {
	return a.num < b.num;
}

bool compfunc_mydata(pair<MyData, double> a, pair<MyData, double> b) {
	return a.second < b.second;
}
bool compfunc_dispair(pair<vector<pair<int, double>>, double> a, pair<vector<pair<int, double>>, double> b)
{
	return a.second < b.second;
}
bool compfunc_descend(pair<int, double> a, pair<int, double> b) {
	return a.second > b.second;
}

string getPrefix(string dirname) {
	int found;
	found = dirname.find_last_of("\\");
	return dirname.substr(found + 1);
}

void extractData(vector<MyData> &X, vector<MyData> &T, string dirname, int foldnum) {
	//configure file names
	string prefix = getPrefix(dirname);
	string datadir = dirname + "\\" + prefix + ".data";
	
	//for folds
	string foldname = prefix + "_fold" + to_string(foldnum) + ".cv";
	string folddir = dirname + "\\" + foldname;

	//start reading from .data
	ifstream in(datadir);
	ifstream infold(folddir);
	cout << "reading from " << foldname << endl;

	int data_num, feature_num;
	int fold_data;
	char comma;

	in >> data_num >> comma >> feature_num;
	for (int i = 0; i < data_num; i++) {
		MyData temp_data;
		in >> temp_data.num >> comma;
		infold >> fold_data;
		for (int j = 0; j < feature_num; j++) {
			double temp;
			in >> temp >> comma;
			temp_data.features.push_back(temp);
		}
		in >> temp_data.label;
		if (fold_data == -1) {
			temp_data.is_train = false;
			T.push_back(temp_data);
		}
		else{
			temp_data.is_train = true;
			X.push_back(temp_data);
		}		
	}
}

void extractData(vector<MyData> &X, vector<MyData> &XT, vector<MyData> &T, string dataname, string labelname) {

	//start reading from .data
	ifstream in(dataname);
	ifstream infold(labelname);
	cout << "reading from " << labelname << endl;

	int data_num, feature_num;
	int fold_data;
	char comma;

	in >> data_num >> comma >> feature_num;
	for (int i = 0; i < data_num; i++) {
		MyData temp_data;
		in >> temp_data.num >> comma;
		infold >> fold_data;
		for (int j = 0; j < feature_num; j++) {
			double temp;
			in >> temp >> comma;
			temp_data.features.push_back(temp);
		}
		in >> temp_data.real_label;
		if (fold_data == -1) {
			temp_data.is_train = false;
			temp_data.label = -1;			
			T.push_back(temp_data);
		}
		else if(fold_data == -2){
			temp_data.is_train = false;
			temp_data.label = -2;
			XT.push_back(temp_data);
		}
		else {
			temp_data.is_train = true;
			temp_data.label = temp_data.real_label;
			X.push_back(temp_data);
		}
	}
}

int checkResult(vector<int> &result, vector<MyData> &T) {
	int vsize = result.size();
	int ans = 0;
	for (int i = 0; i < vsize; i++) {
		if (result[i] != T[i].real_label) ans++;
	}
	return ans;
}
double calDistance(MyData a, MyData b, int dis_type) {
	double temp_dis;
	switch (dis_type)
	{
	case EU_DIS:
		temp_dis = euDistance(a, b);
		break;
	default:
		temp_dis = 0;
		break;
	}
	return temp_dis;
}
double euDistance(MyData a, MyData b) {

	if (a.features.size() != b.features.size()) {
		cout << "Euclidean distance error, size mismatch.\n";
		return -1;
	}

	int vsize = a.features.size();
	double tempsquare = 0;
	for (int i = 0; i < vsize; i++) {
		tempsquare += pow(a.features[i] - b.features[i], 2);
	}
	return sqrt(tempsquare);
}

void genDismatrix(vector<MyData> &X, vector<vector<double>> &dis_matrix, int dis_type) {
	dis_matrix.clear();
	for (int i = 0; i < X.size(); i++) {
		vector<double> row_vector;
		for (int j = 0; j < X.size(); j++) {
			if (i == j) {
				row_vector.push_back(0);
			}
			else if (i > j) {
				row_vector.push_back(dis_matrix[j][i]);
			}
			else {
				double temp_dis;
				temp_dis = calDistance(X[i], X[j], dis_type);
				row_vector.push_back(temp_dis);
			}
		}
		dis_matrix.push_back(row_vector);
	}
}

void printDismatrix(vector<vector<double>> &dis_matrix) {
	ofstream out("out.txt");
	for (int i = 0; i < dis_matrix.size(); i++) {
		for (int j = 0; j < dis_matrix[i].size(); j++) {
			out << left << fixed << setprecision(18) << setw(21) << dis_matrix[i][j];
		}
		out << endl;
	}
}
void CreateFolder(const string  path)
{
	if (!CreateDirectoryA(path.c_str(), NULL))
	{
		return;
	}
}
void printDismatrix(vector<vector<double>> &dis_matrix, ofstream &out) {
	for (int i = 0; i < dis_matrix.size(); i++) {
		for (int j = 0; j < dis_matrix[i].size(); j++) {
			out << fixed << setprecision(18) << setw(21) << dis_matrix[i][j];
		}
		out << endl;
	}
}
void printTestDis(vector<vector<vector<double>>> dis_matrixs, int num, const vector<MyData> &total_data) {
	string title = "testing_dis" + to_string(num) + ".txt";
	ofstream out(title);
	for (int i = 0; i < dis_matrixs.size(); i++)
	{
		for (int j = 0; j < dis_matrixs[i].size(); j++) {
			if(total_data[j].label!=-1)
				out << fixed << setprecision(18) << setw(21) << dis_matrixs[i][num][j];
		}
		out << endl;
	}

}
void printlabel(vector<MyData>& total_data, ofstream &out)
{
	int j = -1;
	int k = 0;
	while (k<total_data.size())
	{
		
		for (int i = 0; i < total_data.size(); i++)
		{	if(total_data[i].num==j+1)
			{
				if (total_data[i].real_label != total_data[i].knn_label)
					out << 1 << ' ';
				else
					out << 0 << ' ';
				
				j++;
				k++;
				//cout << total_data[i].num << ' ';
				break;
			}
			if(i== total_data.size()-1)
				j++;
		}

		
	}
}
/*void printTestDis(vector<vector<vector<double>>> dis_matrixs,int num , const vector<MyData> &total_data, ofstream &out) {
	for (int i = 0; i < dis_matrixs.size(); i++)
	{
		for (int j = 0; j < dis_matrixs[i].size(); j++) {
			if (total_data[j].label != -1)
				out << fixed << setprecision(18) << setw(21) << dis_matrixs[i][num][j];
		}
		out << endl;
	}
	
}*/
void indexSortedMatrix(vector<MyData> &total_data, vector<vector<double>> &dis_matrix, vector<vector<double>> &new_dis) {
	new_dis = dis_matrix;
	for (int j = 0; j < dis_matrix.size(); j++) {
		for (int k = 0; k < dis_matrix.size(); k++) {
			int indexj, indexk;
			for (int a = 0; a < total_data.size(); a++) {
				if (total_data[a].num == j) {
					indexj = a;
					break;
				}
			}
			for (int a = 0; a < total_data.size(); a++) {
				if (total_data[a].num == k) {
					indexk = a;
					break;
				}
			}
			new_dis[j][k] = dis_matrix[indexj][indexk];
		}
	}
}

void indexSortedAllMatrix(vector<MyData>& total_data, vector<vector<vector<double>>>& dis_matrixs, vector<vector<vector<double>>>& new_diss)
{
	new_diss = dis_matrixs;
	for (int i=0; i < dis_matrixs.size(); i++) {
		for (int j = 0; j < dis_matrixs[i].size(); j++) {
			for (int k = 0; k < dis_matrixs[i].size(); k++) {
				int indexj, indexk;
				for (int a = 0; a < total_data.size(); a++) {
					if (total_data[a].num == j) {
						indexj = a;
						break;
					}
				}
				for (int a = 0; a < total_data.size(); a++) {
					if (total_data[a].num == k) {
						indexk = a;
						break;
					}
				}
				new_diss[i][j][k] = dis_matrixs[i][indexj][indexk];
			}
		}
	}

}
