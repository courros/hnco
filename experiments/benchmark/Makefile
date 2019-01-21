all: default-parameters run doc

default-parameters:
	hnco --print-defaults > log.defaults

run:
	echo `date` >> log.err
	hnco-benchmark-skeleton.pl
	time -o log.err -a hnco-benchmark-run.pl

doc:
	hnco-benchmark-stat.pl
	./candlesticks.gp
	./clouds.gp
	sed '/^$$/d' plan.json > plan-compact.json
	make -C report

clean:
	rm -f plan-compact.json
	rm -f log.defaults log.err log.parallel
	rm -f commands.txt
	rm -f candlesticks.gp clouds.gp
	rm -rf graphics
	rm -rf results

.PHONY: all default-parameters run doc clean
