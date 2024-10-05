#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <utility>

using namespace std;

vector<vector<double> > readCSV(const string &filename) {
    vector<vector<double> > data;
    ifstream file(filename);
    string line;

    // Başlık satırını atla
    getline(file, line);

    while (getline(file, line)) {
        stringstream lineStream(line);
        string cell;

        vector<double> row;
        int colIndex = 0;

        while (getline(lineStream, cell, ',')) {
            if (colIndex != 0) { // İlk sütunu atla
                try {
                    // Boş hücreleri ve geçersiz verileri kontrol et
                    if (cell.empty() || cell.find_first_not_of("0123456789.") != string::npos) {
                        cerr << "Warning: Skipping invalid data at column " << colIndex << ": " << cell << endl;
                        continue;
                    }
                    double value = stod(cell);
                    row.push_back(value);
                } catch (const invalid_argument& e) {
                    cerr << "Warning: Skipping invalid data at column " << colIndex << ": " << cell << endl;
                }
            }
            colIndex++;
        }

        if (!row.empty()) {
            data.push_back(row);
        }
    }

    return data;
}
pair<double,double> initial_Weight(){
    double w=0.0;
    double b=0.0;
    return make_pair(w,b);
}
double predict(double x,double w,double b){
    return w*x+b;
}
double computeMSE(const vector<vector<double> >&data,double w,double b){
    double mse=0.0;
    int n=data.size();
    for(int i=0;i<n;i++){
        double x=data[i][0];
        double y=data[i][1];
        double y_pred=predict(x,w,b);
        mse+=pow(y-y_pred,2);
    }
    return mse/n;
}
pair<double,double>gradient_Descent(const vector<vector<double> >&data,double w,double b,double learningRate){
    double w_grad=0.0;
    double b_grad=0.0;
    int n=data.size();
    for (int i = 0; i < n; i++)
    {
        double x=data[i][0];
        double y=data[i][1];
        double error=predict(x,w,b);
        w_grad+=error*x;
        b_grad+=error;
    }
    w_grad=(2.0/n)*w_grad;
    b_grad=(2.0/n)*b_grad;

    w-=learningRate*w_grad;
    b-=learningRate*b_grad;

    return make_pair(w,b);
}
pair<double, double>linear_Regression(const vector<vector<double> >&data,int epochs,double learningRate){
    pair<double,double>weights=initial_Weight();
    double w=weights.first;
    double b=weights.second;
    int n=data.size();

    for(int i=0;i<n;i++){
        pair<double,double>result=gradient_Descent(data,w,b,learningRate);
        w=result.first;
        b=result.second;
        double mse=computeMSE(data,w,b);
    }
    return make_pair(w,b);
}
int main(){
    string filename="data.csv";
    vector<vector<double> >data=readCSV(filename);

    int epochs=1000;
    double learningRate=0.01;
    pair<double,double>coefficents=linear_Regression(data,epochs,learningRate);
    double w=coefficents.first;
    double b=coefficents.second;

    cout<<"Final coefficents: w= "<<w<<", b= "<<b<<endl
}