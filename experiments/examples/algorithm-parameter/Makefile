all: default-parameters run report

default-parameters:
	hnco --print-defaults > log.defaults

run:
	echo `date` >> log.err
	hnco-algorithm-parameter-skeleton.pl
	time -a -o log.err hnco-algorithm-parameter-run.pl

report:
	hnco-algorithm-parameter-stat.pl
	./candlesticks.gp
	./mean.gp
	./stddev.gp
	sed '/^$$/d' plan.json > report/pretty.json
	make -C report

clean:
	rm -f candlesticks.gp mean.gp stddev.gp
	rm -f report/pretty.json
	rm -f log.defaults log.err log.parallel
	rm -rf graphics
	rm -rf results

.PHONY: all default-parameters run report clean
