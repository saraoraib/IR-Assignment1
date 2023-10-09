#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

const int MAX_DOCUMENTS = 1000;
const int MAX_POSITIONS = 100;
const int MAX_TERMS = 100;

struct PositionInfo {
    int documentId;
    int positions[MAX_POSITIONS];
    int numPositions;
};

struct InvertedIndexEntry {
    std::string term;
    int numDocuments;
    PositionInfo documents[MAX_DOCUMENTS];
};

struct DocIdFilePathMapping {
    int documentId;
    std::string filePath;
};

void addToInvertedIndex(InvertedIndexEntry invertedIndex[MAX_TERMS], int& numTerms, const std::string& term, int documentId, int position) {
    for (int i = 0; i < numTerms; ++i) {
        if (invertedIndex[i].term == term) {
            int docIndex = invertedIndex[i].numDocuments;
            invertedIndex[i].documents[docIndex].documentId = documentId;
            invertedIndex[i].documents[docIndex].positions[invertedIndex[i].documents[docIndex].numPositions] = position;
            invertedIndex[i].documents[docIndex].numPositions++;
            invertedIndex[i].numDocuments++;
            return;
        }
    }

    invertedIndex[numTerms].term = term;
    invertedIndex[numTerms].documents[0].documentId = documentId;
    invertedIndex[numTerms].documents[0].positions[0] = position;
    invertedIndex[numTerms].documents[0].numPositions = 1;
    invertedIndex[numTerms].numDocuments = 1;
    numTerms++;
}

void writeInvertedIndexToJson(const InvertedIndexEntry invertedIndex[MAX_TERMS], int numTerms) {
    std::ofstream outputFile("pos_inverted_index.json");
    if (outputFile.is_open()) {
        for (int i = 0; i < numTerms; ++i) {
            outputFile << "\\\"" << invertedIndex[i].term << "\\\": ["
                << invertedIndex[i].numDocuments << ", {";
            for (int j = 0; j < invertedIndex[i].numDocuments; ++j) {
                if (j > 0) outputFile << ", ";
                outputFile << invertedIndex[i].documents[j].documentId << " : [";
                for (int k = 0; k < invertedIndex[i].documents[j].numPositions; ++k) {
                    if (k > 0) outputFile << ", ";
                    outputFile << invertedIndex[i].documents[j].positions[k];
                }
                outputFile << "]";
            }
            outputFile << "}]";
            if (i < numTerms - 1) outputFile << ", ";
        }
        outputFile.close();
    }
}

void processQuery(const InvertedIndexEntry invertedIndex[MAX_TERMS], int numTerms, const std::string& query) {
    std::unordered_set<int> documentIds;

    for (int i = 0; i < numTerms; ++i) {
        if (invertedIndex[i].term == query) {
            for (int j = 0; j < invertedIndex[i].numDocuments; ++j) {
                documentIds.insert(invertedIndex[i].documents[j].documentId);
            }
            break;
        }
    }

    if (!documentIds.empty()) {
        std::cout << "Documents containing the term \\\"" << query << "\\\":\\n";
        for (int docId : documentIds) {
            std::cout << "Document ID: " << docId << "\\n";
        }
    }
    else {
        std::cout << "No documents found containing the term \\\"" << query << "\\\":\\n";
    }
}


int main() {
    InvertedIndexEntry invertedIndex[MAX_TERMS];
    int numTerms = 1;

    std::string query;
    std::cout << "Enter a query: ";
    std::getline(std::cin, query);

    processQuery(invertedIndex, numTerms, query);

    return 0;
}