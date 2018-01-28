#include<iostream>
#include"KnnBayesSemi.h"
#include"NonlinearSemi.h"
#include"Utility.h"
#include"MyData.h"
#include"ClusterSemi.h"

using namespace std;
int main() {

	//---user define params---
	ofstream afineout("nonlinear.txt");
	ofstream inverseout("inverse.txt");
	ofstream clusterout("cluster.txt");
	string dirname = "C:\\Users\\Administrator\\Desktop\\testData2\\cleveland_s.data";
	//string dirname = "C:\\Users\\steven954211\\Source\\Repos\\testData2\\d1_s.data";
	string labeldir = "C:\\Users\\Administrator\\Desktop\\testData2\\cleveland_s\\label";
	//string labelname ="C:\\Users\\steven954211\\Source\\Repos\\testData2\\d1_s\\label01.txt";
	string folder = "C:\\Users\\Administrator\\Documents\\GitHub\\CS_project_ML\\matrix\\";
	int k = 1;
	int fold_num = 50;
	//------------------------

	double validation_err = 0;
	double accuracy;
	int wrong_count = 0;

	for (int i = 1; i <= fold_num; i++) {

		vector<MyData> X;
		vector<MyData> XT; //for semi-supervised
		vector<MyData> T;
		vector<int> result;
		vector<vector<double>> new_dis;

		string labelname =  labeldir + to_string(i/10) + to_string(i%10) + ".txt";
		extractData(X, XT, T, dirname, labelname);
		//extractData(X, T, dirname, i);

		//SemiTransD
		KnnBayesSemi stransd(X, XT, k);
		stransd.setT(T);
		stransd.performTrans();
		inverseout << stransd.getScore() << endl;
		
		//ClusterSemi
		ClusterSemi Cstransd(X, XT, k);
		Cstransd.setT(T);
		Cstransd.performTrans();
		clusterout << Cstransd.getScore() << endl;
		if (i == 1)
		{
			CreateFolder(folder);
			Cstransd.printSortedMatrixs(folder);
		}
			
		//AffineSemi
		/*NonlinearSemi ntransd(X, XT, k);
		ntransd.setT(T);
		ntransd.performTrans();
		afineout << ntransd.getScore() << endl;*/

		//TransD		
		/*TransD transd(X, T, k);
		transd.performTrans(new_dis);
		transd.getSortedMatrix(new_dis);
		printDismatrix(new_dis);*/

		//NMI
		/*NMIClassifier nmi(X, 1);
		result = nmi.prediction(T);*/

		//printing result
		/*wrong_count = checkResult(result, T);
		validation_err += wrong_count;
		accuracy = (double)(T.size() - wrong_count) / (double)T.size() * 100;
		cout << "Fold " << i << " done with accuracy " << accuracy << "%" << endl << endl;*/

		//addtional testing
		/*if (i == 1) {
			int vsize = result.size();
			for (int j = 0; j < vsize; j++) {
				if (result[j] != T[j].label) {
					cout << "Data No." << T[j].num << " should be " << T[j].label << endl;
					cout << "But labeled as " << result[j] << endl;
				}
			}
		}*/

		X.clear();
		T.clear();
		XT.clear();
		wrong_count = 0;
	}

	//validation_err /= 10;
	cout << "Data analyzing done." << endl;
	//cout << "Model validation value = " << validation_err << endl;
	system("pause");

	return 0;
}
