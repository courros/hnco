_hnco-mo() 
{
    local cur prev opts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"
    opts="--algorithm --bv-size --description-path --ea-lambda --ea-mu --fn-name --fp-expression --fp-lower-bound --fp-num-bits --fp-precision --fp-upper-bound --function --ga-crossover-bias --ga-crossover-probability --ga-tournament-size --mutation-rate --num-iterations --num-threads --path --rep-categorical-representation --rep-num-additional-bits --results-path --seed --solution-path --allow-no-mutation --concrete-solution --fn-display --fn-get-bv-size --help --help-ea --help-fp --help-rep --load-solution --print-defaults --print-description --print-header --print-results --print-solution --save-description --save-results --save-solution --version"

    if [[ ${cur} == -* ]] ; then
        COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
        return 0
    fi
}
complete -F _hnco-mo hnco-mo
