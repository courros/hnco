all: default-parameters run doc

default-parameters:
	hnco --print-defaults > log.defaults

run:
	echo `date` >> log.err
	hnco-parameter-skeleton.pl
	time -a -o log.err hnco-parameter-run.pl

doc:
	hnco-parameter-stat.pl
	./candlesticks.gp
	./mean.gp
	./stddev.gp
	sed '/^$$/d' plan.json > plan-compact.json
	make -C report

clean:
	rm -f candlesticks.gp plan-compact.json log.defaults log.err mean.gp stddev.gp
	rm -rf results
	rm -rf graphics

.PHONY: all default-parameters run doc clean
