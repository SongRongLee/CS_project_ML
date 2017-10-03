#include"Utility.h"

string getPrefix(string dirname) {
	int found;
	found = dirname.find_last_of("\\");
	return dirname.substr(found + 1);
}

void extractData(vector<MyData> &X, vector<MyData> &T, string dirname, int foldnum) {
	//configure file names
	string prefix = getPrefix(dirname);
	string datadir = dirname + "\\" + prefix + ".data";
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

int checkResult(vector<int> &result, vector<MyData> &T) {
	int vsize = result.size();
	int ans = 0;
	for (int i = 0; i < vsize; i++) {
		if (result[i] != T[i].label) ans++;
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
	cout << "-----------------------------" << endl;;
	for (int i = 0; i < dis_matrix.size(); i++) {
		for (int j = 0; j < dis_matrix[i].size(); j++) {
			cout <<left<<setw(15)<< dis_matrix[i][j];
		}
		cout << endl;
	}
}