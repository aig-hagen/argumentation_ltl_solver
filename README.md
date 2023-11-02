# ltl-argumentation-solver v0.1 (11-2023)

Compile via
```
    ./build.sh
```

Implements the problem DS-PR and supports the
TGF format for abstract argumentation frameworks. 

Works with any external SAT solver that can read dimacs input from <stdin>.
Build the SAT solver seperately and provide the link to the executable in 'ltl-solver.sh'.

Example usage:
```
  ./ltl-solver.sh -p DS-PR -fo tgf -f <file in TGF format> -a <argument>
```
