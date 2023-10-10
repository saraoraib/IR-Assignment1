#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

const int MAX_DOCUMENTS = 1000;
const int MAX_POSITIONS = 100;
const int MAX_TERMS = 100;

std::unordered_set<std::string> stopWords = { "and", "the", "is" };

struct PositionInfo {
    int documentId;
    int positions[MAX_POSITIONS];
    int numPositions;
    bool containsQueryTerm; 
};

struct InvertedIndexEntry {
    std::string term;
    int numDocuments;
    PositionInfo documents[MAX_DOCUMENTS];
};

void processDocument(const std::string& filePath, InvertedIndexEntry invertedIndex[MAX_TERMS], int& numTerms, int& numDocuments, int documentId, const std::string& query, std::unordered_map<int, std::string>& docIdFilePathMapping) {
    std::ifstream file(filePath);
    std::string word;
    int position = 0;
    bool containsQueryTerm = false;

    while (file >> word) {
        ++position;
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);

        if (stopWords.find(word) == stopWords.end() && word.length() >= 3) {
            bool termFound = false;
            for (int i = 0; i < numTerms; ++i) {
                if (invertedIndex[i].term == word) {
                    int docIndex = invertedIndex[i].numDocuments;
                    invertedIndex[i].documents[docIndex].documentId = documentId;
                    invertedIndex[i].documents[docIndex].positions[invertedIndex[i].documents[docIndex].numPositions] = position;
                    invertedIndex[i].documents[docIndex].numPositions++;
                    invertedIndex[i].numDocuments++;
                    termFound = true;
                    break;
                }
            }
            
            if (!termFound) {
                invertedIndex[numTerms].term = word;
                invertedIndex[numTerms].documents[0].documentId = documentId;
                invertedIndex[numTerms].documents[0].positions[0] = position;
                invertedIndex[numTerms].documents[0].numPositions = 1;
                invertedIndex[numTerms].numDocuments = 1;
                numTerms++;
            }

            
            if (word == query) {
                containsQueryTerm = true;
            }
        }
    }

    
    if (containsQueryTerm) {
        docIdFilePathMapping[documentId] = filePath;
    }

    numDocuments++;
}

void insertDocument(const std::string& filePath, InvertedIndexEntry invertedIndex[MAX_TERMS], int& numTerms, int& numDocuments, int documentId, const std::string& query, std::unordered_map<int, std::string>& docIdFilePathMapping) {
    std::ifstream file(filePath);
    std::string word;
    int position = 0;
    bool containsQueryTerm = false;

    while (file >> word) {
        ++position;
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);

        
        if (stopWords.find(word) == stopWords.end() && word.length() >= 3) {
            
            bool termFound = false;
            for (int i = 0; i < numTerms; ++i) {
                if (invertedIndex[i].term == word) {
                    int docIndex = invertedIndex[i].numDocuments;
                    invertedIndex[i].documents[docIndex].documentId = documentId;
                    invertedIndex[i].documents[docIndex].positions[invertedIndex[i].documents[docIndex].numPositions] = position;
                    invertedIndex[i].documents[docIndex].numPositions++;
                    invertedIndex[i].numDocuments++;
                    termFound = true;
                    break;
                }
            }
            
            if (!termFound) {
                invertedIndex[numTerms].term = word;
                invertedIndex[numTerms].documents[0].documentId = documentId;
                invertedIndex[numTerms].documents[0].positions[0] = position;
                invertedIndex[numTerms].documents[0].numPositions = 1;
                invertedIndex[numTerms].numDocuments = 1;
                numTerms++;
            }

            
            if (word == query) {
                containsQueryTerm = true;
            }
        }
    }

    
    if (containsQueryTerm) {
        docIdFilePathMapping[documentId] = filePath;
    }

    numDocuments++;
}

