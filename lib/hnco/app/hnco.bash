_hnco() 
{
    local cur prev opts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"
    opts="--algorithm --bm-mc-reset-strategy --bm-num-gs-cycles --bm-num-gs-steps --bm-sampling --budget --bv-size --description-path --ea-crossover-bias --ea-crossover-probability --ea-it-initial-hamming-weight --ea-it-replacement --ea-lambda --ea-mu --ea-mutation-rate --ea-mutation-rate-max --ea-mutation-rate-min --ea-success-ratio --ea-tournament-size --ea-update-strength --expression --fn-name --fn-num-traps --fn-prefix-length --fn-threshold --fp-default-double-precision --fp-default-double-rep --fp-default-double-size --fp-default-int-rep --fp-default-long-rep --fp-expression --fp-expression-source --fp-representations --fp-representations-source --function --hea-reset-period --learning-rate --map --map-input-size --map-path --map-ts-length --map-ts-sampling-mode --neighborhood --neighborhood-iterator --noise-stddev --num-iterations --num-threads --path --pn-mutation-rate --pn-neighborhood --pn-radius --population-size --pv-log-num-components --radius --rep-categorical-representation --rep-num-additional-bits --representations-path --results-path --rls-patience --sa-beta-ratio --sa-initial-acceptance-probability --sa-num-transitions --sa-num-trials --seed --selection-size --solution-path --target --additive-gaussian-noise --bm-log-norm-1 --bm-log-norm-infinite --bm-negative-positive-selection --cache --cache-budget --concrete-solution --ea-allow-no-mutation --ea-it-log-center-fitness --ea-log-mutation-rate --fn-display --fn-get-bv-size --fn-get-maximum --fn-has-known-maximum --fn-provides-incremental-evaluation --fn-walsh-transform --hea-bound-moment --hea-log-delta-norm --hea-log-herding-error --hea-log-target --hea-log-target-norm --hea-randomize-bit-order --help --help-alg --help-bm --help-ctrl --help-ea --help-eda --help-fn --help-fp --help-hea --help-ls --help-map --help-mod --help-pn --help-rep --help-sa --incremental-evaluation --load-solution --log-improvement --map-display --map-random --map-surjective --mmas-strict --negation --parsed-modifier --pn-allow-no-mutation --print-default-parameters --print-description --print-parameters --print-results --print-solution --prior-noise --pv-log-entropy --pv-log-pv --record-evaluation-time --record-total-time --restart --rls-strict --rw-log-value --save-description --save-results --save-solution --stop-on-maximum --stop-on-target --version"

    if [[ ${cur} == -* ]] ; then
        COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
        return 0
    fi
}
complete -F _hnco hnco
