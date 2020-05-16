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
	rm -f candlesticks.gp mean.gp stddev.gp
	rm -f plan-compact.json
	rm -f log.defaults log.err log.parallel
	rm -rf graphics
	rm -rf results

.PHONY: all default-parameters run doc clean
