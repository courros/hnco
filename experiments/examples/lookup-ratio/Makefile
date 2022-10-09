all: default-parameters run report

default-parameters:
	hnco --print-default-parameters > log.defaults

run:
	echo `date` >> log.err
	hnco-benchmark-skeleton.pl
	time -o log.err -a hnco-benchmark-run.pl

report:
	hnco-lookup-ratio-stat.pl
	./candlesticks.gp
	sed '/^$$/d' plan.json > report/pretty.json
	make -C report

clean:
	rm -f log.defaults log.err
	rm -f report/pretty.json
	rm -f candlesticks.gp clouds.gp
	rm -rf results
	rm -rf graphics
	rm -rf commands.txt

.PHONY: all default-parameters run report clean
