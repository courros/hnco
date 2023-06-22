all: default-parameters run report

default-parameters:
	hnco --print-default-parameters > log.defaults

run:
	echo `date` >> log.err
	uname -a >> log.err
	hnco-runtime-skeleton.pl
	time -a -o log.err hnco-runtime-run.pl

report:
	hnco-runtime-stat.pl
	./candlesticks.gp
	./mean.gp
	./stddev.gp
	sed '/^$$/d' plan.json > report/pretty.json
	make -C report

clean:
	rm -f commands.txt
	rm -f log.defaults log.err log.parallel
	rm -f candlesticks.gp mean.gp stddev.gp
	rm -f report/pretty.json
	rm -rf results
	rm -rf graphics

.PHONY: all default-parameters run report clean
