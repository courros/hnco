all: default-parameters run report

default-parameters:
	hnco --print-default-parameters > log.defaults

run:
	echo `date` >> log.err
	hnco-benchmark-skeleton.pl
	time -o log.err -a hnco-fixed-target-run.pl

report:
	hnco-fixed-target-stat.pl
	./candlesticks.gp
	sed '/^$$/d' plan.json > report/pretty.json
	make -C report

clean:
	rm -f report/pretty.json
	rm -f log.defaults log.err log.parallel
	rm -f commands.txt
	rm -f candlesticks.gp
	rm -rf graphics
	rm -rf results

.PHONY: all default-parameters run report clean
