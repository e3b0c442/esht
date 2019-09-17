# esht - e3b0c442's simple hash table
Not pronounced: e-sh*t

This project aims to implement a simple hash table in ANSI standard C for use in trivial and semi-trivial applications. It is designed with the following principles in mind:

* **Eliminate bookkeeping.** _esht_ manages its own memory, making copies both on insert/update and retrieval. This simplifies memory management greatly; the end user can use ephemeral keys and data without needing to copy, and is only responsible for freeing values retrieved from the table and the table itself.
* **Minimal storage.** _esht_ stores the absolute minimum possible amount of data to function. 
* **Simple to use.** _esht_ aims to behave like maps or dictionaries in higher-level languages: `esht_update` either inserts a new value or updates a value already existing with that key; `esht_get` retrieves the value for the key. No messing with structs or other abstractions.
* **Simple implementation.** _esht_ aims to keep its implementation as simple as possible to improve testability and keep bugs at a minimum.
* **No external dependencies.** _esht_ compiles on any machine that adheres to the ANSI C standard, with no other dependency.

The following are explicitly NOT goals of the project:
* **Performance**. In order to simplify implementation, Daniel J. Bernstein's _DJB2a_ hash function is utilized, instead of more performant functions with better data distribution like _xxhash_. In addition, buckets are implemented with singly linked lists and as such can only be traversed linearly.
* **Minimizing allocations**. In order to simplify both the implementation and end-user memory management, heap allocations are made every time the table is resized or a value is inserted. If your use case is allocation- or fragmentation-sensitive (e.g. embedded), this implementation is not optimal for you.

## API
* `esht *esht_create()`: Allocates and returns a pointer to a new hash table. Use `esht_destroy` to free the table's memory when no longer in use.
* `int esht_update(esht *table, char *key, void *value, size_t len)`: Adds a new item to the table, or updates an item already existing with that key. Returns `0` on success or `1` on failure to allocate memory. In case of failure, the previous value is retained in the table.
  * The key and value are copied into the table, allowing for literals and temporary values to be used, or for allocated values to immediately be freed.
  * If a value is updated, the previous value is freed in the table
  * The table will automatically expand as needed to maintain a sane load factor
* `void *esht_get(esht *table, char *key, size_t *len)`: Retrieves a value from the table. Returns `NULL` if the key does not exist in the table, or a pointer to a copy of the key's value. If `len` is not `NULL`, it is populated with the size of the returned data.
  * The value is copied from the table. The caller is responsible for freeing the returned value, and subsequent updates to the key in the table will not be reflected in the returned value.
  * Set `len` to `NULL` If you do not need the length of the raw data.
* `int esht_remove(esht *table, char *key)`: Removes a value from the table. Returns `0` if the value was found and removed, or `1` if the value was not found in the table.
  * The table will automatically contract as needed to maintain a sane load factor.
* `void esht_destroy(esht *table)`: Destroys the table, freeing all allocated memory for keys and values and the table structure. The user is responsible for freeing keys and values previously provided to or retrieved from the table, as the table makes copies on all update and get operations.