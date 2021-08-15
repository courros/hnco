all: default-parameters run report

default-parameters:
	hnco --print-defaults > log.defaults

run:
	echo `date` >> log.err
	hnco-walsh-transforms-skeleton.pl
	time -o log.err -a hnco-walsh-transforms-run.pl

report:
	hnco-walsh-transforms-stat.pl
	./graphics.gp
	sed '/^$$/d' plan.json > report/pretty.json
	make -C report

clean:
	rm -f report/pretty.json
	rm -f log.defaults log.err
	rm -f graphics.gp
	rm -rf results
	rm -rf graphics

.PHONY: all default-parameters run report clean
