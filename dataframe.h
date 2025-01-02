#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <iostream>
#include <vector>
#include <string>

#define MAX_FIELD_SIZE 256

using namespace std;

struct Row {
    string date;
    string ticker;
    float open = 0.0f;
    float high = 0.0f;
    float low = 0.0f;
    float close = 0.0f;
    int volume = 0;
    int openInt = 0;
    float movAvg7 = 0.0f;
    float movAvg30 = 0.0f;
};

class DataFrame {
private:
    vector<string> columns;
    vector<Row> rows;
    int numCols;
    int numRows;
    char delim;
    void trimWhitespace(string &line);
    int countRows(const string &filepath);
    void printRow(const Row &row);

public:
    DataFrame();
    void readCSV(const string &filename);
    void printHead(int n = 10);
    void printDataFrame();
    ~DataFrame();
};

#endif