void deleteDocument(int documentId, InvertedIndexEntry invertedIndex[MAX_TERMS], int& numTerms, int& numDocuments, std::unordered_map<int, std::string>& docIdFilePathMapping) {
    
    for (int i = 0; i < numTerms; ++i) {
        for (int j = 0; j < invertedIndex[i].numDocuments; ++j) {
            if (invertedIndex[i].documents[j].documentId == documentId) {
                
                for (int k = j; k < invertedIndex[i].numDocuments - 1; ++k) {
                    invertedIndex[i].documents[k] = invertedIndex[i].documents[k + 1];
                }
                invertedIndex[i].numDocuments--;
                j--;
            }
        }
    }

    
    docIdFilePathMapping.erase(documentId);

    numDocuments--;
}



void writeInvertedIndexToJson(const InvertedIndexEntry invertedIndex[MAX_TERMS], int numTerms) {
    std::ofstream outputFile("pos_inverted_index.json");
    if (outputFile.is_open()) {
        outputFile << "{";

        for (int i = 0; i < numTerms; ++i) {
            outputFile << "\"" << invertedIndex[i].term << "\": {";
            outputFile << "\"numDocuments\": " << invertedIndex[i].numDocuments << ",";
            outputFile << "\"documents\": [";

            for (int j = 0; j < invertedIndex[i].numDocuments; ++j) {
                outputFile << "{";
                outputFile << "\"documentId\": " << invertedIndex[i].documents[j].documentId << ",";
                outputFile << "\"positions\": [";

                for (int k = 0; k < invertedIndex[i].documents[j].numPositions; ++k) {
                    outputFile << invertedIndex[i].documents[j].positions[k];
                    if (k < invertedIndex[i].documents[j].numPositions - 1) {
                        outputFile << ",";
                    }
                }

                outputFile << "]";
                outputFile << "}";
                if (j < invertedIndex[i].numDocuments - 1) {
                    outputFile << ",";
                }
            }

            outputFile << "]";
            outputFile << "}";
            if (i < numTerms - 1) {
                outputFile << ",";
            }
        }

        outputFile << "}";
        outputFile.close();
        std::cout << "Inverted index successfully written to pos_inverted_index.json." << std::endl;
    }
    
}

void writeDocIdFilePathMapping(const std::string& filePath, const std::unordered_map<int, std::string>& docIdFilePathMapping) {
    std::ofstream outputFile(filePath);
    if (outputFile.is_open()) {
        for (const auto& pair : docIdFilePathMapping) {
            outputFile << pair.first << "," << pair.second << "\n";
        }
        outputFile.close();
        std::cout << "DocId to FilePath mapping successfully written to " << filePath << std::endl;
    }
    
}

int main() {
    InvertedIndexEntry invertedIndex[MAX_TERMS];
    int numTerms = 0;
    int numDocuments = 0;
    std::unordered_map<int, std::string> docIdFilePathMapping;

    // Array of file paths to be processed
    std::string filePaths[] = {
        "C:/Users/HP/OneDrive/Desktop/document1.txt",
        "C:/Users/HP/OneDrive/Desktop/document2.txt",
    };

    std::string query;
    std::cout << "Enter a query: ";
    std::getline(std::cin, query);

    // Process documents using a for loop
    for (int i = 0; i < sizeof(filePaths) / sizeof(filePaths[0]); ++i) {
        processDocument(filePaths[i], invertedIndex, numTerms, numDocuments, i + 1, query, docIdFilePathMapping);
    }

    // Example usage of insertDocument function
    std::string newDocumentPath = "C:/Users/HP/OneDrive/Desktop/new_document.txt"; 
    insertDocument(newDocumentPath, invertedIndex, numTerms, numDocuments, numDocuments + 1, query, docIdFilePathMapping);
    std::cout << "New document inserted into the inverted index." << std::endl;


    // Example usage of deleteDocument function
    int documentIdToDelete = 2; 
    deleteDocument(documentIdToDelete, invertedIndex, numTerms, numDocuments, docIdFilePathMapping);
    std::cout << "Document with ID " << documentIdToDelete << " deleted from the inverted index." << std::endl;

    // Check and print documents containing the query term
    bool queryFound = false;
    for (const auto& pair : docIdFilePathMapping) {
        std::cout << "Document ID: " << pair.first << ", Document Path: " << pair.second << std::endl;
        queryFound = true;
    }

    if (!queryFound) {
        std::cout << "No documents found containing the query term." << std::endl;
    }

    return 0;
}