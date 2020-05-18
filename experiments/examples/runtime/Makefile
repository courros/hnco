all: default-parameters run report

default-parameters:
	hnco --print-defaults > log.defaults

run:
	echo `date` >> log.err
	hnco-algorithm-parameter-skeleton.pl
	time -a -o log.err hnco-algorithm-parameter-run.pl

report:
	hnco-runtime-stat.pl
	./candlesticks.gp
	./mean.gp
	./stddev.gp
	sed '/^$$/d' plan.json > pretty.json
	make -C report

clean:
	rm -f candlesticks.gp pretty.json log.defaults log.err mean.gp stddev.gp
	rm -rf results
	rm -rf graphics

.PHONY: all default-parameters run report clean
