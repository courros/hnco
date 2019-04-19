_ffgen() 
{
    local cur prev opts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"
    opts="--bv-size --ep-upper-bound --function --ising-num-columns --ising-num-rows --ms-num-clauses --ms-num-literals-per-clause --nk-k --path --seed --stddev --walsh-num-features --help --ising-periodic-boundary-condition --ms-planted-solution --version"

    if [[ ${cur} == -* ]] ; then
        COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
        return 0
    fi
}
complete -F _ffgen ffgen
