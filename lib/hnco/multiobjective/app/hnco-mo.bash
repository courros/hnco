_hnco-mo() 
{
    local cur prev opts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"
    opts="--algorithm --bv-size --description-path --ea-crossover-probability --ea-mu --ea-mutation-rate --ea-tournament-size --fn-name --fp-expression --fp-lower-bound --fp-num-bits --fp-precision --fp-upper-bound --function --num-iterations --num-threads --path --rep-categorical-representation --rep-num-additional-bits --results-path --seed --solution-path --ea-allow-no-mutation --fn-display --fn-get-bv-size --fn-get-output-size --help --help-ea --help-fp --help-rep --print-defaults --print-description --print-header --version"

    if [[ ${cur} == -* ]] ; then
        COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
        return 0
    fi
}
complete -F _hnco-mo hnco-mo
