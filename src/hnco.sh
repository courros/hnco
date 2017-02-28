_hnco() 
{
    local cur prev opts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"
    opts="--algorithm --bm-mc-reset-strategy --bm-num-gs-cycles --bm-num-gs-steps --bm-sampling --budget --bv-size --ea-lambda --ea-mu --fun-num-traps --fun-prefix-length --fun-threshold --function --ga-crossover-probability --ga-tournament-size --hea-binary-dynamics --hea-delay --hea-num-par-updates --hea-num-seq-updates --hea-rate-strategy --hea-reset-period --hea-sampling-method --hea-time-constant --hea-weight --learning-rate --map --map-input-size --map-path --neighborhood --neighborhood-iterator --noise-stddev --num-iterations --path --patience --plugin-function-name --population-size --pv-log-num-components --radius --sa-initial-acceptance-probability --sa-num-transitions --sa-num-trials --sa-rate --scaled-mutation-probability --seed --selection-size --additive-gaussian-noise --bm-log-norm-infinite --bm-log-norm-l1 --bm-negative-positive-selection --cache --describe-problem --describe-solution --hea-bound-moment --hea-log-delta --hea-log-dtu --hea-log-error --hea-log-selection --hea-randomize-bit-order --log-improvement --map-random --negation --no-header --print-default-parameters --print-performances --print-solution --pv-log-entropy --pv-log-pv --restart --stop-on-maximum --help --version"

    if [[ ${cur} == -* ]] ; then
        COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
        return 0
    fi
}
complete -F _hnco hnco
