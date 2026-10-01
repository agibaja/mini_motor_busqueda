# Mini Search Engine

Mini search engine developed in C++ as a Data Structures and Algorithms project.

The project explores how different data structures can be used to build an efficient search engine. It starts by creating an inverted index for a collection of documents and progressively adds duplicate handling, boolean queries and ordered request processing.

## Project structure

The project is divided into four main parts.

### Part 1 — Inverted index

The first part builds an **inverted index** that maps each relevant word to the documents in which it appears.

The index is implemented using:

`unordered_map<string, vector<int>>`

Each word is used as the key of the hash table, while the associated vector stores the IDs of the documents containing that word.

Using a hash table provides average **O(1)** complexity for both insertion and lookup.

This part generates the basic index used by the rest of the project.

---

### Part 2 — Duplicate control

The second part extends the inverted index so that duplicated documents do not generate duplicated entries.

Instead of storing document IDs in a vector, the index uses:

`unordered_map<string, unordered_set<int>>`

The `unordered_set` automatically guarantees that each document ID appears only once for each word while maintaining average O(1) insertion and lookup.

This version is useful when the input corpus may contain duplicated documents.

---

### Part 3 — Boolean queries

The third part allows searches containing multiple words and the boolean operators:

- `AND`
- `OR`

The program loads the previously generated index and processes each query using the **Shunting Yard algorithm**.

Queries are first converted from infix notation to postfix notation so that operator precedence can be handled correctly:

`AND` has higher precedence than `OR`.

The postfix expression is then evaluated using set operations over the document lists:

- `AND` → intersection of document IDs
- `OR` → union of document IDs

The result is the list of documents that satisfy the query.

---

### Part 4 — Request processing

The fourth part extends the query system by introducing search requests with:

- a timestamp
- a user
- a query

Requests are stored in a `std::queue` and processed in **FIFO order** so that the first request received is also the first one processed.

Each query is evaluated using the same boolean-query system developed in Part 3.

Timestamps are also checked to verify that the requests have been processed in their expected order.

---

## Query processing

Parts 3 and 4 share several helper functions.

### `cargarIndice()`

Loads the index generated in Part 1 and creates the hash table used to perform searches.

### `esOperador()`

Checks whether a token represents `AND`, `OR`, or a search term.

### `precedencia()`

Defines the priority of boolean operators so that `AND` is evaluated before `OR`.

### `infixAPostfix()`

Implements the Shunting Yard algorithm to transform a query from infix notation into postfix notation.

For example:

`data AND structures OR algorithms`

is converted into a postfix expression that can be evaluated using a stack.

### `interseccion()`

Computes the intersection of two document-ID vectors.

It implements the behaviour of the `AND` operator.

### `unionVec()`

Computes the union of two document-ID vectors.

It implements the behaviour of the `OR` operator.

### `evaluarPostfix()`

Evaluates a complete postfix query using the inverted index and returns the matching document IDs.

### `formatearIds()`

Converts a vector of document IDs into text so that the results can be written to an output file.

---

## Performance experiments

The project also includes several experiments to study the behaviour of the selected data structures.

### Hash table performance

The first experiment measures insertion and search times for corpora of different sizes.

The results are used to compare the observed performance with the expected complexity of a hash table:

- inserting `n` words results in approximately O(n) total work
- searching for an individual word remains approximately O(1)

The tests also show the effect of caching: the first execution tends to be slower because files and memory structures have not yet been loaded into memory or cache.

### Duplicate handling

A second experiment compares indexing with and without duplicate control.

The test uses a corpus containing duplicated documents and compares:

- indexing time
- number of index entries
- total number of stored document IDs

Without duplicate control, repeated document IDs can appear multiple times in the index.

Using `unordered_set` requires slightly more work during insertion, but ensures that the resulting index contains unique document IDs.

---

## Data structures used

The project uses several standard C++ data structures:

- `unordered_map` — inverted index and fast word lookup
- `vector` — storage of document IDs
- `unordered_set` — duplicate prevention
- `stack` — evaluation of boolean expressions and Shunting Yard
- `queue` — FIFO processing of search requests

The project is designed to show how the choice of data structure affects both the correctness and the performance of an application.
