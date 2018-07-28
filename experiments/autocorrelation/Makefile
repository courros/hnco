all: default-parameters run doc

default-parameters:
	hnco --print-defaults > log.defaults

run:
	hnco-walsh-skeleton.pl
	echo `date` >> log.err
	time -o log.err -a hnco-autocorrelation-run.pl

doc:
	hnco-autocorrelation-stat.pl
	./graphics.gp
	sed '/^$$/d' plan.json > plan-compact.json
	make -C report

clean:
	rm -f graphics.gp plan-compact.json log.defaults log.err
	rm -rf results
	rm -rf graphics

.PHONY: all default-parameters run doc clean
