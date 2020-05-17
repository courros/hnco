all: default-parameters run report

default-parameters:
	hnco --print-defaults > log.defaults

run:
	echo `date` >> log.err
	hnco-benchmark-skeleton.pl
	time -o log.err -a hnco-benchmark-run.pl

report:
	hnco-lookup-ratio-stat.pl
	./candlesticks.gp
	sed '/^$$/d' plan.json > pretty.json
	make -C report

clean:
	rm -f candlesticks.gp clouds.gp pretty.json log.defaults log.err
	rm -rf results
	rm -rf graphics
	rm -rf commands.txt

.PHONY: all default-parameters run doc clean
