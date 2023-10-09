# IR-Assignment1

## Creating and Querying Positional Indexes

<h3> I have built more than one function in the code, and here I will write a simple explanation about each one </h3>
<h3>addToInvertedIndex function</h3>
<h4>The function that adds entries to the inverted index. The input consists of the inverted index array, currently active documents, a term, a document ID, and a position. It looks whether the word indexed previously. It therefore updates the existing positions or create new entries.</h4>

<h3>writeInvertedIndexToJson function</h3>
<h4>This function converts the inverted index into a JSON file. The index is represented in the form of a JSON file named pos_inverted_index.json, which takes an input of an inverted index array and a number of terms.</h4>

<h3>processQuery function</h3>
<h4>User query passes through this function. The input involves the inverted index array, the number of terms, and a query term. It queries the index with the given search term, retrieving the document IDs where the given term appears.</h4>
