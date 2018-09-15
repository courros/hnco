_hnco() 
{
    local cur prev opts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"
    opts="--algorithm --bm-mc-reset-strategy --bm-num-gs-cycles --bm-num-gs-steps --bm-sampling --budget --bv-size --ea-lambda --ea-mu --fn-name --fn-num-traps --fn-prefix-length --fn-threshold --function --ga-crossover-bias --ga-crossover-probability --ga-tournament-size --hea-binary-dynamics --hea-delay --hea-num-par-updates --hea-num-seq-updates --hea-rate-strategy --hea-reset-period --hea-sampling-method --hea-time-constant --hea-weight --learning-rate --map --map-input-size --map-path --mutation-probability --neighborhood --neighborhood-iterator --noise-stddev --num-iterations --num-threads --path --pn-mutation-probability --pn-neighborhood --pn-radius --population-size --pv-log-num-components --radius --rls-patience --sa-beta-ratio --sa-initial-acceptance-probability --sa-num-transitions --sa-num-trials --seed --selection-size --target --additive-gaussian-noise --allow-stay --bm-log-norm-infinite --bm-log-norm-l1 --bm-negative-positive-selection --cache --cache-budget --describe-solution --fn-display --fn-get-bv-size --fn-get-maximum --fn-has-known-maximum --fn-provides-incremental-evaluation --fn-walsh-transform --hea-bound-moment --hea-log-delta --hea-log-dtu --hea-log-error --hea-log-selection --hea-randomize-bit-order --help --help-bm --help-hea --help-map --help-pn --incremental-evaluation --log-improvement --map-random --map-surjective --mmas-strict --negation --pn-allow-stay --print-defaults --print-header --print-performance --print-solution --prior-noise --pv-log-entropy --pv-log-pv --restart --rls-strict --rw-log-value --stop-on-maximum --stop-on-target --version"

    if [[ ${cur} == -* ]] ; then
        COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
        return 0
    fi
}
complete -F _hnco hnco
