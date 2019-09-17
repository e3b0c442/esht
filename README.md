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