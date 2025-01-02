#include "dataFrame.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdexcept>

using namespace std;

DataFrame::DataFrame() : delim(',') {}

void DataFrame::readCSV(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Unable to open file");
    }

    string ticker = filename.substr(filename.find_last_of("/") + 1);
    size_t secondLastDot = ticker.find_last_of(".", ticker.find_last_of(".") - 1);
    ticker = ticker.substr(0, secondLastDot);

    string line;
    int rowCount = 0;
    float movingTotal7 = 0.0, movingTotal30 = 0.0;

    rows.reserve(10000);

    while (getline(file, line)) {
        if (rowCount == 0) {
            line.erase(0, line.find_first_not_of(" \t\n\r"));
            line.erase(line.find_last_not_of(" \t\n\r") + 1);
        }

        istringstream stream(line);
        string token;

        if (rowCount == 0) {
            getline(stream, token, delim);
            columns.push_back(token);
            columns.push_back("Ticker");
            while (getline(stream, token, delim)) {
                columns.push_back(token);
            }
            columns.push_back("MVA7");
            columns.push_back("MVA30");
            numCols = columns.size();
        } else {
            Row row;
            getline(stream, row.date, delim);
            row.ticker = ticker;
            stream >> row.open;
            stream.ignore(1);
            stream >> row.high;
            stream.ignore(1);
            stream >> row.low;
            stream.ignore(1);
            stream >> row.close;
            stream.ignore(1);
            stream >> row.volume;
            stream.ignore(1);
            stream >> row.openInt;

            if (rowCount > 7) {
                row.movAvg7 = movingTotal7 / 7;
                movingTotal7 -= rows[rowCount - 8].close;
            } else {
                row.movAvg7 = 0;
            }
            movingTotal7 += row.close;

            if (rowCount > 30) {
                row.movAvg30 = movingTotal30 / 30;
                movingTotal30 -= rows[rowCount - 31].close;
            } else {
                row.movAvg30 = 0;
            }
            movingTotal30 += row.close;

            rows.push_back(row);
        }
        rowCount++;
    }
    numRows = rows.size();
}

void DataFrame::printRow(const Row &row) {
    cout << setw(15) << row.date
              << setw(15) << row.ticker
              << setw(15) << row.open
              << setw(15) << row.high
              << setw(15) << row.low
              << setw(15) << row.close
              << setw(15) << row.volume
              << setw(15) << row.openInt
              << setw(15) << row.movAvg7
              << setw(15) << row.movAvg30
              << endl;
}

void DataFrame::printHead(int n) {
    for (const auto &col : columns) {
        cout << setw(15) << col;
    }
    cout << endl;

    for (int i = 0; i < n && i < numRows; i++) {
        printRow(rows[i]);
    }
}

void DataFrame::printDataFrame() {
    for (const auto &col : columns) {
        cout << setw(15) << col;
    }
    cout << endl;

    for (const auto &row : rows) {
        printRow(row);
    }
}

DataFrame::~DataFrame() {}