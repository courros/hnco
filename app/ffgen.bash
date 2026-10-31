_ffgen() 
{
    local cur prev opts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"
<<<<<<< HEAD
    opts="--bv-size --coupling-constant --ep-upper-bound --field-constant --function --lin-distance --lin-generator --lin-initial-weight --lin-ratio --nk-k --nn1-generator --nn2-generator --nn2-num-columns --nn2-num-rows --part-upper-bound --path --sat-num-clauses --sat-num-literals-per-clause --seed --stddev --sudoku-num-empty-cells --walsh2-generator --walsh2-ising-alpha --walsh-num-features --help --periodic-boundary-conditions --sat-planted-solution --version"
=======
    opts="--bv-size --coupling-constant --ep-upper-bound --field-constant --function --lin-distance --lin-generator --lin-initial-weight --lin-ratio --ms-num-clauses --ms-num-literals-per-clause --nk-k --nn1-generator --nn2-generator --nn2-num-columns --nn2-num-rows --part-upper-bound --path --seed --stddev --sudoku-num-empty-cells --sys-degree --sys-num-equations --sys-num-terms-max --walsh2-generator --walsh2-ising-alpha --walsh-num-features --help --ms-planted-solution --periodic-boundary-conditions --version"
>>>>>>> function-system-polynomial-equations

    if [[ ${cur} == -* ]] ; then
        COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
        return 0
    fi
}
complete -F _ffgen ffgen
