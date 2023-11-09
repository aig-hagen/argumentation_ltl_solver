# ltl-argumentation-solver v1.0 (11-2023)

Compile via
```
    ./build.sh
```

Implements the problem DS-PR and supports the
i23 format for abstract argumentation frameworks. 

Currentl< only creates a nuSMV-encoding of the problem that can be handed to a nuSMV solver.

Example usage:
```
  ./ltl-solver -p DS-PR -fo i23 -f <file in i23 format> -a <argument>
```
