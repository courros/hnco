all: default-parameters run report

default-parameters:
	hnco --print-default-parameters > log.defaults

run:
	echo `date` >> log.err
	hnco-observable-evolution-skeleton.pl
	time -o log.err -a hnco-observable-evolution-run.pl

report:
	hnco-observable-evolution-stat.pl
	./graphics.gp
	sed '/^$$/d' plan.json > report/pretty.json
	make -C report

clean:
	rm -f log.defaults log.err log.parallel
	rm -f commands.txt
	rm -f report/pretty.json
	rm -f graphics.gp
	rm -rf results
	rm -rf graphics

.PHONY: all default-parameters run report clean
