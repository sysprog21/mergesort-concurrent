# concurrent merge sort on singly-linked list

`mergesort-concurrent` implements merge sort on linked list concurrently.

Tasks give a framework for studying concurrency:

* Restricting number of threads in thread pools.
* Relaxing task boundaries so that overhead associated with task management
  is kept reasonable.

The strategy to implement merge sort concurrently:

* Make recursive calls tasks
* No need to wait for tasks to complete after spawning them
* Tasks that creates sub-tasks can be allowed to terminated

## Build
```bash
make
```

## Run
```bash
./sort [thread count] [data count]
```
Then input integer data line-by-line

# Install customized Git pre-commit hooks
Git hooks are scripts that Git executes before or after events such as: commit, push, and receive. We provide the customized hooks for spelling errors and software quality validation while the change is about to be committed.

Before installing the Git hooks, please ensure [astyle](http://astyle.sourceforge.net/) and [cppcheck](http://cppcheck.sourceforge.net/) installed. You can install the packages via `apt-get`:
```sh
sudo apt-get install astyle cppcheck
```

Install Git pre-commit hook to check C/C++ source file format and quality:
```sh
scripts/install-git-hooks
```

# Licensing
`mergesort-concurrent` is freely redistributable under the two-clause BSD
License. Use of this source code is governed by a BSD-style license that can
be found in the `LICENSE` file.
