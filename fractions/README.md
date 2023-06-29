# fractions project

## Getting Started

This project represents a collection of basic operations on fractions. The problem that this project tries to solve is that upon dividing, multiplying and other operations, a lot of information is lost on every step, now the project api covers the most used functions for fractions, and keep the double data as fractions.

For example *0.5*, will be codified as *(1, 2, plus)*

Every double number can be represented as a tuple of numerator, denumerator and sign.

**If you do not care about** aproximations and data loss, then you should not use the *frac_t* structure. The structure stores 2 unsigned ints on 32 bits and enum on 1 byte, so the total ammount of memory at most is 3 bytes, which is only a byte more than the double data type, however the frac structure can store upon an infinite amount of digits after the comma, by encoding the fractions as shown before.

The API consists in the following functions:

* *fxy* - creates a fraction.
* *feval* - evauluates a fraction as double.
* *fadd* - adds two fractions and returns the result.
* *fsub* - subtracts two fractions and returns the result.
* *fmul* - multiplies two fractions and returns the result.
* *fdiv* - divides two fractions and returns the result.
* *fmconst* - multiplies a fraction with a constant.
* *fdconst* - divides a fraction with a constant.

The API also comes with *pointer* functions, that do the same thing, however upates the first fraction instead of returning a new fraction.

The methods are:

* *faddp* - adds two fractions and returns the result.
* *fsubp* - subtracts two fractions and returns the result.
* *fmulp* - multiplies two fractions and returns the result.
* *fdivp* - divides two fractions and returns the result.
* *fmconstp* - multiplies a fraction with a constant.
* *fdconstp* - divides a fraction with a constant.

## Building the project

The project can be compiled in:

* *dynamic library*
* *static library*

The builder of the project will create the both of them and you can choose which to use.

**In order** to compile the project, enter the prompt of an terminal and change directory inside the `c-language-data-structures`.

```bash
  cd path/to/dir/c-language-data-structures
```

Enter the `fractions` folder and enter the `build` directory:

```bash
  cd fractions
  cd build
```

Build the project by running the `make` utility:

```bash
  make
```

In the root of the *fractions* project a `libs` folder will appear, which will contain the dynamic and static libraries.

## Testing the project

This project comes with a wide unit testing file.

In order to test the functionalities of the fractions API and to see some examples of how to use the fractions in your future projects, enter the *tests* folder and run *make* to run the tests:

```bash
  cd tests
  make
```

The libraries will be deleted if there was a previous *build* and the makefile will call again the *make* from the build folder and will start the tests, that can be found under the **units.c** file.

## Infinity and NaN

The fractions API allows for *infinity* and *not a number* principles. All the operators test if the number is Nan or infinity and performs the action regarding the mathematics principles, like **inf + inf = inf**

I encourge you to look upon the tests, because they discribe very well for every method what are the corner cases, and how can infinity and nan be used.

## fbool

Because we are working with infinity and NaN, we need a new layer of abstraction, some functions like comparing and equalities, would not know what the result can be *true* or *false*.

For example:

```text
  inf == inf -> this calls and indeterminated operation, because we can reduce it to the following expression:

  inf - inf == 0, as we know the inf - inf can be any number including zero, so this statement has not a truth value, so in exchange we return a `funknown` state.
```

A loot of examples like this are covered in the test cases.
